#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"
#define e 2.718
#define _CRT_SECURE_NO_WARNINGS

Image *MedianFilt(Image *image){
	unsigned char *tempin, *tempout, local[9];
	int size, i, j, k, t;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (i = 1; i < image->Height - 1; ++i){
		for (j = 1; j < image->Width - 1; ++j){
			int pos = 0;
			for (k = -1; k < 2; ++k)
			for (t = -1; t < 2; ++t)
				local[pos++] = tempin[image->Width * (i + k) + (j + t)];
			tempout[image->Width * i + j] = findMedian(local, 9);
		}
	}
	return(outimage);
}

Image* noise(Image* image){
	unsigned char* tempin, *tempout, *temp;
	int t, P, Q;
	int i, j;
	double dis, temp1;
	double cutoff = 100.0;
	double W = 5;
	double n = 1;
	Image* outimage, *IDLPF_image;
	int width = image->Width;
	int height = image->Height;
	int size = image->Width * image->Height;
	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;
	P = 2 * height; Q = 2 * width;
	IDLPF_image = CreateNewSizeImage(image, Q, P, "#padded imaged");
	temp = IDLPF_image->data;
	int size2 = P * Q;
	struct _complex* src = (struct _complex*)malloc(sizeof(struct _complex) * size2);
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < Q; j++) {
			if (i < height && j < width) {
				temp[i * Q + j] = tempin[i * width + j];
			}
			else {
				temp[i * Q + j] = 0;
			}
			if (temp[i * Q + j] < 0) temp[i * Q + j] = 0;
		}
	}

	for (int i = 0; i < P; i++) {
		for (int j = 0; j < Q; j++) {
			temp[i * Q + j] = temp[i * Q + j] * pow(-1.0, P + Q);
		}
	}

	for (int i = 0; i < size2; ++i) {
		src[i].x = 1.0 * temp[i];
		src[i].y = 0.0;
	}
	fft(src, src, 1);

	double cx, cy, H;
	cx = (double)Q / 2; cy = (double)P / 2;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < Q; j++) {
			dis = sqrt(pow(((double)i - cy), 2) + pow(((double)j - cx), 2));
			H = 1 / (1 + pow(((dis * W) / (dis * dis - cutoff * cutoff)), 2 * n));
			src[i * Q + j].x = H * src[i * Q + j].x;
			src[i * Q + j].y = H * src[i * Q + j].y;
		}
	}

	fft(src, src, -1);
	temp = Normal(getResult(src, size2), size2, 255);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tempout[i * width + j] = temp[i * P + j];
		}
	}

	return(outimage);
}