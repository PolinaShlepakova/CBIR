#include "descriptor.h"

using cv::Mat;
using cv::imshow;
using std::vector;
using std::cout;
using std::endl;
using std::string;

// sets all the values of double array hist with size nBins to 0
void clearHist(double* hist, int nBins)
{
	for (int i = 0; i < nBins; i++) 
	{
		hist[i] = 0;
	}
}

// returns the index of (hBin, sBin, vBin) in flat array
int histIndexHSV(int hBin, int sBin, int vBin)
{
	return	hBin * SATURATION_BINS * VALUE_BINS +
			sBin * VALUE_BINS +
			vBin;
}

// calculates HSV histogram for HSV-Mat img
// and saves it to double array hist with size HIST_BINS
// if clear is true, all values in hist will be set to zero before filling it in
void calcHistHSV(const Mat& img, double* hist, bool clear)
{
	// separate 3 channels
	vector<Mat> channels;
	cv::split(img, channels);
	// clear hist array if requested
	if (clear) clearHist(hist, HIST_BINS);
	int nPixels = img.rows * img.cols;
	// iterate over image
	for (int i = 0; i < nPixels; i++) 
	{
		// take the value of i-th pixel from the channel
		// and divide it by bin size to determine the index of bin it falls into
		int hBin = static_cast<int>(channels[0].data[i] / HUE_BIN_SIZE);
		int sBin = static_cast<int>(channels[1].data[i] / SATURATION_BIN_SIZE);
		int vBin = static_cast<int>(channels[2].data[i] / VALUE_BIN_SIZE);
		// increment the hist's bin which represents this color
		hist[histIndexHSV(hBin, sBin, vBin)]++;
	}
}

// normalizes double array hist with size nBins
// so that every cell contains a number from 0 to 1 
// (represents percentage of pixels that have fallen into this bin)
// nPixels = 100%
void normalizeHist(double* hist, int nBins, int nPixels)
{
	for (int i = 0; i < nBins; i++) 
	{
		hist[i] /= nPixels;
	}
}

// resizes image to fit MAX_IMAGE_DIMENSION and returns it
Mat resize(const Mat& img)
{
	int dim = img.rows * img.cols;
	if (dim > MAX_IMAGE_DIMENSION)
	{
		double factor = static_cast<double>(MAX_IMAGE_DIMENSION) / dim;
		int newRows = static_cast<int>(img.rows * factor);
		int newCols = static_cast<int>(img.cols * factor);
		Mat imgRes = Mat(newRows, newCols, CV_8UC3);
		cv::resize(img, imgRes, cv::Size(newCols, newRows), 0, 0, CV_INTER_AREA);
		return imgRes;
	}
	else
	{
		return img;
	}
}

// calculates HSV histogram for RGB-Mat img
// and saves it to double array hist with size HIST_BINS
// if clear is true, all values in hist will be set to zero before filling it in
// if dimensions of an image is bigger than MAX_IMAGE_DIMENSION it is resized to make calculation faster
void calcVector(const Mat& img, double* hist, bool clear)
{
	// resize image if needed
	Mat imgRes = resize(img);
	// convert to HSV
	Mat imgHSV;
	cv::cvtColor(imgRes, imgHSV, cv::COLOR_BGR2HSV);
	// calculate hist
	calcHistHSV(imgHSV, hist, clear);
	// normalize hist
	normalizeHist(hist, HIST_BINS, imgHSV.rows * imgHSV.cols);
}

// returns max value in double array hist with size nBins
double maxHistValue(double* hist, int nBins)
{
	double max = 0;
	for (int i = 0; i < nBins; i++)
	{
		if (hist[i] > max)
		{
			max = hist[i];
		}
	}
	return max;
}

// creates Mat with histogram and displays it in a window
void displayHist(string name, double* hist, int nBins, int height, int topBorder, int enlarge)
{
	Mat histImg(height + topBorder, nBins, CV_8UC1, cv::Scalar(0));
	double multiplier = height / maxHistValue(hist, nBins);
	int maxIndex = nBins * (height + 3);
	for (int i = 0; i < nBins; i++)
	{
		int row = static_cast<int>(height + topBorder - (hist[i] * multiplier) - 1);
		for (int index = row * nBins + i; index < maxIndex; index += nBins)
		{
			histImg.data[index] = 255;
		}
	}
	int newRows = histImg.rows * 2;
	int newCols = histImg.cols * 2;
	Mat imgRes = Mat(newRows, newCols, CV_8UC3);
	cv::resize(histImg, imgRes, cv::Size(newCols, newRows), 0, 0, CV_INTER_AREA);
	imshow(name, imgRes);
}

// outputs values of a histogram to os
void outputHist(double* hist, int nBins, std::ostream& os)
{
	for (int i = 0; i < nBins; i++)
	{
		os << i << ": \t" << hist[i] << endl;
	}
}