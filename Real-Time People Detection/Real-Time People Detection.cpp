// Real-Time People Detection.cpp : Defines the entry point for the console application.
// Jake Welton - University of Lincoln 2013

#include "stdafx.h"
#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;

string generateFileName(int);
int processImage(IplImage*, Mat&);

int _tmain(int argc, _TCHAR* argv[])
{
	//declare some variable we will need
	IplImage *img;
	const char* place;
	string location;
	int result;
	string value;

	cin >> value;
	if(value == "start")
	{
	//convert from the IplImage structure to a 2D matrix (no need for IplImage structure as we won't display this on screen)
	Mat templateMatrix(cvLoadImage("template.jpg"));

	//create a window to show the images in
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);

	//loop through all of the images
	for(int i = 1; i <= 1332; i++) //1332
	{
		//generate the current file name
		location = generateFileName(i);

		//convert from string to const char *
		place = location.c_str();
		
		//load the correct image
		img = cvLoadImage(place);

		//carry out all of the processes on the current image
		result = processImage(img, templateMatrix);

		if(result <= 500)
		{
			cout << "Person detected in image " << i << endl;
		}

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

int processImage(IplImage* image, Mat &templateMatrix)
{
	//used for calculating image difference 
	int counter = 0;

	//perform a threshold to get a binary image
	cvThreshold(image, image, 135, 255, CV_THRESH_BINARY); //was 106

	//convert from the IplImage structure to a 2D matrix
	Mat imageMatrix(image);

	//loop through the matrix
	for(int i = 1; i < image->height; i++)
	{
		for(int j = 1; j < image->width; j++)
		{
			//cout << (int)imageMatrix.at<unsigned char>(i, j);
			//cout << (int)templateMatrix.at<unsigned char>(i, j);

			int currentImage = imageMatrix.at<unsigned char>(i, j);
			int templateImage = templateMatrix.at<unsigned char>(i, j);

			if((currentImage == 0 && templateImage == 1)||(currentImage == 1 && templateImage == 0))
			{
				counter++;
			}
		}
	}
	//cout << "Rating = " << counter << endl;
	return counter;
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
