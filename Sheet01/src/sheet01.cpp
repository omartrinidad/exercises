#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;


void showHistogram(Mat& img, Mat& histogram)
{
    int bins = 256;             // number of bins
    int nc = img.channels();    // number of channels
    vector<Mat> hist(nc);       // array for storing the histograms
    vector<Mat> canvas(nc);     // images for displaying the histogram
    int hmax[3] = {0,0,0};      // peak value for each histogram

    for (int i = 0; i < hist.size(); i++)
        hist[i] = Mat::zeros(1, bins, CV_32SC1);
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            for (int k = 0; k < nc; k++)
            {
                uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
                hist[k].at<int>(val) += 1;
            }
        }
    }
    
//    cout << hist[0].size() << endl;
//    cout << hist[0] << endl;
    
    
    for (int i = 0; i < nc; i++)
    {
        for (int j = 0; j < bins-1; j++)
            hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
    }
    const char* wname[3] = { "blue", "green", "red" };
    Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

    for (int i = 0; i < nc; i++)
    {
        canvas[i] = Mat::ones(125, bins, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
        {
            line(
                canvas[i], 
                Point(j, rows), 
                Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])), 
                nc == 1 ? Scalar(200,200,200) : colors[i], 
                1, 8, 0
            );
        }
        
        canvas[i].copyTo(histogram);
        //imshow(nc == 1 ? "value" : wname[i], canvas[i]);
    }
}


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
        r = Rect(Point(x1, y1), Point(x2, y2));
        // Drawing rectangles Randomly
        rectangle(img_task1, r, Scalar(255, 0, 0), 1);
	}
	//TODO: Display the image "img_task1" with the rectangles
    imshow("Rectangles", img_task1);
	waitKey(0);
    // ====(a)==== complete

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
//cout << bonn_gray(r) << endl;
//printf("y: %d , height: %d , x: %d , width: %d\n", r.y, r.height, r.x, r.width);
			for (int y = r.y; y < r.y + r.height; ++y) {
				for (int x = r.x; x < r.x+r.width; ++x) {
//printf("x %d , y %d\n", x, y);
				  	//TODO: Sum up all pixels at "bonn_gray.at<uchar>(y,x)" inside the rectangle and store it in "sumOfPixels"
					sumOfPixels += bonn_gray.at<uchar>(y,x);
				}
			}
		}
	}
    
	tock = getTickCount();
    cout << "Task(1-b)" << endl;
	cout << "Summing up each pixel gives " << sumOfPixels << " computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;

	// //====(c)==== integral image method - using OpenCV function "integral" ==== 
	// ref.: http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#integral
	//TODO: implement your solution of here
	tick = getTickCount();
    for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
		for (auto& r : rects) {
            Mat roi = bonn_gray(r);//Rect(pt1, pt2));
            Mat result;
            integral(roi, result, -1);
            //cout << result << endl;
            //imshow("integral", result);
            sumOfPixels += result.at<int>(result.size().height-1, result.size().width-1);
		}
	}
	tock = getTickCount();
    cout << "Task(1-c)" << endl;
	//cout << "An integral image computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;	
    cout << "Computing integral images and the value is " << sumOfPixels << " computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;
    
	//====(d)==== integral image method - custom implementation====
    //TODO: implement your solution here
	tick = getTickCount();
    for (size_t n = 0; n < 10; ++n) {
		sumOfPixels = 0.0;
        for (auto& r : rects) {
            Mat roi = bonn_gray(r);
            Mat result_custom;
            result_custom = Mat::zeros(roi.size().height+1, roi.size().width+1, CV_32S);
            
            //cout << "1" << endl;
            
            for (int y = 1; y < roi.size().height+1; y++) {
                for (int x = 1; x < roi.size().width+1; x++) {
                    result_custom.at<int>(y,x) = roi.at<uchar>(y-1,x-1) 
                        + result_custom.at<int>(y-1,x) + result_custom.at<int>(y,x-1) 
                        - result_custom.at<int>(y-1,x-1);            
                }
            }
            //imshow("custome integral", result_custom);
//            cout << result_custom << endl;
            sumOfPixels += result_custom.at<int>(result_custom.size().height-1, result_custom.size().width-1);
		}
	}
	tock = getTickCount();
    cout << "Task(1-d)" << endl;
	cout << "Computing integral images - custom implementation - and the value is " << sumOfPixels << " computed in " << (tock-tick)/getTickFrequency() << " seconds." << endl;

	waitKey(0); // waits until the user presses a button and then continues with task 2 -> uncomment this
	destroyAllWindows(); // closes all open windows -> uncomment this

