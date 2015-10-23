#ifndef DEFINE_H
#define DEFINE_H

#include <list>

#ifndef EXCUTION_CYCLE
#define EXCUTION_CYCLE 50000
#endif

#ifndef NUM_ROUTER
#define NUM_ROUTER 2
#endif

#ifndef NUM_CORE
#define NUM_CORE 2
#endif
//
#ifndef NODE1_ADDR
#define NODE1_ADDR "00000000000000000000000000001111"
#endif

#ifndef NODE2_ADDR
#define NODE2_ADDR "00000000000000000000000011110000"
#endif  

#ifndef NODE3_ADDR
#define NODE3_ADDR "00000000000000000000111100000000"
#endif  

#ifndef NODE4_ADDR
#define NODE4_ADDR "00000000000000001111000000000000"
#endif  

#ifndef NODE5_ADDR
#define NODE5_ADDR "00000000000011110000000000000000"
#endif  

#ifndef NODE6_ADDR
#define NODE6_ADDR "00000000111100000000000000000000"
#endif  

#ifndef NODE7_ADDR
#define NODE7_ADDR "00001111000000000000000000000000"
#endif  
//
#ifndef ADDR_SIZE
#define ADDR_SIZE 3
#endif

#ifndef TYPE_SIZE
#define TYPE_SIZE 2
#endif

#ifndef MAX_PORT
#define MAX_PORT 2
#endif

#ifndef DATA_BUS_SIZE
#define DATA_BUS_SIZE 32
#endif

#ifndef ADDR_BUS_SIZE
#define ADDR_BUS_SIZE 32
#endif

#ifndef EMPTY_BUS
#define EMPTY_BUS "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"
#endif

//std::list<list_elem> my_scenario;
//extern list<list_elem>::iterator itor=my_scenario.begin();

struct list_elem{
	int src_id;
	int dest_id;

	int tx_cycle;
	int *tx_src;
	int *tx_dest;

	int rx_cycle;
	int *rx_src;
};

struct data_elem{
	int core_id;
	long data[3];
}

//std::list<list_elem> my_scenario;

#ifndef Release
#define Release "00"
#endif

#ifndef Continue
#define Continue "01"
#endif

#ifndef Fail
#define Fail "10"
#endif

#ifndef Grant
#define Grant "11"
#endif

#endif
