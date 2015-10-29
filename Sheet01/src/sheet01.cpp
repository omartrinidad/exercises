#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    
//	==================== Load image =========================================
  	// Read the image file - bonn.png is of type CV_8UC3
	const Mat bonn = imread(argv[1], IMREAD_COLOR);

	// Create gray version of bonn.png
	Mat bonn_gray;
	cvtColor(bonn, bonn_gray, CV_BGR2GRAY);
//	=========================================================================
//	==================== Solution of task 1 =================================
//	=========================================================================
	cout << "Task 1:" << endl;
	//====(a)==== draw random rectangles ====
	
	// Create a copy of the input image to draw the rectangles into
	Mat img_task1;
	bonn_gray.copyTo(img_task1);
	
	// Create a random number generator
	RNG rng(getTickCount());
	Rect rects[100];
	for (auto& r : rects) {
		// Random coordinates of the rectangle
		const int x1 = rng.uniform(0, bonn_gray.cols);
		const int x2 = rng.uniform(0, bonn_gray.cols);
		const int y1 = rng.uniform(0, bonn_gray.rows);
		const int y2 = rng.uniform(0, bonn_gray.rows);
        rectangle(img_task, Point(x1, y1), Point(x2, y2), Scalar(255, 0, 0), 1);
	}
	//TODO: Display the image "img_task1" with the rectangles
	// ...

    imshow("Rectangles", bonn_gray);
	waitKey(0);
	
	//====(b)==== summing up method ====
	// Variables for time measurements
	int64_t tick, tock;
	double sumOfPixels;

	// Measure the time
	tick = getTickCount();
	
	// Repeat 10 times
	for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
			for (int y = r.y; y < r.y + r.height; ++y) {
				for (int x = r.x; x < r.x+r.width; ++x) {
				  	//TODO: Sum up all pixels at "bonn_gray.at<uchar>(y,x)" inside the rectangle and store it in "sumOfPixels" 
					// ...
				}
			}
		}
	}
	tock = getTickCount();
	cout << "Summing up each pixel gives " << sumOfPixels << " computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;

	// //====(c)==== integral image method - using OpenCV function "integral" ==== 
	
	//TODO: implement your solution of here
	// ...
	
	
	//====(d)==== integral image method - custom implementation====
	//TODO: implement your solution here
	// ...

	// waitKey(0); // waits until the user presses a button and then continues with task 2 -> uncomment this
	// destroyAllWindows(); // closes all open windows -> uncomment this
	
//	=========================================================================	
//	==================== Solution of task 2 =================================
//	=========================================================================
	cout << "Task 2:" << endl;
	
	
	//====(a)==== Histogram equalization - using opencv "equalizeHist" ====
	Mat ocvHistEqualization;
	//TODO: implement your solution of here
	// ...
	
	
	//====(b)==== Histogram equalization - custom implementation ====
	Mat myHistEqualization(bonn_gray.size(), bonn_gray.type()); 

	// Count the frequency of intensities
	Vec<double,256> hist(0.0);
	for (int y=0; y < bonn_gray.rows; ++y) {
		for (int x=0;  x < bonn_gray.cols; ++x) {
			++hist[bonn_gray.at<uchar>(y,x)];
		}
	}
	// Normalize vector of new intensity values to sum up to a sum of 255
	hist *= 255. / sum(hist)[0];
	
	// Compute integral histogram - representing the new intensity values
	for (size_t i=1; i < 256; ++i) {
		hist[i] += hist[i-1];
	}
	
	// TODO: Fill the matrix "myHistEqualization" -> replace old intensity values with new intensities taken from the integral histogram
	// ...
	
	// TODO: Show the results of (a) and (b)
	// ...
	
	//====(c)==== Difference between openCV implementation and custom implementation ====
	// TODO: Compute absolute differences between pixel intensities of (a) and (b) using "absdiff"
	// ... Just uncomment the following lines:
	// Mat diff;
	// absdiff(myHistEqualization, ocvHistEqualization, diff);
	// double minVal, maxVal;
	// minMaxLoc(diff, &minVal, &maxVal);
	// cout << "maximum pixel error: " << maxVal << endl;

	//waitKey(0);
	//destroyAllWindows();
	
