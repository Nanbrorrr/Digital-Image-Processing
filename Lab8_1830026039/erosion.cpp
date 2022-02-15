#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int main()
{
	//‘ÿ»Î‘≠Õº  
	Mat img1 = imread("noise_fingerprint.pgm");
	imshow("source",img1);
	Mat out1;

	Mat img2 = imread("noisy_rectangle.pgm");
	namedWindow("source_rectangle", cv::WINDOW_NORMAL);
	imshow("source_rectangle", img2);
	Mat out2;

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	//erode
	erode(img1, out1, element);
	erode(img2, out2, element);
	imshow("erosion_fingerprint", out1);
	namedWindow("erosion_rectangle", cv::WINDOW_NORMAL);
	imshow("erosion_rectangle", out2);
	waitKey(0);

	return 0;
}

