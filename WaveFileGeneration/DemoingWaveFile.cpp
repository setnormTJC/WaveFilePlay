#include "DemoingWaveFile.h"
#include <random>

#pragma region SimpleTesting
void SimpleTesting::demo880HzSineWave()
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

void SimpleTesting::demoPianoNoteAndWaveTypeConstructor()
{
	WaveFile C4(PianoNote("C3", 2.0f, PianoNote::Loudness::Pianissimo), WaveFile::WaveType::Sine);

	C4.writeToWaveFile("C4.wav");
}

void SimpleTesting::playAll88Notes_withHalfSecondDuration()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

	PianoNote::initialize(); //fills the88Notes and notesToFrequencies

	std::vector<PianoNote> notes;

	const float duration = 0.5f; //seconds 

	for (const std::string& noteName : PianoNote::the88Notes)
	{
		PianoNote currentNote(noteName, duration, PianoNote::Loudness::Piano);
		notes.push_back(currentNote); 
	}


	//WaveFile waveFile(notes, WaveFile::WaveType::Sine);

	//waveFile.writeToWaveFile("all88PianoNotes_withHalfSecondDuration.wav");
}



void SimpleTesting::demoAmazingGrace()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

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

	//WaveFile aWaveFile(amazingGraceNotes, WaveFile::WaveType::Sine);
	//aWaveFile.writeToWaveFile("amazingGrace.wav");

	////reverse audio: 
	//aWaveFile.reverseAudio(); 

	////increase volume: 
	//aWaveFile.modifyVolume(3.0); 

	//decrease volume: 
	//aWaveFile.modifyVolume(0.5);

	time_t now = time(nullptr);

	std::string filename = std::to_string(localtime(&now)->tm_hour) + "_"
		+ std::to_string(localtime(&now)->tm_min);

	filename += ".wav";

	//aWaveFile.writeToWaveFile(filename);
}

void SimpleTesting::demoSimpleWaveFileReading()
{
	std::string inputFilename = "First wave!.wav";
	WaveFile inputWaveFile(inputFilename);

	std::string csvFilename = "firstWave.csv";
	//inputWaveFile.writeSoundDataToCSV(csvFilename);

	std::string outputFilename = "outversion.wav";
	inputWaveFile.writeToWaveFile(outputFilename);

}

void SimpleTesting::demoMeSpeakingInReverse()
{
	std::string infileName = "meSpeaking.wav";

	WaveFile inputWaveObject(infileName);

	std::string outfileName = "echoingMe.wav";
	inputWaveObject.writeToWaveFile(outfileName);

	std::string reversedOutFileName = "reversedMe.wav";
	inputWaveObject.reverseAudio();
	inputWaveObject.writeToWaveFile(reversedOutFileName);
}

void SimpleTesting::skywardSwordDemo()
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

void SimpleTesting::simpleWaveAnalysis()
{
	int sampleCount = 44'100 / 10; //4410 (should be 0.1 second duration) 
	int amplitude = 32'767; //give full dynamic range -> for ease of visualization 
	float frequency = 100.0f; //nice easy to work with - also easy to hear 

	std::ostringstream filename;
	filename << sampleCount << "samples_With_A=" << amplitude << "_andF=" << frequency;

	//const char* filename = tempFilename.str().c_str(); 

	WaveFile outWave(sampleCount, amplitude, frequency);

	//outWave.writeSoundDataToCSV(filename.str() + ".csv");
	outWave.writeToWaveFile(filename.str() + ".wav");
}

void SimpleTesting::demoCMajorChord()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

	PianoNote C4("C4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote E4("E4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote G4("G4", 2.0f, PianoNote::Loudness::Piano);


	std::vector<PianoNote> harmonicSongNotes = { C4, E4, G4 };

	std::string chordFileName = "C4E4G4_CMaj";

	//WaveFile cMajorChord(harmonicSongNotes);

	//cMajorChord.writeSoundDataToCSV(chordFileName + ".csv");
	//cMajorChord.writeToWaveFile(chordFileName + ".wav");

}

