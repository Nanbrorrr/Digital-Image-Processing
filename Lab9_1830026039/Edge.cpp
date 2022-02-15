//ID:	1830026039
//NAME:	Gabriel Hou
//IDE:	VS2013
//DATE:	2021/3/25
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

Image *Roberts(Image *image);
Image *Prewitt(Image *image);
Image *Sobel(Image *image);

int main(int argc, char **argv)
{

	TestReadImage(argv[1], argv[2]);

	return(0);
}

int TestReadImage(char *filename, char *outfilename)
{
	Image *image1;
	Image *image2;
	Image *image3;
	Image *outimage;

	image1 = ReadPNMImage("headCT-Vandy.pgm");
	image2 = ReadPNMImage("building_original.pgm");
	image3 = ReadPNMImage("noisy_fingerprint.pgm");

	outimage = Sobel(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\sobelheadCT.pgm");
	outimage = Sobel(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\sobelbuilding.pgm");
	outimage = Sobel(image3);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\sobelfingerprint.pgm");
	
	outimage = Roberts(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\RobertsheadCT.pgm");
	outimage = Roberts(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\Robertsbuilding.pgm");
	outimage = Roberts(image3);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\Robertsfingerprint.pgm");

	outimage = Prewitt(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\PrewittsheadCT.pgm");
	outimage = Prewitt(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\Prewittbuilding.pgm");
	outimage = Prewitt(image3);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\Prewittfingerprint.pgm");

	return(0);
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

Image *Roberts(Image *image){
	unsigned char *tempin, *tempout;
	int size;
	int result;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (int x = 1; x < image->Height - 1; ++x){
		for (int y = 1; y < image->Width - 1; ++y){
			double dx = (tempin[x * image->Width + y]
				- tempin[(x + 1) * image->Width + y + 1]);

			double dy = (tempin[(x + 1) * image->Width + y]
				- tempin[x * image->Width + y + 1]);

			result = abs(dx) + abs(dy);
			if (result > 255)
				tempout[x * image->Width + y] = 255;
			else
				tempout[x * image->Width + y] = result;
		}
	}
	return(outimage);
}

Image *Sobel(Image *image){
	unsigned char *tempin, *tempout;
	int result;
	int size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (int x = 1; x < image->Height - 1; ++x){
		for (int y = 1; y < image->Width - 1; ++y){
			int dx = abs(tempin[(x + 1) * image->Width + y - 1]
				+ 2 * tempin[(x + 1) * image->Width + y]
				+ tempin[(x + 1) * image->Width + y + 1]
				- tempin[(x - 1) * image->Width + y - 1]
				- 2 * tempin[(x - 1) * image->Width + y]
				- tempin[(x - 1) * image->Width + y + 1]);

			int dy = abs(tempin[(x - 1) * image->Width + y + 1]
				+ 2 * tempin[x * image->Width + y - 1]
				+ tempin[(x + 1) * image->Width + y - 1]
				- tempin[(x - 1) * image->Width + y + 1]
				- 2 * tempin[x * image->Width + y + 1]
				- tempin[(x + 1) * image->Width + y + 1]);

			result = abs(dx) + abs(dy);
			if (result > 255)
				tempout[x * image->Width + y] = 255;
			else
				tempout[x * image->Width + y] = result;
		}
	}
	return(outimage);
}

Image *Prewitt(Image *image){
	unsigned char *tempin, *tempout;
	int size;
	int result;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (int x = 1; x < image->Height - 1; ++x){
		for (int y = 1; y < image->Width - 1; ++y){
			int dx = abs(tempin[(x + 1) * image->Width + y - 1]
				+ tempin[(x + 1) * image->Width + y]
				+ tempin[(x + 1) * image->Width + y + 1]
				- tempin[(x - 1) * image->Width + y - 1]
				- tempin[(x - 1) * image->Width + y]
				- tempin[(x - 1) * image->Width + y + 1]);

			int dy = abs(tempin[(x - 1) * image->Width + y + 1]
				+ tempin[x * image->Width + y - 1]
				+ tempin[(x + 1) * image->Width + y - 1]
				- tempin[(x - 1) * image->Width + y + 1]
				- tempin[x * image->Width + y + 1]
				- tempin[(x + 1) * image->Width + y + 1]);

			result = abs(dx) + abs(dy);
			if (result > 255)
				tempout[x * image->Width + y] = 255;
			else
				tempout[x * image->Width + y] = result;
		}
	}
	return(outimage);
}