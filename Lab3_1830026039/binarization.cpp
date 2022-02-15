//ID:	1830026039
//NAME: Gabriel Hou
//IDE: VS2013
//DATE: 2021/3/19
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <ctype.h>
#include "proto.h"

void SavePNMImage(Image *, char *);
Image *SwapImage(Image *);
Image *ReadPNMImage(char *);
Image *CreateNewImage(Image *, char *comment);
int TestReadImage(char *, char *);
unsigned char findMedian(unsigned char*, int lenth);
Image *binarization3(Image *);
Image *binarization5(Image *);

int findmax(unsigned char*, int num);
int findmin(unsigned char*, int num);

int main(int argc, char **argv)
{
	TestReadImage(argv[1], argv[2]);

	return(0);
}

int TestReadImage(char *filename, char *outfilename)
{
	Image *image1;
	Image *image2;
	Image *outimage;

	image1 = ReadPNMImage("bridge.pgm");
	image2 = ReadPNMImage("lena.pgm");

	outimage = binarization3(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab3_1830026039\\outimage\\3binarizationbridge.pgm");
	outimage = binarization3(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab3_1830026039\\outimage\\3binarizationlena.pgm");

	outimage = binarization5(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab3_1830026039\\outimage\\5binarizationbridge.pgm");
	outimage = binarization5(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab3_1830026039\\outimage\\5binarizationlena.pgm");
	return(0);
}

Image *SwapImage(Image *image)
{
	unsigned char *tempin, *tempout;
	int i, size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (i = 0; i<size; i++)
	{
		*tempout = *tempin;
		tempin++;
		tempout++;
	}
	return(outimage);
}

/*******************************************************************************/
//Read PPM image and return an image pointer                                   
/**************************************************************************/
Image *ReadPNMImage(char *filename)
{
	char ch;
	int  maxval, Width, Height;
	int size, num, j;
	FILE *fp;
	Image *image;
	int num_comment_lines = 0;


	image = (Image *)malloc(sizeof(Image));

	if ((fp = fopen(filename, "rb")) == NULL){
		printf("Cannot open %s\n", filename);
		exit(0);
	}

	printf("Loading %s ...", filename);

	if (fscanf(fp, "P%c\n", &ch) != 1) {
		printf("File is not in ppm/pgm raw format; cannot read\n");
		exit(0);
	}
	if (ch != '6' && ch != '5') {
		printf("File is not in ppm/pgm raw format; cannot read\n");
		exit(0);
	}

	if (ch == '5')image->Type = GRAY;  // Gray (pgm)
	else if (ch == '6')image->Type = COLOR;  //Color (ppm)
	/* skip comments */
	ch = getc(fp);
	j = 0;
	while (ch == '#')
	{
		image->comments[num_comment_lines][j] = ch;
		j++;
		do {
			ch = getc(fp);
			image->comments[num_comment_lines][j] = ch;
			j++;
		} while (ch != '\n');     /* read to the end of the line */
		image->comments[num_comment_lines][j - 1] = '\0';
		j = 0;
		num_comment_lines++;
		ch = getc(fp);            /* thanks, Elliot */
	}

	if (!isdigit((int)ch)){
		printf("Cannot read header information from ppm file");
		exit(0);
	}

	ungetc(ch, fp);               /* put that digit back */

	/* read the width, height, and maximum value for a pixel */
	fscanf(fp, "%d%d%d\n", &Width, &Height, &maxval);

	/*
	if (maxval != 255){
	printf("image is not true-color (24 bit); read failed");
	exit(0);
	}
	*/

	if (image->Type == GRAY)
		size = Width * Height;
	else  if (image->Type == COLOR)
		size = Width * Height * 3;
	image->data = (unsigned char *)malloc(size);
	image->Width = Width;
	image->Height = Height;
	image->num_comment_lines = num_comment_lines;

	if (!image->data){
		printf("cannot allocate memory for new image");
		exit(0);
	}

	num = fread((void *)image->data, 1, (size_t)size, fp);
	//printf("Complete reading of %d bytes \n", num);
	if (num != size){
		printf("cannot read image data from file");
		exit(0);
	}

	//for(j=0;j<image->num_comment_lines;j++){
	//      printf("%s\n",image->comments[j]);
	//      }

	fclose(fp);

	/*-----  Debug  ------*/

	if (image->Type == GRAY)printf("..Image Type PGM\n");
	else printf("..Image Type PPM Color\n");
	/*
	printf("Width %d\n", Width);
	printf("Height %d\n",Height);
	printf("Size of image %d bytes\n",size);
	printf("maxvalue %d\n", maxval);
	*/
	return(image);
}

void SavePNMImage(Image *temp_image, char *filename)
{
	int num, j;
	int size;
	FILE *fp;
	//char comment[100];


	printf("Saving Image %s\n", filename);
	fp = fopen(filename, "wb");
	if (!fp){
		printf("cannot open file for writing");
		exit(0);
	}

	//strcpy(comment,"#Created by Dr Mohamed N. Ahmed");

	if (temp_image->Type == GRAY){  // Gray (pgm)
		fprintf(fp, "P5\n");
		size = temp_image->Width * temp_image->Height;
	}
	else  if (temp_image->Type == COLOR){  // Color (ppm)
		fprintf(fp, "P6\n");
		size = temp_image->Width * temp_image->Height * 3;
	}

	for (j = 0; j<temp_image->num_comment_lines; j++)
		fprintf(fp, "%s\n", temp_image->comments[j]);

	fprintf(fp, "%d %d\n%d\n", temp_image->Width, temp_image->Height, 255);

	num = fwrite((void *)temp_image->data, 1, (size_t)size, fp);

	if (num != size){
		printf("cannot write image data to file");
		exit(0);
	}

	fclose(fp);
}

/*************************************************************************/
/*Create a New Image with same dimensions as input image                 */
/*************************************************************************/

Image *CreateNewImage(Image * image, char *comment)
{
	Image *outimage;
	int size, j;

	outimage = (Image *)malloc(sizeof(Image));

	outimage->Type = image->Type;
	if (outimage->Type == GRAY)   size = image->Width * image->Height;
	else if (outimage->Type == COLOR) size = image->Width * image->Height * 3;

	outimage->Width = image->Width;
	outimage->Height = image->Height;
	outimage->num_comment_lines = image->num_comment_lines;

	/*--------------------------------------------------------*/
	/* Copy Comments for Original Image      */
	for (j = 0; j<outimage->num_comment_lines; j++)
		strcpy(outimage->comments[j], image->comments[j]);

	/*----------- Add New Comment  ---------------------------*/
	strcpy(outimage->comments[outimage->num_comment_lines], comment);
	outimage->num_comment_lines++;


	outimage->data = (unsigned char *)malloc(size);
	if (!outimage->data){
		printf("cannot allocate memory for new image");
		exit(0);
	}
	return(outimage);
}

Image *binarization3(Image *image){
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
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					local[pos++] = tempin[image->Width * (i + k) + (j + t)];
				}
			}
			int max = findmax(local, 9);
			int min = findmin(local, 9);
			int mid = findMedian(local, 9);
			if (tempin[i * image->Width + j] >= mid){
				tempout[i * image->Width + j] = max;
			}
			else{
				tempout[i * image->Width + j] = min;
			}
		}
	}
	return(outimage);
}

