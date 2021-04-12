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

// 1. ������ ������� �о���δ�.
Mat image = imread("C:/Users/Minseon/Desktop/3.1/iu_2.jpg", 0);
float scale = (255.0 / (WIDTH * HEIGHT));

void main()
{
	// 5. �Է� ����� ���� ũ���� ���ο� ������ �غ��Ѵ�. (1 channel, 8bit unsigned byte)
	Mat changeImage;
	changeImage.create(HEIGHT, WIDTH, CV_8UC1);
	changeImage = Scalar(0);

	int HistoList[256];		//���
	int cdfList[256];		//��������
	int EqualizeList[256];

	// making instantly to Histogram equalization
	// 2. 0 - 255���� ��⿡ �ش��ϴ� histogram�� 0���� �ʱ�ȭ �Ѵ�.

	for (int i = 1; i < MAXBRIGHT; i++)
	{
		HistoList[i] = 0;
		cdfList[i] = 0;
		EqualizeList[i] = 0;
	}

	//3. ������ ��� pixel�� ���Ͽ� �ڽ��� ��⿡ �ش��ϴ� histogram ���� 1�� ������Ų��.
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
		{
			HistoList[image.at<uchar>(Point(x, y))]++;
		}

	//4. ���� histogram�� ����Ѵ�.
	cdfList[0] = HistoList[0];
	for (int i = 1; i < MAXBRIGHT; i++)
	{
		cdfList[i] = cdfList[i - 1] + HistoList[i];
		EqualizeList[i] = round((double)cdfList[i] * scale);
	}
	// 6. �Է� ������ �� �ȼ��� ���Ͽ� ���� histogram�� �������� ��⸦ ��ȯ (intensity transform)�Ͽ� ���ο� ������ �ش� �ȼ��� ������ �����Ѵ�
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
		{
			// ���� �迭 �ٽ� �̹���ȭ �ϱ�
			changeImage.at<uchar>(Point(x, y)) = EqualizeList[image.at<uchar>(Point(x, y))];
		}

	//7. ��ȯ�� ������ ȭ�鿡 ���δ�.
	imshow("", changeImage);
	waitKey();
}