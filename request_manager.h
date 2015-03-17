#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <systemc>
#include "length_config.h"
#include "request_generator.h"
#include "parser_arbiter.h"

using namespace sc_core;
using namespace sc_dt;

//This basically has the fifo that communicates the request generator with its reader
class request_manager : public sc_module
{ 
public:
   //##ports:
   sc_in<bool> clk;                                    //The clock ... obviously!
   
   //##signals
     //requests
   sc_signal<bool>  req_valid_signal[REQ_MODULES];        //valid signal for req generators and the parsers arbiter
   sc_signal<bool>  req_done_signal[REQ_MODULES];         //done signal for req generators and the parsers arbiter
   sc_signal<bool>  req_free_signal[REQ_MODULES];         //free signal for req generators and the parsers arbiter
   sc_signal<Req_t> req_data_signal[REQ_MODULES];         //data signal for req generators and the parsers arbiter

   //##local modules
   req_generator req_gen;                    
   parser_arbiter parse_arbiter;

   //##constructor:
   request_manager( sc_module_name req_m ); 
   //##methods,threads:
   void manager();
};
#endif
