#include "Opanalysis.hpp"
#include "Inputfile.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <getopt.h> 


int main(int argc, char* argv[]){
   /*
   argv[1] -> ptx
   argv[2] -> numtraces
   argv[3] -> pelgrom
   argv[4] -> platnoise
   */
   // max traces : 12500
   // max samples : 5918
   //Check of the arguments
   unsigned int numtraces, startsample, numsamples, ptx;
   float pelgromcoeff, platnoise;
   bool known, unknown, cpa, save;
   /*
   if (argv[1] == NULL) { std::cout << "choose number of traces: "; std::cin >> argv[1];}
   while(true)
   {
      if(atof(argv[1]) < 0 || atof(argv[1]) > 12500 )
         { std::cout << "wrong chooice of number of traces (0,12500): "; std::cin >> argv[1];}
      else break;
   }
   int numtraces = atof(argv[1]);
   if (argv[2] == NULL) { std::cout << "choose num samples: "; std::cin >> argv[2];}
   while(true)
   {
      if(atof(argv[2]) < 0 || atof(argv[2]) > 5918 )
         { std::cout << "wrong chooice of number of samples (0,5918): "; std::cin >> argv[2];}
      else break; 
   }*/
   //HANDLE OPTIONS

   std::string fn = "/Users/jacopo/Dropbox/University-ComputerEngineering/Specialistica/Cryptography/Project/jdisimone/aes_test_traces.dat";
   Inputfile aes(fn.c_str());

   numtraces = aes.getNumTraces();
   startsample = 0;
   numsamples = aes.getNumSamplesPerTrace();
   ptx = 0;
   pelgromcoeff = 1;
   platnoise = 1;
   known = true;
   unknown = false;
   cpa = false;
   save = false;

   const struct option longopts[] =
   {
      {"numtraces",     required_argument,   0, 'n'   },
      {"startsample",   required_argument,   0, 's'   },
      {"numsamples",    required_argument,   0, 'm'   },
      {"ptx",           required_argument,   0, 'x'   },
      {"pelgromcoeff",  required_argument,   0, 'e'   },
      {"platnoise",     required_argument,   0, 'l'   },
      {"noknown",       no_argument,         0, 'k'   },
      {"unknown",       no_argument,         0, 'u'   },
      {"cpa",           no_argument,         0, 'c'   },
      {"save",          no_argument,         0, 'v'   },
      {0,               0,                   0,  0    },
   };
   int index;
   int iarg = 0;
   //turn off getopt error message
   opterr = 1; 
   while(iarg != -1)
   {
      iarg = getopt_long(argc, argv, "kucvn:s:m:x:e:l:", longopts, &index);
      switch (iarg)
      {
         case 'n':
            if(optarg)
            {
               while(atoi(optarg) > numtraces) {
                   std::cout << "Wrong value for number of traces, choose another one in range (1," + std::to_string(numtraces) + "): ";
                   std::cin >> optarg;
               }
               numtraces = atoi(optarg);
            }
            break;
         case 's':
            if(optarg)
            {
               while(atoi(optarg) > numtraces) {
                   std::cout << "Wrong value for number of traces, choose another one in range (1," + std::to_string(numtraces) + "): ";
                   std::cin >> optarg;
               }
               startsample = atof(optarg);
            }
            break;
         case 'm':
            if(optarg)
               numsamples = atof(optarg);
            break;
         case 'x':
            if(optarg)
               ptx = atof(optarg);
            break;
         case 'e':
            if(optarg)
               pelgromcoeff = atof(optarg);
            break;
         case 'l':
            if(optarg)
               platnoise = atof(optarg);
            break;
         case 'k':
            known = false;
            break;
         case 'u':
            unknown = true;
            break;
         case 'c':
            cpa = true;
            break;
         case 'v':
            save = true;
            break;
      }
   }
   while(numsamples < startsample || (startsample + numsamples) >= aes.getNumSamplesPerTrace() ) 
   {
       std::cout << "Wrong value for startsample and numsamples.\n Choose another start in range (0, " + std::to_string(numsamples - 1) + "): ";
       std::cin >> startsample;
       std::cout << "\n Choose numsamples in range (0, " + std::to_string(numsamples - startsample) + "): ";
   }

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
   if (known) Opanalysis::DoMKnownmodel(traces_matrix, power_model_sbox, startsample, numsamples, numtraces, 'G', 'S', ptx, save);
   if (unknown) Opanalysis::DoMUnknownmodel(traces_matrix, power_model_sbox, startsample, numsamples, numtraces, 'S', pelgromcoeff, platnoise, ptx, save);
   if (cpa) Opanalysis::CPA(traces_matrix, power_model_sbox, startsample, numsamples, numtraces, 'S', ptx, save);

}
