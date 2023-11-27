#pragma once

#include "ISP.h"
/*
  요구사항
  1. "상한 허용치", "하향 허용치", "정량 위치" 라인 그리기
  2. 상한 허용치와 하향 허용치 range안에 있으면 "ok" 
     else "NG" return
*/

/*
  < 흰색 빈공간에서 기준점을 찾아내는 교수님의 알고리즘 >
  원본 영상에서 마름모모양의 흰색 공간을 지나가는 행을 탐색하면서, 
  회색에서 흰색으로 갑자기 값이 올라가는 지점을 포인트로 찍어서 거기서 수직으로 올라가면
  바로 앰플 용액에서 ROI영역의 위치 값을 바로 찍어낼 수 있다.

  동일한 패턴이 반복되는 영상에서 기준점을 찾아내는 알고리즘의 핵심은,
  영상 행렬에서 패턴이 반복되는 지점이 있는 행렬의 한 라인을 탐색하면서, 
  값이 0에서 255로 갑자기 뛰는 지점을 point로 잡는 것이다.
  그리고 그 기준점point를 바탕으로 ROI를 산출할 수 있게 되는 것이다.
  
  (내가 썼든 findContour()를 활용한 알고리즘은 영상의 종류가 바뀌면, 
  내가 인정하는 외곽선의 면적 값을 다시 수정해줘야하는 문제가 발생한다.
  하지만 교수님의 알고리즘을 쓰면 패턴이 다른 영상이 들어와도,
  언제나 동일한 규칙을 가지고서 기준점을 잡아서 ROI를 산출할 수 있게되는 것이다.)
*/

int main()
{
	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_07.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// ampoules를 나누는 기준점을 잡기 위한 영상 이진화
	Mat src_bin;
	threshold(src_gray, src_bin, 210, 255, THRESH_BINARY);

	// Morphology 연산으로 영상 정제
	int kernelSz = 6;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1);
	Mat SE = cv::getStructuringElement(shape, sz);

	cv::morphologyEx(src_bin, src_bin, MorphTypes::MORPH_OPEN, SE);
	cv::morphologyEx(src_bin, src_bin, MorphTypes::MORPH_CLOSE, SE);

	// 외곽선을 찾고, 면적이 3000 초과인 외곽선들만 필터링
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_bin, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<vector<Point>> contours;
	for (size_t i = 0; i < contours_temp.size(); i++)
	{
		if (static_cast<int> (contourArea(contours_temp[i])) > 3000)
		{
			contours.push_back(contours_temp[i]);
			printf("%d\n", (int)contourArea(contours_temp[i]));
		}
	}

	// 외곽선들 순회하며, 외곽선들의 무게중심 구하기
	vector<Point> centers;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int sum_x = 0;
		int sum_y = 0;
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			sum_x += contours[i].at(j).x;
			sum_y += contours[i].at(j).y;
		}
		centers.push_back(Point(sum_x / contours[i].size(), sum_y / contours[i].size()));
	}

	// 무게중심을 바탕으로 각 ampoules의 구역을 사각형의 모습으로 바라볼 수 있도록, 부분행렬 도출
	vector<Point> liquid_line_points;
	for (size_t i = 0; i < centers.size(); i++)
	{
		if ((int)centers[i].x - 60 < 0) continue; // 예외처리

		Point topLeft = Point(centers[i].x - 60, centers[i].y - 200);
		Mat ROI = src_gray(Rect(topLeft.x, topLeft.y, 40, 150)).clone(); // deep copy해야 아래 로직에서 올바른 인덱스 접근 가능
		Mat temp = src_gray(Rect(topLeft.x, topLeft.y, 40, 150)); // 설명을 위한 shallow copy 시각자료

		// 대비 증가를 위한 히스토그램 평활화
		equalizeHist(ROI, ROI); 
		equalizeHist(temp, temp); // 설명을 위한 shallow copy 시각자료

		// projection? 
		vector<int> projection;
		for (size_t row = 0; row < ROI.rows; row++)
		{
			int sum_of_row = 0;
			for (size_t col = 0; col < ROI.cols; col++)
			{
				sum_of_row += ROI.data[row * ROI.cols + col];
			}
			projection.push_back(sum_of_row);
		}

		// 값이 가장 급격하게 변하는 경계선 인덱스 위치 도출
		vector<int> vector_dy;
		for (size_t k = 0; k < projection.size() - 1; k++)
		{
			int dy = abs(projection[k] - projection[k + 1]);
			vector_dy.push_back(dy);
		}
		int maxIdx = max_element(vector_dy.begin(), vector_dy.end()) - vector_dy.begin();
		Point liquid_point = Point(topLeft.x, topLeft.y + maxIdx);

		liquid_line_points.push_back(liquid_point);
	}

	// 선 그리기
	cvtColor(src_gray, src_gray, CV_GRAY2BGR);
	int min_y = 90, max_y = 120;
	line(src_gray, Point(0, min_y), Point(src_gray.cols, min_y), Scalar(0, 255, 255), 2); // 상한치 라인
	line(src_gray, Point(0, max_y), Point(src_gray.cols, max_y), Scalar(0, 255, 255), 2); // 하한치 라인

	for (size_t i = 0; i < liquid_line_points.size(); i++)
	{
		if (liquid_line_points[i].y > min_y && liquid_line_points[i].y < max_y) // OK
		{
			line(src_gray, 
				liquid_line_points[i], Point(liquid_line_points[i].x + 40, liquid_line_points[i].y),
				Scalar(255, 100, 0), 3);
			putText(src_gray, "OK", liquid_line_points[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 0), 2, 8);
		}
		else // NG
		{
			line(src_gray,
				liquid_line_points[i], Point(liquid_line_points[i].x + 40, liquid_line_points[i].y),
				Scalar(0, 0, 255), 3);
			putText(src_gray, "NG", liquid_line_points[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 2, 8);
		}
	}


	return 1;
}
