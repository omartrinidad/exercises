#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;

/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com
# Exercise Sheet 01
	Please read the "README" file to compile and execute this source code.
=========================================*/

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
	cout << endl << "Task 1:" << endl;
	//====(a)==== draw random rectangles ====
	
	// Create a copy of the input image to draw the rectangles into
	Mat img_task1;
	bonn_gray.copyTo(img_task1);

	// Create a random number generator
	RNG rng(getTickCount());
	Rect rects[100];
	// iterate i times to draw rectangles
    for (auto& r : rects) {
		// Random coordinates of the rectangle
		const int x1 = rng.uniform(0, bonn_gray.cols);
		const int x2 = rng.uniform(0, bonn_gray.cols);
		const int y1 = rng.uniform(0, bonn_gray.rows);
		const int y2 = rng.uniform(0, bonn_gray.rows);
        // TODO: Drawing rectangles Randomly - complete
		// store randomly created rectangles and draw them on "img_task1" image.
        r = Rect(Point(x1, y1), Point(x2, y2));
        rectangle(img_task1, r, Scalar(255, 0, 0), 1);
	}
	//TODO: Display the image "img_task1" with the rectangles - complete
    imshow("img_task1 - 100 rectrangles", img_task1);

	//====(b)==== summing up method ====
	// Variables for time measurements
	int64_t tick, tock;
	double sumOfPixels;		// sum of pixels inside of rectangles

	// Measure the time
	tick = getTickCount();
	
	// Repeat 10 times
	for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
			for (int y = r.y; y < r.y + r.height; ++y) {
				for (int x = r.x; x < r.x+r.width; ++x) {
				  	//TODO: Sum up all pixels at "bonn_gray.at<uchar>(y,x)" inside the rectangle and store it in "sumOfPixels" - complete
					sumOfPixels += bonn_gray.at<uchar>(y,x);
				}
			}
		}
	}
	tock = getTickCount(); // Measure the end time
    cout << " Summing up each pixel gives " << sumOfPixels << " computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl << endl;

	//====(c)==== integral image method - using OpenCV function "integral" ==== 
	//TODO: implement your solution of here - complete
	tick = getTickCount();		// Measure the time
    for (size_t n = 0; n < 10; ++n) {		// Repeat 10 times
		sumOfPixels = 0.0;
		for (auto& r : rects) {
            Mat rectRoi = bonn_gray(r);		// get the patch of image (area : the rectangle)
            Mat integralImg;	// integral image matrix
            // using function "integral" to get integral image 
			integral(rectRoi, integralImg, -1);
			// get the sumOfPixel value from the last pixel of the integral image 
            sumOfPixels += integralImg.at<int>(integralImg.size().height-1, integralImg.size().width-1);
		}
	}
	tock = getTickCount();	// Measure end time
    
    cout << " Computing integral images computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;
	cout << " The integral value is " << sumOfPixels << endl << endl;
    
	//====(d)==== integral image method - custom implementation====
    //TODO: implement your solution here - complete
	tick = getTickCount();		// Measure the time
    for (size_t n = 0; n < 10; ++n) {		// Repeat 10 times
		sumOfPixels = 0.0;
        for (auto& r : rects) {
            Mat rectRoi = bonn_gray(r);		// get the patch of image (area : the rectangle)
            Mat custom_IntegralImg;		// custom implemented integral image
            // Assign an ( h+1 x w+1 ) Matrix which is filled by 0, where h and w is the height and width of rectRoi.
			// The function "integral" creates the destination image which has 1 more pixel over source image in terms of width and height    
			custom_IntegralImg = Mat::zeros(rectRoi.size().height+1, rectRoi.size().width+1, CV_32S);
            
			
			
            for (int y = 1; y < custom_IntegralImg.size().height; y++) {
                for (int x = 1; x < custom_IntegralImg.size().width; x++) {
					// formula: integral value (y,x) = integral(y-1,x) + integral(y,x-1) - integral(y-1,x-1) + source(y,x)
					// since the computing process of opencv "integral" function is a little different from the formula, adjusted the point for the source value.   
                    custom_IntegralImg.at<int>(y,x) = rectRoi.at<uchar>(y-1,x-1) 
                        + custom_IntegralImg.at<int>(y-1,x) + custom_IntegralImg.at<int>(y,x-1) 
                        - custom_IntegralImg.at<int>(y-1,x-1);            
                }
            }
			// get the sumOfPixel value from the last pixel of the customed integral image
            sumOfPixels += custom_IntegralImg.at<int>(custom_IntegralImg.size().height-1, custom_IntegralImg.size().width-1);
		}
	}
	tock = getTickCount();
    cout << " Computing customed integral images computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;
	cout << " The integral value is " << sumOfPixels << endl << endl;
	
	waitKey(0); // waits until the user presses a button and then continues with task 2 -> uncomment this
	destroyAllWindows(); // closes all open windows -> uncomment this


