//ID:	1830026039
//NAME: Gabriel Hou
//IDE: VS2013
//DATE: 2021/5/21
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace std;
using namespace cv;

void RegionGrow(Mat& src, Mat& matDst, Point2i pt, int th)
{
	Point2i ptGrowing;
	int nGrowLable = 0;
	int nSrcValue = 0;
	int nCurValue = 0;
	matDst = Mat::zeros(src.size(), CV_8UC1);
	int dir[8][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, 
					{ 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 } };
	vector<Point2i> vcGrowPt;
	vcGrowPt.push_back(pt);
	matDst.at<uchar>(pt.y, pt.x) = 255;
	nSrcValue = src.at<uchar>(pt.y, pt.x);
	while (!vcGrowPt.empty()) {
		pt = vcGrowPt.back();
		vcGrowPt.pop_back();
		for (int i = 0; i<8; ++i) {
			ptGrowing.x = pt.x + dir[i][0];
			ptGrowing.y = pt.y + dir[i][1];
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) 
						|| (ptGrowing.y > src.rows - 1))
				continue;
			nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);
			if (nGrowLable == 0) {
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
				if (abs(nSrcValue - nCurValue) < th) {
					matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;
					vcGrowPt.push_back(ptGrowing);
				}
			}
		}
	}
}
int main() {
	/*Mat src1 = imread("C:\\Users\\94510\\Desktop\\Lab10_1830026039\\noisy_region.pgm");
	imshow("noisy_region", src1);
	cvtColor(src1, src1, COLOR_RGB2GRAY);
	Point2i pt1; 
	pt1 = Point2i(200, 180);
	Mat dst1, result1;
	RegionGrow(src1, dst1, pt1, th);
	threshold(dst1, result1, 127, 255, cv::THRESH_BINARY);
	imshow("RegionGrow_noisy", dst1);*/
	Mat src2 = imread("C:\\Users\\94510\\Desktop\\Lab10_1830026039\\defective_weld.pgm");
	imshow("defective_weld", src2);
	cvtColor(src2, src2, COLOR_RGB2GRAY);
	Point2i pt2;
	pt2 = Point2i(200, 110);
	Mat dst2, result2;
	int th = 40;
	RegionGrow(src2, dst2, pt2, th);
	threshold(dst2, result2, 127, 255, cv::THRESH_BINARY);
	imshow("RegionGrow_weld", dst2);
	waitKey(0);
	return 0;
}