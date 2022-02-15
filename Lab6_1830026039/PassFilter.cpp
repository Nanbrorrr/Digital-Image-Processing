#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"
#define e 2.718

Image *IDLPF(Image *image, double radius){
	Image *outimage;
	int size = image->Height * image->Width;
	double D0, H;
	outimage = CreateNewSizeImage(image, image->Width, image->Height, "FFT result");
	struct _complex *src = (struct _complex*)malloc(sizeof(struct _complex)*size);

	int i;
	for (i = 0; i < size; ++i){
		src[i].x = 1.0*image->data[i];
		src[i].y = 0.0;
	}
	fft(src, src, 1);
	for (int x = 0; x<image->Height; x++){
		for (int y = 0; y<image->Width; y++){
			D0 = sqrt(pow((double)(y - image->Width / 2), 2) + pow((double)(x - image->Height / 2), 2));
			H = D0 > radius ? 0 : 1;
			src[x * image->Width + y].x *= H;
			src[x * image->Width + y].y *= H;
		}
	}
	fft(src, src, -1);
	outimage->data = Normal(getResult(src, size), size, 255);
	return(outimage);
}

Image *BLPF(Image *image, double radius, double n){
	Image *outimage;
	int size = image->Height * image->Width;
	double D0, H;
	outimage = CreateNewSizeImage(image, image->Width, image->Height, "FFT result");
	struct _complex *src = (struct _complex*)malloc(sizeof(struct _complex)*size);

	int i;
	for (i = 0; i < size; ++i){
		src[i].x = 1.0*image->data[i];
		src[i].y = 0.0;
	}
	fft(src, src, 1);
	for (int x = 0; x<image->Height; x++){
		for (int y = 0; y<image->Width; y++){
			D0 = sqrt(pow((double)(y - image->Width / 2), 2) + pow((double)(x - image->Height / 2), 2));
			H = 1 / (1 + pow(D0 / radius, 2 * n));
			src[x * image->Width + y].x *= H;
			src[x * image->Width + y].y *= H;
		}
	}
	fft(src, src, -1);
	outimage->data = Normal(getResult(src, size), size, 255);
	return(outimage);
}

Image *GLPF(Image *image, double radius){
	Image *outimage;
	int size = image->Height * image->Width;
	double D0, H;
	outimage = CreateNewSizeImage(image, image->Width, image->Height, "FFT result");
	struct _complex *src = (struct _complex*)malloc(sizeof(struct _complex)*size);

	int i;
	for (i = 0; i < size; ++i){
		src[i].x = 1.0*image->data[i];
		src[i].y = 0.0;
	}
	fft(src, src, 1);
	for (int x = 0; x < image->Height; x++){
		for (int y = 0; y < image->Width; y++){
			D0 = sqrt(pow((double)(y - image->Width / 2), 2) + pow((double)(x - image->Height / 2), 2));
			H = pow(e, -1 * D0 * D0 / (2 * radius * radius));
			src[x * image->Width + y].x *= H;
			src[x * image->Width + y].y *= H;
		}
	}
	fft(src, src, -1);
	outimage->data = Normal(getResult(src, size), size, 255);
	return(outimage);
}

Image *IDHPF(Image *image, double radius){
	Image *outimage;
	int size = image->Height * image->Width;
	double D0, H;
	outimage = CreateNewSizeImage(image, image->Width, image->Height, "FFT result");
	struct _complex *src = (struct _complex*)malloc(sizeof(struct _complex)*size);

	int i;
	for (i = 0; i < size; ++i){
		src[i].x = 1.0*image->data[i];
		src[i].y = 0.0;
	}
	fft(src, src, 1);
	for (int x = 0; x < image->Height; x++){
		for (int y = 0; y < image->Width; y++){
			D0 = sqrt(pow((double)(y - image->Width / 2), 2) + pow((double)(x - image->Height / 2), 2));
			H = D0 > radius ? 1 : 0;
			src[x * image->Width + y].x *= H;
			src[x * image->Width + y].y *= H;
		}
	}
	fft(src, src, -1);
	outimage->data = Normal(getResult(src, size), size, 255);
	for (i = 0; i < size; i++){
		if (outimage->data[i] > 100)
			outimage->data[i] = 255;
		else if (outimage->data[i] <= 100)
			outimage->data[i] = 0;
	}
	return(outimage);
}