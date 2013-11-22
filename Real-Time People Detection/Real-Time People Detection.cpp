// Real-Time People Detection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;

string generateFileName(int);

int _tmain(int argc, _TCHAR* argv[])
{
	//declare some variable we will need
	IplImage *img;
	const char* place;
	string location;

	//create a window to show the images in
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);

	//loop through all of the images
	for(int i = 1; i <= 1332; i++) //1332
	{
		//generate the current file name
		location = generateFileName(i);

		//convert from string to const char *
		place = location.c_str();
		
		//load the correct image
		img = cvLoadImage(place);

		cvThreshold(img, img, 135, 255, CV_THRESH_BINARY); //was 106

		//show the image in our window
		cvShowImage("Image", img);

		//give a short pause to make sure the image displays
		cvWaitKey(10);

		//release this image ready for the next one
		cvReleaseImage(&img); 
	}

	//clear up our window when all images have been displayed
	cvDestroyWindow("Image"); 

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
