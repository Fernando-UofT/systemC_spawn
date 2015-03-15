#ifndef REQ_GENERATOR_H
#define REQ_GENERATOR_H

#include <systemc.h>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

#include "length_config.h"

using namespace sc_core;
using namespace sc_dt;

class modulo_pato : public sc_module
{ 
public:
   //ports:
   sc_in_clk clk;
   sc_out<bool> done[REQ_MODULES];
   sc_inout<bool> free[REQ_MODULES];
   sc_out<bool> valid_req[REQ_MODULES];
   sc_port<sc_signal_out_if<REQ_W> > req_out[REQ_MODULES];

   int var;
   //constructor:
   modulo_pato(sc_module_name duck, int n, int var=5); 
   //methods,threads:
   void quack();
   void spawned_thread(unsigned file_id);
private:
   const int n;
};
#endif
