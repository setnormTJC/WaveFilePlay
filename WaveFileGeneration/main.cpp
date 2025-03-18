// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"
#include <random>

/*Requires TimpaniC2.wav file's presence in this cpp file's folder*/
void getFTOfC4WithHarmonics()
{
	std::string baseFilename = "C4withHarmonics";
	//wavefile.writeToWaveFile(baseFilename + ".wav");

	FourierTransform ft(baseFilename + ".wav");

	ft.fillTransformDataAndFrequencyMap();

	ft.writeFTMapToCSV(baseFilename + ".csv");

	std::string callPythonPlottingScript = "python plotFourierTransform.py " + baseFilename + ".csv";
	system(callPythonPlottingScript.c_str());
}

int main()
{
	//MusicMaking::playMysterySong(); 

	//std::cout << "Any key to continue:\n";
	//std::cin.get(); 


	try
	{
		PianoNote singleNote("C4", 2.0f, PianoNote::Loudness::Fortissimo);

		std::vector<std::vector<PianoNote>> notes =
		{
			{singleNote}, //first melodic, single note
			{PianoNote("C#3", 2.0f, PianoNote::Loudness::Piano)}, //second melodic, single note
			{PianoNote("D3", 1.0f, PianoNote::Loudness::Mezzo) },
			{PianoNote("D#3", 2.0f, PianoNote::Loudness::Forte)}, //etc.
			{
				PianoNote("E3", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("G3", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("B3", 1.0f, PianoNote::Loudness::Mezzo) //third melodic CHORD
			}

		};

		WaveFile wavefile(notes);
		std::string wavefileName = "MelodicAndHarmonicNotes.wav";
		//std::string wavefileName = "SingleNote.wav";
		wavefile.writeToWaveFile(wavefileName);

		system(wavefileName.c_str());

	}
	
	catch (const MyException& e)
	{
		std::cout << e.what() << "\n";
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
	
	catch (...)
	{
		std::cout << "Some other? exception type got thrown? \n";
	}
}

