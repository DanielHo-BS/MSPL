// Morphology : Erosion, Dilation, Opening, Closing
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // Read the image
    Mat image;
    image = imread("../images/lena.png", IMREAD_GRAYSCALE);

    // Build the binary-fit image
    if ( argc != 2 || (atoi(argv[1])%2) == 0 || atoi(argv[1]) > image.rows || atoi(argv[1]) > image.cols)
    {
        printf("usage: ./morphology <kernel size(3 ,5 ,7, ..., 2N+1)>\n");
        return -1;
    }
    int kernel;
    kernel =  atoi(argv[1])/2;
    
    // Dilated and Eroded
    Mat dilatedImage, erodedImage;
    dilatedImage = image.clone();
    erodedImage = image.clone();
    // find the maximum and minimum pixel intensity of image
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            uchar maxV = 0;
            uchar minV = 255;
            for (int y = i-kernel; y <= i+kernel; y++)
            {
                for (int x = j-kernel; x <= j+kernel; x++)
                {
                    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows)
                        continue;
                    // Dilated
                    maxV = max<uchar>(maxV, image.at<uchar>(y,x));
                    // Eroded
                    minV = min<uchar>(minV, image.at<uchar>(y,x));
                }
            }
            dilatedImage.at<uchar>(i,j) = maxV;
            erodedImage.at<uchar>(i,j) = minV;
        }
    }

    // Opening and Closing
    Mat openImage, closeImage;
    openImage = image.clone();
    closeImage = image.clone();
    // find the maximum and minimum pixel intensity of image
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            uchar maxV = 0;
            uchar minV = 255;

            for (int y = i-kernel; y <= i+kernel; y++)
            {
                for (int x = j-kernel; x <= j+kernel; x++)
                {
                    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows)
                        continue;
                    // Opening = Dilated(Eroded())
                    maxV = max<uchar>(maxV, erodedImage.at<uchar>(y,x));
                    // Close = Eroded(Dilated())
                    minV = min<uchar>(minV, dilatedImage.at<uchar>(y,x));
                }
            }
            openImage.at<uchar>(i,j) = maxV;
            closeImage.at<uchar>(i,j) = minV;
        }
    }

    // Show image
    namedWindow("Image", WINDOW_AUTOSIZE );
    imshow("Image", image);

    namedWindow("Dilated", WINDOW_AUTOSIZE );
    imshow("Dilated", dilatedImage);
    
    namedWindow("Eroded", WINDOW_AUTOSIZE );
    imshow("Eroded", erodedImage);

    namedWindow("Opening", WINDOW_AUTOSIZE );
    imshow("Opening", openImage);
    
    namedWindow("Closing", WINDOW_AUTOSIZE );
    imshow("Closing", closeImage);
    waitKey(0);

    // Save image
    imwrite("../images/dilatedImage.png", dilatedImage);
    imwrite("../images/erodedImage.png", erodedImage);
    imwrite("../images/openImage.png", openImage);
    imwrite("../images/closeImage.png", closeImage);

    return 0;

}