//	=========================================================================	
//	==================== Solution of task 4 =================================
//	=========================================================================
	cout << "Task 4:" << endl;
	Mat img_gb;
	Mat img_f2D;
	Mat img_sepF2D;
	const double sigma = 2. * sqrt(2.);

    //  ====(a)==== 2D Filtering - using opencv "GaussianBlur()" ====
	tick = getTickCount();
	// TODO: implement your solution here:
	// ...
	tock = getTickCount();
	cout << "OpenCV GaussianBlur() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

    //  ====(b)==== 2D Filtering - using opencv "filter2D()" ==== 
	// Compute gaussian kernel manually
	const int k_width = 3.5 * sigma;
	tick = getTickCount();
	Matx<float, 2*k_width+1, 2*k_width+1> kernel2D;
	for (int y = 0; y < kernel2D.rows; ++y) {
		const int dy = abs(k_width - y);
		for (int x = 0; x < kernel2D.cols; ++x) {
			const int dx = abs(k_width-x);
			//TODO: Fill kernel2D matrix with values of a gaussian
			// ...
		}
	}
	kernel2D *= 1. / sum(kernel2D)[0];
	
	// TODO: implement your solution here - use "filter2D"
	// ...
	
	tock = getTickCount();
	cout << "OpenCV filter2D() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

    //  ====(c)==== 2D Filtering - using opencv "sepFilter2D()" ====
	tick = getTickCount();
	// TODO: implement your solution here
	
	tock = getTickCount();
	cout << "OpenCV sepFilter2D() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

	// TODO: Show result images
	// ...

	// compare blurring methods
	// TODO: Compute absolute differences between pixel intensities of (a), (b) and (c) using "absdiff"
	// ...
		
	// TODO: Find the maximum pixel error using "minMaxLoc"
	// ...

	//waitKey(0);
	//destroyAllWindows();	
	
//	=========================================================================	
//	==================== Solution of task 6 =================================
//	=========================================================================
	cout << "Task 6:" << endl;
    //  ====(a)==================================================================
	// TODO: implement your solution here
	// ...
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	// ...	
	
	
	//waitKey(0);
	//destroyAllWindows();	

//	=========================================================================	
//	==================== Solution of task 7 =================================
//	=========================================================================	
	cout << "Task 7:" << endl;
	// Create an image with salt and pepper noise
	Mat bonn_salt_pepper(bonn_gray.size(), bonn_gray.type());
	randu(bonn_salt_pepper, 0, 100); // Generates an array of random numbers
	for (int y = 0; y < bonn_gray.rows; ++y) {
		for (int x=0; x < bonn_gray.cols; ++x) {
			uchar& pix = bonn_salt_pepper.at<uchar>(y,x);
			if (pix < 15) {
			  // TODO: Set set pixel "pix" to black
			}else if (pix >= 85) { 
			  // TODO: Set set pixel "pix" to white
			}else { 
			  // TODO: Set set pixel "pix" to its corresponding intensity value in bonn_gray
			}
		}
	}
	// imshow("bonn.png with salt and pepper", bonn_salt_pepper);	
    //  ====(a)==================================================================
	// TODO: implement your solution here
	// ...
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	// ...		
    //  ====(c)==================================================================	
	// TODO: implement your solution here
	// ...		
	
	//waitKey(0);
	//destroyAllWindows();	
//	=========================================================================	
//	==================== Solution of task 8 =================================
//	=========================================================================	
	cout << "Task 8:" << endl;
	// Declare Kernels
	Mat kernel1 = (Mat_<float>(3,3) << 0.0113, 0.0838, 0.0113, 0.0838, 0.6193, 0.0838, 0.0113, 0.0838, 0.0113);	
	Mat kernel2 = (Mat_<float>(3,3) << -0.8984, 0.1472, 1.1410, -1.9075, 0.1566, 2.1359, -0.8659, 0.0573, 1.0337);
	
    //  ====(a)==================================================================
	// TODO: implement your solution here
	// ...
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	// ...		
    //  ====(c)==================================================================	
	// TODO: implement your solution here
	// ...			
	
	
	
	
	
  cout << "Program finished successfully" << endl;
  //destroyAllWindows(); 
  return 0;
}
