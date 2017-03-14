#include "Inputfile.hpp"
#include <fcntl.h>
#include <sys/mman.h>

Inputfile::Inputfile(const char * filename)
{
   int byte_read;
   inputfd = open (filename, O_RDONLY);
   if ( inputfd == -1 )
   {
   	std::cout << "Cannot open" << filename << std::endl;
   	exit ( 1 );
   }
   byte_read = read(inputfd, (void *) &header.numtraces , sizeof(header.numtraces) );
   byte_read = read(inputfd, (void *) &header.numsamples_per_trace , sizeof(header.numsamples_per_trace) );
   byte_read = read(inputfd, (void *) &header.datatype , sizeof(header.datatype) );
   byte_read = read(inputfd, (void *) &header.knowndatalength , sizeof(header.knowndatalength) );
   RealFileSize = lseek ( inputfd, 0, SEEK_END );
   fileoffset = mmap ( NULL, RealFileSize, PROT_READ, MAP_SHARED, inputfd, 0 );
   if ( fileoffset == MAP_FAILED )
   {
   	std::cout << "Cannot memory map input file. Cannot continue" << '\n';
   	exit ( 3 );
   }
   switch(header.datatype)
   {
      case 'f': fl = true; break;
      case 'd': fl = false; break;
   }
}

void Inputfile::readSamples ( std::shared_ptr<TracesMatrix>& traces, unsigned long curtrace, unsigned long startingsample, unsigned long numsamples )
{
   if (numsamples + startingsample > header.numsamples_per_trace)
   {
      std::cout << "error, wrong number of samples\n";
      return;
   }
   if (curtrace > header.numtraces)
   {
      std::cout << "error, wrong trace\n";
      return;
   }
   if (fl)
   {
   	float* buffer;
   	buffer = ( float* ) ( ( char* ) fileoffset + getSampleOffset ( curtrace, startingsample ) );
   	for ( unsigned long i = 0; i < numsamples; i++ ) {
   		( *traces ) ( curtrace, i ) = (ANALYSIS_TYPE) buffer[i];
   	}
   } else 
   {
      double* buffer;
      buffer = ( double* ) ( ( char* ) fileoffset + getSampleOffset ( curtrace, startingsample ) );
      for ( unsigned long i = 0; i < numsamples; i++ ) {
         ( *traces ) ( curtrace, i ) = (ANALYSIS_TYPE) buffer[i];
      }
   }
}

void Inputfile::readPtx ( std::shared_ptr<DataMatrix>& data_matrix, unsigned long curtrace)
{
	uint8_t* buffer;
	buffer = ( uint8_t* ) ( ( char* ) fileoffset + getDataOffset (curtrace) );
   for ( unsigned long i = 0; i < (*data_matrix).cols(); i++ ) {
      DataValueType bar (buffer[i]);
		( *data_matrix ) ( curtrace, i ) = bar;
	}
}

unsigned long Inputfile::getNumSamplesPerTrace(){return header.numsamples_per_trace;}
unsigned long Inputfile::getNumTraces(){return header.numtraces;}
unsigned long Inputfile::getNumPtx(){return header.knowndatalength;}


Inputfile::~Inputfile()
{
   munmap ( fileoffset, RealFileSize );
	close ( inputfd );
}