void SimpleTesting::demoSeventhChord()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);


	PianoNote C4("C4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote E4("E4", 2.0f, PianoNote::Loudness::Piano);
	PianoNote G4("G4", 2.0f, PianoNote::Loudness::Piano);
	//SongNote A4("A4", 2.0f, SongNote::Loudness::Piano);
	PianoNote B4("B4", 2.0f, PianoNote::Loudness::Piano);


	//std::vector<SongNote> harmonicSongNotes = { C4, E4, G4, A4 };
	std::vector<PianoNote> harmonicSongNotes = { C4, E4, G4, B4 };


	//WaveFile c6MajorChord(harmonicSongNotes);
	//WaveFile c7MajorChord(harmonicSongNotes);

	std::ostringstream chordFileName;
	//chordFileName << C4.name << E4.name << G4.name << A4.name;
	chordFileName << C4.noteName << E4.noteName << G4.noteName << B4.noteName;


	//c7MajorChord.writeSoundDataToCSV(chordFileName.str() + ".csv");
	//c7MajorChord.writeToWaveFile(chordFileName.str() + ".wav");

}
#pragma endregion




#pragma region MusicMaking namespace 

void MusicMaking::chatGPTTriesLaFilleAuxCheveuxDeLin()
{

	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);


	//ChatGPT attempting first two measures of La Fille Aux Cheveux de Lin given the link: 
// https://musescore.com/r_d/scores/5449385:
	float quarterNoteDuration = 60.0f / 110.0f; // Approximately 0.545 seconds
	float eighthNoteDuration = quarterNoteDuration / 2.0f; // Approximately 0.273 seconds

	std::vector<PianoNote> laFilleMelody = {
		{"F#4", quarterNoteDuration, PianoNote::Loudness::Mezzo}, // Measure 1, Beat 1
		{"G#4", eighthNoteDuration, PianoNote::Loudness::Mezzo},  // Measure 1, Beat 2 (first half)
		{"A#4", eighthNoteDuration, PianoNote::Loudness::Mezzo},  // Measure 1, Beat 2 (second half)
		{"G#4", eighthNoteDuration, PianoNote::Loudness::Mezzo},  // Measure 1, Beat 3 (first half)
		{"F#4", eighthNoteDuration, PianoNote::Loudness::Mezzo},  // Measure 1, Beat 3 (second half)
		{"D#4", quarterNoteDuration, PianoNote::Loudness::Mezzo}, // Measure 2, Beat 1
		// Rest for eighth note (Measure 2, Beat 2 first half) omitted
		{"F#4", eighthNoteDuration, PianoNote::Loudness::Piano},  // Measure 2, Beat 2 (second half)
		{"G#4", quarterNoteDuration, PianoNote::Loudness::Forte}  // Measure 2, Beat 3		
	};

	//WaveFile waveFile(laFilleMelody, WaveFile::WaveType::Sine);
	//waveFile.writeToWaveFile("LaFilleAuxCheveuxDeLin.wav"); //NOPE!
}

void MusicMaking::playMysterySong()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);


	//from: https://musescore.com/user/16242231/scores/3424496

	float tempo = 96.0f; //beats per minute
	float secondsPerMinute = 60.0f;
	float quarterNoteDuration = secondsPerMinute / tempo;
	float eighthNoteDuration = quarterNoteDuration / 2.0f;

	int silentScalingFactor = 8; //dictates how long silence lasts -> see below

	std::vector<PianoNote> mysteryMelody =
	{
		PianoNote("B3", quarterNoteDuration, PianoNote::Loudness::Mezzo), //feet 
		//make a bit of silence for staccato style (same note without silence sounds like double duration!) 
		PianoNote("", eighthNoteDuration / silentScalingFactor, PianoNote::Loudness::Silent),
		PianoNote("B3", quarterNoteDuration, PianoNote::Loudness::Mezzo), //don't
		PianoNote("A3", quarterNoteDuration, PianoNote::Loudness::Mezzo), //fail 
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //me
		PianoNote("A3", eighthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Mezzo), //now ...

		//3/4 rest (note the EMPTY string note name -> silence)
		PianoNote("", eighthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Silent),

		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //take
		PianoNote("", eighthNoteDuration / silentScalingFactor, PianoNote::Loudness::Silent), //again -> stacatto silence
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //me
		PianoNote("", eighthNoteDuration / silentScalingFactor, PianoNote::Loudness::Silent),
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //to
		PianoNote("", eighthNoteDuration / silentScalingFactor, PianoNote::Loudness::Silent),
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //the

		PianoNote("A3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //fi-
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Mezzo), //-NISH
		PianoNote("A3", eighthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Mezzo), //line
	};

	//WaveFile waveFile(mysteryMelody, WaveFile::WaveType::Sine);

	const char* waveFilename = "mysteryMelody.wav";
	//waveFile.writeToWaveFile(waveFilename);

	system(waveFilename);

}

