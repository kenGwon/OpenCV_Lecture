#include "Common.h"

int main()
{
	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src = cv::imread(fileName, cv::ImreadModes::IMREAD_ANYCOLOR);

	uchar* pData = src.data; 
	int length = src.total(); 
	int channels = src.channels();

	
	// 새로운 영상행렬 생성
	cv::Mat src_Y = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_Cb = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_Cr = cv::Mat(src.rows, src.cols, CV_8UC1);
	cv::Mat src_new = cv::Mat(src.rows, src.cols, CV_8UC3);

	for (size_t row = 0; row < src.rows; row++)
	{
		for (size_t col = 0; col < src.cols; col++)
		{
			// mono			
			int index = (row)*src.cols + (col); // src는 일차원 배열로 길게 늘어서 있지만 그것을 행렬의 관점에서 생각하면 당연한 것이다. 
			
			if (channels == 3)
			{

				// RGB -> YCbCr
				int index_B = index * channels + 0; // channels를 곱해주는 이유는 R.G.B이기 때문
				int index_G = index * channels + 1; // channels를 곱해주는 이유는 R.G.B이기 때문
				int index_R = index * channels + 2; // channels를 곱해주는 이유는 R.G.B이기 때문
				int val_B = pData[index_B];
				int val_G = pData[index_G];
				int val_R = pData[index_R];
				
				int val_Y = 0.299 * val_R + 0.587 * val_G + 0.114 * val_B;
				int val_Cb = -0.169 * val_R -0.331 * val_G + 0.500 * val_B;
				int val_Cr = 0.500 * val_R -0.419 * val_G + -0.0813 * val_B;

				src_Y.data[index] = (uchar)val_Y;
				src_Cb.data[index] = (uchar)val_Cb;
				src_Cr.data[index] = (uchar)val_Cr;

				// YCbCr -> RGB
				int new_val_R = 1.000 * val_Y + 1.402 * val_Cr + 0.000 * val_Cb;
				int new_val_G = 1.000 * val_Y - 0.714 * val_Cr - 0.344 * val_Cb;
				int new_val_B = 1.000 * val_Y + 0.000 * val_Cr + 1.772 * val_Cb;

				src_new.data[index_B] = (uchar)new_val_B;
				src_new.data[index_G] = (uchar)new_val_G;
				src_new.data[index_R] = (uchar)new_val_R;
			}
			else
			{
				src_Y.data[index] = src.data[index];
			}

		}
	}

	cv::Mat src_diff = cv::Mat(src.rows, src.cols, CV_8UC3);
	src_diff = src - src_new; // Image Watch창에서 "대비 자동 최대화"옵션을 켜주면 어디서 값이 가장 크게 변했는지 확인할 수 있다.






	cv::Mat temp1 = cv::Mat(src.rows, src.cols, CV_8UC3);
	cv::Mat temp2 = cv::Mat(src.rows, src.cols, CV_8UC3);

	cvtColor(src, temp1, COLOR_BGR2YCrCb);
	cvtColor(src, temp2, COLOR_BGR2HSV);

	vector<Mat> bgr_planes;
	split(src, bgr_planes);


	return 0;
}