#pragma once

#include "ISP.h"
/*
  요구사항
  
  1. "상한 허용치", "하향 허용치", "정량 위치" 라인 그리기
  2. 상한 허용치와 하향 허용치 range안에 있으면 "ok" 
     else "NG" return
*/

/*
  - 히스토그램 평활화를 하여 영상의 밝기 범위를 우선 넓게 한다.
  - 용액들 객체 하나하나를 따로 영역화 해서 볼수 있는 알고리즘 필요
  - 그 영역 안에서 용액의 라인을 잡는 라인의 범위는 105 ~ 125

  - 곱하기를 해줘서 대비를 최대화 해주고
  - 한 행의 데이터 값을 모두 더해서 비교하면 되겠다. 일차원으로 비교하면 되겠다.
  
  - 중간에 흰 영역을 findContour()로 찾아와서 그 무게중심을 찾고 
    그 무게중심을 기준으로 영상을 짤라서 보면서 
  - 미분을 사용해서 중간 라인점을 비교하면 되겠다. 

  
*/


int main()
{
	std::string fileName = "../KCCImageNet/images/ampoules/ampoules_07.png";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// 영상 이진화
	Mat src_bin;
	threshold(src_gray, src_bin, 210, 255, THRESH_BINARY);

	// Morphology 연산
	int kernelSz = 4;
	int shape = MorphShapes::MORPH_ELLIPSE;
	cv::Size sz = Size(2 * kernelSz + 1, 2 * kernelSz + 1); // 여기를 세로방향으로 만드는게 포인트
	Mat SE = cv::getStructuringElement(shape, sz);

	Mat temp;
	cv::morphologyEx(src_bin, temp, MorphTypes::MORPH_OPEN, SE);
	Mat src_masked;
	cv::morphologyEx(temp, src_masked, MorphTypes::MORPH_CLOSE, SE);

	// findContour하고 area가 300미만인 외곽선들의 내부는 0으로 처리
	vector<vector<Point>> contours_temp;
	vector<Vec4i> hierarchy;
	findContours(src_masked, contours_temp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<vector<Point>> contours;
	for (size_t i = 0; i < contours_temp.size(); i++)
	{
		if (static_cast<int> (contourArea(contours_temp[i])) > 300)
		{
			contours.push_back(contours_temp[i]);
		}
	}

	// 외곽선들 순회하며, 외곽선들의 무게중심 vector 구하기
	vector<Point> centers;
	for (size_t i = 0; i < contours.size(); i++)
	{
		// 점들의 집합 순회하여, 한 외곽선의 중심점 찾기
		int sum_x = 0;
		int sum_y = 0;
		for (size_t j = 0; j < contours[i].size(); j++)
		{
			sum_x += contours[i].at(j).x;
			sum_y += contours[i].at(j).y;
		}
		centers.push_back(Point(sum_x / contours[i].size(), sum_y / contours[i].size()));
	}

	// 무게중심을 바탕으로 각각의 구역을 사각형의 모습으로 바라볼 수 있도록, 부분행렬 도출
	vector<Mat> ROIs;
	for (size_t i = 0; i < centers.size(); i++)
	{
		Point topLeft = Point(centers[i].x - 60, centers[i].y - 200);
		ROIs.push_back(src_gray(Rect(topLeft.x, topLeft.y, 40, 150)));
	}

	// 부분행렬들을 projection하여 경계선 point 도출
	vector<int> liquid_lines;
	for (size_t i = 0; i < ROIs.size(); i++)
	{
		// 히스토그램 평활화
		equalizeHist(ROIs[i], ROIs[i]);
		// projection
		vector<int> projection;
		for (size_t row = 0; row < ROIs[i].rows; row++)
		{
			int sum_of_row = 0;
			for (size_t col = 0; col < ROIs[i].cols; col++)
			{
				sum_of_row += ROIs[i].data[row * ROIs[i].cols + col];
			}
			projection.push_back(sum_of_row);
		}
		
		// 값이 가장 급격하게 변하는 경계선 인덱스 위치 도출
		int liquid_line = 0;
		for (size_t i = 0; i < projection.size() - 1; i++)
		{
			if (projection[i] - projection[i + 1] > liquid_line)
			{
				liquid_line = projection[i] - projection[i + 1];
			}
			else
			{
				continue;
			}
		}

		liquid_lines.push_back(liquid_line);
	}


	return 1;
}
