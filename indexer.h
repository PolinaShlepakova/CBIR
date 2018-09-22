#ifndef _INDEXER_H_
#define _INDEXER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <stdlib.h>

#include "descriptor.h"

// precision of vector that is saved
const int PRECISION = 10;

// creates an index of image collection, 
// which is a file with vectors that represent image's histograms
void index(std::vector<std::string>& filenames, std::string& vectorsFile);

// saves filenames from filenamesFile to string array filenames
void saveFilenamesToArray(const std::string& filenamesFile, std::vector<std::string>& filenames);

#endif