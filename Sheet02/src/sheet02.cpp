#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
    //part3();
    //part4();
    // part5();
    
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
    displayPyramid(myGpyr, "Custom Implemented Pyramid");

    // For the laplacian pyramid you should define your own container.
    // If needed perform normalization of the image to be displayed
    std::vector<cv::Mat>   lpyr;
    buildLaplacianPyramid(gpyr, lpyr);
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
    Mat_<float> mask(im_Apple.rows,im_Apple.cols,0.0); //weights matrix filled with 0 
    mask(Range::all(),Range(0,mask.cols/2)) = 1.0;    // fill left half with 1.0
    
    Mat mask_3f;    // 3-channel mask image        
    cvtColor(mask, mask_3f, CV_GRAY2BGR); 
    
    vector<Mat> gpMask;   

    // Perform the blending using a Laplacian Pyramid
    
    // Generate gaussian pyramids for apple and orange images.
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
    displayPyramid(lpBlend);
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

    cv::Mat           im_Traffic_BGR = cv::imread("./images/traffic.jpg"); // traffic.jpg // circles.png
    // Blur to denoise
    cv::GaussianBlur( im_Traffic_BGR, im_Traffic_BGR, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    // BGR to Gray
    cv::Mat                           im_Traffic_Gray;
    cv::cvtColor(     im_Traffic_BGR, im_Traffic_Gray, cv::COLOR_BGR2GRAY ); // cv::COLOR_BGR2GRAY // CV_BGR2GRAY

    // Perform the computations asked in the exercise sheet

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**

    // Use the function **drawArrow** provided at the end of this file in order to
    // draw Vectors showing the Gradient Magnitude and Orientation
    // (to avoid clutter, draw every 10nth gradient,
    // only if the magnitude is above a threshold)

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
    cv::Mat                       im_Traffic_Gray;
    cv::cvtColor( im_Traffic_BGR, im_Traffic_Gray, cv::COLOR_BGR2GRAY );

    // Perform edge detection as described in the exercise sheet

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**

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
    cv::Mat                           im_Traffic_BGR = cv::imread("./images/traffic.jpg");
    cv::GaussianBlur( im_Traffic_BGR, im_Traffic_BGR,  cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    // BGR to Gray
    cv::Mat                           im_Traffic_Gray;
    cv::cvtColor(     im_Traffic_BGR, im_Traffic_Gray, cv::COLOR_BGR2GRAY );

    // Read Template
    cv::Mat im_Sign_BGR = cv::imread("./images/sign.png");
    cv::Mat im_Sign_Gray;
    // BGR to Gray
    cv::cvtColor( im_Sign_BGR, im_Sign_Gray, cv::COLOR_BGR2GRAY ); // cv::COLOR_BGR2GRAY // CV_BGR2GRAY

    // Perform the steps described in the exercise sheet

    // Show results
    // using **cv::imshow and cv::waitKey()** and when necessary **std::cout**
    // In the end, after the last cv::waitKey(), use **cv::destroyAllWindows()**
    // If needed perform normalization of the image to be displayed

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
        cout << "Layer " << i << ":" << endl;
        cout << "width: " << pyr[i].size().width << "  , height: " << pyr[i].size().height << endl;
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
