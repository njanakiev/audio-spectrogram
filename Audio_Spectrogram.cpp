#include "Helper.h"
#include "..\Spectrogram.h"

int main(int argc, char ** argv){

	// Setup for WaveIn audio
	// http://www.techmind.org/wave/
	short int waveIn[SAMPLES];			// 'short int' is a 16-bit type; I request 16-bit samples below
										// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types
	HWAVEIN	hWaveIn;
	WAVEHDR	WaveInHdr;

	// Specify recording parameters
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag=WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels=1;                    //  1=mono, 2=stereo
	pFormat.nSamplesPerSec=SAMPLE_RATE;      // 44100
	pFormat.nAvgBytesPerSec=SAMPLE_RATE * 2;   // = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.nBlockAlign=2;                  // = n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample=16;              //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize=0;

	if (waveInOpen(&hWaveIn, WAVE_MAPPER,&pFormat, 0L, 0L, WAVE_FORMAT_DIRECT)) return -1;

	// Set up header for input
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = SAMPLES * 2;
	WaveInHdr.dwBytesRecorded=0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;

	// Print information
	cout << "Spectrogram" << endl;
	cout << "Sample Rate : " << SAMPLE_RATE << ", N : " << SAMPLES << endl;


	Spectrogram spectrogram(SPECTROGRAM_HEIGHT, SPECTROGRAM_WIDTH);

	// Wait for ESC Key
	while(waitKey(10) != 27) {
		
		// Prepare header for input
		waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
		// Insert a wave input buffer
		if (waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR))) return -1;
		// Commence sampling input
		if (waveInStart(hWaveIn)) return -1;
		// Wait until finished recording
		do {} while (waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR))==WAVERR_STILLPLAYING);
		
		spectrogram.addInput(WaveInHdr);

		// Simple Spectrogram
		//imshow("Spectrogram", spectrogram.getSpectrogram());
		
		// Detailed Spectrogram
		imshow("Spectrogram", spectrogram.getDetailedSpectrogram(20, COLORMAP_JET));
	}

	waveInClose(hWaveIn);
	waitKey();

	return 0;
}