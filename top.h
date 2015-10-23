#ifndef TOP_H
#define TOP_H

#include <systemc>
//#include <list>
#include <iostream>

#include "cs_router.h"
#include "core.h"
#include "reset.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;


//int NODE_INFO[NUM_ROUTER][NUM_CORE];
//int PORT_INFO[NUM_ROUTER][MAX_PORT];
//extern list<list_elem> my_scenario;

SC_MODULE(top){
   //I/O
   sc_in<bool> 				clk;

   //signals
   sc_signal<bool>			reset_s;

   sc_signal<bool>			request_node_router[NUM_CORE];
   sc_signal<bool>			request_router_node[NUM_CORE];
   sc_signal<sc_lv<2> >			ans_node_router[NUM_CORE];
   sc_signal<sc_lv<2> >			ans_router_node[NUM_CORE];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >	addr_r_node_router[NUM_CORE];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >	addr_r_router_node[NUM_CORE];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >	addr_node_router[NUM_CORE];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >	addr_router_node[NUM_CORE];
   sc_signal<sc_lv<DATA_BUS_SIZE> >	data_node_router[NUM_CORE];
   sc_signal<sc_lv<DATA_BUS_SIZE> >	data_router_node[NUM_CORE];
   sc_signal<sc_logic>                  rnw_node_router[NUM_CORE];
   sc_signal<sc_logic>                  rnw_router_node[NUM_CORE];
   sc_signal<sc_logic>                  valid_node_router[NUM_CORE];
   sc_signal<sc_logic>                  valid_router_node[NUM_CORE];
   sc_signal<sc_logic>                  cts_node_router[NUM_CORE];
   sc_signal<sc_logic>                  cts_router_node[NUM_CORE];

   sc_signal<bool>                      request_in[NUM_ROUTER];
   sc_signal<bool>                      request_out[NUM_ROUTER];
   sc_signal<sc_lv<2> >                 ans_in[NUM_ROUTER];
   sc_signal<sc_lv<2> >                 ans_out[NUM_ROUTER]; 
   sc_signal<sc_lv<ADDR_BUS_SIZE> >     addr_r_in[NUM_ROUTER];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >     addr_r_out[NUM_ROUTER];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >     addr_in[NUM_ROUTER];
   sc_signal<sc_lv<ADDR_BUS_SIZE> >     addr_out[NUM_ROUTER];
   sc_signal<sc_lv<DATA_BUS_SIZE> >     data_in[NUM_ROUTER];
   sc_signal<sc_lv<DATA_BUS_SIZE> >     data_out[NUM_ROUTER];
   sc_signal<sc_logic>                  rnw_in[NUM_ROUTER];
   sc_signal<sc_logic>                  rnw_out[NUM_ROUTER];
   sc_signal<sc_logic>                  valid_in[NUM_ROUTER];
   sc_signal<sc_logic>                  valid_out[NUM_ROUTER];
   sc_signal<sc_logic>                  cts_in[NUM_ROUTER];
   sc_signal<sc_logic>                  cts_out[NUM_ROUTER];


   sc_trace_file*			tf;
//   int NODE_TABLE[NUM_ROUTER][MAX_PORT];
//   int PORT_TABLE[NUM_ROUTER][MAX_PORT];

//   list<list_elem> my_scenario;

  //module pointer
   cs_router *CS_ROUTER[NUM_ROUTER];
   core      *CORE[NUM_CORE];
   reset     *RESET;
  

//   SC_HAS_PROCESS(top); 
 
   top(sc_module_name nm) :sc_module(nm) {

      int i,j;
      //reset
      RESET = new reset("RESET");
      RESET->reset_inner(reset_s);

      //Router & Core
      for(i=0;i<NUM_ROUTER;i++)
      {
         char name_temp[20];
         sprintf(name_temp,"cs_router[%d]",i);   
         CS_ROUTER[i] = new cs_router(name_temp, i+1, MAX_PORT);
         CS_ROUTER[i]->clk(clk);
         CS_ROUTER[i]->reset(reset_s);

      }

      for(i=0;i<NUM_CORE;i++)
      {
         char name_temp[20];
         sprintf(name_temp,"core[%d]",i);   
         CORE[i] = new core(name_temp, i+1);
         CORE[i]->clk(clk);
         CORE[i]->reset(reset_s);
      }
    

//      for(i=0;i<NUM_ROUTER;i++)

//	Router - Core connect at port 1
//	assert(false);
	for(i=0; i<NUM_ROUTER; i++){

	CS_ROUTER[i]->request[0](request_node_router[i]);
	CORE[i]->request_out(request_node_router[i]);
	CS_ROUTER[i]->ans_in[0](ans_node_router[i]);
	CORE[i]->ans_out(ans_node_router[i]);
	CS_ROUTER[i]->tx_addr_r[0](addr_r_node_router[i]);
	CORE[i]->tx_addr_r(addr_r_node_router[i]);
	CS_ROUTER[i]->tx_addr[0](addr_node_router[i]);
	CORE[i]->tx_addr(addr_node_router[i]);
	CS_ROUTER[i]->tx_data[0](data_node_router[i]);
	CORE[i]->tx_data(data_node_router[i]); 
	CS_ROUTER[i]->tx_rnw[0](rnw_node_router[i]);
	CORE[i]->tx_rnw(rnw_node_router[i]);
	CS_ROUTER[i]->tx_valid[0](valid_node_router[i]);
	CORE[i]->tx_valid(valid_node_router[i]);
	CS_ROUTER[i]->tx_cts[0](cts_node_router[i]);
	CORE[i]->tx_cts(cts_node_router[i]);

//	assert(false);
	CS_ROUTER[i]->sl_grant[0](request_router_node[i]);
	CORE[i]->request_in(request_router_node[i]);
	CS_ROUTER[i]->ans_out[0](ans_router_node[i]);
	CORE[i]->ans_in(ans_router_node[i]);
	CS_ROUTER[i]->rx_addr_r[0](addr_r_router_node[i]);
	CORE[i]->rx_addr_r(addr_r_router_node[i]);
	CS_ROUTER[i]->rx_addr[0](addr_router_node[i]);
	CORE[i]->rx_addr(addr_router_node[i]);
	CS_ROUTER[i]->rx_data[0](data_router_node[i]);
	CORE[i]->rx_data(data_router_node[i]); 
	CS_ROUTER[i]->rx_rnw[0](rnw_router_node[i]);
	CORE[i]->rx_rnw(rnw_router_node[i]);
	CS_ROUTER[i]->rx_valid[0](valid_router_node[i]);
	CORE[i]->rx_valid(valid_router_node[i]);
	CS_ROUTER[i]->rx_cts[0](cts_router_node[i]);
	CORE[i]->rx_cts(cts_router_node[i]);

	}



//	for(j=0; j<MAX_PORT; j++){

	i=0; j=0;

	CS_ROUTER[i]->request[1](request_in[j]);
	CS_ROUTER[i]->ans_in[1](ans_in[j]);
	CS_ROUTER[i]->tx_addr_r[1](addr_r_in[j]);
	CS_ROUTER[i]->tx_addr[1](addr_in[j]);
	CS_ROUTER[i]->tx_data[1](data_in[j]);
	CS_ROUTER[i]->tx_rnw[1](rnw_in[j]);
	CS_ROUTER[i]->tx_valid[1](valid_in[j]);
	CS_ROUTER[i]->tx_cts[1](cts_in[j]);

	CS_ROUTER[i]->sl_grant[1](request_out[j]);
	CS_ROUTER[i]->ans_out[1](ans_out[j]);
	CS_ROUTER[i]->rx_addr_r[1](addr_r_out[j]);
	CS_ROUTER[i]->rx_addr[1](addr_out[j]);
	CS_ROUTER[i]->rx_data[1](data_out[j]);
	CS_ROUTER[i]->rx_rnw[1](rnw_out[j]);
	CS_ROUTER[i]->rx_valid[1](valid_out[j]);
	CS_ROUTER[i]->rx_cts[1](cts_out[j]);

	i=1; j=0;

	CS_ROUTER[i]->request[1](request_out[j]);
	CS_ROUTER[i]->ans_in[1](ans_out[j]);
	CS_ROUTER[i]->tx_addr_r[1](addr_r_out[j]);
	CS_ROUTER[i]->tx_addr[1](addr_out[j]);
	CS_ROUTER[i]->tx_data[1](data_out[j]);
	CS_ROUTER[i]->tx_rnw[1](rnw_out[j]);
	CS_ROUTER[i]->tx_valid[1](valid_out[j]);
	CS_ROUTER[i]->tx_cts[1](cts_out[j]);

	CS_ROUTER[i]->sl_grant[1](request_in[j]);
	CS_ROUTER[i]->ans_out[1](ans_in[j]);
	CS_ROUTER[i]->rx_addr_r[1](addr_r_in[j]);
	CS_ROUTER[i]->rx_addr[1](addr_in[j]);
	CS_ROUTER[i]->rx_data[1](data_in[j]);
	CS_ROUTER[i]->rx_rnw[1](rnw_in[j]);
	CS_ROUTER[i]->rx_valid[1](valid_in[j]);
	CS_ROUTER[i]->rx_cts[1](cts_in[j]);



//	assert(false);

//	SC_THREAD(set_info);

/*	NODE_INFO = {
		{ 1, 2},
		{ 2, 1}
	};

	PORT_INFO = {
		{ -1, 2},
		{ -2, 1}
	};*/

//	int NODE_INFO[NUM_ROUTER][NUM_CORE];
//	int PORT_INFO[NUM_ROUTER][MAX_PORT];
	
//	memcpy( NODE_INFO, NODE_TABLE, sizeof(NODE_INFO));
//	memcpy( PORT_INFO, PORT_TABLE, sizeof(PORT_INFO));


	//make wave file
//  	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("wave");
   	sc_trace(tf,clk,"clock");
   	sc_trace(tf,reset_s,"reset");
//   	for(i=0;i<NUM_CORE;i++)
//   	{
//		char name_temp[20];
//		sprintf(name_temp,"Request_Node1_Router1");
		sc_trace(tf, request_node_router[0], "Request_Node1_Router1");
		sc_trace(tf, ans_node_router[0], "Ans_Node1_Router1");
		sc_trace(tf, addr_r_node_router[0], "Addr_r_Node1_Router1");
		sc_trace(tf, addr_node_router[0], "Addr_Node1_Router1");
		sc_trace(tf, data_node_router[0], "Data_Node1_Router1");
		sc_trace(tf, rnw_node_router[0], "Rnw_Node1_Router1");
		sc_trace(tf, valid_node_router[0], "Valid_Node1_Router1");
		sc_trace(tf, cts_node_router[0], "Cts_Node1_Router1");

		sc_trace(tf, request_router_node[0], "Request_Router1_Node1");
		sc_trace(tf, ans_router_node[0], "Ans_Router1_Node1");
		sc_trace(tf, addr_r_router_node[0], "Addr_r_Router1_Node1");
//		sc_trace(tf, addr_router_node[0], "Addr_Router1_Node1");
//		sc_trace(tf, data_router_node[0], "Data_Router1_Node1");
//		sc_trace(tf, rnw_router_node[0], "Rnw_Router1_Node1");
//		sc_trace(tf, valid_router_node[0], "Valid_Router1_Node1");
//		sc_trace(tf, cts_router_node[0], "Cts_Router1_Node1");

		sc_trace(tf, request_node_router[1], "Request_Node2_Router2");
		sc_trace(tf, ans_node_router[1], "Ans_Node2_Router2");
		sc_trace(tf, addr_r_node_router[1], "Addr_r_Node2_Router2");
//		sc_trace(tf, addr_node_router[1], "Addr_Node2_Router2");
//		sc_trace(tf, data_node_router[1], "Data_Node2_Router2");
//		sc_trace(tf, rnw_node_router[1], "Rnw_Node2_Router2");
//		sc_trace(tf, valid_node_router[1], "Valid_Node2_Router2");
//		sc_trace(tf, cts_node_router[1], "Cts_Node2_Router2");

		sc_trace(tf, request_router_node[1], "Request_Router2_Node2");
		sc_trace(tf, ans_router_node[1], "Ans_Router2_Node2");
		sc_trace(tf, addr_r_router_node[1], "Addr_r_Router2_Node2");
		sc_trace(tf, addr_router_node[1], "Addr_Router2_Node2");
		sc_trace(tf, data_router_node[1], "Data_Router2_Node2");
		sc_trace(tf, rnw_router_node[1], "Rnw_Router2_Node2");
		sc_trace(tf, valid_router_node[1], "Valid_Router2_Node2");
		sc_trace(tf, cts_router_node[1], "Cts_Router2_Node2");

		sc_trace(tf, request_in[0], "Request_Router2_Router1");
		sc_trace(tf, ans_in[0], "Ans_Router2_Router1");
		sc_trace(tf, addr_r_in[0], "Addr_r_Router2_Router1");
	
		sc_trace(tf, request_out[0], "Request_Router1_Router2");
		sc_trace(tf, ans_out[0], "Ans_Router1_Router2");
		sc_trace(tf, addr_r_out[0], "Addr_r_Router1_Router2");
	


//	}
//	sc_close_vcd_trace_file(tf);
   }

   ~top(){
	sc_close_vcd_trace_file(tf);
   }
};
#endif
