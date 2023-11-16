#pragma once

#include "ISP.h"

int main()
{
	const int data_w = 7;
	const int data_h = 7;
	uchar Datas[data_w * data_h] = {
		1, 1, 1, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 1, 1
	};

	Mat img_Datas = Mat(data_h, data_w, CV_8UC1, Datas);
	Mat img_Datas_erosion = Mat::zeros(data_h, data_w, CV_8UC1);
	Mat img_Datas_dilation = Mat::zeros(data_h, data_w, CV_8UC1);

	/*
	  침식(erosion)
	*/
	const int SE_size = 3;
	int half_SE_size = SE_size / 2;
	int SE_Cross[SE_size * SE_size] = {
		0, 1, 0,
		1, 1, 1,
		0, 1, 0
	};
	int SE_Rect[SE_size * SE_size] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};
	
	uchar* pDst_erosion = img_Datas_erosion.data;
	for (size_t row = half_SE_size; row < data_h - half_SE_size; row++)
	{
		for (size_t col = half_SE_size; col < data_w - half_SE_size; col++)
		{
			// 검사하는 픽셀의 값이 0이면 다음 루프로 continue... 이진영상에 대한 연산이기 때문에 가능한 조건
			if (Datas[row * data_w + col] == 0) continue; 
			
			printf("index[%d]\n", row * data_w + col);

			bool allone = true;
			for (int h = -half_SE_size; h < half_SE_size; h++)
			{
				for (int w = -half_SE_size; w < half_SE_size; w++)
				{
					int index = (row + h) * data_w + (col + w);
					int SE_index = (h + half_SE_size) * SE_size + (w + half_SE_size);
					

					// 침식연산의 기본틀인 교집합 위배 여부 검사
					if (Datas[index] == 0 && SE_Cross[SE_index] == 1) 
					{
						printf("index: %d, SE_index: %d\n", index, SE_index);
						allone = false; 
						break;
					}
				}
			}
			// SE와의 교집합을 위배한 픽셀값 0으로 수정하고, SE와의 교집합을 만족한 픽셀값은 1로 수정
			if (!allone) pDst_erosion[row * data_w + col] = 0; // 침식 발생
			else pDst_erosion[row * data_w + col] = 1;
		}
	}

	/*
	    openCV가 리턴하는 이진영상은 본디 0, 255로 색상값으로 구성되는데, 
	    그것에 대한 모폴로지 연산을 할 때 255라는 정수를 연산하는 것초자 연산량이 많아진다고 판단하여,
	    그 컴퓨팅 리소스를 조금이라도 더 아끼려고 [0,255]이진영상을 [0, 1]이진영상으로 변환하여 모폴로지 연산을 한다.
	*/

	return 1;
}
