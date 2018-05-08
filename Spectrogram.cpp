#include "Spectrogram.h"


void writeData(const Mat _data, string outputPath){
	std::ofstream file;
	file.open(outputPath, std::ios::out | std::ios::app); // Append
	if(!file.is_open()) {
		std::cerr << "writeData() : ERROR with output path : " << outputPath << std::endl;
		return;
	}
	
	cout << "Write to: " << outputPath << endl;

	// Transpose Mat if needed
	Mat data;
	int N = _data.cols;
	if(N == 1){
		transpose(_data, data);
		N = data.cols;
	} else {
		_data.copyTo(data);
	}

	if(data.type() == CV_32F){
		file << data.at<float>(0, 0);
		for(int i=0; i<data.cols; i++){
			file << ", " << data.at<float>(0, i);
		}
		file << "\n";
	}else if(data.type() == CV_32S){
		file << data.at<int>(0, 0);
		for(int i=0; i<data.cols; i++){
			file << ", " << data.at<int>(0, i);
		}
		file << "\n";
	} else if(data.type() == CV_64F){
		file << data.at<double>(0, 0);
		for(int i=0; i<data.cols; i++){
			file << ", " << data.at<double>(0, i);
		}
		file << "\n";
	} else {
		std::cerr << "writeData() : Unsupported file format : " << data.type() << std::endl;
	}

	file.close();
}

Mat plotGraph(Mat src, Mat _data, double minimum, double maximum, Scalar color){
		
	// Transpose Mat if needed
	Mat data;
	int N = _data.cols;
	if(N == 1){
		transpose(_data, data);
		N = data.cols;
	} else {
		_data.copyTo(data);
	}

	Mat dst;
	src.copyTo(dst);

	// Draw values
	if(data.type() == CV_32S){
		float prev = data.at<int>(0, 0);
		for(int i = 1; i < data.cols ; i++){
			float next = data.at<int>(0, i);

			line(dst,
				Point(MAP(i - 1, 0, N, 0, dst.cols),
					  dst.rows - MAP(prev, minimum, maximum, 0, dst.rows)),
				Point(MAP(i, 0, N, 0, dst.cols),
					  dst.rows - MAP(next, minimum, maximum, 0, dst.rows)),
				color);
	
			prev = next;
		}
	}else if(data.type() == CV_32F){
		float prev = data.at<float>(0, 0);
		for(int i = 1; i < data.cols ; i++){

			float next = data.at<float>(0, i);

			line(dst,
				Point(MAP(i - 1, 0, N, 0, dst.cols),
					  dst.rows - MAP(prev, minimum, maximum, 0, dst.rows)),
				Point(MAP(i, 0, N, 0, dst.cols),
					  dst.rows - MAP(next, minimum, maximum, 0, dst.rows)),
				color);
	
			prev = next;
		}
	}else if(data.type() == CV_64F){
		double prev = data.at<double>(0, 0);
		for(int i = 1; i < data.cols ; i++){

			float next = data.at<double>(0, i);

			line(dst,
				Point(MAP(i - 1, 0, N, 0, dst.cols),
					  dst.rows - MAP(prev, minimum, maximum, 0, dst.rows)),
				Point(MAP(i, 0, N, 0, dst.cols),
					  dst.rows - MAP(next, minimum, maximum, 0, dst.rows)),
				color);
	
			prev = next;
		}
	}else{
		cerr << "plotGraph() : Unsupported cv::Mat format : " << data.type() << endl;
	}

	return dst;
}

