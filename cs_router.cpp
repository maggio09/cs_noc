#include <systemc>
#include <iostream>
#include "cs_router.h"
#include "defines.h"

using namespace std;

/******************************************************************************************
Table Arbiter
: The table arbiter receives connection requests and schedules access to the routing table
in the case that multiple requests are received on the same clock cycle. This block is also
responsible for managing the routing table update requests.
*******************************************************************************************/
void cs_router::table_arbiter() {
	int i;
	int temp_request;
	sc_lv<ADDR_BUS_SIZE> temp_tx_addr;

	sc_fifo<int> request_fifo;				// default size 16
	sc_fifo<sc_lv<ADDR_BUS_SIZE> > tx_addr_fifo;		// default size 16

	while(true) {
		if(reset.read()){
			temp_request=-1;

			while( request_fifo.num_available() )
				request_fifo.nb_read(i);
			while( tx_addr_fifo.num_available() )
				tx_addr_fifo.nb_read(temp_tx_addr);

		} 
		else{
			for(i=0; i<MAX_PORT; i++){
				if( request[i].read() ){				// if request is true
					request_fifo.nb_write(i);			// store port number and
					tx_addr_fifo.nb_write(tx_addr[i].read());	// tx_addr
				}
			}

			index=-1;
			src_mask=-1;
			port_valid=false;

			if(request_fifo.nb_read(temp_request)){			// if request fifo is not empty
				tx_addr_fifo.nb_read(temp_tx_addr);
				if( temp_tx_addr == NODE1_ADDR ) index=0;				// translate temp_tx_addr to index
				else if( temp_tx_addr == NODE2_ADDR ) index=1;
				else if( temp_tx_addr == NODE3_ADDR ) index=2;
				else if( temp_tx_addr == NODE4_ADDR ) index=3;
				else if( temp_tx_addr == NODE5_ADDR ) index=4;
				else if( temp_tx_addr == NODE6_ADDR ) index=5;
				else if( temp_tx_addr == NODE7_ADDR ) index=6;
				else index=-1;
				src_mask=temp_request;				// source port number
				port_valid=true;	
			}			
	  	} 
		wait();
	}
}

/******************************************************************************************
Routing Table
: The routing table maps network module addresses to ports that may be used to establish
connections between modules. The node addresses serve as the index to the table and 
the data stored at that index represent the port(s) that may be used to establish the connection path.
*******************************************************************************************/

void cs_router::routing_table() {
	int i;
	int table[NUM_CORE];

	while(true) {
		if(reset.read()){
			if( router_id<1 || router_id>NUM_CORE ){
				for(i=0; i<NUM_CORE; i++)
					table[i]=-1;
			}
			else{
				for(i=0; i<NUM_CORE; i++)
					table[i]=TABLE_INFO[router_id][i];
			}
		}
		else{
			dest_mask = -1;
			if(index >=0) dest_mask = table[index];
		}
		wait();
	}
}

/******************************************************************************************
Port Queue
: This queue is used to maintain the connection request order,
while the requests await availability of the target port(s).
*******************************************************************************************/


void cs_router::port_queue() {

	sc_fifo<int> port_queue[MAX_PORT];
	int i,j;

	while(true) {
		if(reset.read()){	
			for( i=0; i<MAX_PORT; i++ ){
				while( port_queue[i].num_available() ) port_queue[i].nb_read(j);
			}
		}
		else{
			if(port_valid){
				if( dest_mask>=1 || dest_mask<=MAX_PORT )
					port_queue[dest_mask-1].nb_write(src_mask);	
			}

			for(i=0; i<MAX_PORT; i++){
				if( port_enable[i] && port_queue[i].num_available()  ){
					port_queue[i].nb_read(port_mask[i]);
					break;
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

void cs_router::port_arbiter() {

	int target_port;
	int i;

	while(true) {
		if(reset.read()){
			target_port=-1;
		}
		else{
			for(i=0; i<MAX_PORT; i++){
				if( release[i].read() ){
					port_enable[i]=true;
					target_port = i+1;
					break;
				}
				target_port = 0;
			}

			if( target_port ){
				if( port_mask[ target_port-1 ] ){
					grant[ target_port-1 ].write(1);
				        crossbar_link[0] = port_mask[target_port-1]; 
					crossbar_link[1] = target_port-1;	
					crossbar_enable.notify();
				}
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

	while(true) {

		rx_data[crossbar_link[1]].write( tx_data[crossbar_link[0]] );
		rx_addr[crossbar_link[1]].write( tx_addr[crossbar_link[0]] );
		
		wait();
	}
}

