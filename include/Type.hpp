#ifndef TYPE
#define TYPE

#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <unistd.h>

#define TRACE_VALUE_TYPE float
#define ANALYSIS_TYPE double
#define SAMPLE_SIZE 4
#define HEADER_SIZE 10
#define KEY_SIZE 256


using namespace Eigen;

typedef uint8_t DataValueType;
typedef Eigen::Matrix<DataValueType, Eigen::Dynamic, Eigen::Dynamic> DataMatrix;
typedef Eigen::Matrix<ANALYSIS_TYPE, Eigen::Dynamic, Eigen::Dynamic> TracesMatrix;

#endif //TYPE