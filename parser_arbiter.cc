#include "parser_arbiter.h"
//#include "config.h"
#include <systemc>
#include <cstdlib>

using namespace sc_core;
using namespace sc_dt;

parser_arbiter::parser_arbiter( sc_module_name parse_arbiter )
: sc_module( parse_arbiter )
{
   SC_HAS_PROCESS( parser_arbiter ); 
   SC_THREAD( arbitrate );
      sensitive << clk.pos();
      for (unsigned i=0; i<REQ_MODULES; ++i)
      {
         sensitive << valid_req[i];
         sensitive << done[i];
         sensitive << req_in[i];
         sensitive << free[i];
      }
}

void parser_arbiter::arbitrate( )            //this thread will check the contents of the memory, comparing each key with the 
{                                              //requested one
   bool valid_get;

   bool done_get;
   
   Req_t request_get;
   
   int locked = 1;                                         //unlocked                                                                                                        
   int i = -1;
   unsigned free_module[REQ_MODULES] = -1;                 //flags to know if a parser is available
   unsigned module;                                        //req module under analysis
   bool attending[REQ_MODULES];                            //is any request being served? Flags


   for ( module = 0; module < REQ_MODULES; ++module )   //initialization
   {
      free[module]->write(0);
      attending[module] = false;
      free_module[module] = -1;
   }
   while ( true )
   {
      ++i;
      for ( module = 0; module < REQ_MODULES; ++module )  //check all req modules
      {
         if ( attending[module] )    //if we a;ready got a valid, lets see if we are done, an if not, lets read the request
         {
            free[module]->write(0);
            done_get = done[module]->read();
            if ( done_get == 0 )                      //if request is still being sent ...
            {
               request_get = req_in->read();
               std::cout << module << ":" << free_module << " " << request_get << std::endl; //will change to a write to one of the parsers
            }
            else                                      //if we are done, free mutex, clear
            {
               sel_mutex[module].unlock( );
               attending[module] = false;
               free_module[module] = -1;
            }
         }
         valid_get = valid_req[module]->read( );

         if ( valid_get == 1 )                        //if a req generator has a new request
         {
            for ( unsigned out_module = 0; out_module < PARSE_MODULES; ++out_module ) //check if any parser is available
            {
               locked = sel_mutex[out_module].trylock( );        //try to lock a mutex
               if ( locked  == 0 )                               //if success
               {
                  free_module[module] = out_module;                 //take note of the parser we'll use
                  attending[module] = true;                         //we'll attend a request
                  break;
               }
            }
            if ( free_module[module] > -1 )                      //if we found an available parser, tell the req generator
            {
               free[module]->write(1);
            }
         }
      }
      wait( );    //don't monopolize, you bastard
         
   } //while
}
