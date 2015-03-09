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
   cout << endl
        << "\tINFO: spawned_thread "
        << sc_get_current_process_handle().name()
        << "\t @ " << sc_time_stamp() << endl << endl;
   wait(10,SC_NS); 
   cout << "\tINFO: Exiting" << endl;
}
void modulo_pato::quack( )
{
   char nombre[8];
   while(true){
   ++var;
   sprintf(nombre,"patito%d",var);
   wait(n,SC_NS);
   sc_process_handle h = sc_spawn(sc_bind(&modulo_pato::spawned_thread,this),nombre);
   std::cout << "Waiting ..." << std::endl;
   wait(h.terminated_event());
   std::cout << "Done" << std::endl;
   std::cout << "The thread example getting executed in the module instance: "
             << name() << std::endl;
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   std::cout << "Var is: " << var << std::endl;
   }
}

int sc_main(int argc, char *argv[])
{
   sc_set_time_resolution( 1.0, SC_NS );
   const sc_time t_PERIOD (18,SC_NS) ;
   sc_clock clk ("clk", t_PERIOD);   
   modulo_pato pato_cafe("InstancePato",5);
   modulo_pato pato_cafe1("InstancePato1",3);
   pato_cafe(clk);
   pato_cafe1(clk);
   sc_start(25,SC_NS);
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   return 0;
}
