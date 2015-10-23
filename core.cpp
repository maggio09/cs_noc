#include <systemc>
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <list>
#include "core.h"
#include "defines.h"

using namespace std;

//list<list_elem> my_scenario;


int core :: log2( int x ){
   int r_value = 0;
   int mul = 1;
   while ( x > mul ) {
      mul = mul*2;
      r_value++;
   }
   return r_value;
}


int core :: abs_s( int x ){
   if( x < 0 )
      return -x;
   else
      return x;
}


void core :: job_read(){
	if(my_scenario.empty()) assert(false);

	for(list<list_elem>::iterator itor=my_scenario.begin(); itor!=my_scenario.end(); itor++){
//		assert(false);
      		if(itor->src_id == core_id){
//			assert(false);
			temp = *itor;
			job_list.push_back(temp);
		}
	}
}

void core :: transfer_data()
{
   
   while(true) {
      if(reset.read())
      {
		request_out.write(false);
		ans_out.write("zz");
		tx_addr_r.write(EMPTY_BUS);
		tx_addr.write(EMPTY_BUS);
		tx_data.write(EMPTY_BUS);
		tx_rnw.write(sc_logic('z'));
		tx_valid.write(sc_logic('z'));
		tx_cts.write(sc_logic('z'));
      }
      else if(clk.read())
      {
		switch(core_status){
			case IDLE :{
				request_out.write(false);
				tx_addr_r.write(EMPTY_BUS);
				tx_addr.write(EMPTY_BUS);
				tx_data.write(EMPTY_BUS);
				tx_rnw.write(sc_logic('z'));
				tx_valid.write(sc_logic('z'));
				tx_cts.write(sc_logic('z'));
				ans_out.write(Continue);
				break;
			}
			case MASTER_INIT :{
				request_out.write(true);
				tx_addr.write(EMPTY_BUS);
				tx_data.write(EMPTY_BUS);
				tx_rnw.write(sc_logic('z'));
				tx_valid.write(sc_logic('z'));
				tx_cts.write(sc_logic('z'));
				ans_out.write(Continue);

				switch( job.dest_id ){
					case 1: tx_addr_r.write(NODE1_ADDR); break;
					case 2: tx_addr_r.write(NODE2_ADDR); break;
					case 3: tx_addr_r.write(NODE3_ADDR); break;
					case 4: tx_addr_r.write(NODE4_ADDR); break;
					case 5: tx_addr_r.write(NODE5_ADDR); break;
					case 6: tx_addr_r.write(NODE6_ADDR); break;
					case 7: tx_addr_r.write(NODE7_ADDR); break;
					default : assert(false); break;
				}
				break;
			}
			case MASTER_WAIT :{
				request_out.write(false);
				tx_addr_r.write(EMPTY_BUS);
				tx_addr.write(EMPTY_BUS);
				tx_data.write(EMPTY_BUS);
				tx_rnw.write(sc_logic('z'));
				tx_valid.write(sc_logic('z'));
				tx_cts.write(sc_logic('z'));
				ans_out.write(Continue);

				break;
			}
			case MASTER_TRANSFER :{
				// data transmit write followed by read implementation !								
				request_out.write(false);
				
				switch( job.src_id ){
					case 1: tx_addr.write(NODE1_ADDR); break;
					case 2: tx_addr.write(NODE2_ADDR); break;
					case 3: tx_addr.write(NODE3_ADDR); break;
					case 4: tx_addr.write(NODE4_ADDR); break;
					case 5: tx_addr.write(NODE5_ADDR); break;
					case 6: tx_addr.write(NODE6_ADDR); break;
					case 7: tx_addr.write(NODE7_ADDR); break;
					default : assert(false); break;
				}

				tx_addr_r.write(EMPTY_BUS);
				tx_data.write( job.data );
				tx_rnw.write( sc_logic('0') );
				tx_valid.write( sc_logic('1') );
				tx_cts.write( sc_logic('z') );
				ans_out.write(Continue);

				break;
			}
			case MASTER_RECEIVE :{
				break;
			}
			/*case SLAVE_INIT :{
				request_out.write(false);
				tx_addr_r.write(EMPTY_BUS);
				tx_addr.write(EMPTY_BUS);
				tx_data.write(EMPTY_BUS);
				tx_rnw.write(sc_logic('z'));
				tx_valid.write(sc_logic('z'));
				tx_cts.write(sc_logic('z'));
				ans_out.write(Grant);
//				core_status = SLAVE_RUNNING;
				break;
			}
			case SLAVE_RUNNING :{
				//
				//
				break;
			}*/
			default: break;
		}

      }
      wait();
   } 
}


