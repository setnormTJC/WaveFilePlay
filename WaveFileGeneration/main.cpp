// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"Demos.h"

#include"FourierTransform.h"
#include"MyException.h"
#include"MysterySong.h"
#include"Song.h"
#include <random>


void demoMelody()
{
	PianoNote firstNote("G3", 0.5, PianoNote::Loudness::Mezzo);

	std::vector<PianoNote> firstChord =
	{
		firstNote
	};

	std::vector<std::vector<PianoNote>> songNotes =
	{
		firstChord
	};


	WaveFile anotherWaveFile(songNotes);

}

int main()
{

	//WaveFile theWaveFile("abcdefg.wav");
	//theWaveFile.reverseAudio(); 

	//theWaveFile.writeToWaveFile("gfedcba.wav");


	//std::system("gfedcba.wav");

	try
	{
		FourierTransform ft("flute.wav");

		ft.fillTransformDataAndFrequencyMap(); 

		std::string ftFilename = "flute.csv";

		ft.writeFTMapToCSV(ftFilename);

		std::string callPythonToPlotTransform = "python plotFourierTransform.py " + ftFilename;

		system(callPythonToPlotTransform.c_str());

		//FourierTransform::FourierTransform ("440.wav");
		//FourierTransform::FourierTransform()



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

