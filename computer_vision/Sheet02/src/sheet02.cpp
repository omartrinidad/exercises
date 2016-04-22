#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace std;
using namespace cv;

/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com
# Exercise Sheet 02
	Please read the "README" file to compile and execute this source code.
=========================================*/
void part1();
void part2();
void part3();
void part4();
void part5();

void drawArrow(cv::Mat &image, cv::Point p, cv::Scalar color, double scaleMagnitude, double scaleArrowHead, double magnitube, double orientationDegrees);
void matchingImage(Mat& img, Mat& matching, int y, int x, int rows, int cols);

void displayPyramid(vector<cv::Mat> pyr);
void displayPyramid(vector<cv::Mat> pyr, string title);
void buildGaussianPyramid(vector<cv::Mat> &gpyr, Mat img);
void buildGaussianPyramid_implement(vector<cv::Mat> &gpyr, Mat img);
void buildLaplacianPyramid(const vector<cv::Mat> gpyr, vector<cv::Mat> &lpyr);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[])
{
    // Uncomment the part of the exercise that you wish to implement.
    // For the final submission all implemented parts should be uncommented.
    part1();
    part2();
    part3();
    part4();
    part5();
    
    std::cout <<                                                            std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    END    //////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout <<                                                            std::endl;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void part1()
{
    std::cout <<                                                            std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    Part 1    ///////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;

    // read the image file
    cv::Mat im_Traffic_BGR = cv::imread("./images/traffic.jpg", cv::IMREAD_COLOR);
    // gray version of bonn.png
    cv::Mat                      im_Traffic_Gray;
    cv::cvtColor(im_Traffic_BGR, im_Traffic_Gray, CV_BGR2GRAY);

    // construct Gaussian pyramids
    std::vector<cv::Mat>   gpyr;    // this will hold the Gaussian Pyramid created with OpenCV::buildPyramid
    std::vector<cv::Mat> myGpyr;    // this will hold the Gaussian Pyramid created with your custom way

    // Please implement the pyramids as described in the exercise sheet, using the containers given above.
    // ** build a Gaussian pyramid using cv::pyrDown 
    buildGaussianPyramid(gpyr, im_Traffic_Gray);
    // ** build a Gaussian pyramid without using cv::pyrDown and cv::buildPyramid
    buildGaussianPyramid_implement(myGpyr, im_Traffic_Gray);
    
    // Perform the computations asked in the exercise sheet and show them using **std::cout**
    for (int i = 0; i < gpyr.size(); ++i ) {
        Mat diff;	// differences between between pixel intensities
        double minVal, maxVal;
        
        absdiff(gpyr[i], myGpyr[i], diff);  // compute pixel-wise error
        minMaxLoc(diff, &minVal, &maxVal);
        cout << "layer " << i << ": " << endl;
        cout << " Maximum pixel error: " << maxVal << endl << endl;
    }
    
    // Show every layer of the pyramid
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**
    displayPyramid(gpyr, "Gaussian Pyramid");
    waitKey(0);
    cv::destroyAllWindows();
    displayPyramid(myGpyr, "Custom Implemented Pyramid");
    waitKey(0);
    cv::destroyAllWindows();
    
    // For the laplacian pyramid you should define your own container.
    // If needed perform normalization of the image to be displayed
    std::vector<cv::Mat>   lpyr;
    buildLaplacianPyramid(gpyr, lpyr);  // building laplacian pyramid
    displayPyramid(lpyr, "Laplacian Pyramid");
    cv::waitKey(0);
    cv::destroyAllWindows();    
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void part2()
{
    std::cout <<                                                            std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    Part 2    ///////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;

    // apple and orange are CV_32FC3
    cv::Mat im_Apple, im_Orange;
    cv::imread("./images/apple.jpg",  cv::IMREAD_COLOR).convertTo(im_Apple,  CV_32FC3, (1./255.));
    cv::imread("./images/orange.jpg", cv::IMREAD_COLOR).convertTo(im_Orange, CV_32FC3, (1./255.));
    cv::imshow("apple",  im_Apple);
    cv::imshow("orange", im_Orange);
    std::cout << "\n" << "Input images" << "   \t\t\t\t\t\t\t" << "Press any key..." << std::endl;
    cv::waitKey(0);
    
    std::vector<cv::Mat>   gpApple, gpOrange;    // Gaussian pyramid for the apple and orange image
    std::vector<cv::Mat>   lpApple, lpOrange;    // Laplacian pyramid for the apple and orange image
    std::vector<cv::Mat>   lpBlend;    // Laplacian pyramid for the blended image
    // Mat im_blend;

    // Mask image and gaussian pyramid 
    Mat_<float> mask(im_Apple.rows,im_Apple.cols,0.0); //mask matrix filled with 0 
    mask(Range::all(),Range(0,mask.cols/2)) = 1.0;    // fill left half with 1.0
    
    Mat mask_3f;    // use 3-channel mask image to multiply easier        
    cvtColor(mask, mask_3f, CV_GRAY2BGR); 
    
    vector<Mat> gpMask;   

    // Perform the blending using a Laplacian Pyramid
    
    // Generate gaussian pyramids for apple, orange and mask images
    buildGaussianPyramid(gpApple, im_Apple);
    buildGaussianPyramid(gpOrange, im_Orange);
    buildGaussianPyramid(gpMask, mask_3f);
    // Generate laplacian pyramids for apple and orange images.
    buildLaplacianPyramid(gpApple, lpApple);
    buildLaplacianPyramid(gpOrange, lpOrange);
    
    // join the left half of apple and right half of orange in each levels of Laplacian Pyramids
    for (int i = 0; i < lpApple.size(); ++i) {
        Mat apple = lpApple[i];
        Mat orange = lpOrange[i];
        Mat blendImg;
        blendImg = apple.mul(gpMask[i]) + orange.mul(Scalar(1.0,1.0,1.0)-gpMask[i]);  
        
        lpBlend.push_back(blendImg);
    }
    // reconstruct the original image from laplacian pyramid
    for (int i = lpBlend.size()-1; i > 0; --i) {
        Mat up;
        pyrUp(lpBlend[i], up, lpBlend[i-1].size());
        lpBlend[i-1] = up + lpBlend[i-1];
    }
    
    // Show the blending results @ several layers
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**
    displayPyramid(lpBlend, "Blended Pyramid");
    waitKey(0);
    cv::destroyAllWindows();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void part3()
{
    std::cout <<                                                            std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    Part 3    ///////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;

    // traffic.jpg // circles.png
    cv::Mat im_Traffic_BGR = cv::imread("./images/traffic.jpg");

    // Blur to denoise
    cv::GaussianBlur(im_Traffic_BGR, im_Traffic_BGR, cv::Size(7,7), 0, 0, cv::BORDER_DEFAULT);

    // BGR to Gray
    cv::Mat im_Traffic_Gray;
    cv::cvtColor(im_Traffic_BGR, im_Traffic_Gray, CV_BGR2GRAY ); // cv::COLOR_BGR2GRAY // CV_BGR2GRAY

    // Perform the computations asked in the exercise sheet
    cv::Mat horizontal, vertical, sobel;
    cv::Mat magnitude_, angle_; 

    Sobel(im_Traffic_Gray, horizontal, CV_32F, 1, 0);
    Sobel(im_Traffic_Gray, vertical, CV_32F, 0, 1);
    //sobel = abs(vertical) + abs(horizontal);

    /* cartToPolar OpenCV
    cv::Mat magnitude, angle; 
    cartToPolar(horizontal, vertical, magnitude, angle);
    */

    /* cartToPolar2 own implementation */
    int rows = im_Traffic_Gray.rows;
    int cols = im_Traffic_Gray.cols;
    magnitude_ = Mat::zeros(rows, cols, CV_32F);
    angle_ = Mat::zeros(rows, cols, CV_32F);
    
    /* it is possible to improve this section with pointers */
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            float mag = sqrt(pow(horizontal.at<float>(y, x), 2) + pow(vertical.at<float>(y, x), 2));
            magnitude_.at<float>(y, x) = mag;
            float ang = atan2(horizontal.at<float>(y, x), vertical.at<float>(y, x)) * (180/M_PI);
            angle_.at<float>(y, x) = ang;
        }
    }

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**
    
    imshow("Magnitude", magnitude_);
    waitKey();
    imshow("Angle", angle_);
    waitKey();
    cv::destroyAllWindows();

    // Use the function **drawArrow** provided at the end of this file in order to
    // draw Vectors showing the Gradient Magnitude and Orientation
    // (to avoid clutter, draw every 10nth gradient,
    // only if the magnitude is above a threshold)
    
    /* Draw the gradients in the original image */
    Point point;
    for (int y = 0; y < rows; y+=10) {
        for (int x = 0; x < cols; x+=10) {

            point.x = x;
            point.y = y;

            if (magnitude_.at<float>(y, x) > 32) {
                double an = angle_.at<float>(y, x);
                drawArrow(im_Traffic_BGR, point, Scalar(0, 130, 255), 3.555, 1.20, 1.25, an);
            }
        }
    }

    imshow("Gradients", im_Traffic_BGR);
    waitKey(0);
    cv::destroyAllWindows();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void part4()
{
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    Part 4    ///////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;

    cv::Mat im_Traffic_BGR = cv::imread("./images/traffic.jpg");
    // BGR to Gray
    cv::Mat im_Traffic_Gray, im_Canny;
    cv::cvtColor( im_Traffic_BGR, im_Traffic_Gray, cv::COLOR_BGR2GRAY );

    // Perform edge detection as described in the exercise sheet
    Canny(im_Traffic_BGR, im_Canny, 10, 100, 3);

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitkey(), use **cv::destroyAllWindows()**
    
    imshow("gray", im_Traffic_Gray);
    imshow("canny", im_Canny);

    waitKey(0);
    cv::destroyAllWindows();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void part5()
{
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "////    Part 5    ///////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout << "/////////////////////////////////////////////////////" << std::endl;
    std::cout <<                                                            std::endl;

    // Read image, Blur to denoise
    cv::Mat im_Traffic_BGR = cv::imread("./images/traffic.jpg");
    cv::GaussianBlur(im_Traffic_BGR, im_Traffic_BGR,  cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

    // BGR to Gray
    cv::Mat im_Traffic_Gray, im_Edges;
    cv::cvtColor(im_Traffic_BGR, im_Traffic_Gray, cv::COLOR_BGR2GRAY);

    // Read Template
    cv::Mat im_Sign_BGR = cv::imread("./images/sign.png");
    cv::Mat im_Sign_Gray, im_Sign;
    // BGR to Gray
    cv::cvtColor( im_Sign_BGR, im_Sign_Gray, cv::COLOR_BGR2GRAY ); // cv::COLOR_BGR2GRAY // CV_BGR2GRAY
    
    Mat im_Traffic_Edge, im_Sign_Edge;
    Mat im_Traffic_distance;
    // Perform the steps described in the exercise sheet

    Canny(im_Traffic_Gray, im_Traffic_Edge, 10, 600, 3, false );
    resize(im_Sign_Gray, im_Sign_Gray, Size(70,60));    // rescale template image
    Canny(im_Sign_Gray, im_Sign_Edge, 10, 200, 3, false );

    distanceTransform(im_Traffic_Edge, im_Traffic_distance, CV_DIST_L1, DIST_MASK_3);
    
    Mat im_Traffic_distance_normalize;
    normalize(im_Traffic_distance, im_Traffic_distance_normalize, 0, 1., NORM_MINMAX);
    
    // display result images
    imshow("Sign_Edge", im_Sign_Edge);
    imshow("Traffic_distance_normalize", im_Traffic_distance_normalize);
    waitKey(0);

    // compute match score while sliding the image by pixel 
    Mat_<float> im_Voting_Space(im_Traffic_distance.rows,im_Traffic_distance.cols,0.0);
    int x_c = im_Sign_Edge.cols / 2;
    int y_c = im_Sign_Edge.rows / 2;
    cout << "Start computing chamfer matching" << endl;
    for (int y = 0; y < im_Traffic_distance.size().height - im_Sign_Edge.size().height; ++y) {
        for (int x = 0; x < im_Traffic_distance.size().width - im_Sign_Edge.size().width; ++x) {
            float score = 0;
            for (int y1 = 0; y1 < im_Sign_Edge.size().height; ++y1) {
                for (int x1 = 0; x1 < im_Sign_Edge.size().width; ++x1) {
                    if ((int)im_Sign_Edge.at<uchar>(y1,x1) > 0) {
                        score += im_Traffic_distance.at<float>(y+y1, x+x1) * (int)im_Sign_Edge.at<uchar>(y1,x1);
                    }
                }
            }
            im_Voting_Space.at<float>(y+y_c,x+x_c) = score;
        }
    }
    cout << "End computing chamfer matching" << endl;
    imshow("voting space", im_Voting_Space);
    

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**
    // If needed perform normalization of the image to be displayed

    waitKey(0);
    cv::destroyAllWindows();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Use this function for visualizations in part3
void drawArrow(cv::Mat &image, cv::Point p, cv::Scalar color, double scaleMagnitude, double scaleArrowHead, double magnitube, double orientationDegrees)
{
    int arrowHeadAngleCoeff = 10;
    magnitube *= scaleMagnitude;

    double theta = orientationDegrees * M_PI / 180; // rad
    cv::Point q;
    q.x = p.x + magnitube * cos(theta);
    q.y = p.y + magnitube * sin(theta);

    //Draw the principle line
    cv::line(image, p, q, color);

    //compute the angle alpha
    double angle = atan2((double)p.y-q.y, (double)p.x-q.x);
    //compute the coordinates of the first segment
    p.x = (int) ( q.x + static_cast<int>(  round( scaleArrowHead * cos(angle + M_PI/arrowHeadAngleCoeff)) )  );
    p.y = (int) ( q.y + static_cast<int>(  round( scaleArrowHead * sin(angle + M_PI/arrowHeadAngleCoeff)) )  );
    //Draw the first segment
    cv::line(image, p, q, color);

    //compute the coordinates of the second segment
    p.x = (int) ( q.x + static_cast<int>(  round( scaleArrowHead * cos(angle - M_PI/arrowHeadAngleCoeff)) )  );
    p.y = (int) ( q.y + static_cast<int>(  round( scaleArrowHead * sin(angle - M_PI/arrowHeadAngleCoeff)) )  );
    //Draw the second segment
    cv::line(image, p, q, color);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// these functions are used in part1 and part2

// diaply images in a pyramid
void displayPyramid(vector<cv::Mat> pyr) {
    displayPyramid(pyr, "Pyramid");
}
void displayPyramid(vector<cv::Mat> pyr, string title) {
    cout << title << endl; 
    for (int i = 0; i < pyr.size(); ++i ) {
        // cout << "Layer " << i << ":" << endl;
        // cout << "width: " << pyr[i].size().width << "  , height: " << pyr[i].size().height << endl;
        stringstream ess;
        ess << title << i;
        
        imshow(ess.str(), pyr[i]);
    }
    cout << endl;
}

// build a Gaussian pyramid
void buildGaussianPyramid(vector<cv::Mat> &gpyr, Mat img) {
    int maxLevel = -1;
    for (int min_length = min(img.size().width, img.size().height); min_length >= 8; min_length = min_length/2) {
        maxLevel ++;
    }
    
    // Generate gaussian pyramids using pyrDown()
    gpyr.push_back(img);    // original image
    for (int i = 0; i < maxLevel; ++i) {
        pyrDown(img, img);
        gpyr.push_back(img);    // put image into pyramid 
    }
}

// build a Gaussian pyramid without using pyrDown()
void buildGaussianPyramid_implement(vector<cv::Mat> &gpyr, Mat img) {
    int maxLevel = -1;
    for (int min_length = min(img.size().width, img.size().height); min_length >= 8; min_length = min_length/2) {
        maxLevel ++;
    }
    // Generate gaussian pyramids without using pyrDown()
    // Kernel: refered document of pyrDown()
    Mat kernel = (Mat_<float>(5,5) << 1, 4, 6, 4, 1,
                                        4, 16, 24, 16, 4,
                                        6, 24, 36, 24, 6,
                                        4, 16, 24, 16, 4,
                                        1, 4, 6, 4, 1);
    kernel = kernel / 256.;                                        
    
    gpyr.push_back(img);
    Mat nextLayerImg = img;
    
    for (int i = 0; i < maxLevel; ++i) {
        Mat tmp;
        // convolve the image with kernel
        filter2D(nextLayerImg, tmp, -1, kernel, Point(-1,-1), 0, BORDER_DEFAULT);

        // downsample
        int x = (nextLayerImg.size().width + 1) / 2;
        int y = (nextLayerImg.size().height + 1) / 2;        
        resize(tmp, tmp, Size(x,y));
        
        // put the result image into the pyramid
        nextLayerImg = tmp;
        gpyr.push_back(nextLayerImg);
    }
}

// build a Laplacian pyramid
void buildLaplacianPyramid(vector<cv::Mat> gpyr, vector<cv::Mat> &lpyr) {
    
    int maxLevel = gpyr.size() - 1;
    lpyr.insert(lpyr.begin(), gpyr[maxLevel]);
    
    for (int i = maxLevel; i > 0; --i) {
        Mat gpImg, gpUImg, lpImg, tmpImg;
        gpImg = gpyr[i];
        gpUImg = gpyr[i-1];
        
        pyrUp(gpImg, tmpImg, gpUImg.size(), BORDER_DEFAULT);
        subtract(gpUImg, tmpImg, lpImg);
        
        lpyr.insert(lpyr.begin(), lpImg);        
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

