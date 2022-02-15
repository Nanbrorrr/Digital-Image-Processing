//ID:	1830026039
//NAME:	Gabriel Hou
//IDE:	VS2013
//DATE:	2021/4/13
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.1415926
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

Image *DFT(Image *image);

int main(int argc, char **argv)
{

	TestReadImage(argv[1], argv[2]);

	return(0);
}

int TestReadImage(char *filename, char *outfilename)
{
	Image *image;
	Image *outimage;

	image = ReadPNMImage("lena.pgm");

	outimage = DFT(image);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab5_1830026039\\outimage\\DFT.pgm");
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

Image *DFT(Image *image){
	unsigned char *tempin, *tempout, *tempout2, *tempout3;
	int size;
	int move;
	Image *outimage;
	Image *outimage2;
	Image *outimage3;

	outimage = CreateNewImage(image, "#testing Swap");
	outimage2 = CreateNewImage(image, "#testing Swap");
	outimage3 = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;
	tempout2 = outimage2->data;
	tempout3 = outimage3->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (int i = 0; i < image->Width; i++){
		for (int j = 0; j < image->Height; j++){
			double ak = 0;
			double bk = 0;
			for (int ii = 0; ii < image->Width; ii++){
				for (int jj = 0; jj < image->Height; jj++){
					float temp = i * ii / (float)image->Width + j * jj / (float)image->Height;
					move = (ii + jj) % 2 == 0 ? 1 : -1;
					ak += tempin[image->Width * ii + jj] * cos(-2.0 * PI * temp) * move;
					bk += tempin[image->Width * ii + jj] * sin(-2.0 * PI * temp) * move;
				}
			}
			tempout2[image->Width * i + j] = ak / double(size);
			tempout3[image->Width * i + j] = bk / double(size);
			tempout[image->Width * i + j] = sqrt((double)(ak * ak + bk * bk)) / sqrt((double)size);
		}
	}
	SavePNMImage(outimage2, "C:\\Users\\94510\\Desktop\\Lab5_1830026039\\outimage\\DFT_1.pgm");
	SavePNMImage(outimage3, "C:\\Users\\94510\\Desktop\\Lab5_1830026039\\outimage\\DFT_2.pgm");
	return(outimage);
}