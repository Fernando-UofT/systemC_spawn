#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "modulo_pato.h"

//   modulo_pato: the class name, like "adder"
//   duck:        string name of a module to register module in simulation kernel
//   quack:       the process executed by the module, like "perform addition"
//   pato_cafe:   module instance
//   instancePato: instance name of pato_cafe module
//
modulo_pato::modulo_pato(sc_module_name duck, int N=2, int Var)
: sc_module(duck), n(N), var(Var)
{
   SC_HAS_PROCESS(modulo_pato); 
   SC_THREAD(quack);
   sensitive << iclk.neg();
}
void modulo_pato::spawned_thread() // This will be spawned 
{
   std::random_device rd;     // only used once to initialise engine
   std::mt19937 rng(rd());      // random-number engine used
   std::uniform_int_distribution<> uni(1,5); // guaranteed unbiased

   auto number = uni(rng);

   std::cout << "Cycles: " << number << std::endl;

   for(unsigned i=0;i<number;++i)
   {
      wait(iclk->posedge_event());
      cout << endl
           << "\tINFO: spawned_thread "
           << sc_get_current_process_handle().name()
           << "\t @ " << sc_time_stamp() << endl << endl;
   }
}
void modulo_pato::quack( )
{
   char nombre[8];
   unsigned num_procs = 3;
   sc_process_handle h[num_procs];
   std::cout << "Executing in the module instance: "
             << name() << std::endl << std::endl;

   wait(iclk->posedge_event());
   for(unsigned i=0;i<num_procs;++i)
   {
      sprintf(nombre,"patito%d",i);
      

      std::cout << "Spawning thread " << i << " ..." << std::endl;
      h[i] = sc_spawn(sc_bind(&modulo_pato::spawned_thread,this),nombre);
      //wait(h.terminated_event());
   }
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   //for(unsigned i=0;i<num_procs;++i)
   std::cout << "Waiting for threads to finish ... " << std::endl;
   
   do
   {
      bool any_running = false;
      wait(iclk->posedge_event());
      for(unsigned i=0;i<num_procs;++i)
      {
         if ( !h[i].terminated() )
            any_running = true;
      }
      if ( !any_running )
         break;

      //wait(h[0].terminated_event() & h[1].terminated_event() & h[2].terminated_event());
   }
   while(true);
   std::cout << "Done" << std::endl;
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   sc_stop();
}

int sc_main(int argc, char *argv[])
{
   sc_set_time_resolution( 1.0, SC_NS );
   const sc_time t_PERIOD (18,SC_NS) ;
   sc_clock clk ("clk", t_PERIOD);   
   modulo_pato pato_cafe("InstancePato",5);
   //modulo_pato pato_cafe1("InstancePato1",3);
   pato_cafe(clk);
   //pato_cafe1(clk);
   sc_start();
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   return 0;
}
