#ifndef RESET_H
#define RESET_H

#include <systemc>
#include <iostream>

using namespace sc_core;
using namespace std;

SC_MODULE(reset) {

   sc_out<bool> reset_inner;

   SC_CTOR(reset){
      SC_THREAD(reset_handle);
   }

   void reset_handle() 
   {
      reset_inner.write(true);
      wait(40,SC_NS);
      reset_inner.write(false);
      wait(40,SC_NS);
   }
};
#endif
