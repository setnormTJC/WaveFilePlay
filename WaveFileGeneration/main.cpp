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

/*Come back later and make this better*/
void failedShufflePlay()
{
	std::mt19937_64 rng(std::random_device{}());

	while (true)
	{
		auto notes = Utils::generateF3Maj7();

		//while (std::next_permutation(notes.begin(), notes.end())) //requires overloading < !
		for (int i = 0; i < notes.size(); ++i)
		{
			WaveFile wavefile(notes);

			std::string filename = "CyclicSyntheticF3Major6.wav";

			wavefile.writeToWaveFile(filename);

			system(filename.c_str());
			std::cout << "Any key to move on to next permutation of chord:\n";
			std::cin.get();

			std::shuffle(notes.begin(), notes.end(), rng);
		}
	}
}

void demoSixthChordsForTwoOctaves()
{
	PianoNote::initialize();

	int indexOfC3 = 27;
	int indexOfC4 = 39;

	while (true)
	{
		for (int i = indexOfC3; i <= indexOfC4; ++i)
		{
			std::string baseNoteName = PianoNote::the88Notes[i];

			PianoChord pianoChord(baseNoteName, PianoChord::ChordType::majorSixth);

			WaveFile wavefile(pianoChord.theChordNotes);

			std::string filename = pianoChord.theChordNotes.at(0).noteName + "Major6.wav";
			wavefile.writeToWaveFile(filename);
			system(filename.c_str());

			std::cout << "Any key to move on to next chord:\n";
			std::cin.get();
		}
	}
}

int main()
{
	//MusicMaking::playMysterySong(); 

	//std::cout << "Any key to continue:\n";
	//std::cin.get(); 


	try
	{
		//getFTOfA4WithHarmonics(); 

		PianoNote note("C4", 2.0f);
		WaveFile wavefile(note); 
		std::string wavefilename = "C4withHarmonics.wav";
		wavefile.writeToWaveFile(wavefilename);
		system(wavefilename.c_str());

		getFTOfC4WithHarmonics(); 

		PianoNote::initialize();

		int indexOfC3 = 30; 
		int indexOfC4 = 39; 

		while (true)
		{
			for (int i = indexOfC3; i <= indexOfC4; ++i) 
			{
				std::string baseNoteName = PianoNote::the88Notes[i];

				PianoChord pianoChord(baseNoteName, PianoChord::ChordType::majorSixth);

				auto chordAndItsInversions = pianoChord.getChordAndItsInversions(); 

				for (const std::vector<PianoNote>& currentChord : chordAndItsInversions)
				{
					//WaveFile wavefile(currentChord, WaveFile::WaveType::Piano);
					WaveFile wavefile(currentChord);

					std::string filename = "Arpeggiated"+
						baseNoteName + "Major6invertedOver" + currentChord.at(0).noteName + ".wav";

					wavefile.writeToWaveFile(filename);
					system(filename.c_str());

					std::cout << "Any key to move on to next chord:\n";
					std::cin.get();
				}

			}
		}

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

