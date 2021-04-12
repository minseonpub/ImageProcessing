// Assignment03 : Image Processing to skelton restoration
// @@ programming by Minseon Shin(201723307)
/*
������ ������ �ձ۾� �Է� ����(sk1.png/sk2.png/sk3.png)������ �Է� �޾� ȹ�� �β���
�ּ������� �ٲٴ� ����(Skeleton extraction)�� �����ϴ� ���Դϴ�. (��� �׸��� �����Ͻʽÿ�.)

(Hint. '-' operation�� bitwise_and(~T, A...) �� ������ ����Ͻð�,
'+' �� bitwise_or�� | operator�� �̿��Ͻø� �����ϴ�.
���� ������ ��� �ȼ��� 0���� �˱� ���ؼ��� �ּҰ�, �ִ밪�� ã�� minMaxLoc �Լ���
����Ͻø� ���մϴ�. ��� �׸��� cross se�� ����� ���Դϴ�.)
*/
#include <opencv2/core.hpp>	
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <limits>

using namespace cv;

//skeletonization ������ ������ �����ϴ�.
int main(void)
{
	// Get original Img
	Mat origin_img1 = imread("C://Users/minse/Desktop/3.1/����ó��/����3/sk1.png", 0);
	Mat origin_img2 = imread("C://Users/minse/Desktop/3.1/����ó��/����3/sk2.png", 0);
	Mat origin_img3 = imread("C://Users/minse/Desktop/3.1/����ó��/����3/sk3.png", 0);

	//binary
	threshold(origin_img1, origin_img1, 127, 255, THRESH_BINARY);
	threshold(origin_img2, origin_img2, 127, 255, THRESH_BINARY);
	threshold(origin_img3, origin_img3, 127, 255, THRESH_BINARY);

	imshow("origin_binary01", origin_img1);
	imshow("origin_binary02", origin_img2);
	imshow("origin_binary03", origin_img3);
	waitKey();

	//0. ���� ������ �������� �ٲߴϴ�.
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
		//1. �Է¿����� A0, A0�� opening�� ������ T0��� �� �� S0 = A0-T0 ���ϸ� 
		//(���⼭ '-'�� ���� ����) S0�� Opening�� ���� ����� ���� ���� �˴ϴ�.
		//Opening
		erode(origin_img1, eroded01, element);
		dilate(eroded01, temp01, element);

		bitwise_not(temp01, temp01);
		//S = A-T
		bitwise_and(origin_img1, temp01, temp01);

		bitwise_or(skel01, temp01, skel01);
		//2. ���� �β��� ���� ���ð� �ϱ� ���� A�� erode �ϸ� A1�� ������,
		//	A1�� ��� S0�� ��Ÿ���� ���� ��������, �ణ �β��� ���� ���þ����ϴ�.
		eroded01.copyTo(origin_img1);
		minMaxLoc(temp01, 0, &max01);
	} while (max01 != 0); //5. ������ Sn�� ���� �������϶� ���߸� �˴ϴ�.

	//Img02
	do
	{
		//1. �Է¿����� A0, A0�� opening�� ������ T0��� �� �� S0 = A0-T0 ���ϸ� 
		//(���⼭ '-'�� ���� ����) S0�� Opening�� ���� ����� ���� ���� �˴ϴ�.
		//Opening
		erode(origin_img2, eroded02, element);
		dilate(eroded02, temp02, element);

		bitwise_not(temp02, temp02);
		//S = A-T
		bitwise_and(origin_img2, temp02, temp02);

		bitwise_or(skel02, temp02, skel02);
		//2. ���� �β��� ���� ���ð� �ϱ� ���� A�� erode �ϸ� A1�� ������,
		//	A1�� ��� S0�� ��Ÿ���� ���� ��������, �ణ �β��� ���� ���þ����ϴ�.
		eroded02.copyTo(origin_img2);
		minMaxLoc(temp02, 0, &max02);
	} while (max02 != 0);//5. ������ Sn�� ���� �������϶� ���߸� �˴ϴ�.

	//Img03
	do
	{
		//1. �Է¿����� A0, A0�� opening�� ������ T0��� �� �� S0 = A0-T0 ���ϸ� 
		//(���⼭ '-'�� ���� ����) S0�� Opening�� ���� ����� ���� ���� �˴ϴ�.
		//Opening
		erode(origin_img3, eroded03, element);
		dilate(eroded03, temp03, element);

		bitwise_not(temp03, temp03);
		//S = A-T
		bitwise_and(origin_img3, temp03, temp03);

		bitwise_or(skel03, temp03, skel03);
		//2. ���� �β��� ���� ���ð� �ϱ� ���� A�� erode �ϸ� A1�� ������,
		//	A1�� ��� S0�� ��Ÿ���� ���� ��������, �ణ �β��� ���� ���þ����ϴ�.
		eroded03.copyTo(origin_img3);
		minMaxLoc(temp03, 0, &max03);
	} while (max03 != 0);//5. ������ Sn�� ���� �������϶� ���߸� �˴ϴ�.

	//7. ����� ����մϴ�.
	imshow("result01", skel01);
	imshow("result02", skel02);
	imshow("result03", skel03);

	waitKey();
}
