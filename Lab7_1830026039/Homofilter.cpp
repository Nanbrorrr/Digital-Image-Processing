#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"
#define e 2.718

Image* Homofilter(Image* image) {
	Image* outimage;
	int i;
	double c = 1; double D0 = 100.0;
	double yl = 0.5; double yh = 1.5;
	double dis;
	int P = image->Height;
	int Q = image->Width;
	int size = image->Width * image->Height;
	unsigned char *tempin, *tempout, *temp;
	tempin = image->data;
	outimage = CreateNewSizeImage(image, image->Width, image->Height, "...");
	tempout = outimage->data;
	struct _complex* src = (struct _complex*)malloc(sizeof(struct _complex) * size);

	for (i = 0; i < size; ++i) {
		src[i].x = 1.0 * tempin[i];
		src[i].y = 0.0;
	}
	for (i = 0; i < size; ++i) {
		src[i].x = log(src[i].x);
		src[i].y = 0.0;
	}
	fft(src, src, 1);
	double cx, cy, H;
	cx = (double)Q / 2; cy = (double)P / 2;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < Q; j++) {
			dis = pow(((double)i - cy), 2) + pow(((double)j - cx), 2);
			H = (yh - yl) * (1 - exp((-c) * (dis / pow(D0, 2)))) + yl;
			src[i * Q + j].x = H * src[i * Q + j].x;
			src[i * Q + j].y = H * src[i * Q + j].y;
		}
	}
	fft(src, src, -1);
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < Q; j++) {
			src[i * Q + j].x = exp(src[i * Q + j].x);
			src[i * Q + j].y = exp(src[i * Q + j].y);
		}
	}
	outimage->data = Normal(getResult(src, size), size, 255);
	return(outimage);
}
