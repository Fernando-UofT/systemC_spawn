#include <systemc.h>
#include <string>
#include <random>

class modulo_pato : public sc_module
{ 
public:
   //ports:
   sc_in_clk iclk;
   int var;
   //constructor:
   modulo_pato(sc_module_name duck, int n, int var=5); 
   //methods,threads:
   void quack();
   void spawned_thread();
private:
   const int n;
};
