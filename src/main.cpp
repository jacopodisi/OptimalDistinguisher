#include "Opanalysis.hpp"
#include "Inputfile.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>


int main(int argc, char* argv[]){
   /*
   argv[1] -> ptx
   argv[2] -> numtraces
   argv[3] -> pelgrom
   argv[4] -> platnoise
   */
   // max traces : 12500
   // max samples : 5918
   int max_samples = 5918;
   unsigned ptx = atof(argv[2]);

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
   
   std::string opt = argv[1];
   if (opt.find('K') != std::string::npos) Opanalysis::DoMKnownmodel(traces_matrix, power_model_sbox, 0, max_samples, atof(argv[3]), 'G', 'S', ptx);
   if (opt.find('U') != std::string::npos) Opanalysis::DoMUnknownmodel(traces_matrix, power_model_sbox, 0, max_samples, atof(argv[3]), 'S', /*pelgromcoeff*/ atof(argv[4]), /*platnoise*/ atof(argv[5]), ptx);
   if (opt.find('C') != std::string::npos) Opanalysis::CPA(traces_matrix, power_model_sbox, 0, max_samples, atof(argv[3]), 'S', ptx);

}
