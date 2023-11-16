#pragma once

#include "ISP.h"

int main()
{
	std::string fileName = "../KCCImageNet/stop_img.png";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);

	/*
	  요구사항
	  1. 빨간색 영역을 외접하는 원을 그리기
		 - 색공간 RGB -> HSV로 변환
		 - findContour() 활용?
	  2. 빨간색 영역에 대한 각종 정보 텍스트 출력하기
		 - Area = ??
		 - Len = ??
		 - X, Y = (??, ??)
		 - Radius = ??
		 - AvgBrightness = ??
	*/

	// RGB -> hsv 색공간 변환
	cv::Mat src_hsv = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
	cvtColor(src, src_hsv, COLOR_BGR2HSV); // 빨간색 25 ~ 35 // 여기 코드 COLOR_RGB2HSV였음.
	cv::Mat src_hsv_masked = Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);

	// 빨간색 마스킹
#if 0
	Scalar low_red_hsv = Scalar(120, 20, 100);
	Scalar high_red_hsv = Scalar(140, 255, 255);
	inRange(src_hsv, low_red_hsv, high_red_hsv, src_hsv_masked);
#else
	Mat src_hsv_bin_Red_1, src_hsv_bin_Red_2;
	inRange(src_hsv, Scalar(160, 180, 0), Scalar(180, 255, 255), src_hsv_bin_Red_1);
	inRange(src_hsv, Scalar(0, 180, 0), Scalar(10, 255, 255), src_hsv_bin_Red_2);
	src_hsv_masked = src_hsv_bin_Red_1 + src_hsv_bin_Red_2;
#endif

	// 노이즈 제거

#if 0 // 메디안 필터를 활용했던 내 방식
	Mat src_hsv_masked_clean;
	medianBlur(src_hsv_masked, src_hsv_masked_clean, 3);

#else // 모폴로지 연산을 활용하라는 교수님의 요구사항
	Mat src_hsv_masked_clean;
	
	int kernelSz = 2;
	int shape = MorphShapes::MORPH_RECT;
	cv::Size sz = Size(kernelSz + 1, 2 * kernelSz + 1); // 여기를 세로방향으로 만드는게 포인트
	Mat SE = cv::getStructuringElement(shape, sz);

#if 0 // erode랑 dilate의 커널 사이즈를 다르게 주니까 원본 영상이 손상됨
	Mat test1;
	cv::erode(src_hsv_masked, test1, SE);

	kernelSz = 4;
	sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
	SE = cv::getStructuringElement(shape, sz);

	Mat test2;
	cv::dilate(test1, test2, SE); 
#endif

	// "openning 후 closing"
	// 원본영상을 유지하면서 노이즈를 지우고(opening)
	// 원본영상을 유지하면서 빈공간을 채운다(closing)
	Mat temp;
	cv::morphologyEx(src_hsv_masked, temp, MorphTypes::MORPH_OPEN, SE);
	cv::morphologyEx(temp, src_hsv_masked_clean, MorphTypes::MORPH_CLOSE, SE);

#endif

	// 바깥 외곽선만 검출
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_hsv_masked_clean, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat drawing = Mat::zeros(src_hsv_masked_clean.size(), CV_8UC3);

	// ROI의 평균 밝기 계산을 위해 GRAY_SCALE 영상에서 빨간색 영역만 한번더 마스킹
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_RGB2GRAY);
	Mat src_gray_masked;
	src_gray_masked = src_gray - (255 - src_hsv_masked_clean);

	for (size_t i = 0; i < contours.size(); i++)
	{
		printf("for문 몇번 도니?\n");

		// 빨간색 영역을 감싸는 원 그리기
		Point2f target_center;
		float target_radius;
		minEnclosingCircle(contours[i], target_center, target_radius);
		circle(src, target_center, static_cast<int>(target_radius), Scalar(30, 0, 30), 3);

		// 각종 값들 산출
		double area = contourArea(contours[i]);
		double arcLen = arcLength(contours[i], true);

		// 빨간 영역(ROI, Region Of Interest)의 GRAY_SCALE에서 평균 밝기 계산
		int search_range = ((target_center.x + (int)target_radius) - (target_center.x - (int)target_radius))
			* ((target_center.y + (int)target_radius) - (target_center.y - (int)target_radius)); // 어차피 마스킹 된 영상에서 빨간색 추출하면 되기 때문에, 사각형 모양의 ROI 설정
		int start_idx = (target_center.x - (int)target_radius) * src_gray_masked.cols
			+ (target_center.y + (int)target_radius) - (target_center.y - (int)target_radius); // 사각형의 좌상단을 첫번째 탐색위치로 지정
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

		// putText 작업
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
