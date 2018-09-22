#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "descriptor.h"
#include "comparator.h"
#include "indexer.h"
#include "searcher.h"
#include "time.h"

using cv::Mat;
using std::vector;
using std::cout;
using std::endl;

const std::string FILENAMES_FILE = "D:/CBIR/filenames.txt";
const std::string VECTORS_FILE = "D:/CBIR/vectors.txt";
const int N_FILES = 712;

bool display(cv::Mat& img, std::string name) 
{
	if (img.data == 0)
	{
		cout << "Could not open or find " << name << endl;
		return false;
	}
	cv::imshow(name, img);
	return true;
}

void displayHist()
{
	cv::Mat big1, big2, small1, small2;

	// load source image and display it
	big1 = cv::imread("D:/jpg1/test/three.png");
	big2 = cv::imread("D:/jpg1/test/four.png");
	/*big1 = cv::imread("D:/jpg1/jpg/123204.jpg");
	big2 = cv::imread("D:/jpg1/jpg/112403.jpg");*/

	// resize
	small1 = resize(big1);
	small2 = resize(big2);

	// display resized images
	display(small1, "1");
	display(small2, "2");

	// calculate histograms
	double hist1[HIST_BINS];
	double hist2[HIST_BINS];
	calcVector(small1, hist1);
	calcVector(small2, hist2);

	// display histograms
	displayHist("Histogram 1", hist1, HIST_BINS);
	displayHist("Histogram 2", hist2, HIST_BINS);
}

void displayHistsAndDistance()
{
	long long freq = getFrequency();
	long long timeStart = 0;

	cv::Mat big1, big2, small1, small2;

	// load source image and display it
	big1 = cv::imread("D:/jpg1/jpg/112403.jpg");
	big2 = cv::imread("D:/jpg1/jpg/114702.jpg");

	// resize
	timeStart = getTimeInTicks();
	small1 = resize(big1);
	cout << "Resize time: " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl;
	timeStart = getTimeInTicks();
	small2 = resize(big2);
	cout << "Resize time: " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl << endl;

	// display resized images
	display(small1, "1");
	display(small2, "2");

	// calculate histograms
	double hist1[HIST_BINS];
	double hist2[HIST_BINS];
	timeStart = getTimeInTicks();
	calcVector(small1, hist1);
	cout << "Calc hist time " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl;
	timeStart = getTimeInTicks();
	calcVector(small2, hist2);
	cout << "Calc hist time " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl << endl;

	// display histograms
	displayHist("Histogram 1", hist1, HIST_BINS);
	displayHist("Histogram 2", hist2, HIST_BINS);

	timeStart = getTimeInTicks();
	cout << "Intersection: " << intersection(hist1, hist2, HIST_BINS) << endl;
	cout << "Intersection time: " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl;
}

int main()
{
	displayHistsAndDistance();

	/**
	std::vector<std::string> filenames;
	//filenames.push_back("D:/jpg1/test/tiny1.png");
	//filenames.push_back("D:/jpg1/test/tiny2.png");
	filenames.push_back("D:/jpg1/jpg/112400.jpg");
	filenames.push_back("D:/jpg1/jpg/112401.jpg");
	filenames.push_back("D:/jpg1/jpg/112402.jpg");
	filenames.push_back("D:/jpg1/jpg/112403.jpg");
	filenames.push_back("D:/jpg1/jpg/112404.jpg");
	filenames.push_back("D:/jpg1/jpg/112405.jpg");
	filenames.push_back("D:/jpg1/jpg/112406.jpg");
	filenames.push_back("D:/jpg1/jpg/112407.jpg");
	filenames.push_back("D:/jpg1/jpg/112408.jpg");

	Mat res = resultsAsImage(filenames, 3, 4, 400);
	cv::imshow("Results", res);
	*/

	/**

	std::vector<std::string> filenames;
	saveFilenamesToArray(FILENAMES_FILE, filenames);

	//index(filenames, FILENAMES_FILE, VECTORS_FILE);
	const int nResults = 12;
	Searcher searcher(VECTORS_FILE, filenames, nResults);
	//int IDs[nResults];
	//searcher.search("D:/jpg1/jpg/112403.jpg");
	//searcher.search("D:/jpg1/jpg/1.jpg");
	//searcher.search("D:/jpg1/jpg/122500.jpg");
	searcher.search("D:/jpg1/jpg/123600.jpg");	
	
	**/

	/**
	std::vector<int> leaders;
	fillLeadersIDs(712, leaders);
	**/

	cv::waitKey(0);
	return 0;
}