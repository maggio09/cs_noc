#include <systemc>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <list>
#include <math.h>
#include "defines.h"
#include "top.h"
//#include "core.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

double average_delay;
list<data_elem> my_data;
list<list_elem> my_scenario;
int NODE_INFO[NUM_ROUTER][NUM_CORE];
int PORT_INFO[NUM_ROUTER][MAX_PORT];

int sc_main(int argc, char *argv[])
{
   sc_set_time_resolution(1,SC_NS);

   sc_clock clk("clk", 10, SC_NS, 0.5);
   top TOP("TOP");
   TOP.clk(clk);


   ifstream scenario("scenario.txt");

   list_elem elem;

   string line;
   char* ptr;
   char* char_buffer;

   if(scenario.is_open()){
	while( getline(scenario, line) ){
		char_buffer = new char[line.length()];
		strcpy(char_buffer, line.c_str());

		ptr=strtok(char_buffer," ");

		elem.src_id = atoi(ptr);	// source node id

		ptr=strtok(NULL," ");

		elem.dest_id = atoi(ptr);	// destination node id

		ptr=strtok(NULL," ");

		int temp = atoi(ptr);
		if( temp > 0 ){
			elem.tx_cycle = temp;
			elem.tx_src = new int[temp];
			elem.tx_dest = new int[temp];

			for(int i=0; i<temp; i++){
				ptr=strtok(NULL," ");
				elem.tx_src[i] = atoi(ptr);
				ptr=strtok(NULL," ");
				elem.tx_dest[i] = atoi(ptr);
			}
			ptr=strtok(NULL," ");
			if( ptr == NULL ){
				elem.rx_cycle = 0;
				goto end;
			}
			temp = atoi(ptr);
		}
		else if( temp < 0 ) elem.tx_cycle = 0;

		elem.rx_cycle = abs(temp);
		elem.rx_src = new int[elem.rx_cycle];

		for(int i=0; i<elem.rx_cycle; i++){
			ptr = strtok(NULL," ");
			elem.rx_src[i] = atoi(ptr);
		}

	end:
		my_scenario.push_back(elem);

		delete char_buffer;
	}

	if(my_scenario.empty()) assert(false);

	scenario.close();
   }

   ifstream data("data.txt");


   int NODE_TABLE[NUM_ROUTER][NUM_CORE] = {
	{1, 2},
   	{2, 1}
   };

   int PORT_TABLE[NUM_ROUTER][MAX_PORT] = {
		{ -1, 2},
		{ -2, 1}
   };

   memcpy( NODE_INFO, NODE_TABLE, sizeof(NODE_INFO));
   memcpy( PORT_INFO, PORT_TABLE, sizeof(PORT_INFO));


   sc_start(10*EXCUTION_CYCLE+3000,SC_NS);

//   assert(false);
   printf("************************ simulation complete ************************\n");
   printf("Excution cycle : %d\n", EXCUTION_CYCLE);
   printf("Number of Core : %d\n", NUM_CORE);
   printf("Number of Router : %d\n", NUM_ROUTER);
   printf("Average latency : %lf cycle\n", average_delay);
   printf("*********************************************************************\n");

   my_scenario.clear();
   
   return 0; 
}