void core :: receive_data()
{
   sc_lv<2> answer;
//   list<list_elem>::iterator job_itor = job_list.begin();
//   list_elem job;

   while(true) {
      if(reset.read())
      {
      		answer = "zz";
      }
      else if(clk.read())
      {
//		if( ans_in.read() == Release ) core_status = IDLE;

		switch(core_status){
			case IDLE :{


				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				else if( request_in.read() ){
					core_status = SLAVE_INIT;
					break;
				}
				else if( !job_list.empty() ){
					job = job_list.front();			
					job_list.pop_front();
					core_status = MASTER_INIT;
					break;
				}
				else{
					core_status = IDLE;
					break;
				}
			}
			case MASTER_INIT:{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				/*else if( request_in.read() ){
					assert(false);
					break;
				}*/
				else{
					core_status = MASTER_WAIT;
					break;
				}
			}
			case MASTER_WAIT:{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				/*else if( request_in.read() ){
					assert(false);
					break;
				}*/
				else if( ans_in.read() == Grant) {
					core_status = MASTER_TRANSFER;
					break;
				}
				else{
					core_status = MASTER_WAIT;
					break;	
				}
			}
			case MASTER_TRANSFER :{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				else{
					core_status = MASTER_RECEIVE;
					break;
				}
			}
			case MASTER_RECEIVE :{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				else{
					core_status = SLAVE_RECEIVE;
					break;
				}
			}
			case SLAVE_INIT :{	
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				else{
					core_status = SLAVE_RECEIVE;
					break;
				}
			}
			/*case SLAVE_WAIT :{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				if( ans_
			}*/
			case SLAVE_RECEIVE :{
				if( ans_in.read() == Release ){
					core_status = IDLE;
					break;
				}
				else if( rx_valid.read() && !rx_rnw.read() ){
					// receive signal
					core_status = SLAVE_RECEIVE;
					break;
				}
				else if( rx_valid.read() && rx_rnw.read() ){
					bool transfer_enable;

					/*if( !job_list.empty() ){
					}
					else transfer_enable
					//잡리스트를 뒤져서 마스터 노드와 주소가 같은것이 있으면 TRANSFER

					if () core_status = SLAVE_TRANSFER;
					else core_status = RELEASE;*/
					break;
				}
				else{
					core_status = SLAVE_RECEIVE;
					break;
				}
			}
			case RELEASE :{
				core_status = IDLE;
				break;
			}
/*			case SLAVE_RUNNING :{
				answer = ans_in.read();
				if( request_in ){
					// cts
				}
				else if( answer == Release ){
					core_status = IDLE;
				}
				else if( answer == Continue ){
					core_status = SLAVE_RUNNING;
				}
				else if( answer == Fail ){
					assert(false);			
				}
				else if( answer == Grant ){
					assert(false);
				}
				else assert(false);
		
				break;
			}*/
			default : break;
		}
      } 
      wait();
   }
}


void core :: clock_count()
{
   bool start = 0;
   while (true) {
      if(reset.read())
      {
         count = 0;
         start = 1;
      }
      else if(start)
         count ++;
      wait();
   }
}


