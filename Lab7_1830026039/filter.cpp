#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"
#define e 2.718

Image* arithmeticmeanFilt(Image* image, int num) {
	unsigned char* tempin, *tempout;
	int size, i, j, k, t;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	for (i = 1; i < image->Height - 1; ++i) {
		for (j = 1; j < image->Width - 1; ++j) {
			int sum = 0;
			for (k = -1; k < num - 1; ++k)
			for (t = -1; t < num - 1; ++t)
				sum += tempin[image->Width * (i + k) + (j + t)];
			tempout[image->Width * i + j] = sum / (num * num);
		}
	}
	return(outimage);
}

Image* GeometricmeanFilt(Image* image) {
	unsigned char* tempin, *tempout;
	int size, i, j, k, t;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	for (i = 2; i < image->Height - 2; ++i) {
		for (j = 2; j < image->Width - 2; ++j) {
			double temp = 1.0;
			for (k = -2; k < 3; ++k)
			for (t = -2; t < 3; ++t)
				temp *= tempin[image->Width * (i + k) + (j + t)];
			tempout[image->Width * i + j] = pow(temp, 0.04);
		}
	}
	return(outimage);
}

Image* MedianFilt(Image* image, int num) {
	unsigned char* tempin, *tempout, local[100];
	int size, i, j, k, t;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	for (i = 1; i < image->Height - 1; ++i) {
		for (j = 1; j < image->Width - 1; ++j) {
			int pos = 0;
			for (k = -1; k < num - 1; ++k)
			for (t = -1; t < num - 1; ++t)
				local[pos++] = tempin[image->Width * (i + k) + (j + t)];
			tempout[image->Width * i + j] = findMedian(local, num * num);
		}
	}
	return(outimage);
}

Image* AlphatrimmedmeanFilt(Image* image) {
	unsigned char* tempin, *tempout, local[25];
	int size, i, j, k, t;
	int d = 4;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	for (i = 2; i < image->Height - 2; ++i) {
		for (j = 2; j < image->Width - 2; ++j) {
			int pos = 0;
			int sum = 0;
			for (k = -2; k < 3; ++k) {
				for (t = -2; t < 3; ++t) {
					local[pos++] = tempin[image->Width * (i + k) + (j + t)];
				}
			}
			Sort(local, 25);
			for (int i = (d / 2); i < 25 - (d / 2); i++) {
				sum = sum + local[i];
			}
			tempout[image->Width * i + j] = sum / (25 - d);
		}
	}
	return(outimage);
}

Image* adaptivemedianFilter(Image* image) {
	unsigned char* tempin, *tempout, local[25];
	int size, i, j, k, t;
	int max = 7;
	int min = 3;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	for (int i = max / 2; i < image->Height - max / 2; i++) {
		for (int j = max / 2; j < image->Width - max / 2; j++) {
			tempout[i * image->Width + j] = adaptive(image, i, j, min, max);
		}
	}
	return(outimage);
}

unsigned char adaptive(Image* image, int i, int j, int kernelSize, int maxSize) {
	unsigned char temp;
	unsigned char* tempin;
	unsigned char local[100];
	tempin = image->data;
	int count = 0;
	int k, t;
	int x = kernelSize / 2;
	for (k = -x; k < x + 1; ++k) {
		for (t = -x; t < x + 1; ++t) {
			local[count++] = tempin[image->Width * (i + k) + (j + t)];
		}
	}
	Sort(local, kernelSize * kernelSize);
	int zmin, zmax, zmed, zxy;
	zmin = local[0];
	zmax = local[count];
	zmed = local[count / 2];
	zxy = tempin[i * image->Width + j];
	if (zmin < zmed && zmed < zmax) {
		if (zmin < zxy && zxy < zmax)
			return zxy;
		else return zmed;
	}
	else {
		kernelSize += 2;
		if (kernelSize <= maxSize)
			return adaptive(image, i, j, kernelSize, maxSize);
		else return zmed;
	}
}