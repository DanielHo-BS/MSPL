# **Week 2**

In this week, the HWs of MSPL:

* Denoise
  * Mean
  * Median
  * Gaussian
* Edge Detection
  * sobel
  * Laplacian
  * Canny

# Usage

Get the code.

```bash
cd ~
git clone git@github.com:DanielHo-BS/MSPL.git
cd MSPL/week2
```

## Compiler

The code is written in **C++**, so need to use ``CMake`` to compile.

```bash
mkdir build
cd build
cmake ..
make
```
For more details about compiler, you can see the [CMakeLists.txt](CMakeLists.txt).

## Denoise

Show five images:

* Original
* Salt
* Mean
* Median
* Gaussian

```bash
#./denoise <kernel size(3 ,5 ,7, ..., 2N+1)>
./denoise 5
```

## Edge Detection

Show three images:

* Original
* Sobel
* Laplacian


```bash
#./edgeDetect <kernel size(3 ,5 ,7, ..., 2N+1)>
./edgeDetect 5
```

## Canny

show four images:

* Gaussian
* Sobel
* Supperssion
* Double Thershold

```bash
# ./canny
./canny
# Enter kernel size of gaussian filter(3 ,5 ,7, ..., 2N+1): 5
# Enter kernel size of sobel filter(3 ,5 ,7, ..., 2N+1): 3
# Enter kernel size of double thershold(3 ,5 ,7, ..., 2N+1): 3
# Enter low and high value for double thershold: 24 25
```