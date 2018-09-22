#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

/*** CONSTANTS ***/

// number of bins in histogram
const int HUE_BINS			= 8;
const int SATURATION_BINS	= 12;
const int VALUE_BINS		= 3;
const int HIST_BINS			= HUE_BINS * SATURATION_BINS * VALUE_BINS;

// the highest pixel value a channel can have (non-inclusive)
const int HUE_UPPER_BOUND			= 180;
const int SATURATION_UPPER_BOUND	= 256;
const int VALUE_UPPER_BOUND			= 256;

// how many distinct values fit in one bin
const double HUE_BIN_SIZE			= static_cast<double>(HUE_UPPER_BOUND)			/ HUE_BINS;
const double SATURATION_BIN_SIZE	= static_cast<double>(SATURATION_UPPER_BOUND)	/ SATURATION_BINS;
const double VALUE_BIN_SIZE			= static_cast<double>(VALUE_UPPER_BOUND)		/ VALUE_BINS;

// max amount of pixels an image can have
// if it is bigger, it will be resized to fit the size
// (to make the process of calculating a histogram faster)
const int MAX_IMAGE_DIMENSION = 786432;

/*** FUNCTIONS ***/

// calculates HSV histogram for RGB-Mat img
// and saves it to double array hist with size HIST_BINS
// if clear is true, all values in hist will be set to zero before filling it in
// if dimensions of an image are bigger than MAX_IMAGE_DIMENSION it is resized to make calculation faster
void calcVector(const cv::Mat& img, double* hist, bool clear = true);

// calculates HSV histogram for HSV-Mat img
// and saves it to double array hist with size HIST_BINS
// if clear is true, all values in hist will be set to zero before filling it in
void calcHistHSV(const cv::Mat& img, double* hist, bool clear = true);

// normalizes double array hist with size nBins
// so that every cell contains a number from 0 to 1 
// (represents percentage of pixels that have fallen into this bin)
// nPixels = 100%
void normalizeHist(double* hist, int nBins, int nPixels);

// creates Mat with histogram and displays it in a window
void displayHist(std::string name, double* hist, int nBins, int height = 100, int topBorder = 3, int enlarge = 2);

// outputs values of a histogram to os
void outputHist(double* hist, int nBins, std::ostream& os = std::cout);

// resizes image to fit MAX_IMAGE_DIMENSION and returns it
cv::Mat resize(const cv::Mat& img);

// sets all the values of double array hist with size nBins to 0
void clearHist(double* hist, int nBins);

#endif