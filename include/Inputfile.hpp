#ifndef INPUTFILE
#define INPUTFILE

#include "Type.hpp"

struct fileheaders {
   uint32_t numtraces;
   uint32_t numsamples_per_trace;
   char datatype;
   uint8_t knowndatalength;
};

class Inputfile{
   int inputfd;
   fileheaders header;
   long long RealFileSize;
   void* fileoffset;
   bool fl; //true if the trace is saved in float, false if it is save in double

public:
   /* Inputfile constructor: create the object initializing private variables
    * and mapping fileoffset.
    */
   Inputfile(const char * filename);
   /* read some samples present in the file and save them in a TracesMatrix passed
    * as parameter, in the row traces_row.
    */
   void readSamples ( std::shared_ptr<TracesMatrix> traces, unsigned long traces_row, unsigned long trace, unsigned long startingsample, unsigned long numsamples );
   /* read the data of a given trace and save it in data_matrix in the row specified
    * by parameter data_row
    */
   void readPtx ( std::shared_ptr<DataMatrix> data_matrix, unsigned long data_row, unsigned long trace );
   unsigned long getNumSamplesPerTrace();
   unsigned long getNumTraces();
   unsigned long getNumPtx();

   ~Inputfile();

protected:
   unsigned long long getSampleOffset ( unsigned long trace, unsigned long samplenum ) {
      return HEADER_SIZE + trace * ( SAMPLE_SIZE * header.numsamples_per_trace + 16) + SAMPLE_SIZE * samplenum;
   }
   unsigned long long getDataOffset ( unsigned long trace ) {
      return HEADER_SIZE + trace * ( SAMPLE_SIZE * header.numsamples_per_trace + 16) + SAMPLE_SIZE * header.numsamples_per_trace;
   }
};

#endif //INPUTFILE