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
   unsigned long NumSamplesPerTrace;
	unsigned long NumTraces;
	unsigned long NumPtx;

public:
   /* apre il file "filename" e salva:
    * RealFileSize
    * fileoffset
    * SamplesPerTrace
    * NumTraces
    */
   Inputfile(const char * filename);
   //void closefile();
   /* legge tutti i ptx e li salva in data che è un puntatore a un vettore
    * di DATA_SIZE_BIT (DataValueType)
    */
   std::shared_ptr<DataMatrix> readData();
   void readSamples ( std::shared_ptr<TracesMatrix>& traces, unsigned long curtrace, unsigned long startingsample, unsigned long numsamples );
   void readPtx ( std::shared_ptr<DataMatrix>& data_matrix, unsigned long curtrace);
	unsigned long getNumSamplesPerTrace();
	unsigned long getNumTraces();
	unsigned long getNumPtx();

   ~Inputfile();

protected:
   unsigned long long getSampleOffset ( unsigned long trace, unsigned long samplenum ) {
       return HEADER_SIZE + trace * ( SAMPLE_SIZE * NumSamplesPerTrace + DATA_SIZE_BYTE * 16) + SAMPLE_SIZE * samplenum;
   }
   unsigned long long getDataOffset ( unsigned long trace ) {
       return HEADER_SIZE + trace * ( SAMPLE_SIZE * NumSamplesPerTrace + DATA_SIZE_BYTE * 16) + SAMPLE_SIZE * NumSamplesPerTrace;
   }

};
