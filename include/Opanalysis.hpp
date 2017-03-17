#include "Type.hpp"
#include <fstream>
#include <sys/stat.h>

class Opanalysis{

public:

   static void aesModelHW(std::shared_ptr<TracesMatrix>& pModel, std::shared_ptr<DataMatrix>& data_matrix, unsigned long numtraces, unsigned long colptx, char out_model);


   static void DoMKnownmodel(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char noiseAssumption, char out_model, int ptx, bool save, bool savek);

   static void DoMUnknownmodel(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char out_model, double pel_coeff, double plat_noise, int ptx, bool save, bool savek);

   static void CPA(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char out_model, int ptx, bool save, bool savek);

   static void saveInFile(std::shared_ptr<TracesMatrix>& arg, std::string filename, std::string directoryname, unsigned long numsam)
   {
      std::string dirname = "./" + directoryname;
      if (-1 == mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
      {
          printf("Error creating directory!\n");
          exit(1);
      }
      double output[numsam];
      std::string fn = dirname + "/" + filename;
      FILE *fs = fopen(fn.c_str(), "wb");
      if(!fs) {
      std::perror("File opening failed");
      }
      for (size_t i = 0; i < KEY_SIZE; i++)
      {
         for (size_t j = 0; j < numsam; j++)
         {
            output[j] = (*arg)(i,j);
         }
         std::fwrite(output, sizeof(double), numsam, fs);
         if(std::fflush(fs) != 0)
         {
            std::cout << "Error in flushing file" << '\n';
            return;
         }
      }
      if(std::fclose(fs) != 0)
      {
         std::cout << "Error in closing file" << '\n';
      }
   }
};
