#include "Opanalysis.hpp"
#include "Inputfile.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>

int main(int argc, char* argv[]){
   // max traces : 12500
   // max samples : 5918
   unsigned ptx = 0;

   std::string fn = "/Users/jacopo/Dropbox/University-ComputerEngineering/Specialistica/Cryptography/Project/jdisimone/aes_test_traces.dat";
   Inputfile aes(fn.c_str());

   std::shared_ptr<TracesMatrix> traces_matrix = std::make_shared<TracesMatrix>(aes.getNumTraces(), aes.getNumSamplesPerTrace());
   std::shared_ptr<DataMatrix> data_matrix = std::make_shared<DataMatrix>(aes.getNumTraces(), aes.getNumPtx());
   std::shared_ptr<TracesMatrix> power_model_sbox = std::make_shared<TracesMatrix>(aes.getNumTraces(),KEY_SIZE);

   for (size_t curtrace = 0; curtrace < aes.getNumTraces(); curtrace++)
   {
      aes.readSamples(traces_matrix, curtrace, 0, aes.getNumSamplesPerTrace());
      aes.readPtx(data_matrix, curtrace);
   }

   Opanalysis::aesModelHW(power_model_sbox, data_matrix, aes.getNumTraces(), ptx, 'S');

   int numt = 5500;
   int numsam = 600;
   char noise = 'G';
   char output = 'S';

   Opanalysis::optimalExpression(traces_matrix, power_model_sbox, 0, numsam, numt, noise, output);

   for (int i=1; i+1<argc; i++)
   {
      Opanalysis::DoMUnknownmodel(traces_matrix, power_model_sbox, 0, numsam, numt, output, /*pelgromcoeff*/ atof(argv[i]), /*platnoise*/ atof(argv[i+1]));
   }
   
}
