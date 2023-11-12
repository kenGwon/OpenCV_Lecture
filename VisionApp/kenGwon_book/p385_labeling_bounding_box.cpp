#include "Common_kenGwon.h"

int main()
{
	std::string fileName = "../KCCImageNet/keyboard.bmp";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	if (src.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat labels, stats, centroids;
	int num_of_labels = connectedComponentsWithStats(bin, labels, stats, centroids);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (int i = 0; i < num_of_labels; i++)
	{
		int* p = stats.ptr<int>(i);

		if (p[4] < 20) continue; // 면적이 20보다 작으면 박스를 치지 않겠다

		rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 255), 2);
	}

	return 1;
}