//	=========================================================================	
//	==================== Solution of task 2 =================================
//	=========================================================================
	cout << "Task 2:" << endl;
	
	//====(a)==== Histogram equalization - using opencv "equalizeHist" ====
	Mat ocvHistEqualization;
	//TODO: implement your solution of here - complete
	// Using opencv function "equalizeHist". Result stored into ocvHistEqualization.
    equalizeHist(bonn_gray, ocvHistEqualization);    
    
    
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
    
	// TODO: Fill the matrix "myHistEqualization" -> replace old intensity values with new intensities taken from the integral histogram - complete
	for (int y = 0; y < bonn_gray.rows; ++y) {
        for (int x=0;  x < bonn_gray.cols; ++x) {
            int pixel = bonn_gray.at<uchar>(y,x);
            myHistEqualization.at<uchar>(y,x) = hist[pixel];
		}
    }

	// TODO: Show the results of (a) and (b) - complete
    imshow("(a) Histogram equalization - using opencv \"equalizeHist\"", ocvHistEqualization);
    imshow("(b) Histogram equalization - custom implementation", myHistEqualization);

	//====(c)==== Difference between openCV implementation and custom implementation ====
	// TODO: Compute absolute differences between pixel intensities of (a) and (b) using "absdiff" - complete
	// ... Just uncomment the following lines:
    Mat diff;	// matrix of differences between two images
    absdiff(myHistEqualization, ocvHistEqualization, diff);
    
    double minVal, maxVal;
    minMaxLoc(diff, &minVal, &maxVal);
	cout << " Maximum pixel error: " << maxVal << endl;
	cout << " * We guess that the pixel errors between the opencv function driven image and custom implemented image are because of the rounding method." << endl << endl;

	waitKey(0);
	destroyAllWindows();

//	=========================================================================	
//	==================== Solution of task 4 =================================
//	=========================================================================
	cout << "Task 4:" << endl;
	Mat img_gb;		// GaussianBlur function output image
	Mat img_f2D;	// filter2D function output image
	Mat img_sepF2D; // sepFilter2D function output image
	const double sigma = 2. * sqrt(2.);

    //  ====(a)==== 2D Filtering - using opencv "GaussianBlur()" ====
	tick = getTickCount();
	// TODO: implement your solution here:
	// GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT )
	// 		- ksize: if 0, then computed from sigma
	GaussianBlur(bonn_gray, img_gb, Size(0,0), sigma, 0, BORDER_DEFAULT);

    tock = getTickCount();
	cout << " OpenCV GaussianBlur() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

    //  ====(b)==== 2D Filtering - using opencv "filter2D()" ==== 
	// Compute gaussian kernel manually
	
	// compile error occurs when we assign "3.5 * sigma" into constant k_width - while define Matx<> kernel2D.
	// so we changed the value of k_width as 9 = (int)3.5 * sigma;
	const int k_width = 9;//3.5 * sigma;
	tick = getTickCount();
	
    Matx<float, 2*k_width+1, 2*k_width+1> kernel2D;
    for (int y = 0; y < kernel2D.rows; ++y) {
		const int dy = abs(k_width - y);
		for (int x = 0; x < kernel2D.cols; ++x) {
			const int dx = abs(k_width-x);
			
            //TODO: Fill kernel2D matrix with values of a gaussian - complete
			// adapt the gaussian kernel formula
			kernel2D(y,x) = ( exp( -1 * ( pow(dx,2) + pow(dy,2) ) / (2 * pow(sigma,2) ) ) / ( 2*M_PI*pow(sigma,2) ) );
		}
	}
	
	kernel2D *= 1. / sum(kernel2D)[0];	// to rearrange the values as to the sum into 1

	// TODO: implement your solution here - use "filter2D" - complete
	filter2D(bonn_gray, img_f2D, bonn_gray.depth(), kernel2D, Point(-1,-1), 0, BORDER_DEFAULT); 
    
	tock = getTickCount();
	cout << "OpenCV filter2D() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

    //  ====(c)==== 2D Filtering - using opencv "sepFilter2D()" ====
	tick = getTickCount();
	// TODO: implement your solution here
	Matx<float, 1, 2*k_width+1> kernelX;
	
	//kernelX = getGaussianKernel(19, sigma, CV_64F );
	
	//compute gaussian 1D kernel for X
	for (int x = 0; x < kernelX.cols; ++x) {
		const int dx = abs(k_width-x);
		kernelX(0, x) =  ( exp( -1 * pow(dx,2)/(2*pow(sigma,2)) ) / (sqrt(2*M_PI) * sigma) );
	}
	kernelX *= 1. / sum(kernelX)[0];	// to rearrange the values as to the sum into 1
	
	// gaussian filtering - using "sepFilter2D" - complete
	// since the width and height are same, only kernelX is used for production
	sepFilter2D(bonn_gray, img_sepF2D, bonn_gray.depth(), kernelX, kernelX, Point(-1,-1), 0, BORDER_DEFAULT );
	
	tock = getTickCount();
	cout << "OpenCV sepFilter2D() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl << endl;

	// TODO: Show result images
	imshow("(a) 2D Filtering - GaussianBlur()", img_gb);
	imshow("(b) 2D Filtering - filter2D()", img_f2D);
	imshow("(c) 2D Filtering - sepFilter2D()", img_sepF2D);
	

	// compare blurring methods
	// TODO: Compute absolute differences between pixel intensities of (a), (b) and (c) using "absdiff"
    Mat diffAB;	// differences between between pixel intensities of (a), (b)
	Mat diffBC;	// differences between between pixel intensities of (b), (c)
	Mat diffAC;	// differences between between pixel intensities of (a), (c)
    absdiff(img_gb, img_f2D, diffAB);
	absdiff(img_f2D, img_sepF2D, diffBC);
	absdiff(img_gb, img_sepF2D, diffAC);
    		
	// TODO: Find the maximum pixel error using "minMaxLoc"
    double maxValAB, maxValBC, maxValAC;
    minMaxLoc(diffAB, &minVal, &maxValAB);
	minMaxLoc(diffAB, &minVal, &maxValBC);
	minMaxLoc(diffAB, &minVal, &maxValAC);
    cout << " Maximum pixel error between (a) and (b): " << maxValAB << endl;
	cout << " Maximum pixel error between (b) and (c): " << maxValBC << endl;
	cout << " Maximum pixel error between (a) and (c): " << maxValAC << endl;
	cout << endl;

	waitKey(0);
	destroyAllWindows();	
	
