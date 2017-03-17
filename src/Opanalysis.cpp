#include "Opanalysis.hpp"
#include <functional>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Eigen/LU>

using namespace Eigen;

void Opanalysis::aesModelHW(std::shared_ptr<TracesMatrix>& pModel, std::shared_ptr<DataMatrix>& data_matrix, unsigned long numtraces, unsigned long colptx, char out_model)
{
   DataValueType addkey;
   ANALYSIS_TYPE sboxout;
   DataMatrix key (1,KEY_SIZE);
   Matrix<ANALYSIS_TYPE, 1,KEY_SIZE> sbox;
   Matrix<ANALYSIS_TYPE, 1,KEY_SIZE> hw;
   key << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255;
   sbox <<  99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118, 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192, 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21, 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117, 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132, 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207, 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168, 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210, 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115, 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219, 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121, 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8, 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138, 112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158, 225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223, 140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22;
   hw << 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8;
   for (int i = 0; i < numtraces; i++) {
      for (int j = 0; j < KEY_SIZE; j++) {
         addkey = (*data_matrix)(i, colptx) ^ key(j);
         if (out_model == 'S')
         {
            sboxout = sbox(addkey.to_ulong());
            (*pModel)(i,j) = hw(sboxout);
         }
         else if (out_model == 'K')
         {
            (*pModel)(i,j) = hw(addkey.to_ulong());
         }
         else
         std::cerr << "WRONG OUTPUT CHOOSE" << '\n';
      }
   }
}


void Opanalysis::DoMKnownmodel(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char noiseAssumption, char out_model, int ptx, bool save, bool savek)
{
   TracesMatrix y(ntraces,1);
   TracesMatrix trace(ntraces, numsamples);
   TracesMatrix pmodel(ntraces, KEY_SIZE);
   std::shared_ptr<TracesMatrix> arg = std::make_shared<TracesMatrix>(KEY_SIZE, numsamples);
   TracesMatrix ones;
   ones.setOnes(ntraces,1);
   double scalar_product, y_norm;
   float max_diff = 0;
   int best_key = 0;

   trace = ((*traces_matrix).topRows(ntraces));
   pmodel = ((*power_model).topRows(ntraces));

   for (int i = 0; i < KEY_SIZE; i++)
   {
      y = pmodel.col(i);
      int j = 0;
      for (int sample = startingsample; sample < (startingsample+numsamples); sample++, j++)
      {
         if(noiseAssumption == 'G')
         {
            scalar_product = (y.transpose() * trace.col(sample))(0,0);
            y_norm = (y.transpose() * y)(0,0);
            (* arg)(i,j) = scalar_product - (y_norm / 2);
         } else (* arg)(i,j) = -((trace.col(sample)-y).transpose() * ones)(0,0);
     }
   }
   for (int sample = 0; sample < numsamples; sample++)
   {
      float diff;
      float mean = arg->col(sample).sum() / arg->rows();
      for (int key = 0; key < KEY_SIZE; key++)
      {
         diff = (*arg)(key, sample) - mean;
         if (diff > max_diff)
         {
            max_diff = diff;
            best_key = key;
         }
      }
   }
   if(save)
   {
      std::string dn = "DoMKnownmodel";
      std::string fn = "ptx" + std::to_string(ptx) + "numtraces" + std::to_string(ntraces) + "nsam" + std::to_string(numsamples) + "noise" + noiseAssumption + "model" + out_model + ".bin";
      Opanalysis::saveInFile(arg, fn, dn, numsamples);
   }
   if(savek)
   {
      std::ofstream outfile;
      std::string fn = "bestkeys/nt" + std::to_string(ntraces) + "ns" + std::to_string(numsamples) + ".txt";
      outfile.open(fn, std::ios_base::app);
      outfile << best_key << '\n';
   } else std::cout << "Key: " + std::to_string(best_key) + "\n";
}

