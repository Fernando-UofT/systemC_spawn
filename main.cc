#include "request_manager.h"
#include "length_config.h"
#include <systemc>

int sc_main(int argc, char *argv[])
{
   const sc_time t_PERIOD (18,SC_NS) ;
   sc_clock clk ("clk", t_PERIOD);

   request_manager request_manager_0("REQ_M0",5);

   request_manager_0.clk(clk);
   
   if(argc != 2)
      sc_start();
   else
      sc_start(atoi(argv[1]),UNITS);

   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   return 0;
}
