//ID:	1830026039
//NAME: Gabriel Hou
//IDE: VS2013
//DATE: 2021/5/21
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
double dabs(double a, double b);
void calculate_histogram(int height, int width, Image *image, long histogram[]);
void calculate_pi(int height, int width, long histogram[], double pi[]);
double p1(int k, double pi[]);
double m(int k, double pi[]);
double calculate_sigma(int k, double mg, double pi[]);


Image *OTSU(Image *image);

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

	image1 = ReadPNMImage("large_septagon_gaussian_noise_mean_0_std_50_added.pgm");
	image2 = ReadPNMImage("avegmedian.pgm");

	outimage = OTSU(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab10_1830026039\\outimage\\OTSU_septagon.pgm"); 
	outimage = OTSU(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab10_1830026039\\outimage\\OTSU_55septagon.pgm");

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

double dabs(double a, double b){
	if (a < b)
		return b - a;
	else
		return a - b;
}

void calculate_histogram(int height, int width, Image *image,long histogram[]){
	short k;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			k = image->data[i * width + j];
			histogram[k] = histogram[k] + 1;
		}
	}
}

void calculate_pi(int height, int width,long histogram[], double pi[]){
	for (int i = 0; i < 255; ++i){
		pi[i] = (double)histogram[i] / (double)(height * width);
	}
}

double p1(int k, double pi[]){
	double sum = 0.0;
	for (int i = 0; i <= k; i++){
		sum += pi[i];
	}
	return sum;
}

double m(int k, double pi[]){
	double sum = 0.0;
	for (int i = 0; i <= k; i++){
		sum += i * pi[i];
	}
	return sum;
}

double calculate_sigma(int k, double mg, double pi[]){
	double p1k = p1(k, pi);
	double mk = m(k, pi);
	if (p1k < (1e-10) || (1 - p1k) < (1e-10))
		return 0.0;
	else
		return pow(mg * p1k - mk, 2) / (p1k * (1 - p1k));
}

Image *OTSU(Image *image){
	unsigned char *tempin, *tempout;
	int size;
	Image *outimage;
	int height = image->Height;
	int width = image->Width;
	long histogram[256] = {};
	double pi[256] = {};
	double sigma[256] = {};
	double mg;
	short max_count = 0;
	short k = 0;
	double max_value = 0.0;
	double k_star;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	calculate_histogram(height, width, image, histogram);
	calculate_pi(height, width, histogram, pi);
	mg = m(255, pi);

	for (int i = 0; i < 256; i++){
		sigma[i] = calculate_sigma(i, mg, pi);
	}

	max_value = sigma[0];
	max_count = 1;
	k = 0;
	for (int i = 0; i < 256; i++){
		if (dabs(sigma[i], max_value) < 1e-10){
			k += i;
			max_count++;
		}
		else if (sigma[i] > max_value){
			max_value = sigma[i];
			max_count = 1;
			k = i;
		}
	}
	k_star = k / max_count;
	printf("%1f\n", k_star);
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (tempin[i * width + j] < k_star)
				tempout[i * width + j] = 0;
			else
				tempout[i * width + j] = 255;
		}
	}
	return(outimage);
}