void Opanalysis::DoMUnknownmodel(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char out_model, double pel_coeff_squared, double plat_noise, int ptx, bool save, bool savek)
{
   TracesMatrix y(ntraces,1);
   TracesMatrix scalar_product;
   TracesMatrix operand_two;
   TracesMatrix operand_one;
   TracesMatrix I(8,8);
   TracesMatrix gram_matrix(8,8);
   TracesMatrix y_matrix(ntraces,8);
   TracesMatrix trace(ntraces, numsamples);
   TracesMatrix pmodel(ntraces, KEY_SIZE);
   TracesMatrix diff_matrix(KEY_SIZE, 1);
   TracesMatrix old_matrix(KEY_SIZE, 1);
   std::shared_ptr<TracesMatrix> arg = std::make_shared<TracesMatrix>(KEY_SIZE, numsamples);
   TracesMatrix eight_ones;
   I.setIdentity();
   float esnr, max_diff;
   int old_key = -1;
   int best_key = 0;
   max_diff = 0;
   old_matrix.setZero();

   esnr = pel_coeff_squared / plat_noise;
   eight_ones.setOnes(8,1);

   trace = ((*traces_matrix).topRows(ntraces));
   pmodel = ((*power_model).topRows(ntraces));

   for (int i = 0; i < KEY_SIZE; i++)
   {
      y = pmodel.col(i);
      for (int j = 0; j < 8; j++) {
         for (int r = 0; r < y.rows(); r++) {
            y_matrix(r,j) = ((int)y(r) << j) & 128;
         }
      }
      gram_matrix = y_matrix.transpose() * y_matrix; //matrix (8,8)
      int j = 0;
      for (int sample = startingsample; sample < (startingsample+numsamples); sample++, j++)
      {
         scalar_product = (trace.col(sample).transpose() * y_matrix).transpose(); //eight-colvector
	       operand_one = (scalar_product * esnr + eight_ones);//matrix (8,1)
	       operand_two = (gram_matrix * esnr + I); //matrix (8,8)
         (* arg)(i,j) = (operand_one.transpose() * operand_two.inverse() * operand_one)(0,0) - pel_coeff_squared * log(operand_two.determinant()) / 2; //((1,8) * (8,8) * (8,1))
      }
   }
   for (int sample = 0; sample < numsamples; sample++)
   {
      float diff;
      int locmaxkey = 0;
      float locmaxdiff = 0;
      float mean = arg->col(sample).sum() / arg->rows();
      for (int key = 0; key < KEY_SIZE; key++)
      {
         diff = (*arg)(key, sample) - mean;
         if (diff > locmaxdiff)
         {
            locmaxdiff = diff;
            locmaxkey = key;
         }
      }
      if(locmaxkey == old_key) old_matrix(locmaxkey)++;
      old_key = locmaxkey;
   }
   for (int key = 0; key < KEY_SIZE; key++)
   {
      if (old_matrix(key) > max_diff)
      {
         max_diff = old_matrix(key);
         best_key = key;
      }
   }
   if(save)
   {
      std::string dn = "DoMUnknownmodel";
      std::string fn = "ptx" + std::to_string(ptx) + "numtraces" + std::to_string(ntraces) + "nsam" + std::to_string(numsamples) + "Pelgromcoefficient" + std::to_string((int)pel_coeff_squared) + "Platformnoise" + std::to_string((int)plat_noise) + "model" + out_model + ".bin";
      Opanalysis::saveInFile(arg, fn, dn, numsamples);
   }
   if(savek)
   {
      std::ofstream outfile;
      std::string fn = "bestkeys/unt" + std::to_string(ntraces) + "ns" + std::to_string(numsamples) + ".txt";
      outfile.open(fn, std::ios_base::app);
      outfile << best_key << '\n';
   } else std::cout << "Key: " + std::to_string(best_key) + "\n";
}

void Opanalysis::CPA(std::shared_ptr<TracesMatrix>& traces_matrix, std::shared_ptr<TracesMatrix>& power_model, unsigned long startingsample, unsigned long numsamples, unsigned long ntraces, char out_model, int ptx, bool save, bool savek)
{
   TracesMatrix tr(ntraces,1);
   TracesMatrix y(ntraces,1);
   TracesMatrix mx(ntraces,1);
   TracesMatrix my(ntraces,1);
   ANALYSIS_TYPE varx, vary, num;
   TracesMatrix traces(ntraces, numsamples);
   TracesMatrix pmodel(ntraces, KEY_SIZE);
   std::shared_ptr<TracesMatrix> arg = std::make_shared<TracesMatrix>(KEY_SIZE, numsamples);
   float max = 0;
   int best_key = 0;

   traces = ((*traces_matrix).topRows(ntraces));
   pmodel = ((*power_model).topRows(ntraces));

   for (int i = 0; i < KEY_SIZE; i++)
   {
      y = pmodel.col(i);
      int j = 0;
      for (int sample = startingsample; sample < (startingsample+numsamples); sample++, j++)
      {
         tr = traces.col(sample);
         mx.fill((tr.sum()/tr.size()));
         my.fill((y.sum()/y.size()));
         num =  ((tr - mx).transpose() *  (y - my))(0,0);
         varx = ((tr - mx).transpose() * (tr - mx))(0,0) ;
         vary = ((y - my).transpose() * (y - my))(0,0);
         (* arg)(i,j) = std::abs(num) / sqrt(varx*vary);
         if ((*arg)(i, j) > max)
         {
            max = (*arg)(i, j);
            best_key = i;
         }
      }
   }
   if(save)
   {
      std::string dn = "CPA";
      std::string fn = "ptx" + std::to_string(ptx) + "numtraces" + std::to_string(ntraces) + "nsam" + std::to_string(numsamples) + "model" + out_model + ".bin";
      Opanalysis::saveInFile(arg, fn, dn, numsamples);
   }
   if(savek)
   {
      std::ofstream outfile;
      std::string fn = "bestkeys/cpant" + std::to_string(ntraces) + "ns" + std::to_string(numsamples) + ".txt";
      outfile.open(fn, std::ios_base::app);
      outfile << best_key << '\n';
   } else std::cout << "Key: " + std::to_string(best_key) + "\n";
}