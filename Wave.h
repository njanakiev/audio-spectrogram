// http://www.codeproject.com/Articles/29676/CWave-A-Simple-C-Class-to-Manipulate-WAV-Files

#pragma once
#pragma comment( lib, "Winmm.lib" )

#include <stdio.h>
#include <Windows.h>
#include "Mmsystem.h"


#pragma pack(1)
typedef struct __WAVEDESCR
{
	BYTE riff[4];
	DWORD size;
	BYTE wave[4];

} _WAVEDESCR, *_LPWAVEDESCR;

typedef struct __WAVEFORMAT
{
	BYTE id[4];
	DWORD size;
	SHORT format;
	SHORT channels;
	DWORD sampleRate;
	DWORD byteRate;
	SHORT blockAlign;
	SHORT bitsPerSample;

} _WAVEFORMAT, *_LPWAVEFORMAT;
#pragma pack()


class CWave
{
public:
	CWave(void);
	virtual ~CWave(void);

public:
	// Public methods
	BOOL Load(const char* lpszFilePath);
	BOOL Save(const char* lpszFilePath);
	BOOL Play();
	BOOL Stop();
	BOOL Pause();
	BOOL Mix(CWave& wave);
	BOOL IsValid()				{return (m_lpData != NULL);}
	BOOL IsPlaying()			{return (!m_bStopped && !m_bPaused);}
	BOOL IsStopped()			{return m_bStopped;}
	BOOL IsPaused()				{return m_bPaused;}
	LPBYTE GetData()			{return m_lpData;}
	DWORD GetSize()				{return m_dwSize;}
	SHORT GetChannels()			{return m_Format.channels;}
	DWORD GetSampleRate()		{return m_Format.sampleRate;}
	SHORT GetBitsPerSample()	{return m_Format.bitsPerSample;}

private:
	// Pribate methods
	BOOL Open(SHORT channels, DWORD sampleRate, SHORT bitsPerSample);
	BOOL Close();
	BOOL static CALLBACK WaveOut_Proc(HWAVEOUT hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

private:
	// Private members
	_WAVEDESCR m_Descriptor;
	_WAVEFORMAT m_Format;
	LPBYTE m_lpData;
	DWORD m_dwSize;
	HWAVEOUT m_hWaveout;
	WAVEHDR m_WaveHeader;
	BOOL m_bStopped;
	BOOL m_bPaused;
};
