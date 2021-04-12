// Assignment02 : Image Processing to Noise
// @@ program by Minseon Shin(201723307)
//두 번째 과제는 첨부된 noisy 영상의 잡음을 최대한 잘 없애는 것입니다.

#include <opencv2/core.hpp>	
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <limits>
#include <math.h>

using namespace cv;
typedef std::numeric_limits< double > dbl; // for PSNR

#define WIDTH   Origin_img.cols
#define HEIGHT	Origin_img.rows
#define NM WIDTH*HEIGHT

Mat Origin_img = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/original.png", 0);

double getPSNR(const Mat& I1, const Mat& I2);	// to show PSNR db
Mat HarmonicFilter(const Mat input_img);
Mat ContraharmonicFilter(const Mat input_img, double q);
int main(void)
{
	// 프로그램은 한 개의 main에서 noise 의 종류별로 1개씩 파일을 읽은 후
	Mat Gaussian_image = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/Gaussian_0.10.png", 0);
	Mat Pepper_image = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/pepper_0.05.png", 0);
	Mat Salt_image = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/salt_0.05.png", 0);
	Mat SaltPepper_image = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/salt_pepper_0.05.png", 0);
	Mat Uniform_image = imread("C://Users/minse/Desktop/3.1/영상처리/OpenCV/assign2/uniform_0.10.png", 0);

	// 노이즈를 없앤 영상 저장할 각각의 빈 이미지 생성.
	Mat gaussian; 		gaussian.create(HEIGHT, WIDTH, CV_8UC1); gaussian = Scalar(0);
	Mat saltpepper; 	saltpepper.create(HEIGHT, WIDTH, CV_8UC1); saltpepper = Scalar(0);
	Mat salt; 			 salt.create(HEIGHT, WIDTH, CV_8UC1);  salt = Scalar(0);
	Mat pepper; 		pepper.create(HEIGHT, WIDTH, CV_8UC1); pepper = Scalar(0);
	Mat uniform; 		uniform.create(HEIGHT, WIDTH, CV_8UC1); uniform = Scalar(0);

	// noise가 없어진 영상 5개를 화면에 출력하고
	// 1. Gaussian image noise
	bilateralFilter(Gaussian_image, gaussian, 5, 80, 80);
	//imshow("B Gaussian", Gaussian_image);
	imshow("A Gaussian0.10", gaussian);

	// 2. Salt&pepper image noise
	medianBlur(SaltPepper_image, saltpepper, 3);
	//imshow("B SaltPepper", SaltPepper_image);
	imshow("A SaltPepper0.05", saltpepper);

	//3. Salt image noise
	salt= HarmonicFilter(Salt_image);
	//imshow("B Salt", Salt_image);
	imshow("A Salt0.05", salt);

	//4. pepper image noise
	medianBlur(Pepper_image, pepper, 3);

	imshow("B Pepper", Pepper_image);
	imshow("A Pepper0.05", pepper);

	//5.Uniform image noise
	bilateralFilter(Uniform_image, uniform, 3, 25, 25);
	medianBlur(uniform, uniform, 3);
	imshow("B Uniform", Uniform_image);
	imshow("A Uniform0.10", uniform);

	// 각각의 원본 대비 PSNR을 출력하도록 작성합니다.
	std::cout.precision(dbl::max_digits10);
	std::cout << "Original:  " << getPSNR(Origin_img, Origin_img) << std::endl;
	// 1. Gaussian image noise 
	std::cout << "Gaussian :  " << getPSNR(gaussian, Origin_img) << std::endl;
	// 2. Salt&pepper image noise
	std::cout << "Salt&pepper :  " << getPSNR(saltpepper, Origin_img) << std::endl;
	// 3. Salt image noise
	std::cout << "Salt :  " << getPSNR(salt, Origin_img) << std::endl;
	// 4. Salt image noise
	std::cout << "Pepper :  " << getPSNR(pepper, Origin_img) << std::endl;
	// 4. Uniform image noise
	std::cout << "Uniform :  " << getPSNR(uniform, Origin_img) << std::endl;

	waitKey();
}

