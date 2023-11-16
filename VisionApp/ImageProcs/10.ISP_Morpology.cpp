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
	  ħ��(erosion)
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
			// �˻��ϴ� �ȼ��� ���� 0�̸� ���� ������ continue... �������� ���� �����̱� ������ ������ ����
			if (Datas[row * data_w + col] == 0) continue; 
			
			printf("index[%d]\n", row * data_w + col);

			bool allone = true;
			for (int h = -half_SE_size; h < half_SE_size; h++)
			{
				for (int w = -half_SE_size; w < half_SE_size; w++)
				{
					int index = (row + h) * data_w + (col + w);
					int SE_index = (h + half_SE_size) * SE_size + (w + half_SE_size);
					

					// ħ�Ŀ����� �⺻Ʋ�� ������ ���� ���� �˻�
					if (Datas[index] == 0 && SE_Cross[SE_index] == 1) 
					{
						printf("index: %d, SE_index: %d\n", index, SE_index);
						allone = false; 
						break;
					}
				}
			}
			// SE���� �������� ������ �ȼ��� 0���� �����ϰ�, SE���� �������� ������ �ȼ����� 1�� ����
			if (!allone) pDst_erosion[row * data_w + col] = 0; // ħ�� �߻�
			else pDst_erosion[row * data_w + col] = 1;
		}
	}

	/*
	    openCV�� �����ϴ� ���������� ���� 0, 255�� �������� �����Ǵµ�, 
	    �װͿ� ���� �������� ������ �� �� 255��� ������ �����ϴ� ������ ���귮�� �������ٰ� �Ǵ��Ͽ�,
	    �� ��ǻ�� ���ҽ��� �����̶� �� �Ƴ����� [0,255]���������� [0, 1]������������ ��ȯ�Ͽ� �������� ������ �Ѵ�.
	*/

	return 1;
}
