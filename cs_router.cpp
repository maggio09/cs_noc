#include <systemc>
#include <iostream>
#include <math.h>
#include <cassert>
#include <queue>
#include "cs_router.h"
#include "defines.h"

using namespace std;

//extern int NODE_INFO[NUM_ROUTER][NUM_CORE];
//extern int PORT_INFO[NUM_ROUTER][MAX_PORT];


/******************************************************************************************
Table Arbiter
: The table arbiter receives connection requests and schedules access to the routing table
in the case that multiple requests are received on the same clock cycle. This block is also
responsible for managing the routing table update requests.
*******************************************************************************************/
void cs_router::table_arbiter() {	// input : request[MAX_PORT], tx_addr_r[MAX_PORT]
					// output : src_mask[MAX_PORT], dest_mask[MAX_PORT], dest_node[MAX_PORT], port_valid[MAX_PORT]
	bool start = false;
//	enum Character status;

	int i;
	int temp_request;

	int index[MAX_PORT];
	int node_table[NUM_CORE];
	int port_table[MAX_PORT];
//	int NODE_INFO[NUM_ROUTER][NUM_CORE];
//	int PORT_INFO[NUM_ROUTER][MAX_PORT];

	sc_lv<ADDR_BUS_SIZE> 		temp_tx_addr;
	queue<int> 			request_fifo;
	queue<sc_lv<ADDR_BUS_SIZE> >	tx_addr_fifo;
//	sc_fifo<int> request_fifo (MAX_PORT);				
//	sc_fifo<sc_lv<ADDR_BUS_SIZE> > tx_addr_fifo (MAX_PORT);

	while(true) {
		if(reset.read()){
			temp_request=-1;

			for(i=0; i<MAX_PORT; i++){
				index[i] = -1;
				src_mask[i] = -1;
				dest_mask[i] = -1;
				dest_node[i] = -1;
				port_valid[i] = false;
			}

			if( router_id<1 || router_id>NUM_CORE ){
				for(i=0; i<NUM_CORE; i++) node_table[i]=-1;
				for(i=0; i<MAX_PORT; i++) port_table[i]=-1;
			}
			else{
				for(i=0; i<NUM_CORE; i++) node_table[i]=NODE_INFO[router_id-1][i];
				for(i=0; i<MAX_PORT; i++) port_table[i]=PORT_INFO[router_id-1][i];
			}
	

			while( !request_fifo.empty() )
				request_fifo.pop();
			while( !tx_addr_fifo.empty() )
				tx_addr_fifo.pop();

			start = true;
		} 
		else if(start){
			for(i=0; i<MAX_PORT; i++){
				if( request[i].read() ){				// if request is true
					request_fifo.push(i);				// store port number and
					tx_addr_fifo.push(tx_addr_r[i].read());		// tx_addr

//					cout << request_fifo.front() << endl;
//					assert(false);
				}
			}

			for(i=0; i<MAX_PORT; i++){
				index[i] = -1;
				src_mask[i] = -1;
				dest_mask[i] = -1;
				dest_node[i] = -1;
				port_valid[i] = false;
			}

			while(!request_fifo.empty()){			// if request fifo is not empty
				temp_request = request_fifo.front();
				request_fifo.pop();
				temp_tx_addr = tx_addr_fifo.front();
				tx_addr_fifo.pop();

				if( temp_tx_addr == NODE1_ADDR ) index[temp_request] = 0;
				else if( temp_tx_addr == NODE2_ADDR ) index[temp_request] = 1;
				else if( temp_tx_addr == NODE3_ADDR ) index[temp_request] = 2;
				else if( temp_tx_addr == NODE4_ADDR ) index[temp_request] = 3;
				else if( temp_tx_addr == NODE5_ADDR ) index[temp_request] = 4;
				else if( temp_tx_addr == NODE6_ADDR ) index[temp_request] = 5;
				else if( temp_tx_addr == NODE7_ADDR ) index[temp_request] = 6;
				else index[temp_request] = -1;

				src_mask[temp_request] = temp_request;				// source port number

//				assert(false);

				if( index[temp_request] != -1 ){
//					cout<< index[temp_request] << endl;
//					assert(false);
					
					dest_mask[temp_request] = node_table[index[temp_request]];
//					cout<< dest_mask[temp_request] << endl;
//					cout<< node_table[0] << " " << node_table[1] <<endl;
//					assert(false);
					dest_node[temp_request] = index[temp_request];
					if( port_table[temp_request] < 0 ) status_detect[temp_request] = Master;	// input port is connected to node 
					else status_detect[temp_request] = Default;
				}
	
				port_valid[temp_request] = true;	
			}			
	  	} 
		wait();
	}
}


/******************************************************************************************
Port Queue
: This queue is used to maintain the connection request order,
while the requests await availability of the target port(s).
*******************************************************************************************/


void cs_router::port_queue() {		// input : dest_mask[MAX_PORT], dest_node[MAX_PORT], src_mask[MAX_PORT], port_valid[MAX_PORT], port_enable[MAX_PORT]
					// output : port_mask[MAX_PORT], target_node[MAX_PORT]
	bool start = false;
	sc_int<2> temp;

//	sc_fifo<sc_int<2> > port_queue[MAX_PORT];
	queue<sc_int<2> >	port_queue[MAX_PORT];
	int i;

	while(true) {
		if(reset.read()){	
			for( i=0; i<MAX_PORT; i++ ){
				while( !port_queue[i].empty() ) port_queue[i].pop();
				port_mask[i] = -1;
				target_node[i] = -1;
			}
			start = true;
		}
		else if(start){
			for( i=0; i<MAX_PORT; i++ ){	// in
				if( port_valid[i] ){
//					cout << dest_mask[i] << endl;//
//					assert(false);
					if( dest_mask[i]>=1 && dest_mask[i]<=MAX_PORT ){
						temp[0] = src_mask[i];
						temp[1] = dest_node[i];
						port_queue[dest_mask[i]-1].push(temp);
//						assert(false);
					}
				}
			}

			for(i=0; i<MAX_PORT; i++){	// out
				if( port_enable[i] && !port_queue[i].empty() ){
					temp = port_queue[i].front();
					port_queue[i].pop();
					port_mask[i] = temp[0];
					target_node[i] = temp[1];
				}
				else{
					port_mask[i] = -1;
					target_node[i] = -1;
				}
			}
		}
		wait();
	}
}

