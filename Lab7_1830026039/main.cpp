//ID:	1830026039
//NAME:	Gabriel Hou
//IDE:	VS2013
//DATE:	2021/4/30
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "proto.h"
#include "fft.h"
#include "tool.h"

int TestReadImage(char *, char *);

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
	Image *image4;
	Image *image5;
	Image *imageD1;
	Image *imageD2;
	Image *imageD3;
	Image *outimage;

	image1 = ReadPNMImage("bridge.pgm");
	image2 = ReadPNMImage("goldhill.pgm");
	image3 = ReadPNMImage("lena.pgm");
	image4 = ReadPNMImage("renoiselena1.pgm");
	image5 = ReadPNMImage("cameraWithNoise.pgm");

	imageD1 = ReadPNMImage("lenaD1.pgm");
	imageD2 = ReadPNMImage("lenaD2.pgm");
	imageD3 = ReadPNMImage("lenaD3.pgm");

	//outimage = Homofilter(image1);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\Homobridge.pgm");
	//outimage = Homofilter(image2);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\Homogoldhill.pgm");
	
	//outimage = bandreject(image3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\bandlena.pgm");
	
	//outimage = noise(image4);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\renoiselena.pgm");
	//outimage = MedianFilt(image5);  
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\renoisecamera.pgm");

	//outimage = arithmeticmeanFilt(imageD1, 3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\arithmeticD1.pgm");
	//outimage = arithmeticmeanFilt(imageD2, 3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\arithmeticD2.pgm");
	//outimage = arithmeticmeanFilt(imageD3, 3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\arithmeticD3.pgm");

	//outimage = GeometricmeanFilt(imageD1);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\GeometricD1.pgm");
	//outimage = GeometricmeanFilt(imageD2);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\GeometricD2.pgm");
	//outimage = GeometricmeanFilt(imageD3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\GeometricD3.pgm");
	
	outimage = MedianFilt(imageD1);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\MedianD1.pgm");
	outimage = MedianFilt(imageD2);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\MedianD2.pgm");
	outimage = MedianFilt(imageD3);
	SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\MedianD3.pgm");

	//outimage = AlphatrimmedmeanFilt(imageD1);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\AlphatrimD1.pgm");
	//outimage = AlphatrimmedmeanFilt(imageD2);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\AlphatrimD2.pgm");
	//outimage = AlphatrimmedmeanFilt(imageD3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\AlphatrimD3.pgm");

	//outimage = adaptivemedianFilter(imageD1);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\adaptiveD1.pgm");
	//outimage = adaptivemedianFilter(imageD2);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\adaptiveD2.pgm");
	//outimage = adaptivemedianFilter(imageD3);
	//SavePNMImage(outimage, "C:\\Users\\94510\\Desktop\\Lab7_1830026039\\outimage\\adaptiveD3.pgm");

	return(0);
}
