#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "request_generator.h"

//   modulo_pato: the class name, like "adder"
//   duck:        string name of a module to register module in simulation kernel
//   quack:       the process executed by the module, like "perform addition"
//   pato_cafe:   module instance
//   instancePato: instance name of pato_cafe module
//
req_generator::req_generator(sc_module_name req_gen)
: sc_module(req_gen)
{
   SC_HAS_PROCESS(req_generator); 
   SC_THREAD(req_spawner);
      sensitive << clk.pos();
}
void req_generator::spawned_thread(unsigned file_id) // This will be spawned 
{
   std::ifstream requests_source;
   std::stringstream str_build;
   bool read_free = false;
   std::string request;

   str_build << "files/input" << file_id << ".dat";
   std::string file = str_build.str();
   str_build.str( std::string(  ) );

   bool read_line = true;
   bool check_if_free = false;
   bool streaming = false;

   unsigned i = 0;

   requests_source.open (file, std::ios::in | std::ios::binary);
   if (!requests_source.is_open()) 
   {
       std::cout << "Input file couldn't be openned" << std::endl; 
       sc_stop();
   }


   while( true )
   {
      wait(clk->posedge_event());
      done[file_id]->write(0);

      if ( read_line )
      {
         std::cout << "Reading line" << std::endl;
         if ( !std::getline( requests_source,request ) )
            break;
         i = 0;
         read_line = false;
         check_if_free = true;
      }
      else if ( check_if_free )
      {
         std::cout << file_id << " : Checking if free" << std::endl;
         read_free = free[file_id]->read();
         if ( read_free == 1 )
         {
            valid_req[file_id]->write(1);

            check_if_free = false;
            streaming = true;
         }
      }
      else if ( streaming )
      {
         if ( i == 0 )
            valid_req[file_id]->write(0);
         std::cout << "Streaming" << std::endl;
         if ( i < request.size() )
         {
            std::cout << "# " << i << ":" << request.at(i) << " #" << std::endl;
            req_out[file_id]->write(request.at(i));
            ++i;
         }
         else
         {
            done[file_id]->write(1);
            streaming = false;
            read_line = true;
         }
      }
   }
   requests_source.close();
}

void req_generator::req_spawner( )
{
   char nombre[8];
   unsigned num_procs = 3;
   sc_process_handle h[num_procs];
   std::cout << "Executing in the module instance: "
             << name() << std::endl << std::endl;

   wait(clk->posedge_event());
   for(unsigned i=0; i<REQ_MODULES; ++i)
   {
      sprintf(nombre,"patito%d",i);

      std::cout << "Spawning thread " << i << " ..." << std::endl;
      h[i] = sc_spawn(sc_bind(&req_generator::spawned_thread,this,i),nombre);
      //wait(h.terminated_event());
   }
   std::cout << "Time is: " << sc_core::sc_time_stamp() << std::endl;
   //for(unsigned i=0;i<num_procs;++i)
   std::cout << "Waiting for threads to finish ... " << std::endl;
   
   do
   {
      bool any_running = false;
      std::cout << "==========================================================================" << std::endl;
      wait(clk->posedge_event());
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
