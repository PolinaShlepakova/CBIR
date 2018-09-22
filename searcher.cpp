#include "searcher.h"

using std::string;
using std::cout;
using std::endl;
using cv::Mat;
using cv::imread;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::stringstream;
using std::vector;
using std::pair;
using std::make_pair;
using cv::Vec3b;
using cv::Point;

bool vecCompare(pair<double, double> p1, pair<double, double> p2) 
{
	return p1.first > p2.first;
}

template <typename T>
string numberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

// creates a vector of doubles from a string
void readVec(string line, double* vec, const int nBins) 
{
	stringstream lineStream(line);
	string cell;
	int i = 0;
	// read next string until a comma appears, and save it to cell
	while (getline(lineStream, cell, ',')) 
	{
		assert(i < nBins);
		// convert string cell to double and write it to array
		vec[i++] = std::stod(cell);
	}
}

Searcher::Searcher(const string& vectorsFile, const vector<string>& filenames, const int nResults, 
					const int nRows, const int nCols, const int windowHeight, const int border):
	_vectorsFile(vectorsFile), _filenames(filenames), _nResults(nResults),
	_nRows(nRows), _nCols(nCols), _windowHeight(windowHeight), _border(border)
{}

Searcher::~Searcher() {}

// compares image located at filename with images in the collection
// and fills resultsIDs with nResults best matches
void Searcher::fillResultsIDs(const string filename, int* resultsIDs)
{
	double queryVec[HIST_BINS];
	double collectionVec[HIST_BINS];
	Mat query;
	{
		Mat big = cv::imread(filename);
		query = resize(big);
	}
	calcVector(query, queryVec);

	// vector which will contain the distance from query to each image in collection as first elem in pair
	// and the index of collection image as second elem in pair
	vector<pair<double, int>> distances;

	string line;
	ifstream fileReader;
	fileReader.open(_vectorsFile);
	// read next line and store it in line until the end of file is reached
	int i = 0;
	while(getline(fileReader, line))
	{
		// rewrite double array collectionVec so that it contains vector written in line
		readVec(line, collectionVec, HIST_BINS);
		double dist = intersection(queryVec, collectionVec, HIST_BINS);
		distances.push_back(make_pair(dist, i++));
	}
	fileReader.close();
	// sort all distances and write the IDs of best matches to resultsIDs
	std::sort(distances.begin(), distances.end(), vecCompare);
	for (int j = 0; j < _nResults; j++)
	{
		resultsIDs[j] = distances[j].second;	
	}
}

// fills a string vector of filenames using filenames' ids in collection
void Searcher::fillResultsFilenames(int* IDs, std::vector<std::string>& filenames)
{
	filenames.clear();
	for (int i = 0; i < _nResults; i++)
	{
		filenames.push_back(_filenames[IDs[i]]);
	}
}

// output filenames of results to console
void Searcher::outputResultsFilenames(std::vector<std::string>& filenames)
{
	cout << "Results: " << endl;
	int i = 0;
	for (vector<string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		cout << (++i) << ". " << *it << endl;
	}
}

// display results as image
void Searcher::displayResults(std::vector<std::string>& filenames)
{
	cv::imshow("Results", resultsAsImage(filenames));
}

// calculates the width of the image, which will cntain all results
int resultsWidth(vector<Mat> images, int nRows, int nCols)
{
	int* widthes = new int[nRows];
	int sum = 0, imagesIt = 0;
	for (int i = 0; i < nRows; i++) 
	{
		for (int j = 0; j < nCols && imagesIt < images.size(); j++)
		{
			sum += images[imagesIt++].cols;
		}
		widthes[i] = sum;
		sum = 0;
	}
	int max = 0;
	for (int i = 0; i < nRows; i++)
	{
		if (widthes[i] > max)
		{
			max = widthes[i];
		}
	}
	delete[] widthes;
	return max;
}

// resizes query image and shows it
void Searcher::displayQuery(const std::string& filename)
{
	// read image from current filename
	Mat img = cv::imread(filename);
	int height = _windowHeight / _nRows;
	// determine new width
	int width = static_cast<int>((static_cast<double>(height) / img.rows) * img.cols);
	// resize image (so that all images have the same height)
	Mat imgRes = Mat(height, width, CV_8UC3);
	cv::resize(img, imgRes, cv::Size(width, height), 0, 0, CV_INTER_AREA);
	cv::imshow("Query", imgRes);
}

// created images located at filenames
// resizes each of them so that the height will be imgHeight
// and saves to vector images
void resizeImagesForDisplay(vector<string>& filenames, vector<Mat>& images, int imgHeight)
{
	for (vector<string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		// read image from current filename
		Mat currImg = cv::imread(*it);
		// determine new width (depends on imgHeight)
		int currImgWidth = static_cast<int>((static_cast<double>(imgHeight) / currImg.rows) * currImg.cols);
		// resize image (so that all images have the same height)
		Mat currImgResized = Mat(imgHeight, currImgWidth, CV_8UC3);
		cv::resize(currImg, currImgResized, cv::Size(currImgWidth, imgHeight), 0, 0, CV_INTER_AREA);
		// add resized image to vector
		images.push_back(currImgResized);
	}
}

// creates an image, which will contain all results and shows it
Mat Searcher::resultsAsImage(vector<string>& filenames)
{
	// if specified amout of results doesn't fit in the specified amount of rows and cols, 
	// then it's a programmers mistake, so inform him about it with assert
	assert(filenames.size() <= _nRows * _nCols);
	// height of every image in window
	int imgHeight = _windowHeight / _nRows;
	// create src images and save them to vector
	vector<Mat> images;
	resizeImagesForDisplay(filenames, images, imgHeight);

	int canvasWidth = resultsWidth(images, _nRows, _nCols);
	Mat canvas(_windowHeight + _border * (_nRows + 1), canvasWidth + _border * (_nCols + 1), CV_8UC3, cv::Scalar(255, 255, 255));
	int canvasStartY = _border, canvasStartX = _border;
	int nImagesProccessed = 0;
	// iterate over src images
	for (vector<Mat>::iterator it = images.begin(); it != images.end(); ++it, nImagesProccessed++)
	{
		// went to next row
		if (nImagesProccessed && nImagesProccessed % _nCols == 0)
		{
			canvasStartY += imgHeight + _border;
			canvasStartX = _border;		
		}
		// iterate over src image and copy it to canvas
		for (int i = 0; i < (*it).rows; i++)
		{
			for (int j = 0; j < (*it).cols; j++)
			{
				canvas.at<Vec3b>(Point(canvasStartX + j, canvasStartY + i)) = (*it).at<Vec3b>(Point(j, i));
			}
		}
		canvasStartX += (*it).cols + _border;
	}
	return canvas;
}

// searches for the amount of most similar images, specified in constructor
// outputs the time it took to find results and filenames of results to console
// and displays the query and results as images
void Searcher::search(const std::string& filename)
{
	long long freq = getFrequency();
	long long timeStart = getTimeInTicks();

	int* IDs = new int[_nResults];
	vector<string> filenames;
	fillResultsIDs(filename, IDs);

	cout << "Searched in " << elapsedTimeInMillis(timeStart, freq) << " millis" << endl << endl;
	fillResultsFilenames(IDs, filenames);
	outputResultsFilenames(filenames);
	displayQuery(filename);
	displayResults(filenames);
}