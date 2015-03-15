#include "memory_arbiter.h"
//#include "config.h"
#include <systemc>
#include <cstdlib>

using namespace sc_core;
using namespace sc_dt;

memory_arbiter::memory_arbiter( sc_module_name mem_arbiter )
: sc_module( mem_arbiter )
{
   SC_HAS_PROCESS( memory_arbiter ); 
   SC_THREAD( arbitrate );
      sensitive << enable[0];
      sensitive << enable[1];
      sensitive << enable[2];
      sensitive << enable[3];
      sensitive << mem_done;
      sensitive << dummy_done;
   //sensitive << data_in(  );
}

void memory_arbiter::arbitrate( )            //this thread will check the contents of the memory, comparing each key with the 
{                                              //requested one
   bool enable_get;

   bool rw_get;
   
   Datum address_get;
   
   Datum data_get;
   
   int locked = 1;                                         //unlocked                                                                                                        
   bool done_get;
   bool dummy_get;
   int i = -1;

   while ( true )
   {
      ++i;
      for ( unsigned module = 0; module < MEM_MODULES; ++module )
      {
         
         enable_get = enable[module]->read( );
         rw_get = rw[module]->read( );
         dummy_get = dummy->read();
         address_get = address[module]->read();
         //std::cout << i << ",Memory arbiter,check for requests," << module << ",[" << address_get << "]," << sc_time_stamp().value() << " ps" << std::endl;

         data_get = data[module]->read( );
      
         if( enable_get == 1 )
         {
            locked = sel_mutex.trylock( );
          
            if( locked  == 0 )
            {
               ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( " << module << " ) enabled at " << sc_time_stamp().value() << " ps" << std::endl;
               //std::cout << i << ",Memory arbiter," << module << ",enabled,," << sc_time_stamp().value() << " ps" << std::endl;

               ack[module]->write ( 1 );

               if ( dummy_get == 1 )
               {
                  ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( " << module << " ) dummy at " << sc_time_stamp().value() << " ps" << std::endl;
                  //std::cout << i << ",Memory arbiter," << module << ",dummy,," << sc_time_stamp().value() << " ps" << std::endl;
                  wait( MEM_DELAY * PERIOD, UNITS );
                  
                  do
                  {
                     wait( );
                     ack[module]->write( 0 );
                     ++i;
                     done_get = dummy_done->read( );
                  }
                  while( done_get !=  1 );

                  ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( " << module << " ) dummy done at " << sc_time_stamp().value() << " ps" << std::endl;
                  //std::cout << i << ",Memory arbiter," << module << ",dummy done,," << sc_time_stamp().value() << " ps" << std::endl;

                  sel_mutex.unlock( );
               }

               else
               {
                  mem_enable->write( enable_get );
                  mem_rw->write( rw_get );
                  mem_data->write( data_get );
                  mem_address->write( address_get );
                  
                  if ( rw_get == 1 )                       // read request 
                  {
                     do
                     {
                        wait( );
                        ack[module]->write( 0 );
                        ++i;
                        data_get = mem_data->read();
                        done_get = mem_done->read( );
                     }
                     while( done_get !=  1 );
                     ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( read done: " << module << " ) at " << sc_time_stamp().value() << " ps" << std::endl;
                     //std::cout << i << ",Memory arbiter," << module << ",read done,," << sc_time_stamp().value() << " ps" << std::endl;
                     data[MEM_MODULES]->write(data_get);
                     sel_mutex.unlock( );
                  }
                  else                                     // write request
                  {
                     do
                     {
                        wait( );
                        ack[module]->write( 0 );
                        ++i;
                        done_get = mem_done->read( );
                     }
                     while( done_get !=  1 );
                     ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( write done: " << module << " ) at " << sc_time_stamp().value() << " ps" << std::endl;
                     //std::cout << i << ",Memory arbiter," << module << ",write done,," << sc_time_stamp().value() << " ps" << std::endl;
                     sel_mutex.unlock( );
                  } 
                  mem_enable->write( 0 );
               } // else (dummy_get = 0) 

            } // if locked
         } // if enable
      } //for
      ////std::cout << " \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << i << " | Memory arbiter ( lets wait ) at " << sc_time_stamp().value() << " ps" << std::endl;
      //std::cout << i << ",Memory arbiter,lets wait,,," << sc_time_stamp().value() << " ps" << std::endl;
         
      wait( );
         
   } //while
}
