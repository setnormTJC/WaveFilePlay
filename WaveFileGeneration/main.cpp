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
		//getFTOfA4WithHarmonics(); 
		PianoNote::initialize(); 

		for (int i = 45; i < 55; ++i)
		{
			const std::string noteName = PianoNote::the88Notes.at(i);

			PianoNote note(noteName, 2.0f);
			WaveFile wavefile(note);
			std::string wavefilename = noteName + ".wav";
			wavefile.writeToWaveFile(wavefilename);
			std::cout << "Playing " << wavefilename << "\n";
			system(wavefilename.c_str());
			std::cout << "Any key to continue\n";
			std::cin.get(); 

		}

		//getFTOfC4WithHarmonics(); 


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

