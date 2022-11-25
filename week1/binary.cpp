// Binary Fixed & Otsu
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{    
    //Read the image
    Mat image, fixedImage, otsuImage;
    image = imread("../images/lena.png", 0);
    fixedImage = image.clone();
    otsuImage = image.clone();
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
    
    // Build the binary-fit image
    if ( argc != 2 )
    {
        printf("usage: ./binary <binary_fit>\n");
        return -1;
    }
    int fit;
    fit =  atoi(argv[1]);

    for (int y = 0; y < fixedImage.rows; y++)
    {
        for (int x = 0; x < fixedImage.cols; x++)
        {      
            if (fixedImage.at<uchar>(y,x) > fit)
                fixedImage.at<uchar>(y,x) = 255;
            else
                fixedImage.at<uchar>(y,x) = 0;
        }
    }

    // Build the binary-otsu image
    for (int y = 0; y < otsuImage.rows; y++)
    {
        for (int x = 0; x < otsuImage.cols; x++)
        {      
            if (otsuImage.at<uchar>(y,x) > threshold)
                otsuImage.at<uchar>(y,x) = 255;
            else
                otsuImage.at<uchar>(y,x) = 0;
        }
    }
    
    // Show images
    namedWindow("Image", WINDOW_AUTOSIZE );
    imshow("Image", image);

    namedWindow("fixedImage", WINDOW_AUTOSIZE );
    imshow("fixedImage", fixedImage);
    
    namedWindow("Otsu", WINDOW_AUTOSIZE );
    imshow("Otsu", otsuImage);
    waitKey(0);

    // Save images
    imwrite("../images/fixed.png", fixedImage);
    imwrite("../images/otsu.png", otsuImage);
    
    return 0;
}
