// Denoise
#include <stdio.h>
#include <ctime>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//Mean Filter
void meanFilter(Mat &src, Mat &dst, int kernel)
{
    if (!src.data) return;
    
    //at pixel of image
    
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            int sum;
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // sum pixels in the kernel size
                for (int k1 = 0; k1 < (2*kernel+1); k1++)
                    for (int k2 = 0; k2 < (2*kernel+1); k2++)
                        sum += src.at<uchar>(y-kernel+k1,x-kernel+k2);
            sum /= ((2*kernel+1)*(2*kernel+1));
            dst.at<uchar>(y,x)=sum;
            }
        }  
    }
}

// Salt the image
void salt(Mat &image, int num)
{
    if (!image.data) return;
    int x, y;
    srand(time(NULL));
    for (int i = 0; i < num; i++)
    {
        x = rand() % image.rows;
        y = rand() % image.cols;
        image.at<uchar>(y,x) = 255;
    }
}

int main(int argc, char** argv)
{   
    // Read the image
    Mat image = imread("../images/lena.png",0);

    // Add the salt noise to image
    Mat saltImage = image.clone();
    salt(saltImage, 5000);

    // Using the Mean Filter
    if ( argc != 2 || (atoi(argv[1])%2) == 0 || atoi(argv[1]) > image.rows || atoi(argv[1]) > image.cols)
    {
        printf("usage: ./denoise <kernel size(3 ,5 ,7, ..., 2N+1)>\n");
        return -1;
    }
    int kernel;
    kernel =  (atoi(argv[1])-1)/2;
    Mat meanImage = image.clone();
    meanFilter(saltImage, meanImage, kernel);

    imshow("Original",image);
    imshow("Salt",saltImage);
    imshow("Mean",meanImage);
    waitKey();
    
    imwrite("../images/salt.png", saltImage);
    imwrite("../images/mean.png", meanImage);
    
    return 0;
}