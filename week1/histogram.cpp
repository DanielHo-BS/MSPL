// Histogram Equalization
#include <stdio.h>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // Read the image
    Mat image, newImage;
    image = imread("../images/lena.png", 0);
    newImage = image.clone();
    // Initialize variables
    int max_intensity = 255;
    long double histogram[255];
    long double new_histogram[255];
    for (int i = 0; i < max_intensity; i++)
    {
        histogram[i] = 0;
        new_histogram[i] = 0;
    }

    // Compute the image histogram
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;
    
    // calculating total number of pixels
    long double total = image.rows * image.cols;
    long double curr =0;

    for (int i = 0; i < max_intensity; i++)
    {
        curr += histogram[i];
        new_histogram[i]=round((((float)curr) * 255) / total);
    }

    // Performing histogram equalization by mapping new gray levels
    // Compute the image histogram
    for (int y = 0; y < newImage.rows; y++)
        for (int x = 0; x < newImage.cols; x++)
            newImage.at<uchar>(y,x)= new_histogram[newImage.at<uchar>(y,x)];
    
    // Show images
    namedWindow("Image", WINDOW_AUTOSIZE );
    imshow("Image", image);

    namedWindow("Histogram Equalization", WINDOW_AUTOSIZE );
    imshow("Histogram Equalization", newImage);
    waitKey(0);

    // Save image
    imwrite("../images/hq.png", newImage);

    return 0;
}