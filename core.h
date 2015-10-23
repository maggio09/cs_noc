#ifndef CORE_H
#define CORE_H

#include <systemc>
#include <cassert>
#include <list>
#include "defines.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

extern double average_delay;
extern list<list_elem> my_scenario;

SC_MODULE(core) {
   //I/O
    //Global signals
   sc_in <bool>         clk;
   sc_in <bool>         reset;

   sc_in<bool>		request_in;
   sc_in<sc_lv<2> >	ans_in;		 // 00 : release & pend , 01 : continue, 10 : fail, 11 : grant
   sc_in<sc_lv<ADDR_BUS_SIZE> > rx_addr_r;

   sc_out<bool>		request_out;
   sc_out<sc_lv<2> >	ans_out;
   sc_out<sc_lv<ADDR_BUS_SIZE> > tx_addr_r;

   sc_in<sc_lv<ADDR_BUS_SIZE> >	rx_addr;
   sc_in<sc_lv<DATA_BUS_SIZE> > rx_data;
   sc_in<sc_logic>	rx_rnw;
   sc_in<sc_logic>	rx_valid;
   sc_in<sc_logic>	rx_cts;

   sc_out<sc_lv<ADDR_BUS_SIZE> > tx_addr;
   sc_out<sc_lv<DATA_BUS_SIZE> > tx_data;
   sc_out<sc_logic>	tx_rnw;
   sc_out<sc_logic>	tx_valid;
   sc_out<sc_logic>	tx_cts;


//   list<list_elem> my_scenario;
//   list<list_elem>::iterator itor=my_scenario.begin();

   SC_HAS_PROCESS(core);

   core(sc_module_name nm, int id):sc_module(nm) {
      core_id = id;
      core_status = IDLE;

      SC_THREAD(job_read);

      SC_THREAD(transfer_data);
      sensitive << reset.pos() << clk.pos();
       
      SC_THREAD(receive_data);
      sensitive << reset.pos() << clk.pos();

      SC_THREAD(clock_count);
      sensitive << reset.pos() << clk.pos();

   }

//   sc_uint<6> core_id;
   int core_id;
   sc_uint<64> count;
   int core_status;

   enum status{
	IDLE,
	MASTER_INIT,
	MASTER_WAIT,
	MASTER_TRANSFER,
	MASTER_RECEIVE,
	SLAVE_INIT,
	SLAVE_WAIT,
	SLAVE_TRANSFER,
	SLVAE_RECEIVE
   };

//   list<list_elem> my_scenario;
   list<list_elem> job_list;
   list_elem temp;
   list_elem job;
//   list<list_elem> my_scenario::iterator itor = my_scenario.begin();
   void job_read();
   void transfer_data();
   void receive_data();
   void clock_count();
   int log2(int x);
   int abs_s(int x);

};

#endif


