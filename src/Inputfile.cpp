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

void Inputfile::readSamples ( std::shared_ptr<TracesMatrix>& traces, unsigned long it, unsigned long trace, unsigned long startingsample, unsigned long numsamples )
{
   if (fl)
   {
   	float* buffer;
   	buffer = ( float* ) ( ( char* ) fileoffset + getSampleOffset ( trace, startingsample ) );
   	for ( unsigned long i = 0; i < numsamples; i++ ) {
   		( *traces ) ( it, i ) = (ANALYSIS_TYPE) buffer[i];
   	}
   } else 
   {
      double* buffer;
      buffer = ( double* ) ( ( char* ) fileoffset + getSampleOffset ( trace, startingsample ) );
      for ( unsigned long i = 0; i < numsamples; i++ ) {
         ( *traces ) ( it, i ) = (ANALYSIS_TYPE) buffer[i];
      }
   }
}

void Inputfile::readPtx ( std::shared_ptr<DataMatrix>& data_matrix, unsigned long it, unsigned long trace)
{
	DataValueType* buffer;
   buffer = (DataValueType*) fileoffset + getDataOffset (trace);
   for ( unsigned long i = 0; i < (*data_matrix).cols(); i++ ) {
      ( *data_matrix ) ( it, i ) = buffer[i];
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