std::vector<std::vector<PianoNote>> MusicMaking::getMysterySongNotes(const int tempo)
{
	float sixteenthNoteDuration = (60.0f / tempo) / 4.0f;
	float eighthNoteDuration = (60.0f / tempo) / 2.0f; //as in 60 seconds per minute 
	float dottedEighthNoteDuration = eighthNoteDuration * 1.5;
	float quarterNoteDuration =  60.0f / tempo; 
	//units: second PER beat! (where a "beat" is a quarter note in 4/4 time)
	float halfNoteNoteDuration = quarterNoteDuration * 2; 
	float dottedHalfNoteDuration = quarterNoteDuration * 3; 
	float wholeNoteDuration = 4 * quarterNoteDuration; 


	/*m1 means first measure */
	PianoChord m1secondTrebleChord("B4", PianoChord::ChordType::Octave, quarterNoteDuration, PianoNote::Loudness::Forte);

	/*m2 means second measure*/
	PianoChord m2trebleAndBassChord({"E3", "G3", "G4", "G5"}, wholeNoteDuration, PianoNote::Loudness::Mezzo);

	/*m3 means THIRD measure (surprise!)*/
	PianoChord m3trebleAndBassChord({"D3", "F#3", "A4", "A5"}, halfNoteNoteDuration, PianoNote::Loudness::Mezzo);
	PianoChord m3secondTrebleChord("G4", PianoChord::ChordType::Octave, quarterNoteDuration, PianoNote::Loudness::Forte);
	PianoChord m3thirdTrebleChord("F#4", PianoChord::ChordType::Octave, quarterNoteDuration, PianoNote::Loudness::Forte);

	/*fourth measure*/

	PianoChord m4trebleAndBaseChord({ "B3", "E4", "E5" }, eighthNoteDuration, PianoNote::Loudness::Mezzo);
	std::vector<PianoNote> m4Melody =
	{
		PianoNote("C4", eighthNoteDuration, PianoNote::Loudness::Forte),
		PianoNote("D4", eighthNoteDuration, PianoNote::Loudness::Forte),
		PianoNote("C4", eighthNoteDuration, PianoNote::Loudness::Forte),

		PianoNote("B4", eighthNoteDuration, PianoNote::Loudness::Forte),
		PianoNote("C4", eighthNoteDuration, PianoNote::Loudness::Forte),
		PianoNote("B4", quarterNoteDuration, PianoNote::Loudness::Forte)
	};

	/*m5*/
	PianoChord m5TrebleAndBassChord({ "C2", "E4", "G4", "C5", "E5" }, dottedEighthNoteDuration, PianoNote::Loudness::Mezzo);
	//git low, git low, git low!
	std::vector<PianoNote> m5Melody =
	{
		PianoNote("C2", sixteenthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Forte), // and? two

		PianoNote("C2", eighthNoteDuration, PianoNote::Loudness::Forte), //3 
		PianoNote("C2", eighthNoteDuration, PianoNote::Loudness::Forte), //and 

		PianoNote("C2", quarterNoteDuration, PianoNote::Loudness::Forte) //4!
	};

	/*m6*/
	PianoChord m6TrebleAndBassChord({ "E2", "B3", "E4", "G4", "A4" }, dottedEighthNoteDuration, PianoNote::Loudness::Mezzo);
	std::vector<PianoNote> m6Melody =
	{
		PianoNote("E2", sixteenthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Fortissimo), // and? two

		PianoNote("E2", eighthNoteDuration, PianoNote::Loudness::Fortissimo), //3 
		PianoNote("E2", eighthNoteDuration, PianoNote::Loudness::Fortissimo), //and 

		PianoNote("E2", quarterNoteDuration, PianoNote::Loudness::Fortissimo) //4!
	};

	/*m7*/
	PianoChord m7TrebleAndBassChord({ "D2", "G3", "D4", "F#4" }, dottedEighthNoteDuration, PianoNote::Loudness::Mezzo);
	std::vector<PianoNote> m7Melody =
	{
		PianoNote("D2", sixteenthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Fortissimo), 

		PianoNote("D2", eighthNoteDuration, PianoNote::Loudness::Fortissimo),  
		PianoNote("D2", eighthNoteDuration, PianoNote::Loudness::Fortissimo),  

		PianoNote("D2", quarterNoteDuration, PianoNote::Loudness::Fortissimo)
	};

	/*m8*/
	PianoChord m8TrebleAndBassChord({ "C2", "G3", "C4", "E4" }, dottedEighthNoteDuration, PianoNote::Loudness::Mezzo);
	std::vector<PianoNote> m8Melody =
	{
		PianoNote("C2", sixteenthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Fortississimo), 
		//adding "crescendo-y" volume before vocals kick in 

		PianoNote("C2", eighthNoteDuration, PianoNote::Loudness::Fortississimo),
		PianoNote("C2", eighthNoteDuration, PianoNote::Loudness::Fortississimo),

		PianoNote("C2", quarterNoteDuration, PianoNote::Loudness::Fortississimo)
	};

	/*m9*/
	PianoChord m9TrebleAndBassChord({ "G2", "G3", "B3" }, quarterNoteDuration, PianoNote::Loudness::Mezzo); 
	//"feet"
	std::vector<PianoNote> m9Melody =
	{
		PianoNote("B3", quarterNoteDuration, PianoNote::Loudness::Forte), //"don't"
		PianoNote("A3", quarterNoteDuration, PianoNote::Loudness::Forte), //"fail"
		PianoNote("B3", eighthNoteDuration, PianoNote::Loudness::Forte), //"me"
		PianoNote("A3", eighthNoteDuration + quarterNoteDuration, PianoNote::Loudness::Forte) //"now"
	};

	/*m9*/
	PianoChord m10TrebleAndBassChord({ "D2", "F#3"}, quarterNoteDuration + quarterNoteDuration, PianoNote::Loudness::Mezzo);
	//NOTE: not the correct duration above!

	std::vector<std::vector<PianoNote>> mysterySongNotes =
	{
		{PianoChord({"C3", "G3", "E5", "E6"}, dottedHalfNoteDuration, PianoNote::Loudness::Mezzo).getChord()},
		{m1secondTrebleChord.getChord()}, 

		{m2trebleAndBassChord.getChord()}, 
		
		{m3trebleAndBassChord.getChord()},
		{m3secondTrebleChord.getChord()},
		{m3thirdTrebleChord.getChord()},

		{m4trebleAndBaseChord.getChord()},
		{m4Melody[0]},
		{m4Melody[1]},
		{m4Melody[2]},
		{m4Melody[3]},
		{m4Melody[4]},
		{m4Melody[5]},

		{m5TrebleAndBassChord.getChord()}, 
		{m5Melody[0]}, 
		{m5Melody[1]},
		{m5Melody[2]},
		{m5Melody[3]},
		//{m5Melody[4]},

		{m6TrebleAndBassChord.getChord()},
		{m6Melody[0]},
		{m6Melody[1]},
		{m6Melody[2]},
		{m6Melody[3]},

		{m7TrebleAndBassChord.getChord()},
		{m7Melody[0]},
		{m7Melody[1]},
		{m7Melody[2]},
		{m7Melody[3]},

		{m8TrebleAndBassChord.getChord()},
		{m8Melody[0]},
		{m8Melody[1]},
		{m8Melody[2]},
		{m8Melody[3]},

		{m9TrebleAndBassChord.getChord()}, 
		{m9Melody[0]},
		{m9Melody[1]},
		{m9Melody[2]},
		{m9Melody[3]},

		{m10TrebleAndBassChord.getChord()},

	};


	return mysterySongNotes; 

}

