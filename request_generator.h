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

class req_generator : public sc_module
{ 
public:
   //ports:
   sc_in_clk clk;
   sc_in<bool> free[REQ_MODULES];

   sc_out<bool> done[REQ_MODULES];
   sc_out<bool> valid_req[REQ_MODULES];
   sc_port<sc_signal_out_if<Req_t> > req_out[REQ_MODULES];

   //constructor:
   req_generator(sc_module_name req_gen); 
   //methods,threads:
   void req_spawner();
   void spawned_thread(unsigned file_id);
};
#endif