void showGraph(string windowName, Mat _data, int _height, int _width) {
	Mat output(_height, _width, CV_8U, Scalar::all(255));

	// Transpose Mat if needed
	Mat data;
	int N = _data.cols;
	if(N == 1){
		transpose(_data, data);
		N = data.cols;
	} else {
		_data.copyTo(data);
	}
	
	if(data.type() == CV_32S){
		// Find maximim and minimum value
		int maximum = INT_MIN;
		int minimum = INT_MAX;
		for(int i=0; i<(N); i++){
			float value = data.at<int>(0, i);
			if(value > maximum) maximum = value;
			if(value < minimum) minimum = value;
		}

		// Draw values
		float prev = data.at<int>(0, 0);
		for(int i = 1; i < data.cols ; i++){
			int next = data.at<int>(0, i);

			line(output,
				Point(MAP(i-1, 0, N, 0, _width),
					  _height - MAP(prev, minimum, maximum, 0, _height)),
				Point(MAP(i, 0, N, 0, _width),
					  _height - MAP(next, minimum, maximum, 0, _height)),
				cv::Scalar::all(0));
		
			prev = next;
		}
	} else if(data.type() == CV_32F){
		// Find maximim and minimum value
		float maximum = FLT_MIN;
		float minimum = FLT_MAX;
		for(int i=0; i<(N); i++){
			float value = data.at<float>(0, i);
			if(value > maximum) maximum = value;
			if(value < minimum) minimum = value;
		}

		// Draw values
		float prev = data.at<float>(0, 0);
		for(int i = 1; i < data.cols ; i++){
			float next = data.at<float>(0, i);

			line(output,
				Point(MAP(i-1, 0, N, 0, _width),
					  _height - MAP(prev, minimum, maximum, 0, _height)),
				Point(MAP(i, 0, N, 0, _width),
					  _height - MAP(next, minimum, maximum, 0, _height)),
				Scalar::all(0));
		
			prev = next;
		}
	} else if(data.type() == CV_64F) {
		// Find maximim and minimum value
		double maximum = DBL_MIN;
		double minimum = DBL_MAX;
		for(int i=0; i<(N); i++){
			double value = data.at<double>(0, i);
			if(value > maximum) maximum = value;
			if(value < minimum) minimum = value;
		}

		// Draw values
		float prev = data.at<double>(0, 0);
		for(int i = 1; i < data.cols ; i++){
			double next = data.at<double>(0, i);

			line(output,
				Point(MAP(i-1, 0, N, 0, _width),
					  _height - MAP(prev, minimum, maximum, 0, _height)),
				Point(MAP(i, 0, N, 0, _width),
					  _height - MAP(next, minimum, maximum, 0, _height)),
				Scalar::all(0));
		
			prev = next;
		}
	} else {
		cerr << "showGraph() : Unsupported cv::Mat format : " << data.type() << endl;
	}

	imshow(windowName, output);
}

Mat convertValues(WAVEHDR waveInHdr) {
	int N = waveInHdr.dwBufferLength / 2;
	Mat data(N, 1, CV_32S, Scalar::all(0));
	for(int i = 0; i < N; i++){
		data.at<int>(i) = (waveInHdr.lpData[(i*2)] + (waveInHdr.lpData[(i*2) + 1] << 8));
	}
	return data;
}

Spectrogram::~Spectrogram(void)
{
}


Spectrogram::Spectrogram(int _height, int _width) {
		spectrogram = new Mat(_height, _width, CV_32F, Scalar::all(0));
		index = 0;
}

void Spectrogram::hanningWindow(Mat src, Mat dst){
	if(src.type() != CV_32S || dst.type() != CV_32S || src.rows != dst.rows){
		cerr << "Spectrogram::hanningWindow() : ERROR with Input" << endl;
	}
		
	int N = src.rows;
	for(int i=0; i<N; i++){
		float value = ((float)src.at<int>(i) * (0.54 - (0.46 * cos((2 * M_PI * (float)i) / ((float) N - 1) ))));

		dst.at<int>(i) = (value < 0.0) ? ceil(value - 0.5) : floor(value + 0.5);
	}
}

void Spectrogram::addInput(WAVEHDR waveInHdr){
		
	// Fill Audio input data into Mat
	int N = waveInHdr.dwBufferLength / 2;
	Mat audio(N, 1, CV_32S, Scalar::all(0));
	for(int i = 0; i < N; i++){
		audio.at<int>(i) = (waveInHdr.lpData[(i*2)] + (waveInHdr.lpData[(i*2) + 1] << 8));
	}

	addInput(audio, N);
}

