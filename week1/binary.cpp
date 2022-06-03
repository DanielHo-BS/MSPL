#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
    
    //Read the image
    Mat image;
    image = imread("lena.png", 0);
    long double N = image.rows * image.cols;
    
    //Initialize variables
    long double threshold = -1, var_max = 0, 
                sum = 0, sumB = 0, q1 = 0, q2 = 0, 
                u1 = 0, u2 = 0, vb =0;
    long double max_intensity = 255;
    long double histogram[255];
    for (int i = 0; i < max_intensity; i++)
        histogram[i] = 0;
    
    // Compute the image histogram
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;
    
    // Auxiliary value for computing µ2
    for (int i = 0; i < max_intensity; i++)
        sum += i * histogram[i];
    
    // Update qi(t)
    for (int t = 0; t < max_intensity; t++)
    {
        q1 += histogram[t];
        if (q1 == 0)
            continue;
        q2 = N - q1;

        // Update µi(t)
        sumB += t * histogram[t];
        u1 = sumB / q1;
        u2 = (sum - sumB) / q2;

        // Update the between-class variance
        vb = q1 * q2 * (u1 - u2) * (u1 - u2);

        // Update the threshold
        if (vb > var_max)
        {
            threshold = t;
            var_max = vb;
        }
    }
    
    // Build the segmented image
    Mat dsImage = image.clone();
    for (int y = 0; y < dsImage.rows; y++)
    {
        for (int x = 0; x < dsImage.cols; x++)
        {      
            if (image.at<uchar>(y,x) > threshold)
                dsImage.at<uchar>(y,x) = 255;
            else
                dsImage.at<uchar>(y,x) = 0;
        }
    }

    namedWindow("Image", WINDOW_AUTOSIZE );
    imshow("Image", dsImage);
    waitKey(0);

    return 0;
}
