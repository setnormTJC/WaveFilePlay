#include "DemoingWaveFile.h"

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
	PianoNote::initialize(); //fills the88Notes and notesToFrequencies

	std::vector<PianoNote> notes;

	const float duration = 0.5f; //seconds 

	for (const std::string& noteName : PianoNote::the88Notes)
	{
		PianoNote currentNote(noteName, duration, PianoNote::Loudness::Piano);
		notes.push_back(currentNote); 
	}

	WaveFile waveFile(notes, WaveFile::WaveType::Sine);

	waveFile.writeToWaveFile("all88PianoNotes_withHalfSecondDuration.wav");
}



void SimpleTesting::demoAmazingGrace()
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
	aWaveFile.writeToWaveFile("amazingGrace.wav");

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

void SimpleTesting::demoSimpleWaveFileReading()
{
	std::string inputFilename = "First wave!.wav";
	WaveFile inputWaveFile(inputFilename);

	std::string csvFilename = "firstWave.csv";
	inputWaveFile.writeSoundDataToCSV(csvFilename);

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

	outWave.writeSoundDataToCSV(filename.str() + ".csv");
	outWave.writeToWaveFile(filename.str() + ".wav");
}

void SimpleTesting::demoCMajorChord()
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

void SimpleTesting::demoSeventhChord()
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
#pragma endregion


#pragma region FT
void FourierTransform::transform1SecondOf100Hz()
{

	WaveFile waveFile("F = 100_N = 44100_A = 10000.wav"); //the guinea pig

	std::vector<short> soundWave = waveFile.getSoundWave();

	int minFrequency = 20; //around the lowest frequency audible to humans: 
	int maxFrequency = 20'020; //around the highest (added in the 20 for nice "round" number)

	//anticipate transform having 20K amplitudes - one for each Hz value
	std::vector<double> realPartOfTransform(maxFrequency - minFrequency, 0.0); //the second arg inits all elements to 0.0
	std::vector<double> imagPartOfTransform(maxFrequency - minFrequency, 0.0);

	//recall: e^(ix) = cos(x) + i*sin(x)
	//FT involves negative sign in exponent - is it true that: 
	//e^(-ix) = cos(x) - i*sin(x)? 

	const int N = soundWave.size();

	for (int k = 0; k < realPartOfTransform.size(); ++k) //k is each frequency value in the transform 
	{
		for (int n = 0; n < N - 1; ++n) //n is each time point in sound wave 
		{
			double angle = 2 * M_PI * k * n / N;

			realPartOfTransform[k] += soundWave[n] * cos(angle);
			imagPartOfTransform[k] -= soundWave[n] * sin(angle);
		}
	}

	std::vector<double> magnitude(maxFrequency - minFrequency, 0.0);

	for (int k = 0; k < magnitude.size(); ++k)
	{
		magnitude[k] = sqrt(realPartOfTransform[k] * realPartOfTransform[k] +
			imagPartOfTransform[k] * imagPartOfTransform[k]);
	}


	for (int k = 80; k <= 120; ++k)
	{
		std::cout << "Frequency " << k << " Hz: " << magnitude[k] << std::endl;
	}

	//10^8 = ? (amplitude * 44,100 total samples in 1 second audio /2) = AN/2 
	//			==  10'000 (22'050)
	//			==~ 2 * 10 ^8 
}

std::vector<double> FourierTransform::getTransformOfWaveFile(const std::string& filename)
{
	WaveFile waveFile(filename); 

	std::vector<short> soundWave = waveFile.getSoundWave();

	int minFrequency = 20; //around the lowest frequency audible to humans: 
	int maxFrequency = 5'020; //A "little" higher than C8 -> probably an okay cutoff 

	std::cout << "Generating FT from minFrequency = " << minFrequency << " hz to maxFrequency = "
		<< maxFrequency << " hz.\n";

	std::vector<double> realPartOfTransform(maxFrequency - minFrequency, 0.0); //the second arg inits all elements to 0.0
	std::vector<double> imagPartOfTransform(maxFrequency - minFrequency, 0.0);

	const int N = soundWave.size();

	for (int k = 0; k < realPartOfTransform.size(); ++k) //k is each frequency value in the transform 
	{
		for (int n = 0; n < N - 1; ++n) //n is each time point in sound wave 
		{
			double angle = 2 * M_PI * k * n / N;

			realPartOfTransform[k] += soundWave[n] * cos(angle);
			imagPartOfTransform[k] -= soundWave[n] * sin(angle);
		}
	}

	std::vector<double> transform(maxFrequency - minFrequency, 0.0);

	for (int k = 0; k < transform.size(); ++k)
	{
		transform[k] = sqrt(realPartOfTransform[k] * realPartOfTransform[k] +
			imagPartOfTransform[k] * imagPartOfTransform[k]);
	}

	//write out to csv file for visualization: 
	
	//std::string outCSVfilename = filename.substr(0, filename.find(".wav")) + ".csv";

	//std::ofstream fout(outCSVfilename); 
	//for (int k = 0; k < transform.size(); ++k)
	//{
	//	fout << transform[k] << "\n";
	//}
	//fout.close(); 

	return transform;
}



void FourierTransform::Testing::getFTOfSinglePureSineWave()
{
	auto transformOfWaveFile = getTransformOfWaveFile("F = 100_N = 44100_A = 10000.wav");

	PlotImage plotImage(1'000, 1'000, Color(ColorEnum::Black));
	//plotImage.plotData

}

#pragma endregion 



#pragma region MusicMaking namespace 

void MusicMaking::chatGPTTriesLaFilleAuxCheveuxDeLin()
{
	//ChatGPT attempting first two measures of La Fille Aux Cheveux de Lin given the link: 
// https://musescore.com/r_d/scores/5449385:
	float quarterNoteDuration = 60.0 / 110.0; // Approximately 0.545 seconds
	float eighthNoteDuration = quarterNoteDuration / 2.0; // Approximately 0.273 seconds

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

	WaveFile waveFile(laFilleMelody, WaveFile::WaveType::Sine);
	waveFile.writeToWaveFile("LaFilleAuxCheveuxDeLin.wav"); //NOPE!
}

void MusicMaking::playMysterySong()
{
	//from: https://musescore.com/user/16242231/scores/3424496

	float tempo = 96.0f; //beats per minute
	float secondsPerMinute = 60.0f;
	float quarterNoteDuration = secondsPerMinute / tempo;
	float eighthNoteDuration = quarterNoteDuration / 2.0;

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

	WaveFile waveFile(mysteryMelody, WaveFile::WaveType::Sine);

	const char* waveFilename = "mysteryMelody.wav";
	waveFile.writeToWaveFile(waveFilename);

	system(waveFilename);

}


#pragma endregion 

void ADSRTesting::playC4()
{
	WaveFile waveFile(PianoNote("C4", 2.0, PianoNote::Loudness::Forte), WaveFile::WaveType::FancyInstrument);


	const char* waveFilename = "C4_withExponentialDecayEnvelope.wav";

	waveFile.writeToWaveFile(waveFilename);

	std::string plotImageFilename = "plotImage.bmp";

	system(waveFilename);

	//waveFile.writeSoundDataToImagePlot(plotImageFilename);

	//system(plotImageFilename.c_str());

	//system(filename); 
}


