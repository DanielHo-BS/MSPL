/* Edge Detect: Canny
    * Step 1: Apply a Gaussian blur
    * Step 2: Find edge gradient strength and direction
    * Step 3: Trace along the edges
    * Step 4: Suppress non-maximum edges
*/
#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void gaussianFilter(Mat &src, Mat &dst, int kernel, double sigma= 1.4)
{
    // Initialise values
    int size = 2 * kernel + 1;
    double GKernel[size][size];
    double r = 0, s=2.0 * sigma * sigma, sum = 0;

    // Gaussian Kernel
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int y = i -kernel, x = j - kernel;
            r = pow(x,2) + pow(y,2);
            double normal = 1 / (s * M_PI);
            GKernel[y][x] = exp(-(r) / s) * normal;
            sum +=  GKernel[y][x];
        }
    }
    
    // Normalize the kernel
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            GKernel[y][x] /= sum;


    // Convolution image with GKernel
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // Convolution
                sum = 0;
                for (int k1 = 0; k1 < (size); k1++)
                    for (int k2 = 0; k2 < (size); k2++)
                        sum += (double)src.at<uchar>(y-kernel+k1, x-kernel+k2)*GKernel[k1][k2];  
                dst.at<uchar>(y,x)=sum;
            }
        }  
    } 
}

void sobelFilter(Mat &src, Mat &dst, Mat &dst2,int kernel)
{
    // Initialise values
    int size= 2 * kernel + 1;
    double Gx[size][size], Gy[size][size];
    double angle[src.rows][src.cols];
    double G[src.rows][src.cols];
    double Gmax=0.0;
    double sum = 0.0, sumX = 0.0, sumY = 0.0;
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            angle[j][i] = 0.0;
            G[j][i] = 0.0;
        }
    }
    
    // Sobel kernel
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
        } 
    }

    // Convolution image with Gx and Gy
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel > 0)&&(x-kernel > 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // Convolution
                sumX = 0, sumY=0;
                for (int k1 = 0; k1 < (size); k1++)
                    for (int k2 = 0; k2 < (size); k2++)
                    {
                        sumX += src.at<uchar>(y-kernel+k1, x-kernel+k2)*Gx[k1][k2];  
                        sumY += src.at<uchar>(y-kernel+k1, x-kernel+k2)*Gy[k1][k2];          
                    }
                G[y][x] = sqrt(pow(sumX,2) + pow(sumY,2));
                if (G[y][x] > Gmax)
                    Gmax = G[y][x];
                angle[y][x] = (atan2(sumX,sumY) / M_PI) * 180.0;
                if (angle[y][x] < 0)
                    angle[y][x] += 180;
            }
        }  
    }
    
    // Normalize
    for (int y = 0; y <  src.rows; y++)
        for (int x = 0; x <  src.cols; x++)
                dst.at<uchar>(y,x) = (uchar)(G[y][x]  * (255/Gmax));

            
           
    // Non Max Suppression
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                int q = 255, r = 255;
                if ((0 <= angle[y][x] < 22.5) || (157.5 <= angle[y][x] <= 180))
                {
                    q = dst.at<uchar>(y,x+1);
                    r = dst.at<uchar>(y,x-1);
                }
                else if ((22.5 <= angle[y][x]) && (angle[y][x] < 67.5))
                {
                    q = dst.at<uchar>(y+1,x-1);
                    r = dst.at<uchar>(y-1,x+1);
                }
                else if ((67.5 <= angle[y][x]) && (angle[y][x] < 112.5))
                {
                    q = dst.at<uchar>(y+1,x);
                    r = dst.at<uchar>(y-1,x);
                }
                else if ((112.5 <= angle[y][x]) && (angle[y][x] < 157.5))
                {
                    q = dst.at<uchar>(y-1,x-1);
                    r = dst.at<uchar>(y+1,x+1);
                }

                if ((dst.at<uchar>(y,x) >= q) && (dst.at<uchar>(y,x) >= r))
                    dst2.at<uchar>(y,x) = dst.at<uchar>(y,x);
                else
                    dst2.at<uchar>(y,x) = 0;
            }
            else
                dst2.at<uchar>(y,x) = 0;
        }
    }
}

// double thershold
void db_thershold(Mat &src, Mat &dst, int kernel,int low, int high)
{
    int G[src.rows][src.cols];
    int size = 2 * kernel + 1;
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {   
                if (src.at<uchar>(y,x)<low) G[y][x] = 0;
                else if ((src.at<uchar>(y,x)>low) && (src.at<uchar>(y,x)<high))
                {
                    bool edge = false;
                    for (int k1 = 0; k1 < (size); k1++)
                    {
                        for (int k2 = 0; k2 < (size); k2++)
                        {
                            if (src.at<uchar>(y-kernel+k1, x-kernel+k2)>high)
                                {
                                    edge = true;
                                    break;
                                }
                        }
                    }
                    if (!edge) G[y][x] = 0;
                }
                dst.at<uchar>(y,x) = G[y][x];
            }
        }  
    }      
}

int main(int argc, char** argv)
{
    // Read the image
    Mat image = imread("../images/lena.png",0);
    Mat gaussianImage = image.clone();
    Mat sobelImage = image.clone();
    Mat suppressionImage = image.clone();
    Mat dbImage = suppressionImage.clone();
    
    // Gaussian
    int kernel;
    printf("Enter kernel size of gaussian filter(3 ,5 ,7, ..., 2N+1): ");
    scanf("%d",&kernel);
    if ((kernel%2) == 0 || kernel > image.rows || kernel > image.cols || kernel == 1)
        {
            printf("Error value!!\n");
            return -1;
        }
    kernel =  (kernel-1)/2;
    gaussianFilter(image, gaussianImage, kernel);

    // Sobel
    printf("Enter kernel size of sobel filter(3 ,5 ,7, ..., 2N+1): ");
    scanf("%d",&kernel);
    if ((kernel%2) == 0 || kernel > image.rows || kernel > image.cols || kernel == 1)
        {
            printf("Error value!!\n");
            return -1;
        }
    kernel =  (kernel-1)/2;
    sobelFilter(gaussianImage, sobelImage, suppressionImage, kernel);

    // Double thershold
    printf("Enter kernel size of double thershold(3 ,5 ,7, ..., 2N+1): ");
    scanf("%d",&kernel);
    if ((kernel%2) == 0 || kernel > image.rows || kernel > image.cols || kernel == 1)
        {
            printf("Error value!!\n");
            return -1;
        }
    kernel =  (kernel-1)/2;
    int low=0, high=0;
    printf("Enter low and high value for double thershold: ");
    scanf("%d %d",&low, &high);
    if (low > high || high > 254 || low < 0)
        {
            printf("Error value!!%d %d\n",low ,high);
            return -1;
        }
    db_thershold(suppressionImage, dbImage, kernel, low, high);
    
    // Show images
    /*imshow("Original",image);
    imshow("Gaussian", gaussianImage);
    imshow("Sobel", sobelImage);
    imshow("Suppression", suppressionImage);
    imshow("Double thershold", dbImage);
    waitKey();*/
    
    // Save images
    imwrite("../images/1-gaussian.png", gaussianImage);
    imwrite("../images/2-sobel.png", sobelImage);
    imwrite("../images/3-suppression.png", suppressionImage);
    imwrite("../images/4-dbThershold.png", dbImage);

    return 0;
}