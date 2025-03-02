// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include"PianoNote.h"
#include "WaveFile.h"

#include<algorithm> 
#include<chrono> 
#include<sstream>

void demo880HzSineWave()
{
	try
	{


		const int MAX_AMPLITUDE = (int)pow(2, 15) - 1; //32,767 -> INTS expected by PCM audio 

		constexpr int NumSamples = 100'000; //previously 10  
		constexpr float frequency = 880.0; //previously 5.0 

		WaveFile aWaveFile(NumSamples, MAX_AMPLITUDE, frequency);

		time_t now = time(nullptr);

		std::string filename = std::to_string(localtime(&now)->tm_hour) + "_"
			+ std::to_string(localtime(&now)->tm_min);

		filename += ".wav";

		aWaveFile.writeToWaveFile(filename);
	}
	catch (const std::exception& e)
	{
		std::cout << "\033[31m"; // red
		std::cout << "Exception thrown: " << e.what() << "\n";
		std::cout << "\033[39m"; // default?
		//std::cout << "Ro?\n";
	}
}

void demoChromaticScaleFromC3ToC5()
{
	try
	{
		WaveFile aWaveFile{};
		time_t now = time(nullptr);

		std::string filename = std::to_string(localtime(&now)->tm_hour) + "_"
			+ std::to_string(localtime(&now)->tm_min);

		filename += ".wav";

		aWaveFile.writeToWaveFile(filename);
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

void demoAmazingGrace()
{
	/*For "Amazing Grace" -> https://musescore.com/user/2018/scores/1937
*/
	const int tempoForAmazingGrace = 80; //beats per minute 
	float secondsInAMinute = 60.0;
	float quarterNoteDurationIn3_4Time = secondsInAMinute / tempoForAmazingGrace;

	std::vector<PianoNote> amazingGraceNotes =
	{
		{"C3", quarterNoteDurationIn3_4Time,	 PianoNote::Loudness::Mezzo}, //quarter note ("A")
		{"F3", quarterNoteDurationIn3_4Time * 2, PianoNote::Loudness::Forte}, //half note ("may"
		{"A3", quarterNoteDurationIn3_4Time / 2, PianoNote::Loudness::Mezzo}, //eighth note ("zee..")
		{"F3", quarterNoteDurationIn3_4Time / 2, PianoNote::Loudness::Piano}, //another eighth note ("..ing)

		{"A3", quarterNoteDurationIn3_4Time * 2, PianoNote::Loudness::Piano},
		{"G3", quarterNoteDurationIn3_4Time,	 PianoNote::Loudness::Mezzo},
		{"F3", quarterNoteDurationIn3_4Time * 2, PianoNote::Loudness::Piano},
		{"D3", quarterNoteDurationIn3_4Time,	 PianoNote::Loudness::Mezzo},

		{"C3", quarterNoteDurationIn3_4Time * 2, PianoNote::Loudness::Piano}
	};

	WaveFile aWaveFile(amazingGraceNotes, WaveFile::WaveType::Sine);

	////reverse audio: 
	//aWaveFile.reverseAudio(); 

	////increase volume: 
	//aWaveFile.modifyVolume(3.0); 

	//decrease volume: 
	aWaveFile.modifyVolume(0.5);

	time_t now = time(nullptr);

	std::string filename = std::to_string(localtime(&now)->tm_hour) + "_"
		+ std::to_string(localtime(&now)->tm_min);

	filename += ".wav";

	aWaveFile.writeToWaveFile(filename);
}

void demoSimpleWaveFileReading()
{
	std::string inputFilename = "First wave!.wav";
	WaveFile inputWaveFile(inputFilename);

	std::string csvFilename = "firstWave.csv";
	inputWaveFile.writeSoundDataToCSV(csvFilename);

	std::string outputFilename = "outversion.wav";
	inputWaveFile.writeToWaveFile(outputFilename);

}

void demoMeSpeakingInReverse()
{
	std::string infileName = "meSpeaking.wav";

	WaveFile inputWaveObject(infileName);

	std::string outfileName = "echoingMe.wav";
	inputWaveObject.writeToWaveFile(outfileName);

	std::string reversedOutFileName = "reversedMe.wav";
	inputWaveObject.reverseAudio();
	inputWaveObject.writeToWaveFile(reversedOutFileName);
}

void skywardSwordDemo()
{
	try
	{
		std::string infileName = "skywardSword.wav";
		WaveFile inputWaveObject(infileName);

		inputWaveObject.reverseAudio();
		inputWaveObject.writeToWaveFile("reversedskywardSword.wav");

	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

void simpleWaveAnalysis()
{
	int sampleCount = 44'100 / 10; //4410 (should be 0.1 second duration) 
	int amplitude = 32'767; //give full dynamic range -> for ease of visualization 
	float frequency = 100.0f; //nice easy to work with - also easy to hear 

	std::ostringstream filename;
	filename << sampleCount << "samples_With_A=" << amplitude << "_andF=" << frequency;

	//const char* filename = tempFilename.str().c_str(); 

	WaveFile outWave(sampleCount, amplitude, frequency);

	outWave.writeSoundDataToCSV(filename.str() + ".csv");
	outWave.writeToWaveFile(filename.str() + ".wav");
}

void demoCMajorChord()
{
	PianoNote C4("C4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote E4("E4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote G4("G4", 2.0f, PianoNote::Loudness::Piano);


	std::vector<PianoNote> harmonicSongNotes = { C4, E4, G4 };

	std::string chordFileName = "C4E4G4_CMaj";

	WaveFile cMajorChord(harmonicSongNotes);

	cMajorChord.writeSoundDataToCSV(chordFileName + ".csv");
	cMajorChord.writeToWaveFile(chordFileName + ".wav");

}

void demoSeventhChord()
{
	PianoNote C4("C4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote E4("E4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote G4("G4", 2.0f, PianoNote::Loudness::Piano);
	//SongNote A4("A4", 2.0f, SongNote::Loudness::Piano);
	PianoNote B4("B4", 2.0f, PianoNote::Loudness::Piano);


	//std::vector<SongNote> harmonicSongNotes = { C4, E4, G4, A4 };
	std::vector<PianoNote> harmonicSongNotes = { C4, E4, G4, B4 };


	//WaveFile c6MajorChord(harmonicSongNotes);
	WaveFile c7MajorChord(harmonicSongNotes);

	std::ostringstream chordFileName;
	//chordFileName << C4.name << E4.name << G4.name << A4.name;
	chordFileName << C4.name << E4.name << G4.name << B4.name;

	c7MajorChord.writeSoundDataToCSV(chordFileName.str() + ".csv");
	c7MajorChord.writeToWaveFile(chordFileName.str() + ".wav");

}

int main()
{

	try
	{
		PianoNote note; 
	}
	
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

