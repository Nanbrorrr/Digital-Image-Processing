#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

//Edge detection
int main()
{
	Mat img1 = imread("licoln.pgm");
	Mat img2 = imread("U.pgm");

	imshow("source_licoln", img1);
	imshow("source_U", img2);

	Mat grad_x_1, grad_y_1;
	Mat abs_grad_x_1, abs_grad_y_1, dst_1;

	Mat grad_x_2, grad_y_2;
	Mat abs_grad_x_2, abs_grad_y_2, dst_2;

	//Find the x-direction gradient
	Sobel(img1, grad_x_1, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x_1, abs_grad_x_1);
	Sobel(img2, grad_x_2, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x_2, abs_grad_x_2);

	//Find the y-direction gradient
	Sobel(img1, grad_y_1, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y_1, abs_grad_y_1);
	Sobel(img2, grad_y_2, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y_2, abs_grad_y_2);

	//combine gradient
	addWeighted(abs_grad_x_1, 0.5, abs_grad_y_1, 0.5, 0, dst_1);
	addWeighted(abs_grad_x_2, 0.5, abs_grad_y_2, 0.5, 0, dst_2);
	imshow("Bound_licoln", dst_1);
	imshow("Bound_U", dst_2);


	waitKey(0);

}