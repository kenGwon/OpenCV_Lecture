#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/shapes.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	uchar* pData = src_gray.data;
	size_t width = src_gray.cols;
	size_t height = src_gray.rows;

	cv::Mat src_bin = Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::Mat src_obj = Mat::zeros(cv::Size(width, height), CV_8UC1);
	uchar* pDataBin = src_bin.data;

	int threshold_min = 60;//0~255
	int threshold_max = 200;//0~255
	//이진화, Binary
	for (size_t i = 0; i < width * height; i++)
	{
		int value = pData[i];

		//if (value > threshold_max)
		//{
		//	pDataBin[i] = 0;
		//}
		//else
		//	pDataBin[i] = 255;
		(value > threshold_max) ? pDataBin[i] = 0 : pDataBin[i] = 255;
	}
	src_obj = src_bin & src_gray;

	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(src_bin.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}

	// 도형의 무게중심 구하기
	/*
	for (size_t i = 0; i < contours.size(); i++)
	{	
		int CoGx, CoGy; // Center of Gravity... 무게중심
		CoGx = CoGy = 0;
		int accX, accY;
		accX = accY = 0;

		for (size_t n = 0; n < contours[i].size(); n++)
		{
			int n_x = contours[i].at(n).x;
			int n_y = contours[i].at(n).y;

			accX += contours[i].at(n).x;
			accY += contours[i].at(n).y;
		}
		CoGx = accX / contours[i].size();
		CoGy = accY / contours[i].size();

		cv::line(drawing, Point(CoGx - 10, CoGy - 10), Point(CoGx + 10, CoGy + 10), CV_RGB(255, 0, 0), 3);
		cv::line(drawing, Point(CoGx + 10, CoGy - 10), Point(CoGx - 10, CoGy + 10), CV_RGB(255, 0, 0), 3);
	}
	*/

	Mat src_color;
	cv::cvtColor(src_gray, src_color, ColorConversionCodes::COLOR_GRAY2BGR);
	
	// 점 4개를 구해서 line()함수를 4번 호출하여 사각형을 그려라
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		RotatedRect rrt = minAreaRect(contours[i]);
		double arcLen = arcLength(contours[i], true);

		Point ptTxt = Point(rrt.boundingRect().x, rrt.boundingRect().y);
		string msg;
		msg = std::format("area = {:.1f}", area);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 0), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("x,y = {:.1f}, {:.1f}", rrt.center.x, rrt.center.y);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 1), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		msg = std::format("length = {:.1f}", arcLen);
		putText(src_color, msg, Point(ptTxt.x, ptTxt.y + 30 * 2), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(10, 0, 10), 1, 8);
		cv::rectangle(src_color, rrt.boundingRect2f().tl(), rrt.boundingRect2f().br(), CV_RGB(0, 0, 255));
		cv::drawMarker(src_color, rrt.center, CV_RGB(255, 0, 0));

		const int ptSz = 4;
		Point2f pt[ptSz];
		rrt.points(pt);

		for (size_t i = 0; i < ptSz; i++)
		{
			cv::line(src_color, pt[i % ptSz], pt[(i + 1) % ptSz], CV_RGB(255, 0, 255), 1);
		}

		continue;



		int max_x, max_y, min_x, min_y;
		max_x = 0;
		max_y = 0;
		min_x = src_color.cols;
		min_y = src_color.rows;

		for (size_t n = 0; n < contours[i].size(); n++)
		{
			int n_x = contours[i].at(n).x;
			int n_y = contours[i].at(n).y;

			(n_x < min_x) ? min_x = n_x : min_x = min_x;
			(n_y < min_y) ? min_y = n_y : min_y = min_y;
			(n_x > max_x) ? max_x = n_x : max_x = max_x;
			(n_y > max_y) ? max_y = n_y : max_y = max_y;
		}

		vector<Point> four_points;
		four_points.push_back(Point(min_x, min_y));
		four_points.push_back(Point(min_x, max_y));
		four_points.push_back(Point(max_x, max_y));
		four_points.push_back(Point(max_x, min_y));
		
		for (size_t i = 0; i < four_points.size(); i++)
		{
			cv::line(src_color,
				four_points[i % four_points.size()], four_points[(i + 1) % four_points.size()],
				CV_RGB(255, 0, 0), 3);
		}
	}

	return 1;
}
