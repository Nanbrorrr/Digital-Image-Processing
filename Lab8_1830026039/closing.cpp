#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{ 
	Mat img1 = imread("noise_fingerprint.pgm");
	imshow("source_fingerprint", img1);
	Mat out1;

	Mat img2 = imread("noisy_rectangle.pgm");
	namedWindow("opening_rectangle", cv::WINDOW_NORMAL);
	imshow("source_rectangle", img2);
	Mat out2;

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	//closing
	dilate(img1, out1, element);
	Mat out3;
	erode(out1, out3, element);

	dilate(img2, out2, element);
	Mat out4;
	erode(out2, out4, element);
	imshow("closing_fingerprint", out3);
	namedWindow("closing_rectangle", cv::WINDOW_NORMAL);
	imshow("closing_rectangle", out4);
	waitKey(0);
}