void MusicMaking::playCMajorScaleChords()
{
	auto notes = Utils::generateCMajorScaleChords();

	WaveFile wavefile(notes);
	std::string wavefileName = "CMajorChords_thirdOctave.wav";
	wavefile.writeToWaveFile(wavefileName);

	system(wavefileName.c_str());


	std::cout << "Turn around and throw it in reverse (any key to continue)!\n";
	std::cin.get();

	wavefile.reverseAudio();
	std::string reversedWavefileName = "CMajorChords_thirdOctave-Reversed.wav";
	wavefile.writeToWaveFile(reversedWavefileName);

	system(reversedWavefileName.c_str());

}


#pragma endregion 

void SynthesizerTesting::playC4()
{
	WaveFile waveFile(PianoNote("C4", 2.0, PianoNote::Loudness::Forte), WaveFile::WaveType::Sine);


	const char* waveFilename = "C4_withExponentialDecayEnvelope.wav";

	waveFile.writeToWaveFile(waveFilename);

	std::string plotImageFilename = "plotImage.bmp";

	system(waveFilename);

	//waveFile.writeSoundDataToImagePlot(plotImageFilename);

	//system(plotImageFilename.c_str());

	//system(filename); 
}

void SynthesizerTesting::demoSynthesizedPianoNote(const std::string& noteName)
{
	WaveFile waveFile(PianoNote(noteName, 2.0f, PianoNote::Loudness::Forte),
		WaveFile::WaveType::Sine);

	std::string wavefileName = noteName + "PianoSynthesized.wav";

	waveFile.writeToWaveFile(wavefileName);

	system(wavefileName.c_str());
}

