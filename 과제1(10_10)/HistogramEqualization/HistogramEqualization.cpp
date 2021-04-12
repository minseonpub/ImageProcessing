// Exercise : Histogram Equalization using C code
// @@program by Minseon Shin(201723307)
#include <opencv2/core.hpp>	
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

#define WIDTH image.cols
#define HEIGHT	image.rows
#define NM WIDTH*HEIGHT
#define MAXBRIGHT 256

// 1. 영상은 흑백으로 읽어들인다.
Mat image = imread("C:/Users/Minseon/Desktop/3.1/iu_2.jpg", 0);
float scale = (255.0 / (WIDTH * HEIGHT));

void main()
{
	// 5. 입력 영상과 같은 크기의 새로운 영상을 준비한다. (1 channel, 8bit unsigned byte)
	Mat changeImage;
	changeImage.create(HEIGHT, WIDTH, CV_8UC1);
	changeImage = Scalar(0);

	int HistoList[256];		//밝기
	int cdfList[256];		//누적분포
	int EqualizeList[256];

	// making instantly to Histogram equalization
	// 2. 0 - 255까지 밝기에 해당하는 histogram을 0으로 초기화 한다.

	for (int i = 1; i < MAXBRIGHT; i++)
	{
		HistoList[i] = 0;
		cdfList[i] = 0;
		EqualizeList[i] = 0;
	}

	//3. 영상의 모든 pixel에 대하여 자신의 밝기에 해당하는 histogram 값을 1씩 증가시킨다.
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
		{
			HistoList[image.at<uchar>(Point(x, y))]++;
		}

	//4. 누적 histogram을 계산한다.
	cdfList[0] = HistoList[0];
	for (int i = 1; i < MAXBRIGHT; i++)
	{
		cdfList[i] = cdfList[i - 1] + HistoList[i];
		EqualizeList[i] = round((double)cdfList[i] * scale);
	}
	// 6. 입력 영상의 각 픽셀에 대하여 누적 histogram을 바탕으로 밝기를 변환 (intensity transform)하여 새로운 영상의 해당 픽셀의 값으로 저장한다
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
		{
			// 계산된 배열 다시 이미지화 하기
			changeImage.at<uchar>(Point(x, y)) = EqualizeList[image.at<uchar>(Point(x, y))];
		}

	//7. 변환된 영상을 화면에 보인다.
	imshow("", changeImage);
	waitKey();
}