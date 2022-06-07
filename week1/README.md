# **Week 1**

In this week, the HWs of MSPL:

* Binary
  * Fit
  * Otsu
* Histogram
  * Equlization
* Morphology
  * Dilated
  * Eroded
  * Opening
  * Closing

# Usage

## Build

Get the code.

```bash
cd ~
git clone git@github.com:DanielHo-BS/MSPL.git
cd MSPL/week1
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

## Binary

Show three images:
* Original image
* Binary-fit image
* Binary-Otsu image

```bash
#./binary <binary_fit>
./binary 100
```

## Histogram

Show two images:
* Original image
* Histogram Equlization image

```bash
./histograme
```
### PlotHis

Plot the histogram.

```bash
./plotHis
```

## Mophology

Show five images:
* Original image
* Dilated
* Eroded
* Opening
* Closing

```bash
#./morphology <kernel size(3 ,5 ,7, ..., 2N+1)>
./morphology 5
```