void SynthesizerTesting::demo88SynthesizedPianoNotes()
{
	PianoNote::initialize();

	for (const auto& note : PianoNote::the88Notes)
	{
		demoSynthesizedPianoNote(note);
		std::cout << "Any key to continue to next note: \n";
		std::cin.get();
	}
}

void SynthesizerTesting::demoMelodicSynthesizedPianoNote(const std::vector<PianoNote>& notes)
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

	//WaveFile wavefile(notes, WaveFile::WaveType::Sine);

	int noteCount = 0; //non-silent note count
	for (const auto& note : notes)
	{
		if (note.noteName != "")
		{
			noteCount++; 
		}
	}
	std::string wavefileName = std::to_string(noteCount) + "_melodicSynthNotes.wav";

	//wavefile.writeToWaveFile(wavefileName);

	system(wavefileName.c_str());
}


void SynthesizerTesting::failedShufflePlay()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

	std::mt19937_64 rng(std::random_device{}());

	while (true)
	{
		auto notes = Utils::generateF3Maj7();

		//while (std::next_permutation(notes.begin(), notes.end())) //requires overloading < !
		for (int i = 0; i < notes.size(); ++i)
		{
			//WaveFile wavefile(notes);

			std::string filename = "CyclicSyntheticF3Major6.wav";

			//wavefile.writeToWaveFile(filename);

			system(filename.c_str());
			std::cout << "Any key to move on to next permutation of chord:\n";
			std::cin.get();

			std::shuffle(notes.begin(), notes.end(), rng);
		}
	}
}

void SynthesizerTesting::demoSixthChordsForTwoOctaves()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);


	PianoNote::initialize();

	int indexOfC3 = 27;
	int indexOfC4 = 39;

	while (true)
	{
		for (int i = indexOfC3; i <= indexOfC4; ++i)
		{
			std::string baseNoteName = PianoNote::the88Notes[i];

			PianoChord pianoChord(baseNoteName, PianoChord::ChordType::MajorSixth, 2.0f,  
				PianoNote::Loudness::Piano);

			//WaveFile wavefile(pianoChord.theChordNotes);

			//std::string filename = pianoChord.theChordNotes.at(0).noteName + "Major6.wav";
			//wavefile.writeToWaveFile(filename);
			//system(filename.c_str());

			std::cout << "Any key to move on to next chord:\n";
			std::cin.get();
		}
	}
}


