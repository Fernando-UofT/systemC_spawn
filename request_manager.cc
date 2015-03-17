#include <systemc>
#include "request_manager.h"

using namespace sc_core;

request_manager::request_manager(sc_module_name req_m)
: sc_module(req_m), req_gen("req_gen"), 
                    parse_arbiter("parse_arbiter")
{
   //##connect
/****************************************************************************/

   req_gen.clk(clk);                                   //connect clock to request generator
   for ( unsigned i = 0; i < REQ_MODULES; ++i )
   {
      req_gen.free[i](req_free_signal[i]);                //connect free from parser arbiter to req gen
      req_gen.done[i](req_done_signal[i]);              //connect clock to request generator
      req_gen.valid_req[i](req_valid_signal[i]);          //connect clock to request generator
      req_gen.req_out[i](req_data_signal[i]);             //connect clock to request generator
   }

/****************************************************************************/

   for ( unsigned i = 0; i < REQ_MODULES; ++i )
   {
      parse_arbiter.free[i](req_free_signal[i]);              //connect free from parser arbiter to req gen
      parse_arbiter.done[i](req_done_signal[i]);              //connect clock to request generator
      parse_arbiter.valid_req[i](req_valid_signal[i]);        //connect clock to request generator
      parse_arbiter.req_in[i](req_data_signal[i]);            //connect clock to request generator
   }
/****************************************************************************/


   SC_HAS_PROCESS(request_manager); 
   SC_METHOD(manager);
   sensitive << clk.pos();                             //make process sensitive to start of cycle

}
//sc_fifo<int> queue_request;
void request_manager::manager( )
{
   ////std::cout << "This is the main module of the system" << std::endl;  //really useless
}
