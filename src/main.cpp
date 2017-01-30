#include "Opanalysis.hpp"
#include "Inputfile.hpp"
#include <iostream>
#include <vector>
#include <thread>

int main(){
   // max traces : 12500
   // max samples : 5918
   unsigned ptx = 0;

   std::string fn = "/Users/jacopo/Dropbox/University-ComputerEngineering/Specialistica/Cryptography/Project/jdisimone/aes_test_traces.dat";
   Inputfile aes(fn.c_str());

   std::shared_ptr<TracesMatrix> traces_matrix = std::make_shared<TracesMatrix>(aes.getNumTraces(), aes.getNumSamplesPerTrace());
   std::shared_ptr<DataMatrix> data_matrix = std::make_shared<DataMatrix>(aes.getNumTraces(), aes.getNumPtx());
   std::shared_ptr<TracesMatrix> power_model_key = std::make_shared<TracesMatrix>(aes.getNumTraces(),KEY_SIZE);
   std::shared_ptr<TracesMatrix> power_model_sbox = std::make_shared<TracesMatrix>(aes.getNumTraces(),KEY_SIZE);

   for (size_t curtrace = 0; curtrace < aes.getNumTraces(); curtrace++)
   {
      aes.readSamples(traces_matrix, curtrace, 0, aes.getNumSamplesPerTrace());
      aes.readPtx(data_matrix, curtrace);
   }

   Opanalysis::aesModelHW(power_model_key, data_matrix, aes.getNumTraces(), ptx, 'K');
   Opanalysis::aesModelHW(power_model_sbox, data_matrix, aes.getNumTraces(), ptx, 'S');

   int nt [] = { 12500};
   int nsam [] = {1200};
   char noise [] = {'G'};
   char output [] = {'S'};
   double pel_coeff [] = {2, 4};
   double plat_noise [] = {1, 3, 6};

   for (int numt:nt)
   {
      for (int numsam: nsam)
      {
         for (char ch:noise)
         {
            for (char o:output )
            {
               for (double pc:pel_coeff)
               {
                  for (double pn:plat_noise)
                  {
                     if (o == 'K')
                     {
                        Opanalysis::DoMUnknownmodel(traces_matrix, power_model_key, 0, numsam, numt, o, pc, pn);
                     } else
                     {
                        Opanalysis::DoMUnknownmodel(traces_matrix, power_model_sbox, 0, numsam, numt, o, pc, pn);
                     }

                  }
               }
            }
         }
      }
   }

}
