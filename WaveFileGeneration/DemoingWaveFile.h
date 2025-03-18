#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES //for M_PI in Fourier Transform 

#include "WaveFile.h"

#include<algorithm> 
#include<chrono> 
#include<sstream>


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
namespace SynthesizerTesting
{
	/*The sound here is more "natural" - the buildup to max amplitude and the drop off sounds "better" than pure sine wave*/
	void playC4();


	void demoSynthesizedPianoNote(const std::string& noteName); 

	void demo88SynthesizedPianoNotes();

	void demoMelodicSynthesizedPianoNote(const std::vector<PianoNote>& notes);

	/*Come back later and make this better*/
	void failedShufflePlay(); 

	void demoSixthChordsForTwoOctaves();

	void demoMoreMajorSixthChords();
}

namespace Utils
{
	std::vector<PianoNote> generateSomeNotes(); 

	std::vector<PianoNote> generateC3Major();

	/*F3, A3, C4, E4*/
	std::vector<PianoNote> generateF3Maj7();
}