#include "indexer.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::stringstream;
using std::endl;
using cv::Mat;

// writes filenames from string* to file
void writeFilenames(string* filenames, int nFiles, string filenamesFile)
{
	ofstream fileWriter;
	fileWriter.open(filenamesFile);
	for (int i = 0; i < nFiles; i++)
	{
		fileWriter << filenames[i] << endl;
	}
	fileWriter.close();
}

// saves filenames from filenamesFile to string array filenames
void saveFilenamesToArray(const string& filenamesFile, vector<string>& filenames)
{
	ifstream fileReader;
	fileReader.open(filenamesFile);
	string line;
	while(getline(fileReader, line))
	{
		filenames.push_back(line);
	}
	fileReader.close();
}

// writes vector to a file
void writeVector(double* vec, const int nBins, ofstream& fileWriter)
{
	// write first value
	if (nBins > 0) 
	{
		fileWriter << std::setprecision(PRECISION) << vec[0];
	}
	// write comma and next value for all other values
	for (int i = 1; i < nBins; i++)
	{
		fileWriter << ',' << vec[i];
	}
	// go to new line
	fileWriter << endl;
}

// creates an index of image collection, 
// which is a file with vectors that represent image's histograms
void index(vector<string>& filenames, string& vectorsFile)
{
	// write vectors
	double vec[HIST_BINS];
	ofstream fileWriter;
	fileWriter.open(vectorsFile);
	int i = 0;
	std::cout << "Files to process: " << filenames.size() << endl;
	for (vector<string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		Mat big = cv::imread(*it);
		calcVector(big, vec);
		writeVector(vec, HIST_BINS, fileWriter);
		std::cout << (++i) << ". " << *it << endl;
	}
	fileWriter.close();
}

