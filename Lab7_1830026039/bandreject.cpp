#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"
#define e 2.718

Image* bandreject(Image* image) {
	unsigned char* tempin, *tempout, *temp;
	int t, P, Q;
	int i, j;
	double dis, temp1;
	double cutoff = 200.0;
	double W = 10;
	double n = 5;
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

	//add noise
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			temp1 = tempin[i * width + j] + 20 * sin(20.0 * i) + 20 * sin(20.0 * j);
			if (temp1 > 255)
				tempin[i * width + j] = 255;
			else if (temp1 < 0)
				tempin[i * width + j] = 0;
			else
				tempin[i * width + j] = temp1;

		}
	}
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