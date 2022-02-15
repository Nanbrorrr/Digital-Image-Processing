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
	//opening
	erode(img1, out1, element);
	Mat out3;
	dilate(out1, out3, element);

	erode(img2, out2, element);
	Mat out4;
	dilate(out2, out4, element);
	imshow("opening_fingerprint", out3);
	namedWindow("opening_rectangle", cv::WINDOW_NORMAL);
	imshow("opening_rectangle", out4);
	waitKey(0);
}

