// Assignment03 : Image Processing to skelton restoration
// @@ programming by Minseon Shin(201723307)
/*
마지막 과제는 손글씨 입력 파일(sk1.png/sk2.png/sk3.png)파일을 입력 받아 획의 두께를
최소한으로 바꾸는 과정(Skeleton extraction)을 구현하는 것입니다. (결과 그림을 참고하십시오.)

(Hint. '-' operation은 bitwise_and(~T, A...) 의 형식을 사용하시고,
'+' 는 bitwise_or나 | operator를 이용하시면 쉽습니다.
또한 영상의 모든 픽셀이 0인지 알기 위해서는 최소값, 최대값을 찾는 minMaxLoc 함수를
사용하시면 편리합니다. 결과 그림은 cross se를 사용한 것입니다.)
*/
#include <opencv2/core.hpp>	
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <limits>

using namespace cv;

//skeletonization 과정은 다음과 같습니다.
int main(void)
{
	// Get original Img
	Mat origin_img1 = imread("C://Users/minse/Desktop/3.1/영상처리/과제3/sk1.png", 0);
	Mat origin_img2 = imread("C://Users/minse/Desktop/3.1/영상처리/과제3/sk2.png", 0);
	Mat origin_img3 = imread("C://Users/minse/Desktop/3.1/영상처리/과제3/sk3.png", 0);

	//binary
	threshold(origin_img1, origin_img1, 127, 255, THRESH_BINARY);
	threshold(origin_img2, origin_img2, 127, 255, THRESH_BINARY);
	threshold(origin_img3, origin_img3, 127, 255, THRESH_BINARY);

	imshow("origin_binary01", origin_img1);
	imshow("origin_binary02", origin_img2);
	imshow("origin_binary03", origin_img3);
	waitKey();

	//0. 먼저 영상을 역상으로 바꿉니다.
	origin_img1 = 255 - origin_img1;
	origin_img2 = 255 - origin_img2;
	origin_img3 = 255 - origin_img3;

	// create new Mat for saving result.
	Mat skel01(origin_img1.size(), CV_8UC1, Scalar(0));
	Mat skel02(origin_img2.size(), CV_8UC1, Scalar(0));
	Mat skel03(origin_img3.size(), CV_8UC1, Scalar(0));

	Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));

	Mat temp01, temp02, temp03;
	Mat	eroded01, eroded02, eroded03;
	double max01, max02, max03;

	//Img01
	do
	{
		//1. 입력영상을 A0, A0를 opening한 영상을 T0라고 할 때 S0 = A0-T0 라하면 
		//(여기서 '-'는 집합 연산) S0은 Opening에 의해 사라진 가는 선이 됩니다.
		//Opening
		erode(origin_img1, eroded01, element);
		dilate(eroded01, temp01, element);

		bitwise_not(temp01, temp01);
		//S = A-T
		bitwise_and(origin_img1, temp01, temp01);

		bitwise_or(skel01, temp01, skel01);
		//2. 다음 두꺼운 선을 가늘게 하기 위해 A를 erode 하면 A1을 얻으면,
		//	A1의 경우 S0에 나타나는 선은 없어지고, 약간 두꺼운 선이 가늘어집니다.
		eroded01.copyTo(origin_img1);
		minMaxLoc(temp01, 0, &max01);
	} while (max01 != 0); //5. 과정은 Sn이 전부 검은색일때 멈추면 됩니다.

	//Img02
	do
	{
		//1. 입력영상을 A0, A0를 opening한 영상을 T0라고 할 때 S0 = A0-T0 라하면 
		//(여기서 '-'는 집합 연산) S0은 Opening에 의해 사라진 가는 선이 됩니다.
		//Opening
		erode(origin_img2, eroded02, element);
		dilate(eroded02, temp02, element);

		bitwise_not(temp02, temp02);
		//S = A-T
		bitwise_and(origin_img2, temp02, temp02);

		bitwise_or(skel02, temp02, skel02);
		//2. 다음 두꺼운 선을 가늘게 하기 위해 A를 erode 하면 A1을 얻으면,
		//	A1의 경우 S0에 나타나는 선은 없어지고, 약간 두꺼운 선이 가늘어집니다.
		eroded02.copyTo(origin_img2);
		minMaxLoc(temp02, 0, &max02);
	} while (max02 != 0);//5. 과정은 Sn이 전부 검은색일때 멈추면 됩니다.

	//Img03
	do
	{
		//1. 입력영상을 A0, A0를 opening한 영상을 T0라고 할 때 S0 = A0-T0 라하면 
		//(여기서 '-'는 집합 연산) S0은 Opening에 의해 사라진 가는 선이 됩니다.
		//Opening
		erode(origin_img3, eroded03, element);
		dilate(eroded03, temp03, element);

		bitwise_not(temp03, temp03);
		//S = A-T
		bitwise_and(origin_img3, temp03, temp03);

		bitwise_or(skel03, temp03, skel03);
		//2. 다음 두꺼운 선을 가늘게 하기 위해 A를 erode 하면 A1을 얻으면,
		//	A1의 경우 S0에 나타나는 선은 없어지고, 약간 두꺼운 선이 가늘어집니다.
		eroded03.copyTo(origin_img3);
		minMaxLoc(temp03, 0, &max03);
	} while (max03 != 0);//5. 과정은 Sn이 전부 검은색일때 멈추면 됩니다.

	//7. 결과를 출력합니다.
	imshow("result01", skel01);
	imshow("result02", skel02);
	imshow("result03", skel03);

	waitKey();
}
