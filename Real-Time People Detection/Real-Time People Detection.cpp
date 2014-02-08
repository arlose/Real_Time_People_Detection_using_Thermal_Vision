// Real-Time People Detection.cpp : Defines the entry point for the console application.
// Jake Welton - University of Lincoln 2013

#include "stdafx.h"
#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <cmath>
#include <math.h>

using namespace std;
using namespace cv;

string generateFileName(int);
float processImage(Mat&, Mat&);

int _tmain(int argc, _TCHAR* argv[])
{
	//declare some variable we will need
	Mat image;
	const char* fileNameChar;
	string fileName, value;
	float result;

	//convert from the IplImage structure to a 2D matrix (no need for IplImage structure as we won't display this on screen)
	//IplImage* myTemplate = cvLoadImage("testTemplate.png");
	Mat templateMatrix = imread("testTemplate.png", CV_LOAD_IMAGE_GRAYSCALE);

	//create a window to show the images in
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

	//loop through all of the images
	for(int i = 0; i <= 0; i++) //1332
	{
		//generate the current file name
		//fileName = generateFileName(i);
		fileName = "testImage.png";

		cout << "Using filename: " << fileName << endl;
		
		//load the correct image
		image = (imread(fileName, CV_LOAD_IMAGE_GRAYSCALE));

		//carry out all of the processes on the current image
		result = processImage(image, templateMatrix);

		fileNameChar = fileName.c_str();

		//show the image in our window
		cvShowImage("Original", cvLoadImage(fileNameChar));

		//Show the binary version of the image
		cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
		imshow("Binary", image);

		//give a short pause to make sure the image displays
		cvWaitKey(1);
	}

	//clear up our window when all images have been displayed
	cvDestroyWindow("Original"); 
	cvDestroyWindow("Binary");

	return 0;
}

float processImage(Mat &image, Mat &templateImage)
{
	// Declare the variables required
	int counter = 0;
	double value;
	Mat storage((image.rows - (templateImage.rows - 1)), (image.cols - (templateImage.cols - 1)), CV_64F);

	threshold(image, image, 135, 255, CV_THRESH_BINARY);

	//cout << endl;

	for(int imageRow = 0; imageRow < (image.rows - (templateImage.rows - 1)); imageRow++)
	{
		for(int imageCol = 0; imageCol < (image.cols - (templateImage.cols - 1)); imageCol++)
		{
			for(int templateRow = 0; templateRow < templateImage.rows; templateRow++)
			{
				for(int templateCol = 0; templateCol < templateImage.cols; templateCol++)
				{
					int imageValue = image.at<unsigned char>(templateRow + imageRow, templateCol + imageCol);
					int templateValue = templateImage.at<unsigned char>(templateRow, templateCol);

					//cout << "Image value = " << imageValue << endl << "Template value = " << templateValue << endl;

					if((imageValue == 0 && templateValue == 0)||(imageValue == 255 && templateValue == 255))
					{
						counter++;
					}
				}
			}

			value = (double)counter/(double)(templateImage.cols*templateImage.rows);

			storage.at<double>(imageRow, imageCol) = value;

			// Output for development purposes
			//cout << "Storing in location " << imageCol << ", " << imageRow << endl;
			//cout << "Value for [" << imageCol << ", " << imageRow << "] = " << value << endl << endl;

			counter = 0;
			value = 0;
		}
		// Output for development purposes
		//cout << endl;
	}

	double max = 0;

	// Search for exact matches
	for(int row = 0; row < storage.rows; row++)
	{
		for(int col = 0; col < storage.cols; col++)
		{
			double data = storage.at<double>(row, col);

			//cout << "[" << row << ", " << col << "]" << " = " << fixed << data << endl;

			if (data > max)
			{
				max = data;
			}
			
			if(data == 1)
			{
				cout << " ** Exact match detected at position [" << row << ", " << col << "] ** " << endl;
			}
		}
	}

	cout << "Max value: " << max << endl << endl;

	return 0;
}

string generateFileName(int currentPlace)
{
	//declare some requires variables
	char temp[256]; 
	string location;

	//add the required number of 0's to the front of our number
	sprintf(temp, "%06d", currentPlace);

	//add the rest of the image location information
	location = "images/thermal" + string(temp) + ".jpg";

	//return the new file location
	return location;
}
