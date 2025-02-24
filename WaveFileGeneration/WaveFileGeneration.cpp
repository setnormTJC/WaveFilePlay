// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include"SongNote.h"
#include "WaveFile.h"

#include<algorithm> 
#include<chrono> 

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

	std::vector<SongNote> amazingGraceNotes =
	{
		{"C3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo}, //quarter note ("A")
		{"F3", quarterNoteDurationIn3_4Time * 2, SongNote::Loudness::Forte}, //half note ("may"
		{"A3", quarterNoteDurationIn3_4Time / 2, SongNote::Loudness::Mezzo}, //eighth note ("zee..")
		{"F3", quarterNoteDurationIn3_4Time / 2, SongNote::Loudness::Piano}, //another eighth note ("..ing)

		{"A3", quarterNoteDurationIn3_4Time * 2, SongNote::Loudness::Piano},
		{"G3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo},
		{"F3", quarterNoteDurationIn3_4Time * 2, SongNote::Loudness::Piano},
		{"D3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo},

		{"C3", quarterNoteDurationIn3_4Time * 2, SongNote::Loudness::Piano}
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

int main()
{

	//demo880HzSineWave(); 
	try
	{
		//WaveFile aWaveFile{}; 
		//WaveFile aWaveFile("C3", WaveFile::WaveType::Sine, 2);
		//WaveFile aWaveFile("A4", WaveFile::WaveType::Square, 2); 
		//demoAmazingGrace(); 

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

