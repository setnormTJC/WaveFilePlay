#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES //for M_PI in Fourier Transform 

#include "WaveFile.h"

#include<algorithm> 
#include<chrono> 
#include<sstream>



namespace FourierTransform
{
	/*requires this file to be present in current directory: "F = 100_N = 44100_A = 10000.wav"*/
	void transform1SecondOf100Hz(); 

	std::vector<double> getTransformOfWaveFile(const std::string& filename); 

	/*Calls `getTransformOfWaveFile` to get the vector*/
	std::map<int, int> getMapOfFrequencyToAmplitude(const std::string& filename); 

	namespace Testing
	{
		/*F = 100_N = 44100_A = 10000.wav*/
		void getFTOfSinglePureSineWave();
	}

}

namespace SimpleTesting
{

	void demo880HzSineWave();

	void demoPianoNoteAndWaveTypeConstructor();

	void playAll88Notes_withHalfSecondDuration(); 

	void demoAmazingGrace();

	void demoSimpleWaveFileReading();

	void demoMeSpeakingInReverse();


	void skywardSwordDemo();

	void simpleWaveAnalysis();

	void demoCMajorChord();

	void demoSeventhChord();
}

/*"Music" is ... debatable*/
namespace MusicMaking
{
	/*AND it doesn't do too well, I think*/
	void chatGPTTriesLaFilleAuxCheveuxDeLin();

	/*VAGUELY recognizable*/
	void playMysterySong();
}

/*Attack, decay, sustain, release envelope*/
namespace ADSRTesting
{
	/*The sound here is more "natural" - the buildup to max amplitude and the drop off sounds "better" than pure sine wave*/
	void playC4();
}