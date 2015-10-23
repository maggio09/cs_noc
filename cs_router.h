#ifndef CS_ROUTER_H
#define CS_ROUTER_H

#include <systemc>
#include <iostream>
#include "defines.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;


extern int NODE_INFO[NUM_ROUTER][NUM_CORE];
extern int PORT_INFO[NUM_ROUTER][MAX_PORT];

SC_MODULE(cs_router){

/************************** I/O pin *********************************/

    ///////////////////// Global pins //////////////////////
	sc_in<bool>                       clk;
	sc_in<bool>                       reset;

    //////////////////// Control signals ///////////////////
	sc_in<bool>                       request[MAX_PORT];	// initiates either a router update request or a connection request
	sc_in<sc_lv<2> >		  ans_in[MAX_PORT];	// 00 : release & pend , 01 : continue, 10 : fail, 11 : grant
	sc_in<sc_lv<ADDR_BUS_SIZE> >	  tx_addr_r[MAX_PORT];

	sc_out<bool>                      sl_grant[MAX_PORT];	// request out signal or slave indicator
	sc_out<sc_lv<2> >		  ans_out[MAX_PORT];	// 00 : release & pend , 01 : continue, 10 : fail, 11 : grant
	sc_out<sc_lv<ADDR_BUS_SIZE> >	  rx_addr_r[MAX_PORT];

    /////////////////// Address signals ////////////////////
	sc_in <sc_lv<ADDR_BUS_SIZE> >     tx_addr[MAX_PORT];	// tx address bus of parameterisable width
	sc_out <sc_lv<ADDR_BUS_SIZE> >    rx_addr[MAX_PORT];	// rx address bus of parameterisable width

    /////////////////// Rx, Tx signals /////////////////////
	sc_in<sc_logic>                   tx_rnw[MAX_PORT];	// tx read-not-write signal
	sc_in<sc_logic>                   tx_valid[MAX_PORT];	// indicates valid tx data, address and rnw signals
	sc_in<sc_logic>                   tx_cts[MAX_PORT];	// tx clear-to-send signal

	sc_out<sc_logic>                  rx_rnw[MAX_PORT];	// rx read-not-write signal
	sc_out<sc_logic>                  rx_valid[MAX_PORT];	// indicates valid rx data, address and rnw singals
	sc_out<sc_logic>                  rx_cts[MAX_PORT];	// rx clear-to-send singnal

    ////////////////////// Data signals ////////////////////
	sc_in  <sc_lv<DATA_BUS_SIZE> >    tx_data[MAX_PORT];	// tx data bus of parameterisable width
	sc_out <sc_lv<DATA_BUS_SIZE> >    rx_data[MAX_PORT];	// rx data bus of parameterisable width

    /*  Flow control is dictated by the use of the cts signals.
	When the receiving node detects almost full at its receive
	FIFO, its tx_cts signal is lowered and is propagated to the
	transmitting node’s rx_cts signal. At this point, the transmit-
	ting node must stall until its rx_cts is again raised, indicat-
	ing that the receiving node is capable of accepting more data. 
	As long as the slave’s cts (not shown) is held high, the master can initiate
	a new data transaction on every clock cycle. In the event,
	the slave cannot keep up, it can de-assert its tx_cts,
	thereby causing the master to temporarily suspend data
	transfers.*/

/********************************************************************/

	SC_HAS_PROCESS(cs_router);

    //circuit-switching router
	cs_router(sc_module_name nm, int id, int port)
	:sc_module(nm)
	{
		router_id = id;
		num_port = port;

		for(i=0; i<MAX_PORT; i++) router_status[i] = None;

		SC_THREAD(table_arbiter);
		sensitive << reset.pos() << clk.pos();   


		SC_THREAD(port_queue);
		sensitive << reset.pos() << clk.pos();   

		SC_THREAD(port_arbiter);
		sensitive << reset.pos() << clk.pos();   

		SC_THREAD(switchbox);
		sensitive << crossbar_enable;
 
	}

    //funtions
	void table_arbiter();
	void port_queue();
	void port_arbiter();
	void switchbox();
	
    //variables
	sc_uint<6>  router_id;
	int         num_port;
	int	    router_status[MAX_PORT];
	int	    status_detect[MAX_PORT];


	enum Character{
	        None = 0,
	        Master = 1,
	        Default = 2,
	        Slave = 3
	};


//	enum Character status;
//	enum ANS ans;

	sc_event    crossbar_enable;

	int         src_mask[MAX_PORT];
	bool        port_valid[MAX_PORT];
	int         dest_mask[MAX_PORT];
	int	    dest_node[MAX_PORT];

	int         port_mask[MAX_PORT];
	int	    target_node[MAX_PORT];

	bool        port_enable[MAX_PORT];
	bool	    crossbar_start_map[MAX_PORT][MAX_PORT];

	int i;

};

#endif