//	=========================================================================	
//	==================== Solution of task 6 =================================
//	=========================================================================
	cout << "Task 6:" << endl;
    //  ====(a)==================================================================
	// TODO: implement your solution here
	Mat img_gb_a1, img_gb_a2;		// GaussianBlur function output image
	const double sigma_task6a = 2.;
	GaussianBlur(bonn_gray, img_gb_a1, Size(0,0), sigma_task6a, 0, BORDER_DEFAULT);
	GaussianBlur(img_gb_a1, img_gb_a2, Size(0,0), sigma_task6a, 0, BORDER_DEFAULT);
	
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	Mat img_gb_b;		// GaussianBlur function output image
	const double sigma_task6b = 2. * sqrt(2.);
	GaussianBlur(bonn_gray, img_gb_b, Size(0,0), sigma_task6b, 0, BORDER_DEFAULT);

	// display results
	imshow("(a)", img_gb_a2);
	imshow("(b)", img_gb_b);
	 	
    absdiff(img_gb_a2, img_gb_b, diff);
	minMaxLoc(diff, &minVal, &maxVal);
    cout << " Maximum pixel error between (a) and (b): " << maxVal << endl << endl;
	
	waitKey(0);
	destroyAllWindows();	

//	=========================================================================	
//	==================== Solution of task 7 =================================
//	=========================================================================	
	cout << "Task 7:" << endl;
	// Create an image with salt and pepper noise
	Mat bonn_salt_pepper(bonn_gray.size(), bonn_gray.type());
    Mat bonn_salt_pepper_gb;
    Mat bonn_salt_pepper_md;
    Mat bonn_salt_pepper_bf;
	
	randu(bonn_salt_pepper, 0, 100); // Generates an array of random numbers
	for (int y = 0; y < bonn_gray.rows; ++y) {
		for (int x=0; x < bonn_gray.cols; ++x) {
			uchar& pix = bonn_salt_pepper.at<uchar>(y,x);
			if (pix < 15) {
			  // TODO: Set set pixel "pix" to black
                pix = 255;
			}else if (pix >= 85) { 
			  // TODO: Set set pixel "pix" to white
                pix = 0;
			}else { 
			  // TODO: Set set pixel "pix" to its corresponding intensity value in bonn_gray
                pix = bonn_gray.at<uchar>(y,x);
			}
		}
	}
    imshow("bonn.png with salt and pepper", bonn_salt_pepper);	
    
    //  ====(a)==================================================================
	// TODO: implement your solution here
	// to get the best visual result, adjusted sigma value.
	GaussianBlur(bonn_salt_pepper, bonn_salt_pepper_gb, Size(0,0), 1.2, 0, 0 );
    imshow("bonn.png with salt and pepper Gaussian", bonn_salt_pepper_gb);	
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	// to get the best visual result, adjusted kernel size
	medianBlur(bonn_salt_pepper, bonn_salt_pepper_md, 5);
    imshow("bonn.png with salt and pepper Median", bonn_salt_pepper_md);	
    //  ====(c)==================================================================	
	// TODO: implement your solution here
	//bilateralFilter(InputArray src, OutputArray dst, int d, double sigmaColor, double sigmaSpace, int borderType=BORDER_DEFAULT )
	// to get the best visual result, adjusted d (diameter of each pixel) and sigma color
	bilateralFilter(bonn_salt_pepper, bonn_salt_pepper_bf, 9, 255, 0 ) ;
	imshow("bonn.png with salt and pepper bilateral", bonn_salt_pepper_bf);	
	
	waitKey(0);
	destroyAllWindows();	
  
