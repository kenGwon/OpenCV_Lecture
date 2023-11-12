/*

	https://patiencelee.tistory.com/816
	위 블로그를 보고 솔루션을 우클릭하여 [속성]에 들어가서 
	[시작 프로젝트] 탭에서 "한개의 시작 프로젝트"로 설정되어있던 라디오 버튼을 "현재 선택영역"으로 바꿔주었다.
	그렇게 함으로써 이제 디버그 버튼을 누르면 하나의 솔루션에 다수의 프로젝트를 로드하고 있어도 현재 커서가 위치한 프로젝트를 컴파일 하여 실행하게 되었다.
	(원래 이렇게 쓰는게 맞는지는 모르겠다.)

*/

#include "Common_kenGwon.h"
#include <numeric>

int main()
{
	///////////////////////////////
	// 필터를 이용한 blur 자작코드 // 
	///////////////////////////////

	std::string fileName = "../thirdparty/opencv_480/sources/samples/data/lena.jpg";
	cv::Mat src_gray = cv::imread(fileName, cv::ImreadModes::IMREAD_GRAYSCALE);

	// 아 이런식으로도 동일한 크기의 행렬을 만들어서 초기화 해줄 수 있구나
	cv::Mat src_gray_blur = src_gray.clone();
	src_gray_blur = 0;

	//filter 3x3 ... 1/9
	const int filter_sz = 3;
	float blur[] = {
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
		1.0 / 9, 1.0 / 9, 1.0 / 9,
	};

	//for (size_t row = 0; row < src_gray.rows - filter_sz; row++)
	for (size_t row = 0; row < src_gray.rows; row++)
	{
		//for (size_t col = 0; col < src_gray.cols - filter_sz; col++)
		for (size_t col = 0; col < src_gray.cols; col++)
		{
			int sum = 0;
			for (size_t f_row = 0; f_row < filter_sz; f_row++)
			{
				for (size_t f_col = 0; f_col < filter_sz; f_col++)
				{
					int index = (row + f_row) * src_gray.cols + (col + f_col);
					int f_index = (f_row)*filter_sz + (f_col);
					sum += src_gray.data[index] * blur[f_index];
				}
			}
			int index = (row)*src_gray.cols + (col);
			src_gray_blur.data[index] = static_cast<uchar>(sum);
		}
	}


	return 1;
}