//	=========================================================================	
//	==================== Solution of task 2 =================================
//	=========================================================================
	cout << "Task 2:" << endl;
	
	
	//====(a)==== Histogram equalization - using opencv "equalizeHist" ====
	Mat ocvHistEqualization;
    Mat hist1, hist2;
	// ref.: http://docs.opencv.org/2.4/modules/imgproc/doc/histograms.html?highlight=equalizehist#equalizehist
    //TODO: implement your solution of here
    
    equalizeHist(bonn_gray, ocvHistEqualization);
    //imshow("original", bonn_gray);
    //imshow("(a)Histogram equalization - using opencv \"equalizeHist\"", ocvHistEqualization);
    //showHistogram(bonn_gray, hist1);
    //showHistogram(ocvHistEqualization, hist2);
    //imshow("his1", hist1);
    //imshow("his2", hist2);
    
    
    
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
//    cout << "===============after calc==============" << endl;
//	cout << hist << endl;
	// Compute integral histogram - representing the new intensity values
	for (size_t i=1; i < 256; ++i) {
		hist[i] += hist[i-1];
	}
    
//    cout << "===============final calc==============" << endl;
//	cout << hist << endl;


	// TODO: Fill the matrix "myHistEqualization" -> replace old intensity values with new intensities taken from the integral histogram
	for (int y = 0; y < bonn_gray.rows; ++y) {
        for (int x=0;  x < bonn_gray.cols; ++x) {
            int pixel = bonn_gray.at<uchar>(y,x);
            myHistEqualization.at<uchar>(y,x) = hist[pixel];
		}
    }

	// TODO: Show the results of (a) and (b)
    imshow("(a)Histogram equalization - using opencv \"equalizeHist\"", ocvHistEqualization);
    imshow("myHistEq", myHistEqualization);
    //Mat test = ocvHistEqualization - myHistEqualization;
    //imshow("test", test);
    waitKey(0);
	

	//====(c)==== Difference between openCV implementation and custom implementation ====
	// TODO: Compute absolute differences between pixel intensities of (a) and (b) using "absdiff"
	// ... Just uncomment the following lines:
    Mat diff;
    absdiff(myHistEqualization, ocvHistEqualization, diff);
    //cout << diff << endl;
    double minVal, maxVal;
    minMaxLoc(diff, &minVal, &maxVal);
    cout << "maximum pixel error: " << maxVal << endl;
    
	waitKey(0);
	destroyAllWindows();

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
	GaussianBlur(bonn_gray, img_gb, Size(0,0), sigma, 0, 0 );

    tock = getTickCount();
	cout << "OpenCV GaussianBlur() method takes " << (tock-tick)/getTickFrequency() << " seconds." << endl;

    imshow("gaussianblur", img_gb);
    waitKey(0);

    //  ====(b)==== 2D Filtering - using opencv "filter2D()" ==== 
	// Compute gaussian kernel manually
	const int k_width = (int)9.899494936611665;//3.5 * sigma;
	tick = getTickCount();
	
    //Matx<float, 2*k_width+1, 2*k_width+1> kernel2D;
    Matx<float, k_width, k_width> kernel2D;
	
    //kernel2D = getGaussianKernel(1, 1, CV_32F);
    cout << kernel2D << endl;
    for (int y = 0; y < kernel2D.rows; ++y) {
		const int dy = abs(k_width - y);
		for (int x = 0; x < kernel2D.cols; ++x) {
			const int dx = abs(k_width-x);
			
            //TODO: Fill kernel2D matrix with values of a gaussian
			//kernel2D(y,x) = 1/(2. * M_PI * pow(sigma, 2.0)) * pow(exp(1),-((pow(x,2)+pow(y,2))/(2*pow(sigma,2))));
		}
	}
    //cout << kernel2D << endl;
//	kernel2D *= 1. / sum(kernel2D)[0];
/*	
	// TODO: implement your solution here - use "filter2D"
	// ...
	filter2D(bonn_gray, img_f2D, bonn_gray.depth(), kernel2D, Point anchor=Point(-1,-1), double delta=0, int borderType=BORDER_DEFAULT 
    
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
*/
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
	GaussianBlur(bonn_salt_pepper, bonn_salt_pepper_gb, Size(0,0), 1, 0, 0 );
    imshow("bonn.png with salt and pepper Gaussian", bonn_salt_pepper_gb);	
    //  ====(b)==================================================================	
	// TODO: implement your solution here
	medianBlur(bonn_salt_pepper, bonn_salt_pepper_md, 5);
    imshow("bonn.png with salt and pepper Median", bonn_salt_pepper_md);	
    //  ====(c)==================================================================	
	// TODO: implement your solution here
	bilateralFilter(bonn_salt_pepper, bonn_salt_pepper_bf, 30, 90, 90 ) ;
	imshow("bonn.png with salt and pepper bilateral", bonn_salt_pepper_bf);	
	waitKey(0);
	//destroyAllWindows();	
/*    
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
*/
    return 0;
}
