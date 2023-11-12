#include "Common_kenGwon.h"

void setLabel(Mat& img, const vector<Point>& pts, const String& label)
{
	Rect rc = boundingRect(pts);
	rectangle(img, rc, Scalar(0, 0, 255), 1);
	putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
}

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/detect_blob.png";
	cv::Mat img = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);

	if (img.empty())
	{
		std::cerr << "Image load fail" << std::endl;
		return -1;
	}

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	Mat bin;
	threshold(gray, bin, 200, 255, THRESH_BINARY_INV | THRESH_OTSU);

	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	for (auto& pts : contours)
	{
		if (contourArea(pts) < 400)
		{
			continue;
		}

		vector<Point> approx;
		approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);

		int vtc = static_cast<int>(approx.size());

		if (vtc == 3)
		{
			setLabel(img, pts, "Triangle");
		}
		else if (vtc == 4)
		{
			setLabel(img, pts, "Rectancle");
		}
		else if (vtc > 4)
		{
			double len = arcLength(pts, true);
			double area = contourArea(pts);
			double ratio = 4. * CV_PI * area / (len * len);

			if (ratio > 0.8)
			{
				setLabel(img, pts, "Circle");
			}
		}
	}

	imshow("img", img);
	waitKey();


	return 1;
}