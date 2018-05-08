#pragma once
#pragma comment( lib, "Winmm.lib" )	

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\contrib\contrib.hpp>

#include <Windows.h>
#include <iostream>
#include <fstream>

#define SAMPLE_RATE 44100
#define AUDIO_DISPLAY_WIDTH 640
#define AUDIO_DISPLAY_HEIGHT 480

#define AUDIO_RANGE 75000
#define FREQUENCY_RANGE 60000

#define M_PI 3.14159265358979323846

#define MAP(value, start1, stop1, start2, stop2) (start2 + (((stop2 - start2) * (value - start1)) / (stop1 - start1)) )

using namespace cv;
using namespace std;

void writeData(const Mat _data, string outputPath);

Mat plotGraph(Mat src, Mat _data, double minimum, double maximum, Scalar color = Scalar::all(0));

void showGraph(string windowName, Mat _data, int _height, int _width);

Mat convertValues(WAVEHDR waveInHdr);

class Spectrogram
{
private:
	Mat* spectrogram;
	int index;

	void Spectrogram::hanningWindow(Mat src, Mat dst);

public:
	Spectrogram(int _height, int _width);
	~Spectrogram(void);

	void Spectrogram::addInput(WAVEHDR waveInHdr);
	void Spectrogram::addInput(Mat input, int N);

	Mat Spectrogram::getSpectrogram(int colorMap = COLORMAP_JET);
	Mat Spectrogram::getDetailedSpectrogram(int padding = 10, int colorMap = COLORMAP_JET);
};

