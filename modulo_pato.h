#include <systemc.h>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

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
   void spawned_thread(unsigned file_id);
private:
   const int n;
};
