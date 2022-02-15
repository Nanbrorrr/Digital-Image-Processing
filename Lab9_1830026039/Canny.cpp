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
void TraceEdge(int w, int h, int nThrLow, Image* pResult, int *pMag);
int min(int x, int y);

Image *IOG(Image *image);
Image *Canny(Image *image);

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

	image1 = ReadPNMImage("headCT-Vandy.pgm");
	image2 = ReadPNMImage("noisy_fingerprint.pgm");

	//outimage = IOG(image1);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\IOGheadCT.pgm");
	//outimage = IOG(image2);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\IOGfingerprint.pgm");

	outimage = Canny(image1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\CannyheadCT.pgm");
	outimage = Canny(image2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab9_1830026039\\outimage\\Cannyfingerprint.pgm");


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

Image *IOG(Image *image){
	unsigned char *tempin, *tempout;
	int size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (int x = 2; x < image->Height - 2; ++x){
		for (int y = 2; y < image->Width - 2; ++y){
			int d = 16 * tempin[x * image->Width + y]
				- tempin[(x - 2) * image->Width + y]
				- tempin[(x - 1) * image->Width + y - 1]
				- 2 * tempin[(x - 1) * image->Width + y]
				- tempin[(x - 1) * image->Width + y + 1]
				- tempin[x * image->Width + y - 2]
				- 2 * tempin[x * image->Width + y - 2]
				- 2 * tempin[x * image->Width + y + 1]
				- tempin[x * image->Width + y + 2]
				- tempin[(x + 1) * image->Width + y - 1]
				- 2 * tempin[(x + 1) * image->Width + y]
				- tempin[(x + 1) * image->Width + y + 1]
				- tempin[(x + 2) * image->Width + y];

			if (d > 255)
				tempout[x * image->Width + y] = 255;
			else
				tempout[x * image->Width + y] = 0;
		}
	}
	return(outimage);
}

int min(int x, int y){
	if (x>y)
		return y;
	else
		return x;
}

Image *Canny(Image *image){
	unsigned char *tempin, *tempout;
	double nSigma = 0.2;
	int nWindowSize = 1 + 2 * ceil(3 * nSigma);
	int nCenter = nWindowSize / 2;
	int nWidth = image->Width;
	int nHeight = image->Height;
	Image *pCanny;
	pCanny = CreateNewImage(image, "#testing Swap");
	tempin = image->data;

	//Generate two - dimensional filter kernel
	double *pKernel_2 = new double[nWindowSize * nWindowSize];
	double d_sum = 0.0;
	for (int i = 0; i < nWindowSize; i++){
		for (int j = 0; j < nWindowSize; j++){
			double n_Disx = i - nCenter;
			double n_Disy = j - nCenter;
			pKernel_2[j*nWindowSize + i] = exp(-0.5*(n_Disx*n_Disx + n_Disy*n_Disy) / (nSigma*nSigma)) / (2.0*3.1415926)*nSigma*nSigma;
			d_sum = d_sum + pKernel_2[j*nWindowSize + i];
		}
	}
	for (int i = 0; i<nWindowSize; i++)//Normalization
	{
		for (int j = 0; j<nWindowSize; j++)
		{
			pKernel_2[j*nWindowSize + i] = pKernel_2[j*nWindowSize + i] / d_sum;
		}
	}
	//Output template
	for (int i = 0; i<nWindowSize*nWindowSize; i++)
	{
		if (i % (nWindowSize) == 0)
		{
			printf("\n");
		}
		printf("%.10f ", pKernel_2[i]);
	}

	for (int s = 0; s<nWidth; s++)
	{
		for (int t = 0; t<nHeight; t++)
		{
			double dFilter = 0;
			double dSum = 0;
			//Current coordinates (s, t)
			//Get 8 neighborhoods
			for (int x = -nCenter; x <= nCenter; x++)
			{
				for (int y = -nCenter; y <= nCenter; y++)
				{
					if ((x + s >= 0) && (x + s<nWidth) && (y + t >= 0) && (y + t<nHeight))//Determine whether it is out of bounds
					{
						double currentvalue = (double)tempin[(y + t)*image->Width + x + s];
						dFilter += currentvalue*pKernel_2[x + nCenter + (y + nCenter)*nCenter];
						dSum += pKernel_2[x + nCenter + (y + nCenter)*nCenter];
					}
				}
			}
			pCanny->data[t*image->Width + s] = unsigned char(dFilter / dSum);
		}
	}

	double *P = new double[nWidth*nHeight];
	double *Q = new double[nWidth*nHeight];
	int *M = new int[nWidth*nHeight];
	double *Theta = new double[nWidth*nHeight];
	int nwidthstep = pCanny->Width;
	for (int iw = 0; iw < nWidth - 1; iw++){
		for (int jh = 0; jh < nHeight - 1; jh++){
			P[jh*nWidth + iw] = (double)(pCanny->data[min(iw + 1, nWidth - 1) + jh*nwidthstep] - pCanny->data[iw + jh*nwidthstep] +
				pCanny->data[min(iw + 1, nWidth - 1) + min(jh + 1, nHeight - 1)*nwidthstep] - pCanny->data[iw + min(jh + 1, nHeight - 1)*nwidthstep]) / 2;
			Q[jh*nWidth + iw] = (double)(pCanny->data[iw + jh*nwidthstep] - pCanny->data[iw + min(jh + 1, nHeight - 1)*nwidthstep] +
				pCanny->data[min(iw + 1, nWidth - 1) + jh*nwidthstep] - pCanny->data[min(iw + 1, nWidth - 1) + min(jh + 1, nHeight - 1)*nwidthstep]) / 2;
		}
	}
	for (int iw = 0; iw < nWidth - 1; iw++){
		for (int jh = 0; jh < nHeight - 1; jh++){
			M[jh*nWidth + iw] = (int)sqrt(P[jh*nWidth + iw] * P[jh*nWidth + iw] + Q[jh*nWidth + iw] * Q[jh*nWidth + iw] + 0.5);
			Theta[jh*nWidth + iw] = atan2(Q[jh*nWidth + iw], P[jh*nWidth + iw]) * 180 / 3.1415;
			if (Theta[jh*nWidth + iw] < 0){
				Theta[jh*nWidth + iw] = 360 + Theta[jh*nWidth + iw];
			}
		}
	}

	Image *N;//Non-maximum suppression results
	N = CreateNewImage(image, "#testing Swap");
	Image *OpencvCannyimg;
	OpencvCannyimg = CreateNewImage(image, "#testing Swap");
	int g1 = 0, g2 = 0, g3 = 0, g4 = 0;
	double dTmp1 = 0.0, dTmp2 = 0.0;
	double dWeight = 0.0;

	for (int i = 1; i < nWidth - 1; i++){
		for (int j = 1; j < nHeight - 1; j++){
			if (M[i + j*nWidth] == 0){
				N->data[i + j*nwidthstep] = 0;
			}
			else{
				if ((Theta[i + j*nWidth] >= 90 && Theta[i + j*nWidth] < 135) || (Theta[i + j*nWidth] >= 270 && Theta[i + j*nWidth] < 315)){
					g1 = M[i - 1 + (j - 1)*nWidth];
					g2 = M[i + (j - 1)*nWidth];
					g3 = M[i + (j + 1)*nWidth];
					g4 = M[i + 1 + (j + 1)*nWidth];
					dWeight = fabs(P[i + j*nWidth]) / fabs(Q[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
				}
				else if ((Theta[i + j*nWidth] >= 135 && Theta[i + j*nWidth]<180) || (Theta[i + j*nWidth] >= 315 && Theta[i + j*nWidth]<360)){
					g1 = M[i - 1 + (j - 1)*nWidth];
					g2 = M[i - 1 + (j)*nWidth];
					g3 = M[i + 1 + (j)*nWidth];
					g4 = M[i + 1 + (j + 1)*nWidth];
					dWeight = fabs(Q[i + j*nWidth]) / fabs(P[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
				}
				else if ((Theta[i + j*nWidth] >= 45 && Theta[i + j*nWidth]<90) || (Theta[i + j*nWidth] >= 225 && Theta[i + j*nWidth]<270)){
					g1 = M[i + 1 + (j - 1)*nWidth];
					g2 = M[i + (j - 1)*nWidth];
					g3 = M[i + (j + 1)*nWidth];
					g4 = M[i - 1 + (j + 1)*nWidth];
					dWeight = fabs(P[i + j*nWidth]) / fabs(Q[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
				}
				else if ((Theta[i + j*nWidth] >= 0 && Theta[i + j*nWidth]<45) || (Theta[i + j*nWidth] >= 180 && Theta[i + j*nWidth]<225)){
					g1 = M[i + 1 + (j - 1)*nWidth];
					g2 = M[i + 1 + (j)*nWidth];
					g3 = M[i - 1 + (j)*nWidth];
					g4 = M[i - 1 + (j + 1)*nWidth];
					dWeight = fabs(Q[i + j*nWidth]) / fabs(P[i + j*nWidth]);
					dTmp1 = g1*dWeight + (1 - dWeight)*g2;
					dTmp2 = g4*dWeight + (1 - dWeight)*g3;
				}
			}
			if ((M[i + j*nWidth] >= dTmp1) && (M[i + j*nWidth] >= dTmp2)){
				N->data[i + j*nwidthstep] = 200;
			}
			else{
				N->data[i + j*nwidthstep] = 0;
			}
		}
	}

	int nHist[1024];//Histogram
	int nEdgeNum;//The number of all edge points
	int nMaxMag = 0;//The magnitude of the maximum gradient
	for (int k = 0; k < 1024; k++){
		nHist[k] = 0;
	}
	for (int wx = 0; wx < nWidth; wx++){
		for (int hy = 0; hy < nHeight; hy++){
			if ((unsigned char)N->data[wx + hy*N->Width] == 200){
				int Mindex = M[wx + hy*nWidth];
				nHist[M[wx + hy*nWidth]]++;//Gradient histogram
			}
		}
	}
	nEdgeNum = 0;
	for (int index = 1; index < 1024; index++){
		if (nHist[index] != 0){
			nMaxMag = index;
		}
		nEdgeNum += nHist[index];//How many edge pixels are there after non-maximum suppression
	}
	//Calculate two thresholds
	int nThrHigh;
	int nThrLow;
	double dRateHigh = 0.7;
	double dRateLow = 0.5;
	int nHightcount = (int)(dRateHigh*nEdgeNum + 0.5);
	int count = 1;
	nEdgeNum = nHist[1];
	while ((nEdgeNum <= nHightcount) && (count < nMaxMag - 1)){
		count++;
		nEdgeNum += nHist[count];
	}
	nThrHigh = count;
	nThrLow = (int)(nThrHigh*dRateLow + 0.5);
	printf("\nThe length of the histogram %d \n  ", nMaxMag);
	printf("\nThreshold amplitude of gradient large %d small %d \n  ", nThrHigh, nThrLow);

	for (int is = 1; is < nWidth - 1; is++){
		for (int jt = 1; jt < nHeight - 1; jt++){
			int currentvalue = (unsigned char)(N->data[is + jt*N->Width]);
			if ((currentvalue == 200) && (M[is + jt*nWidth] >= nThrHigh)){
				N->data[is + jt*nwidthstep] = 255;
				TraceEdge(is, jt, nThrLow, N, M);//Neighborhood point judgment
			}
		}
	}
	for (int si = 1; si < nWidth - 1; si++){
		for (int tj = 1; tj < nHeight - 1; tj++){
			if ((unsigned char)N->data[si + tj*nwidthstep] != 255){
				N->data[si + tj*nwidthstep] = 0;
			}
		}
	}
	return N;
}

void TraceEdge(int w, int h, int nThrLow, Image* pResult, int *pMag){
	int xNum[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int yNum[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int xx = 0;
	int yy = 0;
	bool change = true;
	while (change){
		change = false;
		for (int k = 0; k < 8; k++){
			xx = w + xNum[k];
			yy = h + yNum[k];
			//   If the pixel is a possible boundary point, it has not been processed  
			//   And the gradient is greater than the threshold  
			int curgrayvalue = (unsigned char)pResult->data[xx + yy*pResult->Width];
			int curgrdvalue = pMag[xx + yy*pResult->Width];
			if (curgrayvalue == 200 && curgrdvalue>nThrLow){
				change = true;
				//Set this point as a boundary point  
				pResult->data[xx + yy*pResult->Width] = 255;
				h = yy;
				w = xx;
				break;
			}
		}
	}
}