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
float processImage(IplImage*, Mat&);

int _tmain(int argc, _TCHAR* argv[])
{
	//declare some variable we will need
	IplImage *img;
	const char* place;
	string location;
	float result;
	string value;

	//cout << "Type start to begin: ";
	//cin >> value;
	value = "start";

	if(value == "start")
	{
		//convert from the IplImage structure to a 2D matrix (no need for IplImage structure as we won't display this on screen)
		IplImage* myTemplate = cvLoadImage("template.jpg");
		cvThreshold(myTemplate, myTemplate, 135, 255, CV_THRESH_BINARY); //was 106
		Mat templateMatrix(myTemplate);		

		//create a window to show the images in
		cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

		//loop through all of the images
		for(int i = 1; i <= 1332; i++) //1332
		{
			//generate the current file name
			location = generateFileName(i);
			//location = "images/thermal000264.jpg";

			cout << "Using filename: " << location << endl;

			//convert from string to const char *
			place = location.c_str();
		
			//load the correct image
			img = cvLoadImage(place);

			//carry out all of the processes on the current image
			result = processImage(img, templateMatrix);

			//show the image in our window
			cvShowImage("Original", cvLoadImage(place));

			cvNamedWindow("Binary", CV_WINDOW_AUTOSIZE);
			cvShowImage("Binary", img);

			//give a short pause to make sure the image displays
			cvWaitKey(10);

			//release this image ready for the next one
			cvReleaseImage(&img); 
		}

		//clear up our window when all images have been displayed
		cvDestroyWindow("Original"); 
		cvDestroyWindow("Binary");
	}
	else
	{
		cout << "Command unknown" << endl;
	}

	return 0;
}

float processImage(IplImage* image, Mat &templateMatrix)
{
	//used for calculating image difference 
	int counter = 0;
	Mat storage((image->width), (image->height), 1, CV_32F);
	float max = 0;
	int location1 = 0, location2 = 0;
	int templateRowCounter = 0, templateColCounter = 0;

	//perform a threshold to get a binary image
	cvThreshold(image, image, 135, 255, CV_THRESH_BINARY); //was 106

	//convert from the IplImage structure to a 2D matrix
	Mat imageMatrix(image);

	//Two outer for loops are for moving throughout the entire image
	//The middle for loops are for checking a section with the template image
	//The inner for loops are for moving through the template matrix
	for(int i = 0; i < 320 - templateMatrix.cols; i++)
	{
		for(int n = 0; n < 240 - templateMatrix.rows; n++)
		{
			for(int j = i; j < i + 62; j++) //cols
			{
				for(int z = n; z < i + 83; z++) //rows
				{
					int currentImage = imageMatrix.at<unsigned char>(j, z);
					int templateImage = templateMatrix.at<unsigned char>(j, z);

					//cout << currentImage << " / " << templateImage << endl;

					if((currentImage == 0 && templateImage == 1)||(currentImage == 1 && templateImage == 0))
					{
						counter++;
					}
				}
			}

			float value = (float)counter/(float)(templateMatrix.cols*templateMatrix.rows);

			//cout << fixed << "Counter: " << counter << " Value: " << value << endl; 

			//store the score for this section within our storage matrix
			storage.at<float>(i, n) = value;

			counter = 0;
		}
	}

	//Check for best match within the storage matrix
	for(int i = 0; i < (image->height); i++)
	{
		for(int j = 0; j < (image->width); j++)
		{
			if(storage.at<float>(i, j) > max)
			{
				max = storage.at<float>(i, j);
				location1 = i;
				location2 = j;
			}
		}
	}

	cout << fixed << "Best hit has score " << max << " in location " << location1 << ", " << location2 << endl << endl;

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