void SynthesizerTesting::demoMoreMajorSixthChords()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);

	PianoNote::initialize();

	int indexOfC3 = 30;
	int indexOfC4 = 39;

	while (true)
	{
		for (int i = indexOfC3; i <= indexOfC4; ++i)
		{
			std::string baseNoteName = PianoNote::the88Notes[i];

			PianoChord pianoChord(baseNoteName, PianoChord::ChordType::MajorSixth, 2.0f,
				PianoNote::Loudness::Piano);

			auto chordAndItsInversions = pianoChord.getChordAndItsInversions();

			for (const std::vector<PianoNote>& currentChord : chordAndItsInversions)
			{
				//WaveFile wavefile(currentChord, WaveFile::WaveType::Piano);
				//WaveFile wavefile(currentChord);

				std::string filename = "Arpeggiated" +
					baseNoteName + "Major6invertedOver" + currentChord.at(0).noteName + ".wav";

				//wavefile.writeToWaveFile(filename);
				system(filename.c_str());

				std::cout << "Any key to move on to next chord:\n";
				std::cin.get();
			}

		}
	}
}

void SynthesizerTesting::playSomeMiddleishNotes()
{
	throw MyException("you deleted the Wavefile constructor needed to run this code ... ", __FILE__, __LINE__);
	
	
	PianoNote::initialize();

	for (int i = 45; i < 55; ++i)
	{
		const std::string noteName = PianoNote::the88Notes.at(i);

		PianoNote note(noteName, 2.0f);
		//WaveFile wavefile(note);
		std::string wavefilename = noteName + ".wav";
		//wavefile.writeToWaveFile(wavefilename);
		std::cout << "Playing " << wavefilename << "\n";
		system(wavefilename.c_str());
		std::cout << "Any key to continue\n";
		std::cin.get();

	}
}

std::vector<std::vector<PianoNote>> SynthesizerTesting::getSomeChordsAndMelodicNotes()
{
	PianoNote longNote("A4", 5.0f, PianoNote::Loudness::Forte);

	PianoNote singleNote("C4", 2.0f, PianoNote::Loudness::Piano);
	std::vector<PianoNote> firstChord =
	{
		singleNote,
		PianoNote("E4", 2.0f, PianoNote::Loudness::Piano),
		PianoNote("G4", 2.0f, PianoNote::Loudness::Piano)
	};

	PianoNote restNote("C8", 0.125f, PianoNote::Loudness::Silent); //for now, just ignore the C8 -> note the silence 

	std::vector<std::vector<PianoNote>> notes =
	{
		{longNote },
		{restNote },
		{firstChord}, //first melodic, harmonic note
		{restNote},
		{PianoNote("C#3", 2.0f, PianoNote::Loudness::Piano)}, //second melodic, single note
		{restNote},
		{PianoNote("D3", 1.0f, PianoNote::Loudness::Mezzo) },
		{restNote},
		{PianoNote("D#3", 2.0f, PianoNote::Loudness::Forte)}, //etc.
		{restNote},
		{
			PianoNote("E3", 1.0f, PianoNote::Loudness::Piano),
			PianoNote("G3", 1.0f, PianoNote::Loudness::Piano),
			PianoNote("B3", 1.0f, PianoNote::Loudness::Piano) //third melodic CHORD
		}
	};

	return notes; 
}

std::vector<PianoNote> Utils::generateSomeNotes()
{
	const float duration = 1.0f;
	std::vector<PianoNote> notes =
	{
		PianoNote("C4", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent), //pause 
		PianoNote("E4", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent),
		PianoNote("G4", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent),
		PianoNote("B4", duration, PianoNote::Loudness::Mezzo),

		PianoNote("C5", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent),
		PianoNote("E5", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent),
		PianoNote("G5", duration, PianoNote::Loudness::Mezzo),
		PianoNote("", duration / 2, PianoNote::Loudness::Silent),
		PianoNote("B5", duration, PianoNote::Loudness::Mezzo),
	};

	return notes; 
}

