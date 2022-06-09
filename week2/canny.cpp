/* Edge Detect: Canny
    * Step 1: Apply a Gaussian blur
    * Step 2: Find edge gradient strength and direction
    * Step 3: Trace along the edges
    * Step 4: Suppress non-maximum edges
*/
#include <stdio.h>
#include <ctime>
#include <cmath>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// Gaussian Filter

void gaussianFilter(Mat &src, Mat &dst, int kernel, double sigma= 0.84089642)
{
    // Initialise values
    int size = 2 * kernel + 1;
    double GKernel[size][size];
    double r, s=2.0 * sigma * sigma;
    double sum=0.0;

    // Generate the gaussian kernel
    for (int y = 0; y < (size); y++)
    {
        for (int x = 0; x < (size); x++)
        {
            r = ((x-kernel)*(x-kernel)+(y-kernel)*(y-kernel));
            GKernel[y][x] = (exp(-r/s)) / (M_PI * s);
            sum += GKernel[y][x];
        }
    }

    // Normalize the kernel
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            {GKernel[y][x] /= sum;
            //cout << GKernel[y][x]<<endl;
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
                for (int k1 = 0; k1 < (size); k1++)
                    for (int k2 = 0; k2 < (size); k2++)
                        sum += (double)src.at<uchar>(y-kernel+k1, x-kernel+k2)*GKernel[k1][k2];  
                dst.at<uchar>(y,x)=sum;
            }
        }  
    }                
}

// Sobel Filter
void sobelKernel(Mat &src, Mat &dst, Mat &dst2, int kernel)
{
    // Initialise values
    int size= 2 * kernel + 1;
    double Gx[size][size], Gy[size][size];
    int edgeDir[src.rows][src.cols];

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
        } 
    }

    // Convolution image with soble kernel
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            double sumx = 0, sumy = 0;
            double angle = 0;
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
            {
                // Convolution
                for (int k1 = 0; k1 < size; k1++)
                {
                    for (int k2 = 0; k2 < size; k2++)
                    {
                        sumx += src.at<uchar>(y-kernel+k1, x-kernel+k2) * Gx[k1][k2];
                        sumy += src.at<uchar>(y-kernel+k1, x-kernel+k2) * Gy[k1][k2]; 
                    }  
                }         
                dst2.at<uchar>(y,x) = dst.at<uchar>(y,x) = (int)sqrt(pow(sumx,2) + pow(sumy,2));
                angle = (atan2(sumx,sumy) / M_PI) * 180.0;                     
            }
            else
            {
                dst2.at<uchar>(y,x) = dst.at<uchar>(y,x)=0;
                angle = 0;
            }

            /* Convert actual edge direction to approximate value */
            if ( ( (angle < 22.5) && (angle > -22.5) ) || (angle > 157.5) || (angle < -157.5) )
                edgeDir[y][x] = 0;
            if ( ( (angle > 22.5) && (angle < 67.5) ) || ( (angle < -112.5) && (angle > -157.5) ) )
                edgeDir[y][x] = 45;
            if ( ( (angle > 67.5) && (angle < 112.5) ) || ( (angle < -67.5) && (angle > -112.5) ) )
                edgeDir[y][x] = 90;
            if ( ( (angle > 112.5) && (angle < 157.5) ) || ( (angle < -22.5) && (angle > -67.5) ) )
                edgeDir[y][x] = 135;
        }  
    }

    // Non-maximum suppression
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            if ((y-kernel >= 0)&&(x-kernel >= 0)&&(y+kernel < src.rows)&&(x+kernel < src.cols))
                switch (edgeDir[y][x])
                {
                case 0:
                    if ((dst.at<uchar>(y,x) < dst.at<uchar>(y,x-1))||(dst.at<uchar>(y,x) < dst.at<uchar>(y,x+1)))
                        dst2.at<uchar>(y,x)=0;
                    break;

                case 45:
                    if ((dst.at<uchar>(y,x) < dst.at<uchar>(y+1,x-1))||(dst.at<uchar>(y,x) < dst.at<uchar>(y-1,x+1)))
                        dst2.at<uchar>(y,x)=0;
                    break;

                case 90:
                    if ((dst.at<uchar>(y,x) < dst.at<uchar>(y-1,x))||(dst.at<uchar>(y,x) < dst.at<uchar>(y+1,x)))
                        dst2.at<uchar>(y,x)=0;
                    break;

                case 135:
                if ((dst.at<uchar>(y,x) < dst.at<uchar>(y-1,x-1))||(dst.at<uchar>(y,x) < dst.at<uchar>(y+1,x+1)))
                        dst2.at<uchar>(y,x)=0;
                    break;
                }
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
                dst.at<uchar>(y,x)=dst.at<uchar>(y,x-1)=dst.at<uchar>(y,x+1)=G[y][x];
            }
        }  
    }      
}


int main(int argc, char** argv)
{
    // Read the image
    Mat image = imread("../images/lena.png",0);
    
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
    Mat gaussianImage = image.clone();
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
    Mat sobelImage = image.clone();
    Mat suppressionImage = image.clone();
    sobelKernel(gaussianImage, sobelImage, suppressionImage, kernel);

    // Double thershold
    printf("Enter kernel size of double thershold(3 ,5 ,7, ..., 2N+1): ");
    scanf("%d",&kernel);
    if ((kernel%2) == 0 || kernel > image.rows || kernel > image.cols || kernel == 1)
        {
            printf("Error value!!\n");
            return -1;
        }
    kernel =  (kernel-1)/2;
    Mat dbImage = suppressionImage.clone();
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
    imshow("Original",image);
    imshow("Gaussian", gaussianImage);
    imshow("Sobel", sobelImage);
    imshow("Suppression", suppressionImage);
    imshow("Double thershold", dbImage);
    waitKey();
    
    // Save images
    imwrite("../images/1-gaussian.png", gaussianImage);
    imwrite("../images/2-sobel.png", sobelImage);
    imwrite("../images/3-suppression.png", suppressionImage);
    imwrite("../images/4-dbThershold.png", dbImage);
}