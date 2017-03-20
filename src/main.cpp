#include "Opanalysis.hpp"
#include "Inputfile.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <getopt.h> 
#include <thread>


int main(int argc, char* argv[]){
   srand(time(0));
   // max traces : 12500
   // max samples : 5918
   //Check of the arguments
   unsigned long numtraces; 
   unsigned startsample, numsamples, ptx;
   float pelgromcoeff, platnoise;
   bool known, unknown, cpa, save, savek, random;
   char noiseassumption, model;

   std::string fn = "../aes_test_traces.dat";
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
   savek = false;
   random = false;
   noiseassumption = 'G';
   model = 'S';
   //HANDLE OPTIONS
   const struct option longopts[] =
   {
      {"numtraces",     required_argument,   0, 'n'   },
      {"startsample",   required_argument,   0, 's'   },
      {"numsamples",    required_argument,   0, 'm'   },
      {"ptx",           required_argument,   0, 'p'   },
      {"pelgromcoeff",  required_argument,   0, 'e'   },
      {"platnoise",     required_argument,   0, 'l'   },
      {"noknown",       no_argument,         0, 'k'   },
      {"unknown",       no_argument,         0, 'u'   },
      {"cpa",           no_argument,         0, 'c'   },
      {"save",          no_argument,         0, 'v'   },
      {"savek",         no_argument,         0, 'x'   },
      {"random",        no_argument,         0, 'r'   },
      {"laplacian",     no_argument,         0, 'a'   },
      {"addroundkey",   no_argument,         0, 'o'   },
      {0,               0,                   0,  0    },
   };
   int index;
   int iarg = 0;
   //turn off getopt error message
   opterr = 1; 
   while(iarg != -1)
   {
      iarg = getopt_long(argc, argv, "rxkucovn:s:m:e:l:p:", longopts, &index);
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
         case 'p':
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
         case 'x':
            savek = true;
            break;
         case 'r':
            random = true;
            break;
         case 'a':
            noiseassumption = 'L';
            break;
         case 'o':
            model = 'K';
            break;
      }
   }
   while((startsample + numsamples) > aes.getNumSamplesPerTrace() ) 
   {
      std::cout << "Wrong value for startsample and numsamples.\n Choose another start in range (0, " + std::to_string(aes.getNumSamplesPerTrace() - 1) + "): ";
      std::cin >> startsample;
      std::cout << "Choose numsamples in range (0, " + std::to_string(aes.getNumSamplesPerTrace() - startsample) + "): ";
      std::cin >> numsamples;
   }

   std::shared_ptr<TracesMatrix> traces_matrix = std::make_shared<TracesMatrix>(numtraces, aes.getNumSamplesPerTrace());
   std::shared_ptr<DataMatrix> data_matrix = std::make_shared<DataMatrix>(numtraces, aes.getNumPtx());
   data_matrix->fill(0);
   std::shared_ptr<TracesMatrix> power_model = std::make_shared<TracesMatrix>(numtraces,KEY_SIZE);
   std::vector<bool> list_traces(aes.getNumTraces(), false);
   for (unsigned long i = 0; i < numtraces; i++)
   {
      int tr = i;
      if (random && numtraces<12000)
      {
         tr = rand() % aes.getNumTraces();
         while (list_traces[tr])
         {
            tr = rand() % aes.getNumTraces();
         }
         list_traces[tr] = true;
      }
      aes.readSamples(traces_matrix, i, tr, 0, aes.getNumSamplesPerTrace());
      aes.readPtx(data_matrix, i, tr);
   }
   std::cout << "cacca";
   
   Opanalysis::aesModelHW(power_model, data_matrix, numtraces, ptx, model);
   
   if (known) Opanalysis::DoMKnownmodel(traces_matrix, power_model, startsample, numsamples, numtraces, noiseassumption, model, ptx, save, savek);
   if (unknown) Opanalysis::DoMUnknownmodel(traces_matrix, power_model, startsample, numsamples, numtraces, model, pelgromcoeff, platnoise, ptx, save, savek);
   if (cpa) Opanalysis::CPA(traces_matrix, power_model, startsample, numsamples, numtraces, model, ptx, save, savek);
}