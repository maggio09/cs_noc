#ifndef CS_ROUTER_H
#define CS_ROUTER_H

#include <systemc>
#include <iostream>
#include "defines.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(cs_router){
   // I/O
    //global pins
	sc_in<bool> clk;
	sc_in<bool> reset;
    //control signals
	sc_in<bool> request[MAX_PORT];
	sc_in<bool> release[MAX_PORT];

	sc_out<bool> grant[MAX_PORT];
	sc_out<bool> sl_grant[MAX_PORT];
	sc_out<bool> pend[MAX_PORT];

    //address signals
	sc_in <sc_lv<ADDR_BUS_SIZE> > tx_addr[MAX_PORT];
	sc_out <sc_lv<ADDR_BUS_SIZE> > rx_addr[MAX_PORT];

    //rx, tx signals
	sc_in<bool> tx_rnw[MAX_PORT];
	sc_in<bool> tx_valid[MAX_PORT];
	sc_in<bool> tx_cts[MAX_PORT];

	sc_out<bool> rx_rnw[MAX_PORT];
	sc_out<bool> rx_valid[MAX_PORT];
	sc_out<bool> rx_cts[MAX_PORT];

    //data signals
	sc_in  <sc_lv<DATA_BUS_SIZE> > tx_data[MAX_PORT];
	sc_out <sc_lv<DATA_BUS_SIZE> > rx_data[MAX_PORT];



	SC_HAS_PROCESS(cs_router);


    //circuit-switching router
	cs_router(sc_module_name nm, int id, int port)
	:sc_module(nm)
	{
		router_id = id;
		num_port = port;

		SC_THREAD(table_arbiter);
		sensitive << reset.pos() << clk.pos();   
		SC_THREAD(routing_table);
		sensitive << reset.pos() << clk.pos();   
		SC_THREAD(port_queue);
		sensitive << reset.pos() << clk.pos();   
		SC_THREAD(port_arbiter);
		sensitive << reset.pos() << clk.pos();   
		SC_THREAD(switchbox);
		sensitive << crossbar_enable;
 
		sc_fifo<int> request_fifo();                            // default size 16
		sc_fifo<sc_lv<ADDR_BUS_SIZE> > tx_addr_fifo();          // default size 16
	}

    //funtions
	void table_arbiter();
	void routing_table();
	void port_queue();
	void port_arbiter();
	void switchbox();
	

    //variables
	sc_uint<6>  router_id;
	int         num_port;

	sc_event crossbar_enable;
	int index;
	int src_mask;
	bool port_valid;
	int dest_mask;
	int port_mask[MAX_PORT];
	bool port_enable[MAX_PORT];
	int crossbar_link[2];




};

#endif