std::vector<PianoNote> Utils::generateC3Major()
{
	const float duration = 2.0f;
	std::vector<PianoNote> notes =
	{
		PianoNote("C3", duration, PianoNote::Loudness::Mezzo),
		PianoNote("E3", duration, PianoNote::Loudness::Mezzo),
		PianoNote("G3", duration, PianoNote::Loudness::Mezzo)
	};


	return notes; 
}

std::vector<PianoNote> Utils::generateF3Maj7()
{
	const float duration = 3.0f;
	std::vector<PianoNote> notes =
	{
		PianoNote("F3", duration, PianoNote::Loudness::Mezzo),
		PianoNote("A3", duration, PianoNote::Loudness::Mezzo),
		PianoNote("C4", duration, PianoNote::Loudness::Mezzo),
		PianoNote("E4", duration, PianoNote::Loudness::Mezzo)
	};

	return notes;
}

std::vector<std::vector<PianoNote>> Utils::generateCMajorScaleChords()
{
	constexpr float duration = 3.0f; 
	PianoNote::Loudness fundamentalAmplitude = PianoNote::Loudness::Forte;

	std::vector<std::vector<PianoNote>> chords(8); 

	PianoChord firstChord("C3", PianoChord::ChordType::Major, duration, fundamentalAmplitude);
	PianoChord secondChord("D3", PianoChord::ChordType::minor, duration, fundamentalAmplitude);
	PianoChord thirdChord("E3", PianoChord::ChordType::minor, duration, fundamentalAmplitude);
	PianoChord fourthChord("F3", PianoChord::ChordType::Major, duration, fundamentalAmplitude);
	PianoChord fifthChord("G3", PianoChord::ChordType::Major, duration, fundamentalAmplitude);
	PianoChord sixthChord("A3", PianoChord::ChordType::minor, duration, fundamentalAmplitude);
	PianoChord seventhChord("B3", PianoChord::ChordType::diminished, duration, fundamentalAmplitude);
	PianoChord eighthChord("C4", PianoChord::ChordType::Major, duration, fundamentalAmplitude);

	chords =
	{
		firstChord.getChord(), secondChord.getChord(), thirdChord.getChord(), fourthChord.getChord(),
		fifthChord.getChord(), sixthChord.getChord(), seventhChord.getChord(), eighthChord.getChord()
	};

	return chords; 
}


void FT::getFTOfC4WithHarmonics()
{
	std::string baseFilename = "C4withHarmonics";
	//wavefile.writeToWaveFile(baseFilename + ".wav");

	FourierTransform ft(baseFilename + ".wav");

	ft.fillTransformDataAndFrequencyMap();

	ft.writeFTMapToCSV(baseFilename + ".csv");

	std::string callPythonPlottingScript = "python plotFourierTransform.py " + baseFilename + ".csv";
	system(callPythonPlottingScript.c_str());
}

void FT::plotMelodicAndHarmonicSoundWaveAndFT()
{
	std::string baseFileName = "MelodicAndHarmonicNotes";

	FourierTransform ft("MelodicAndHarmonicNotes.wav");


	ft.writeSoundWaveToCSV("soundwave_" + baseFileName + ".csv");
	std::ostringstream callPythonSoundWaveScript;
	callPythonSoundWaveScript << "python plotSoundWave.py " << "soundwave_" << baseFileName << ".csv";

	system(callPythonSoundWaveScript.str().c_str());

	ft.fillTransformDataAndFrequencyMap();
	ft.writeFTMapToCSV("transform_" + baseFileName + ".csv");

	std::ostringstream callPythonTransformScript;
	callPythonTransformScript << "python plotSoundWave.py " << "transform_" << baseFileName << ".csv";

	system(callPythonTransformScript.str().c_str());

}
