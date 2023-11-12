#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <memory>
#include <fstream> // ofstream header
#include <format>

#define OPENCV_480
#include <opencv2/opencv.hpp> // C/C++ >> "일반" 옵션에서 "추가 포함 디렉토리"에다가 opencv-include폴더를 넣어뒀기 때문에 이게 인클루드 될수 있다.
#include <opencv2/core/ocl.hpp>
#ifdef OPENCV_480
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#endif // OPENCV_480

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib") // 링커 >> "입력" 옵션에서 "추가 종속성에다가 적어줘야 할 것을 여기 코드에 대신 적은 것이다.
#else	//RELEASE
#pragma comment(lib,"opencv_world480.lib") // 
#endif
using namespace std;
using namespace cv;