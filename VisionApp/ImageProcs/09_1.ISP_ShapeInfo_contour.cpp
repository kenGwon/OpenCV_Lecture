#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);

	/*
	  �䱸����
	  1. ������ ���� min max������ ��ǥ 4�� ���ϱ�(�������� �� ������ �����ʿ�) --> HSV�� ��ȯ�Ͽ� �ذ��ϸ� ��
	  2. ������ 4���� ������ �׸���
	  3. �ؽ�Ʈ �߰��ϱ� = putText() �Լ�
	     - Area = ??
		 - Len = ??
		 - X, Y = (??, ??)
		 - Radius = ??
		 - AvgBrightness = ??
	*/

	// RGB -> hsv ������ ��ȯ
	cv::Mat src_hsv = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
	cvtColor(src, src_hsv, COLOR_RGB2HSV); // ������ 25 ~ 35
	cv::Mat src_hsv_masked = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
	
	// ������ ����ŷ
	Scalar low_red_hsv = Scalar(120, 20, 100);
	Scalar high_red_hsv = Scalar(140, 255, 255);
	inRange(src_hsv, low_red_hsv, high_red_hsv, src_hsv_masked);

	// ������ ����
	Mat src_hsv_masked_clean;
	medianBlur(src_hsv_masked, src_hsv_masked_clean, 3);

	// �ٱ� �ܰ����� ����
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_hsv_masked_clean, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(src_hsv_masked_clean.size(), CV_8UC3);
	
	// ROI�� ��� ��� ����� ���� GRAY_SCALE ���󿡼� ������ ������ �ѹ��� ����ŷ
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_RGB2GRAY);
	Mat src_gray_masked;
	src_gray_masked = src_gray - (255 - src_hsv_masked_clean);

	for (size_t i = 0; i < contours.size(); i++)
	{
		printf("for�� ��� ����?\n");

		// ������ ������ ���δ� �� �׸���
		Point2f target_center;
		float target_radius;
		minEnclosingCircle(contours[i], target_center, target_radius);
		circle(src, target_center, static_cast<int>(target_radius), Scalar(30, 0, 30), 3);

		// ���� ���� ����
		double area = contourArea(contours[i]);
		double arcLen = arcLength(contours[i], true);
		
		// ���� ����(ROI, Region Of Interest)�� GRAY_SCALE���� ��� ��� ���
		int search_range = ((target_center.x + (int)target_radius) - (target_center.x - (int)target_radius))
			* ((target_center.y + (int)target_radius) - (target_center.y - (int)target_radius));
		int start_idx = (target_center.x - (int)target_radius) * src_gray_masked.cols
			+ (target_center.y + (int)target_radius) - (target_center.y - (int)target_radius);
		int sum_of_brightness = 0;
		int pixel_cnt = 0;
		for (size_t i = 0; i < search_range; i++)
		{
			int pixel_brightness = src_gray_masked.data[start_idx++];
			if (pixel_brightness == 0)
			{
				continue;
			}
			else
			{
				sum_of_brightness += pixel_brightness;
				pixel_cnt++;
			}
		}
		float AvgBrightness = sum_of_brightness / pixel_cnt;

		// putText �۾�
		Point ptTxt = Point(target_center.x + static_cast<int>(target_radius),
			target_center.y - static_cast<int>(target_radius));

		String msg;
		msg = std::format("area = {:.1f}", area);
		putText(src, msg, Point(ptTxt.x, ptTxt.y + 30 * 0), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0), 1, 8);
		msg = std::format("length = {:.1f}", arcLen);
		putText(src, msg, Point(ptTxt.x, ptTxt.y + 30 * 1), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0), 1, 8);
		msg = std::format("x,y = {:.1f}, {:.1f}", target_center.x, target_center.y);
		putText(src, msg, Point(ptTxt.x, ptTxt.y + 30 * 2), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0), 1, 8);
		msg = std::format("Radius = {:.1f}", target_radius);
		putText(src, msg, Point(ptTxt.x, ptTxt.y + 30 * 3), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0), 1, 8);
		msg = std::format("AvgBrightness = {:.1f}", AvgBrightness);
		putText(src, msg, Point(ptTxt.x, ptTxt.y + 30 * 4), FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0), 1, 8);
	}

	return 1;
}
