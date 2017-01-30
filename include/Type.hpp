//Define type and struct
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <unistd.h>

#define DATA_SIZE_BIT 8
#define DATA_SIZE_BYTE (DATA_SIZE_BIT >> 3)
#define TRACE_VALUE_TYPE float
#define ANALYSIS_TYPE double
#define SAMPLE_SIZE 4
#define HEADER_SIZE 10
#define KEY_SIZE 256


using namespace Eigen;

typedef std::bitset<DATA_SIZE_BIT> DataValueType;
typedef Eigen::Matrix<DataValueType, Eigen::Dynamic, Eigen::Dynamic> DataMatrix;
typedef Eigen::Matrix<ANALYSIS_TYPE, Eigen::Dynamic, Eigen::Dynamic> TracesMatrix;
typedef Eigen::Matrix<ANALYSIS_TYPE, Eigen::Dynamic, Eigen::Dynamic> AnalysisType;
