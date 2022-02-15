#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
	Mat img1 = imread("noise_fingerprint.pgm");
	imshow("source_fingerprint", img1);
	Mat out1;

	Mat img2 = imread("noisy_rectangle.pgm");
	namedWindow("source_rectangle", cv::WINDOW_NORMAL);
	imshow("source_rectangle", img2);
	Mat out2;

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); 
	//dilation
	dilate(img1, out1, element);
	dilate(img2, out2, element);
	imshow("dilation_fingerprint", out1);
	namedWindow("dilation_rectangle", cv::WINDOW_NORMAL);
	imshow("dilation_rectangle", out2);
	waitKey(0);

	return 0;
}
