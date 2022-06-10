// Edge Detect: Sobel, Laplacian
#include <stdio.h>
#include <cmath>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void sobelKernel(Mat &src, Mat &dst, int kernel, int sita)
{
    // Initialise values
    int size= 2 * kernel + 1;
    double Gx[size][size], Gy[size][size], G[size][size];

    // Generate sobel kernel
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if (y != kernel || x != kernel)
            {
                // Gx f(x,y) dot (1,0)
                Gx[y][x] = (double)(x-kernel) / (double)((x-kernel)*(x-kernel) + (y-kernel)*(y-kernel));
                // Gy f(x,y) dot (0,1)
                Gy[y][x] = (double)(y-kernel) / (double)((x-kernel)*(x-kernel) + (y-kernel)*(y-kernel));
            }
            else
                Gx[y][x]=Gy[y][x]=0;
            // G = (Gx*cosa + Gy*sina)/2
            G[y][x] = (Gx[y][x] * cos(sita) + Gy[y][x] * sin(sita));
        } 
    }

    // Convolution image with soble kernel
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // Convolution
                double sum = 0;
                for (int k1 = 0; k1 < size; k1++)
                    for (int k2 = 0; k2 < size; k2++)
                        sum += src.at<uchar>(y-kernel+k1, x-kernel+k2) * G[k1][k2];  
                dst.at<uchar>(y,x)=abs(sum);
            }
            else
                dst.at<uchar>(y,x)=0;
        }  
    }
}

// Combine two images to one
void addImage(Mat &src1, Mat &src2,Mat &dst)
{
    for (int y = 0; y < dst.rows; y++)
        for (int x = 0; x < dst.cols; x++)
            dst.at<uchar>(y,x)=sqrt(pow(src1.at<uchar>(y,x),2) + pow(src2.at<uchar>(y,x),2));
}

// Laplacian  Filter
void laplacianFilter(Mat &src, Mat &dst, int kernel,double sigma = 84089642)
{
    // Initialise values
    int size = 2 * kernel + 1;
    double LKernel[size][size];
    
    // Generate sobel kernel
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if (y != kernel || x != kernel)
                LKernel[y][x] = -1;
            else
                LKernel[y][x] = size * size - 1;
        } 
    }
    
    // Convolution image with GKernel
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // Convolution
                double sum = 0;
                for (int k1 = 0; k1 < size; k1++)
                    for (int k2 = 0; k2 < size; k2++)
                        sum += (double)src.at<uchar>(y-kernel+k1, x-kernel+k2)*LKernel[k1][k2];  
                dst.at<uchar>(y,x)=abs(sum);
            }
            else
                dst.at<uchar>(y,x) = 0;     
        }  
    }
}


int main(int argc, char** argv)
{

    // Read the image
    Mat image = imread("../images/lena.png",0);
    
    // Setting the kernel size
    if ( argc != 2 || (atoi(argv[1])%2) == 0 || atoi(argv[1]) > image.rows || atoi(argv[1]) > image.cols)
    {
        printf("usage: ./edgeDetect <kernel size(3 ,5 ,7, ..., 2N+1)>\n");
        return -1;
    }
    int kernel;
    kernel =  (atoi(argv[1])-1)/2;
    
    // Sobel
    Mat sobelImageX = image.clone(), sobelImageY = image.clone(), sobelImage = image.clone();
    sobelKernel(image, sobelImageX, kernel, (0*M_PI/180));
    sobelKernel(image, sobelImageY, kernel, (90*M_PI/180));
    addImage(sobelImageX, sobelImageY, sobelImage);
    
    // Laplacian of Gaussian
    Mat LaplacianImage = image.clone();
    laplacianFilter(image,LaplacianImage,kernel);

    // Show images
    imshow("Original",image);
    imshow("Sobel",sobelImage);
    imshow("Laplacian", LaplacianImage);

    waitKey();
    
    // Save images
    imwrite("../images/sobel.png", sobelImage);
    imwrite("../images/sobelX.png", sobelImageX);
    imwrite("../images/sobelY.png", sobelImageY);
    imwrite("../images/laplacian.png", LaplacianImage);
    
}
