// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"
#include <random>



int main()
{
	//MusicMaking::playMysterySong(); 

	//std::cout << "Any key to continue:\n";
	//std::cin.get(); 


	try 
	{
		constexpr int tempo = 96; //bpm
		//std::vector < std::vector<PianoNote>> mysterySongNotes = MusicMaking::getMysterySongNotes(tempo); 




		/*m1 means first measure */
		PianoChord m1TrebleChord
		(
			{
				PianoNote("C3", 1.0f, PianoNote::Loudness::Mezzo), //stronger, because it lasts longer ...:)
			}
		);
		PianoChord m1secondTrebleChord
		(
			{
				PianoNote("B4",  2.0f, PianoNote::Loudness::Mezzo)
			}
		);

		/*m2 means second measure*/
		PianoChord m2trebleChord({ "G5"}, 3.0f, PianoNote::Loudness::Mezzo);

		std::vector < std::vector<PianoNote>> trebleNotes =
		{
			m1TrebleChord.getChord(),
			m1secondTrebleChord.getChord(),
			m2trebleChord.getChord()
		};




		WaveFile wavefile(trebleNotes); 
	
		std::string wavefileName = "attemptAt2Tracks.wav";
		system(wavefileName.c_str());

		std::cout << "Just played the treble layer (track) - now press any key to play both treble and bass:\n";
		std::cin.get(); 



		/*Now add dat bass:*/
		PianoChord m1BassChord
		(
			{
				PianoNote("E1", 1.0f, PianoNote::Loudness::Mezzo), //randomish bass notes
				PianoNote("B2", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("F2", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("A2", 1.0f, PianoNote::Loudness::Mezzo) 
			}
		);

		PianoChord m1secondBassChord
		(
			{
				PianoNote("F1", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("B3", 1.0f, PianoNote::Loudness::Mezzo),
				PianoNote("E4", 2.0f, PianoNote::Loudness::Mezzo),
				PianoNote("G4", 2.0f, PianoNote::Loudness::Mezzo)
			}
		);

		/*m2 means second measure*/
		PianoChord m2BassChord
		(
			{
				PianoNote("C5", 3.0f, PianoNote::Loudness::Mezzo),
				PianoNote("E5", 3.0f, PianoNote::Loudness::Mezzo)
			}
		);

		std::vector < std::vector<PianoNote>> bassNotes =
		{
			m1BassChord.getChord(),
			m1secondBassChord.getChord(),
			m2BassChord.getChord()
		};

		/*The interesting bit of code here: */
		wavefile.addTrack(bassNotes);

		//write the UPDATED wave file (with the bass layered in) 
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

