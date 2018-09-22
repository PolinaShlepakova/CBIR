#ifndef _SEARCHER_H_
#define _SEARCHER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

#include "descriptor.h"
#include "comparator.h"
#include "time.h"

class Searcher
{
public:
	/*
	 * vectorsFile - file, which contains vectors of all images (can be created by indexer)
	 * filenames - filenames of files in the collection
	 * nResults - amount of results to search for and display
	 * nRows - number of rows of images in results window
	 * nCols - number of cols of images in results window
	 * windowHeight - the height of results window
	 * border - border between images in results window
	 */
	Searcher(const std::string& vectorsFile, const std::vector<std::string>& filenames, const int nResults = 10, 
		const int nRows = 3, const int nCols = 4, const int windowHeight = 550, const int border = 5);
	~Searcher();

	// searches for the amount of most similar images, specified in constructor
	// outputs the time it took to find results and filenames of results to console
	// and displays the query and results as images
	void search(const std::string& filename);

private:
	// file, which contains vectors of all images (can be created by indexer)
	std::string _vectorsFile;
	// filenames of files in the collection
	std::vector<std::string> _filenames;
	// amount of results to search for and display
	int _nResults;

	/*** parameters of results window ***/
	// number of rows of images in results window
	int _nRows;
	// number of cols of images in results window
	int _nCols;
	// the height of results window
	int _windowHeight;
	// border between images in results window
	int _border;

	// compares image located at filename with images in the collection
	// and fills resultsIDs with nResults best matches
	void fillResultsIDs(const std::string filename, int* resultsIDs);

	// writes filenames, which correspond to given IDs, to a vector
	void fillResultsFilenames(int* IDs, std::vector<std::string>& filenames);

	// outputs the filenames of results to cout
	void outputResultsFilenames(std::vector<std::string>& filenames);

	// display results as image
	void displayResults(std::vector<std::string>& filenames);

	// creates white Mat, copies images, corresponding to given filenames, and returns it
	cv::Mat resultsAsImage(std::vector<std::string>& filenames);

	// resizes query image and shows it
	void displayQuery(const std::string& filename);

	// forbidden (private and not implemented)
	Searcher(const Searcher& s);
	Searcher& operator=(const Searcher& s);
};

#endif