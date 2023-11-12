#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/ellipses.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);
	
	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	// erosion & dilation
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat dst_erode, dst_dilate;
	erode(src, dst_erode, Mat());
	dilate(src, dst_dilate, noArray());

	// opening(erode ÈÄ dilate) & closing(dilate ÈÄ erode)
	Mat dst_open, dst_close, dst_gradient;
	morphologyEx(src, dst_open, MORPH_OPEN, noArray());
	morphologyEx(src, dst_close, MORPH_CLOSE, Mat());
	morphologyEx(src, dst_gradient, MORPH_GRADIENT, Mat());

	return 1;
}
