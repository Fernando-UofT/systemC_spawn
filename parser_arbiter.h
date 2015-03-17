#ifndef PARSER_ARBITER_H
#define PARSER_ARBITER_H

#include <systemc>
#include <bitset>
#include "length_config.h"

using namespace sc_core;
using namespace sc_dt;

class parser_arbiter : public sc_module
{ 
public:
   //ports:
   sc_in<bool> valid_req[REQ_MODULES];
   sc_in<bool> done[REQ_MODULES];
   sc_port<sc_signal_in_if<Req_t> > req_in[REQ_MODULES];

   sc_out<bool> free[REQ_MODULES];

   //mutex
   sc_mutex sel_mutex[PARSE_MODULES];
   //constructor:
   parser_arbiter(sc_module_name mem_arbiter); 
   //methods,threads:
   void arbitrate();
};
#endif