//	=========================================================================	
//	==================== Solution of task 8 =================================
//	=========================================================================	
	cout << endl << "Task 8:" << endl;
	// Declare Kernels
	Mat kernel1 = (Mat_<float>(3,3) << 0.0113, 0.0838, 0.0113, 0.0838, 0.6193, 0.0838, 0.0113, 0.0838, 0.0113);	
	Mat kernel2 = (Mat_<float>(3,3) << -0.8984, 0.1472, 1.1410, -1.9075, 0.1566, 2.1359, -0.8659, 0.0573, 1.0337);
    //  ====(a)==================================================================
	// TODO: implement your solution here
	// filter the image using "filter2D"
	Mat img_f2D_k1, img_f2D_k2;
	filter2D(bonn_gray, img_f2D_k1, bonn_gray.depth(), kernel1, Point(-1,-1), 0, BORDER_DEFAULT);
	filter2D(bonn_gray, img_f2D_k2, bonn_gray.depth(), kernel2, Point(-1,-1), 0, BORDER_DEFAULT); 
     
	//display the result images
	imshow("filter2D - Kernel1", img_f2D_k1);
	imshow("filter2D - Kernel2", img_f2D_k2);
	
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	// seperate each kernel with opencv SVD class
	Mat img_sepF2D_k1, img_sepF2D_k2;
	
	// ======seperate kernal1 using SVD 
	SVD kernel1_svd = SVD(kernel1, 0);
	Mat u1, vt1;
	
	// kernel X
	u1 = kernel1_svd.u.col(0);
	u1 *= sqrt( kernel1_svd.w.at<float>(0,0) );	

	// kernel Y
	vt1 = kernel1_svd.vt.row(0);
	vt1 *= sqrt( kernel1_svd.w.at<float>(0,0) );

	// apply sepFilter2D with 2 seperated kernels.
	sepFilter2D(bonn_gray, img_sepF2D_k1, bonn_gray.depth(), u1, vt1, Point(-1,-1), 0, BORDER_DEFAULT );


	// ======seperate kernal2 using SVD 
	SVD kernel2_svd = SVD(kernel2, 0);
	Mat u2, vt2; 
	
	cout << "kernel2 - w" << endl;
	cout << kernel2_svd.w << endl << endl;
	
	// kernel X
	u2 = kernel2_svd.u.col(0);
	u2 *= sqrt( kernel2_svd.w.at<float>(0,0) );
	
	// kernel Y
	vt2 = kernel2_svd.vt.row(0);
	vt2 *= sqrt( kernel2_svd.w.at<float>(0,0) );
	
	// apply sepFilter2D with 2 seperated kernels.
	sepFilter2D(bonn_gray, img_sepF2D_k2, bonn_gray.depth(), u2, vt2, Point(-1,-1), 0, BORDER_DEFAULT );
	
	// display filtered result		
	imshow("sepfilter2D - Kernel1", img_sepF2D_k1);
	imshow("sepfilter2D - Kernel2", img_sepF2D_k2);
	
			
    //  ====(c)==================================================================	
	// TODO: implement your solution here
	// compare blurring methods
	// TODO: Compute absolute differences between pixel intensities of (a), (b) and (c) using "absdiff"
    Mat diff8_k1;	// differences between between pixel intensities
	Mat diff8_k2;	// differences between between pixel intensities
    absdiff(img_f2D_k1, img_sepF2D_k1, diff8_k1);
	absdiff(img_f2D_k2, img_sepF2D_k2, diff8_k2);
	
	//imshow("diff1", diff8_k1);
	//imshow("diff2", diff8_k2);
			
	// TODO: Find the maximum pixel error using "minMaxLoc"
    double maxVal8_k1, maxVal8_k2;
    minMaxLoc(diff8_k1, &minVal, &maxVal8_k1);
	minMaxLoc(diff8_k2, &minVal, &maxVal8_k2);
    cout << " Maximum pixel error between filtered images using kernel1 and seperated kernel1 " << maxVal8_k1 << endl;
	cout << " Maximum pixel error between filtered images using kernel2 and seperated kernel2 " << maxVal8_k2 << endl;
	cout << endl;

	waitKey(0);
	cout << "Program finished successfully" << endl;
	destroyAllWindows(); 

    return 0;
}