/******************************************************************************************
Port Arbiter
: Once the target port(s) becomes available, the port arbiter establishes the desired connection 
and issues the appropriate grant signals. This block also monitors the release signals for 
the removal of connections.
*******************************************************************************************/

void cs_router::port_arbiter() {	// input : port_mask[MAX_PORT], target_node[i], ans_in[MAX_PORT], 
					// output : port_enable[MAX_PORT], ans_out[MAX_PORT], sl_grant[MAX_PORT],
					//	    crossbar_enable, crossbar_link[MAX_PORT][2], crossbar_start_map[MAX_PORT][MAX_PORT], rx_addr_r[MAX_PORT] 
	bool start = false;
//	enum Character status;

	int PORT_INFO[NUM_ROUTER][MAX_PORT];
	int i,j;
	sc_lv<2> temp_ans;

	while(true) {
		if(reset.read()){
			for(i=0; i<MAX_PORT; i++){
				port_enable[i] = true;
				ans_out[i].write("zz");
				sl_grant[i].write(false);
				rx_addr_r[i].write(EMPTY_BUS); 
				for(j=0; j<MAX_PORT; j++) crossbar_start_map[i][j] = false;
			}
			start = true;
		}
		else if(start){
			for(i=0; i<MAX_PORT; i++){
				temp_ans = ans_in[i].read();		

				if( temp_ans == Release ){
					for(j=0; j<MAX_PORT; j++){
						if( crossbar_start_map[i][j] || crossbar_start_map[j][i] ){
							crossbar_start_map[i][j] = crossbar_start_map[j][i] = false;
							ans_out[j] = Release;
							router_status[i] = router_status[j] = None;
							port_enable[i] = port_enable[j] = true;
							break;
						}
					}
				}
				else if( temp_ans == Continue ){
					ans_out[i] = Continue;
				}
				else if( temp_ans == Fail ){
				}
				else if( temp_ans == Grant ){
					for(j=0; j<MAX_PORT; j++){
						if( crossbar_start_map[i][j] || crossbar_start_map[j][i] ){
							ans_out[j] = Grant;
							if( (status_detect[j]==Master) && (status_detect[i]==Default) ) router_status[i]=router_status[j]=Master;
							else if( (status_detect[j]==Default) && (status_detect[i]==Slave) ) router_status[i]=router_status[j]=Slave;
							else if( (status_detect[j]==Default) && (status_detect[i]==Default) ) router_status[i]=router_status[j]=Default;
//							else assert(false);
							break;
						}
					}
				}
//				else assert(false);


				if( port_mask[i] != -1 ){
//					assert(false);
					sl_grant[i] = true;
					port_enable[i] = port_enable[ port_mask[i] ] = false;
					crossbar_start_map[i][ port_mask[i] ] = true;

					switch( target_node[i] ){
						case 0: rx_addr_r[i].write(NODE1_ADDR); break;
						case 1: rx_addr_r[i].write(NODE2_ADDR); break;
						case 2: rx_addr_r[i].write(NODE3_ADDR); break;
						case 3: rx_addr_r[i].write(NODE4_ADDR); break;
						case 4: rx_addr_r[i].write(NODE5_ADDR); break;
						case 5: rx_addr_r[i].write(NODE6_ADDR); break;
						case 6: rx_addr_r[i].write(NODE7_ADDR); break;
						default : rx_addr_r[i].write(EMPTY_BUS); break;	
					}

					if( PORT_INFO[router_id][i] < 0 ) status_detect[i] = Slave;  // it's target port is connected to node
					else status_detect[i] = Default;
					// crossbar setup;
				}
				else{
					status_detect[i] = None;
					sl_grant[i] = false;
				}
				crossbar_enable.notify();
			}
		}
		wait();
	}
}

/********************************************************************************************
Switchbox
: The switchbox forms the actual connections between modules by enabling tri-state buffers
that allow the rx signals to drive the appropriate tx signals.
********************************************************************************************/

void cs_router::switchbox() {

	int i, j;


	while(true) {
		for(i=0; i<MAX_PORT; i++){
			rx_addr[i].write(EMPTY_BUS);
			rx_data[i].write(EMPTY_BUS);
			rx_rnw[i].write(sc_logic('z'));
			rx_valid[i].write(sc_logic('z'));
			rx_cts[i].write(sc_logic('z'));
		}


		for(i=0; i<MAX_PORT; i++){
			for(j=0; j<MAX_PORT; j++){
				if(crossbar_start_map[i][j] || crossbar_start_map[j][i]){
					rx_data[i].write( tx_data[j] );
					rx_addr[i].write( tx_addr[j] );
					rx_rnw[i].write( tx_rnw[j] );
					rx_valid[i].write( tx_valid[j] );
					rx_cts[i].write( tx_cts[j] );

					rx_data[j].write( tx_data[i] );
					rx_addr[j].write( tx_addr[i] );	
					rx_rnw[j].write( tx_rnw[i] );
					rx_valid[j].write( tx_valid[i] );
					rx_cts[j].write( tx_cts[i] );
				}
			}
		}
		wait();
	}
}