double getPSNR(const Mat& Img1, const Mat& Img2)
{
	Mat s1;
	absdiff(Img1, Img2, s1);
	s1.convertTo(s1, CV_32F);
	s1 = s1.mul(s1);

	Scalar s = sum(s1);

	double sse = s.val[0] + s.val[1] + s.val[2];

	if (sse <= 1e-10) // for small values return zero
		return 0;
	else
	{
		double mse = sse / (double)(Img1.channels() * Img1.total());
		double psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}
Mat HarmonicFilter(const Mat input_img)
{
	Mat output;
	output.create(HEIGHT, WIDTH, CV_8UC1);
	output = Scalar(0);

	double sum = 0;      int harmonic = 0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++)
		{
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				output.at<uchar>(Point(x, y)) = input_img.at<uchar>(Point(x, y));
			else
			{
				sum = 1. / input_img.at<uchar>(Point(x - 1, y - 1)) + 1. / input_img.at<uchar>(Point(x - 1, y)) + 1. / input_img.at<uchar>(Point(x - 1, y + 1))
					+ 1. / input_img.at<uchar>(Point(x, y - 1)) + 1. / input_img.at<uchar>(Point(x, y)) + 1. / input_img.at<uchar>(Point(x, y + 1))
					+ 1. / input_img.at<uchar>(Point(x + 1, y - 1)) + 1. / input_img.at<uchar>(Point(x + 1, y)) + 1. / input_img.at<uchar>(Point(x, y + 1));

				harmonic = 3 * 3 / sum;
				output.at<uchar>(Point(x, y)) = harmonic;
			}
		}
	}
	return output;
}
Mat ContraharmonicFilter(const Mat input_img, double q)	//구현은 했는데,,,,오류를 잡지 못했습니다 교수님...;;;
{
	Mat output;
	output.create(HEIGHT, WIDTH, CV_8UC1);
	output = Scalar(0);
	double sum = 0;      int contraharmonic = 0;
	double sum2 = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++)
		{
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				output.at<uchar>(Point(x, y)) = input_img.at<uchar>(Point(x, y));
			else
			{
				sum = pow(input_img.at<uchar>(Point(x - 1, y - 1)), q + 1) + pow(input_img.at<uchar>(Point(x - 1, y)), q + 1) + pow(input_img.at<uchar>(Point(x - 1, y + 1)), q + 1)
					+ pow(input_img.at<uchar>(Point(x, y - 1)), q + 1) + pow(input_img.at<uchar>(Point(x, y)), q + 1) + pow(input_img.at<uchar>(Point(x, y + 1)), q + 1)
					+ pow(input_img.at<uchar>(Point(x + 1, y - 1)), q + 1) + pow(input_img.at<uchar>(Point(x + 1, y)), q + 1) + pow(input_img.at<uchar>(Point(x + 1, y + 1)), q + 1);
				
				sum2 = pow(input_img.at<uchar>(Point(x - 1, y - 1)), q) + pow(input_img.at<uchar>(Point(x - 1, y)), q ) + pow(input_img.at<uchar>(Point(x - 1, y + 1)), q)
					+ pow(input_img.at<uchar>(Point(x, y - 1)), q) + pow(input_img.at<uchar>(Point(x, y)), q) + pow(input_img.at<uchar>(Point(x, y + 1)), q)
					+ pow(input_img.at<uchar>(Point(x + 1, y - 1)), q) + pow(input_img.at<uchar>(Point(x + 1, y)), q) + pow(input_img.at<uchar>(Point(x + 1, y + 1)), q);

				contraharmonic = (int)(sum/sum2);
				output.at<uchar>(Point(x, y)) = contraharmonic;
			}
		}
	}
	return output;
}
