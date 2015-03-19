#ifndef LENGTH_CONFIG_H
#define LENGTH_CONFIG_H
//
//initial values and other constraints
//
#define INPUT_FILE  "files/input_big.dat"  //requests source
#define LENGTH                         64  //bytes
#define LINE_SIZE                       8  //bytes
#define MEM_SIZE                536870912  //number of words
#define MODULES                         1  //number of modules sharing memory allocator
#define REQ_MODULES                     3  //number of modules for requests generation
#define PARSE_MODULES                   1  //number of modules to parse requests
#define MEM_MODULES                     2  //number of modules sharing memory allocator
#define KEY_SIZE                      256  //number of bits for key
#define MEM_DELAY                       1  //number of modules sharing memory allocator
#define PERIOD                        500  //period of the clock (in the units specified by UNITS)
#define UNITS                       SC_PS  //number of modules sharing memory allocator


#define FIND_D                          PERIOD*181
#define GETL_D                          PERIOD*370
#define SUBSTR_D                        PERIOD*243
#define RESIZE_D                        PERIOD*356
#define FREE_D                          PERIOD*33
#define MEMCPY_D                        PERIOD*17
#define COPY_D                          PERIOD*34

#define NEW_D                           PERIOD*200
#define ALLOC_D                         PERIOD*186

#define INSERT_D                        PERIOD*5082
#define SEEK_D                          PERIOD*15122

/*
#define FIND_D                          PERIOD
#define GETL_D                          PERIOD
#define SUBSTR_D                        PERIOD
#define RESIZE_D                        PERIOD
#define FREE_D                          PERIOD
#define MEMCPY_D                        PERIOD
#define COPY_D                          PERIOD

#define INSERT_D                        PERIOD
#define SEEK_D                          PERIOD
*/

#define FIFO_DEPTH                   8192

typedef uint64_t Datum;
typedef uint8_t  Req_t;
typedef uint16_t s_type;
#endif