void Spectrogram::addInput(Mat input, int N){
	
	//writeData(audio, AUDIO_OUTPUT_PATH);
	//hanningWindow(input, input);
	//writeData(audio, FILTERED_AUDIO_OUTPUT_PATH);

	// Calculation of FFT
	// http://docs.opencv.org/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
	Mat planes[] = {Mat_<float>(input), Mat::zeros(input.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix
	
	split(complexI, planes);                    // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]); // planes[0] = magnitude
	Mat magI = planes[0];
	//writeData(magI, FFT_OUTPUT_PATH);

	// Smooth the frequency spectrum
	// http://docs.opencv.org/modules/imgproc/doc/filtering.html#Mat%20getGaussianKernel%28int%20ksize,%20double%20sigma,%20int%20ktype%29
	//int ksize = 10;
	//Mat smoothKernel = getGaussianKernel(ksize, 0.3*((ksize-1)*0.5 - 1) + 0.8);
	//filter2D(magI, magI, -1, smoothKernel);

	// Cut the input into half
	magI = magI(Rect(0, 0, 1, N/2));

	// Display Audio Input
	//Mat output(AUDIO_DISPLAY_HEIGHT, AUDIO_DISPLAY_WIDTH, CV_8UC3, Scalar::all(255));
	//output = plotGraph(output, magI, 0, FREQUENCY_RANGE, Scalar::all(200));
	//output = plotGraph(output, input, -AUDIO_RANGE/2, AUDIO_RANGE/2);
	//imshow("Audio Input", output);

	magI += Scalar::all(1); // switch to logarithmic scale
	log(magI, magI);
	//writeData(magI, LOG_FFT_OUTPUT_PATH);
		
	// Represent Information
	Mat line;
	magI.copyTo(line);
	resize(line, line, Size(1, spectrogram->rows));
		
	// Normalize values to the Range : 0.0 to 1.0
	normalize(line, line, 0, 1, CV_MINMAX);
	flip(line, line, 0);
		
	// Add line to the spectrogram Mat
	hconcat(line, spectrogram->colRange(0, spectrogram->cols - 1), *spectrogram);
}

Mat Spectrogram::getSpectrogram(int colorMap){
	Mat cm_spectrogram;
	(*spectrogram).copyTo(cm_spectrogram);
	cm_spectrogram.convertTo(cm_spectrogram, CV_8UC3, 255.0);

	// http://docs.opencv.org/master/modules/contrib/doc/facerec/colormaps.html
	applyColorMap(cm_spectrogram, cm_spectrogram, colorMap = COLORMAP_JET);
		
	return cm_spectrogram;
}

Mat Spectrogram::getDetailedSpectrogram(int padding, int colorMap){
	Mat output = getSpectrogram(colorMap);
	copyMakeBorder(output, output, padding, padding, 100 + padding, padding, BORDER_CONSTANT, Scalar::all(190));

	Mat axis = output(Rect(padding, padding, 100, output.rows - (2 * padding)));
	for(int i=0; i < SAMPLE_RATE / 2; i += 1000){
			
		// lines and axis labeling
		if((i % 5000) == 0){
			line(axis, 
				Point(70, axis.rows - 1 - MAP(i, 0, (SAMPLE_RATE / 2), 0, axis.rows)),
				Point(axis.cols, axis.rows - 1 - MAP(i, 0, (SAMPLE_RATE / 2), 0, axis.rows)),
				Scalar::all(0));

			char str[200];
			sprintf(str, "%6d Hz", i);
			putText(axis, str, 
				Point(10, axis.rows - 5 - MAP(i, 0, (SAMPLE_RATE / 2), 0, axis.rows)), 
				CV_FONT_NORMAL,
				0.5, Scalar::all(0));

		} else {
			line(axis, 
				Point(80, axis.rows - 1 - MAP(i, 0, (SAMPLE_RATE / 2), 0, axis.rows)),
				Point(axis.cols, axis.rows - 1 - MAP(i, 0, (SAMPLE_RATE / 2), 0, axis.rows)),
				Scalar::all(128));
		}
	}

	return output;
}