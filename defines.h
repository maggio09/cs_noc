#ifndef DEFINE_H
#define DEFINE_H

#ifndef EXCUTION_CYCLE
#define EXCUTION_CYCLE 50000
#endif

#ifndef NUM_ROUTER
#define NUM_ROUTER 5
#endif

#ifndef NUM_CORE
#define NUM_CORE 7
#endif
//
#ifndef NODE1_ADDR
#define NODE1_ADDR 0x0000000f
#endif

#ifndef NODE2_ADDR
#define NODE2_ADDR 0x000000f0
#endif  

#ifndef NODE3_ADDR
#define NODE3_ADDR 0x00000f00
#endif  

#ifndef NODE4_ADDR
#define NODE4_ADDR 0x0000f000
#endif  

#ifndef NODE5_ADDR
#define NODE5_ADDR 0x000f0000
#endif  

#ifndef NODE6_ADDR
#define NODE6_ADDR 0x00f00000
#endif  

#ifndef NODE7_ADDR
#define NODE7_ADDR 0x0f000000
#endif  
//
#ifndef ADDR_SIZE
#define ADDR_SIZE 3
#endif

#ifndef TYPE_SIZE
#define TYPE_SIZE 2
#endif

#ifndef MAX_PORT
#define MAX_PORT 6
#endif

#ifndef DATA_BUS_SIZE
#define DATA_BUS_SIZE 64
#endif

#ifndef ADDR_BUS_SIZE
#define ADDR_BUS_SIZE 64
#endif

#ifndef EMPTY_BUS
#define EMPTY_BUS 0xffffffff
#endif

int TABLE_INFO[NUM_ROUTER][NUM_CORE] ={
	{1,1,1,1,1,1,1},
	{2,2,2,2,2,2,2},
	{3,3,3,3,3,3,3},
	{4,4,4,4,4,4,4},
	{5,5,5,5,5,5,5}
};


//#ifndef INDEX_SIZE
//#define INDEX_SIZE 20
//#endif

#endif