Image *binarization5(Image *image){
	unsigned char *tempin, *tempout, local[25];
	int size, i, j, k, t;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (i = 2; i < image->Height - 2; ++i){
		for (j = 2; j < image->Width - 2; ++j){
			int pos = 0;
			for (k = -1; k < 4; ++k){
				for (t = -1; t < 4; ++t){
					local[pos++] = tempin[image->Width * (i + k) + (j + t)];
				}
			}
			int max = findmax(local, 25);
			int min = findmin(local, 25);
			int mid = findMedian(local, 25);
			if (tempin[i * image->Width + j] >= mid){
				tempout[i * image->Width + j] = max;
			}
			else{
				tempout[i * image->Width + j] = min;
			}
		}
	}
	return(outimage);
}

unsigned char findMedian(unsigned char* arr, int length){
	unsigned char temp;
	int i, j;
	for (i = 0; i < length / 2 + 1; ++i)
		for (j = i + 1; j < length; ++j)
			if (arr[j] < arr[i]){
				temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
	}
	return arr[length / 2];
}

int findmax(unsigned char *arr, int num){
	int max = arr[0];
	for (int i = 1; i < num; i++){
		if (arr[i] > max){
			max = arr[i];
		}
	}
	return max;
}

int findmin(unsigned char *arr, int num){
	int min = arr[0];
	for (int i = 1; i < num; i++){
		if (arr[i] < min){
			min = arr[i];
		}
	}
	return min;
}