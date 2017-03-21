#ifndef OPANALYSIS
#define OPANALYSIS

#include "Type.hpp"
#include <fstream>
#include <sys/stat.h>

class Opanalysis{

public:

   /* Compute the Hamming Weight model of the byte "byte" of the given data_matrix, 
    * using the parameter aes_model as an option to choose the model at the output
    * of the first sbox using 'S', or, at the first add round key, using 'K'.
    */
   static void aesModelHW(std::shared_ptr<TracesMatrix> pModel, std::shared_ptr<DataMatrix> data_matrix, unsigned long numtraces, unsigned long byte, char aes_model);

   /* Apply the Optimal Distinguisher for known model, saving the traces (as they are)
    * in a file, saving the finded key in another file and displaying it.
    * All these actions depend on option passed as parameter to the function.
    * To find the key, the algorithm compute, for each sample, the mean of the results 
    * for each key, the new results will be the difference of the old results with
    * the mean.
    */
   static void DoMKnownmodel(std::shared_ptr<TracesMatrix> traces_matrix, std::shared_ptr<TracesMatrix> power_model, unsigned long startingsample, unsigned long numsamples, unsigned long numtraces, char noiseAssumption, char aes_model, int byte, bool save, bool savek);

   /* Apply the optimal distinguisher for partially known model, saving the traces 
    * (as they are) in a file, saving the finded key in an other file and displaying it.
    * All these action depend on option passed as parameter.
    * To retrieve the best key it find the argmax for each sample and take the one
    * which appear more times.
    */
   static void DoMUnknownmodel(std::shared_ptr<TracesMatrix> traces_matrix, std::shared_ptr<TracesMatrix> power_model, unsigned long startingsample, unsigned long numsamples, unsigned long numtraces, char aes_model, double pel_coeff, double plat_noise, int byte, bool save, bool savek);

   /* Apply the correlation power analysis, saving the traces 
    * in a file, saving the finded key in an other file and displaying it.
    * All these action depend on option passed as parameter.
    */
   static void CPA(std::shared_ptr<TracesMatrix> traces_matrix, std::shared_ptr<TracesMatrix> power_model, unsigned long startingsample, unsigned long numsamples, unsigned long numtraces, char aes_model, int byte, bool save, bool savek);

   static void saveInFile(std::shared_ptr<TracesMatrix> results, std::string filename, std::string directoryname, unsigned long numsamples)
   {
      std::string dirname = "./" + directoryname;
      mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      double output[numsamples];
      std::string fn = dirname + "/" + filename;
      FILE *fs = fopen(fn.c_str(), "wb");
      if(!fs) {
      std::perror("File opening failed");
      }
      for (size_t i = 0; i < KEY_SIZE; i++)
      {
         for (size_t j = 0; j < numsamples; j++)
         {
            output[j] = (*results)(i,j);
         }
         std::fwrite(output, sizeof(double), numsamples, fs);
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

#endif //OPANALYSIS