#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void detKeypoints1()
{
    // load image from file and convert to grayscale
    cv::Mat imgGray;
    cv::Mat img = cv::imread("../images/img1.png");
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // Shi-Tomasi detector
    int blockSize = 6;       //  size of a block for computing a derivative covariation matrix over each pixel neighborhood
    double maxOverlap = 0.0; // max. permissible overlap between two features in %
    double minDistance = (1.0 - maxOverlap) * blockSize;
    int maxCorners = img.rows * img.cols / max(1.0, minDistance); // max. num. of keypoints
    double qualityLevel = 0.01;                                   // minimal accepted quality of image corners
    double k = 0.04;
    bool useHarris = false;

    vector<cv::KeyPoint> kptsShiTomasi;
    vector<cv::Point2f> corners;
    double t1 = (double)cv::getTickCount();
    cv::goodFeaturesToTrack(imgGray, corners, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, useHarris, k);
    t1 = ((double)cv::getTickCount() - t1) / cv::getTickFrequency();
    cout << "Shi-Tomasi with n= " << corners.size() << " keypoints in " << 1000 * t1 / 1.0 << " ms" << endl;

    for (auto it = corners.begin(); it != corners.end(); ++it)
    { // add corners to result vector

        cv::KeyPoint newKeyPoint;
        newKeyPoint.pt = cv::Point2f((*it).x, (*it).y);
        newKeyPoint.size = blockSize;
        kptsShiTomasi.push_back(newKeyPoint);
    }

    // visualize results
    cv::Mat visImage1 = img.clone();
    cv::drawKeypoints(img, kptsShiTomasi, visImage1, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    string windowName1 = "Shi-Tomasi Results";
    cv::namedWindow(windowName1, 1);
    imshow(windowName1, visImage1);
    cv::waitKey(0);

    // TODO: use the OpenCV library to add the FAST detector
    // in addition to the already implemented Shi-Tomasi 
    // detector and compare both algorithms with regard to 
    // (a) number of keypoints, (b) distribution of 
    // keypoints over the image and (c) processing speed.

    vector<cv::KeyPoint> kptsFAST;
    vector<cv::Point2f> FASTcorners;

    int threshold = 40;
    bool nms = true;
    cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::TYPE_9_16;
    double t2 = (double)cv::getTickCount();    
    cv::FAST(img, kptsFAST, threshold, nms, type);
    t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
    cout << "FAST with n= " << corners.size() << " keypoints in " << 1000 * t2 / 1.0 << " ms" << endl;

    // visualize results
    cv::Mat visImage2 = img.clone();
    cv::drawKeypoints(img, kptsFAST, visImage2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    string windowName2 = "FAST Results";
    cv::namedWindow(windowName2, 1);
    imshow(windowName2, visImage2);
    cv::waitKey(0);

}

int main()
{
    detKeypoints1();
}