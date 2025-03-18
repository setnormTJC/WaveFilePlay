#define _USE_MATH_DEFINES //for M_E and M_PI 

#include"MyException.h"
#include "PianoNote.h"
#include"FourierTransform.h"

#include<algorithm>
#include<fstream> 
#include <random>


#pragma region PianoNote
/*******************************STATIC stuff*********************/
std::vector<std::string> PianoNote::the88Notes; 
std::map<std::string, double, PianoNoteComparator> PianoNote::notesToFrequencies;

bool PianoNote::initialized = false; 



/*Example of why this is needed: std::map<string, float> puts "C8" before "C#1" 
(because '#' ASCII value is 35 and '8' dec value = 56)*/
PianoNoteComparator::PianoNoteComparator()
{
	std::vector<std::string> the88Notes = generateThe88Notes();

	for (int i = 0; i < the88Notes.size(); ++i)
	{
		noteOrder[the88Notes[i]] = i;
	}
}


bool PianoNoteComparator::operator()(const std::string& firstNote, const std::string& secondNote) const
{
	if (noteOrder.find(firstNote) == noteOrder.end() || noteOrder.find(secondNote) == noteOrder.end())
		throw MyException("PianoNoteComparator::operator() could not find one of the two notes",
			__FILE__, __LINE__);

	return noteOrder.at(firstNote) < noteOrder.at(secondNote);
}




PianoNote::PianoNote()
{
	initialize(); 
}

PianoNote::PianoNote(const std::string& noteName, const float durationInSeconds)
	:noteName(noteName), durationInSeconds(durationInSeconds)
{
	initialize(); 

	totalNumberOfSamples = durationInSeconds * samplesPerSecond;

	mapFrequenciesToAmplitudes();

	
	soundWaveData.assign(totalNumberOfSamples, static_cast<short>(0));
	fillSoundWaveData();

}

PianoNote::PianoNote(const std::string& name, const float durationInSeconds, Loudness fundamentalAmplitude)
	:noteName(name), durationInSeconds(durationInSeconds), fundamentalAmplitude(fundamentalAmplitude)
{
	initialize(); 

	totalNumberOfSamples = durationInSeconds * samplesPerSecond;

	mapFrequenciesToAmplitudes();

	soundWaveData.assign(totalNumberOfSamples, static_cast<short>(0));
	fillSoundWaveData();

}

void PianoNote::mapFrequenciesToAmplitudes()
{
	double fundamentalFrequency = notesToFrequencies.at(noteName); 

	//constexpr int numberOfOvertonesDesired = 5; //a previous setting
	constexpr int numberOfOvertonesDesired = 8; 

	//calculate the first `numberOfOvertonesDesired` frequencies (multiples of the fundamental): 
	std::vector<double> frequencies;
	for (int i = 2; i <= numberOfOvertonesDesired + 1; ++i)
		frequencies.push_back(fundamentalFrequency * i);

	//now the amplitudes ROUGHLY decay exponentially - according to FT analysis of digital piano recording. 
	std::vector<short> amplitudes;
	for (double i = 2; i <= numberOfOvertonesDesired + 1; ++i)
		//A = Afundamental*e^-overtoneNumber -> MIGHT not be the "best" mimic for piano overtones 
		amplitudes.push_back(static_cast<short>((short)fundamentalAmplitude * pow((1.0/M_E), i - 1)));

	/*first insert the fundamental and its amplitude*/
	frequenciesToAmplitudes.insert({ fundamentalFrequency, static_cast<short>(fundamentalAmplitude) });
	/*Then insert the overtones*/
	for (int i = 0; i < frequencies.size(); ++i)
		frequenciesToAmplitudes.insert({ frequencies[i], amplitudes[i] });

}



void PianoNote::applyType1ADSRtoSoundWave()
{
	int totalNumberOfSamples = soundWaveData.size(); 
	
	/*Tweek as needed*/
	//constexpr float attackTimeInSeconds = 0.1f;
	float attackTimeInSeconds = durationInSeconds * 0.01;

	int numberOfSamplesInAttackPhase = attackTimeInSeconds * samplesPerSecond; //should be 4'440 samples in
	int numberOfSamplesToReachDecayPhase = numberOfSamplesInAttackPhase + (totalNumberOfSamples * 0.45);
	int numberOfSamplesToReachReleasePhase = totalNumberOfSamples - (totalNumberOfSamples * 0.1);

	short peakLevel = *std::max_element(soundWaveData.begin(), soundWaveData.end());
	short sustainLevel = static_cast<short>(peakLevel * 0.5);

	for (int timePoint = 0; timePoint < numberOfSamplesInAttackPhase; ++timePoint)
	{
		float attackProgress = static_cast<float>(timePoint) / numberOfSamplesInAttackPhase;
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * attackProgress); 
	}

	for (int timePoint = numberOfSamplesInAttackPhase; timePoint < numberOfSamplesToReachDecayPhase; ++timePoint)
	{
		float decayProgress = static_cast<float>(timePoint - numberOfSamplesInAttackPhase) /
			(numberOfSamplesToReachDecayPhase - numberOfSamplesInAttackPhase);

		short targetAmplitude = peakLevel - static_cast<short>(decayProgress * (peakLevel - sustainLevel));
		
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * (static_cast<float>(targetAmplitude) / peakLevel));
	}

	for (int timePoint = numberOfSamplesToReachDecayPhase; timePoint < numberOfSamplesToReachReleasePhase; ++timePoint)
	{
		//soundWaveData[timePoint] = sustainLevel; //NO!
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * (static_cast<float>(sustainLevel) / peakLevel));
	}

	//"smoothing" period? 
	soundWaveData[numberOfSamplesToReachReleasePhase] = soundWaveData[numberOfSamplesToReachReleasePhase - 1];

	for (int timePoint = numberOfSamplesToReachReleasePhase + 1; timePoint < totalNumberOfSamples; ++timePoint)
	{
		// The release phase starts right where the sustain phase ends
		float releaseProgress = static_cast<float>(timePoint - numberOfSamplesToReachReleasePhase) /
			(totalNumberOfSamples - numberOfSamplesToReachReleasePhase);

		// Ensure smooth decay by using sustain level as the baseline for release
		short targetAmplitude = sustainLevel * pow(1.0f - releaseProgress, 2); //2 means quadratic decay 
		//std::cout << targetAmplitude << "\n"; 
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * (static_cast<float>(targetAmplitude) / peakLevel));
	}
}

void PianoNote::applyType2ADSRtoSoundWave()
{
	int totalNumberOfSamples = soundWaveData.size();

	float attackTimeInSeconds = durationInSeconds * 0.01;

	int numberOfSamplesInAttackPhase = attackTimeInSeconds * samplesPerSecond; //should be 4'440 samples in

	short peakLevel = *std::max_element(soundWaveData.begin(), soundWaveData.end());

	for (int timePoint = 0; timePoint < numberOfSamplesInAttackPhase; ++timePoint)
	{
		float attackProgress = static_cast<float>(timePoint) / numberOfSamplesInAttackPhase;
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * attackProgress);
	}

	for (int timePoint = numberOfSamplesInAttackPhase; timePoint < totalNumberOfSamples; ++timePoint)
	{
		float releaseProgress = static_cast<float>(timePoint - numberOfSamplesInAttackPhase) /
			(totalNumberOfSamples - numberOfSamplesInAttackPhase);

		// Ensure smooth decay by using sustain level as the baseline for release
		short targetAmplitude = peakLevel * pow(1.0f - releaseProgress, 2); //2 means quadratic decay 
		//std::cout << targetAmplitude << "\n"; 
		soundWaveData[timePoint] = static_cast<short>(soundWaveData[timePoint] * (static_cast<float>(targetAmplitude) / peakLevel));
	}

}

void PianoNote::fillSoundWaveData()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	double noiseScalingFactor = 0.01; //0.1 is a BIT fuzzy, 1.0 is "quite" fuzzy 
	std::uniform_real_distribution<double> phaseDistribution(0, noiseScalingFactor);
	//re: this phase distribution -> summing pure sine waves can lead to UNEXPECTED cancellation 
	//...destructive intereference that destroys certain overtones!

	for (int timePoint = 0; timePoint < totalNumberOfSamples; ++timePoint)
	{
		//int overtoneCount = 0; 
		for (const auto& [frequency, amplitude] : frequenciesToAmplitudes)
		{
			double phaseOffset = phaseDistribution(gen); //again, this is to prevent unintended harmonic cancellation

			double sineArgument = (frequency * timePoint / samplesPerSecond) + phaseOffset;
			double theSinePortion = sin(2 * M_PI * sineArgument);
			//making these temp vars for my analysis (stepping through in Debug mode) 
			
			soundWaveData[timePoint] = std::clamp(
				static_cast<short>(soundWaveData[timePoint] +
				amplitude * theSinePortion), 
				std::numeric_limits<short>::min(), std::numeric_limits<short>::max()); 
		}
	}

	normalize();

	/*NOw apply ADSR - intentionally using separate loops for ease of comprehension (not better efficiency)*/
	//applyType1ADSRtoSoundWave(); //MIGHT? apply better to low notes (that persist for longer) 
	applyType2ADSRtoSoundWave(); 
	//a "percussive" envelope -> as seen in: https://www.muzines.co.uk/articles/back-to-basics/1882
	//might be better for high notes (that do not long endure)

	/*Uncomment for visualization*/
	//writeSoundWaveDataToCSVAndPlot(); 
	//writeDFTToCSVAndPlot(); 
}

void PianoNote::normalize()
{
	short maxAmplitude = *std::max_element(soundWaveData.begin(), soundWaveData.end());
	if (maxAmplitude > 0) {
		double scalingFactor = static_cast<double>(fundamentalAmplitude) / maxAmplitude;
		for (auto& sample : soundWaveData) {
			sample = static_cast<short>(sample * scalingFactor);
		}
	}
}

void PianoNote::writeSoundWaveDataToCSVAndPlot()
{
	const std::string csvFilename = "soundwaveData.csv";

	std::ofstream fout(csvFilename);

	int timePoint = 0;
	for (const short amplitudeAtTimePoint : soundWaveData)
	{
		fout << timePoint << "," << amplitudeAtTimePoint << "\n";
		timePoint++;
	}
	fout.close();

	const std::string callToPythonScript = "python plotSoundWave.py " + csvFilename; //note the space ... 
	system(callToPythonScript.c_str());
}

void PianoNote::writeDFTToCSVAndPlot()
{
	const std::string csvFilename = "FourierTransform.csv";

	/*get the transform first:*/
	FourierTransform fourierTransform(soundWaveData);
	fourierTransform.fillTransformDataAndFrequencyMap();
	fourierTransform.writeFTMapToCSV(csvFilename);

	const std::string callToPythonScript = "python plotFourierTransform.py " + csvFilename; //note the space ... 
	system(callToPythonScript.c_str());

}

std::vector<short> PianoNote::getSoundWaveData() const
{
	return soundWaveData;
}

void PianoNote::initialize()
{
	/*Using: https://en.wikipedia.org/wiki/Piano_key_frequencies*/

	//std::cout << "NOTE from PianoNote::initialize: \nsamplesPerSecond is currently set to: \n"
	//	<< samplesPerSecond << "\n";

	if (initialized) return; // Ensure this runs only once
	initialized = true;

	the88Notes = generateThe88Notes();

	const double twelfthRootOf2 = pow(2, (1.0 / 12));
	constexpr double A4Frequency = 440.0;

	for (int n = 0; n < the88Notes.size(); ++n)
	{
		notesToFrequencies[the88Notes[n]] = A4Frequency * pow(twelfthRootOf2, (n - 48));
	}
}




std::vector<std::string> generateThe88Notes()
{
	std::vector<std::string> the88Notes; 


	/*Used as the skeleton for generating all 88 notes*/
	std::vector<std::string> theChromaticScale =
	{
		"C",
		"C#",
		"D",
		"D#",
		"E",
		"F",
		"F#",
		"G",
		"G#",
		"A",
		"A#",
		"B"
	};


	//then the fill pattern SHOULD be straightforward with a loop: 
	for (int octave = 1; octave <= 7; ++octave)
	{
		for (const std::string& basicNoteName : theChromaticScale)
		{
			std::ostringstream fullNoteName;
			fullNoteName << basicNoteName << octave;

			the88Notes.push_back(fullNoteName.str());
		}
	}

	//now, the "strangely-numbered" ones at the front: 
	the88Notes.insert(the88Notes.begin(), { "A0", "A#0", "B0" });

	//and the final C: 
	the88Notes.insert(the88Notes.end(), "C8");

	return the88Notes; 
}
#pragma endregion

#pragma region PianoChord
void PianoChord::getMajorChord(const std::string& baseNoteName)
{

	//first note: 
	PianoNote baseNote(baseNoteName, duration, loudness);

	/********Second note stuff************************/
	int indexOfFirstNote = getIndexOfBaseNote(baseNoteName);
	int indexOfSecondNote = indexOfFirstNote + (int)Interval::MajorThird; 

	//add out of bounds check later, if desired 
	std::string secondNoteName = PianoNote::the88Notes.at(indexOfSecondNote);

	PianoNote secondNote(secondNoteName, duration, loudness); 

	/********Third note stuff************************/
	int indexOfThirdNote = indexOfFirstNote + (int)Interval::PerfectFifth; 
	
	PianoNote thirdNote(PianoNote::the88Notes.at(indexOfThirdNote), duration, loudness); 

	theChordNotes = { baseNote, secondNote, thirdNote };
}

void PianoChord::getMajor6thChord(const std::string& baseNoteName)
{
	getMajorChord(baseNoteName); 

	int indexOfBaseNote = getIndexOfBaseNote(baseNoteName);
	int semitoneDistanceToFourthNote =  (int)Interval::MajorSixth;
	int indexOfFourthNote = indexOfBaseNote + semitoneDistanceToFourthNote;

	theChordNotes.push_back(PianoNote(PianoNote::the88Notes.at(indexOfFourthNote), duration, loudness));

}

std::vector<std::string> PianoChord::getChordNoteNames()
{
	if (theChordNotes.size() == 0)
		throw MyException("cannot get note names if chord notes is empty", __FILE__, __LINE__);

	std::vector<std::string> chordNoteNames; 

	for (const auto& note: theChordNotes)
	{
		chordNoteNames.push_back(note.noteName);
	}

	return chordNoteNames;
}

std::vector<std::vector<PianoNote>> PianoChord::getChordAndItsInversions()
{
	if (theChordNotes.size() == 0)
		throw MyException("cannot invert nuthin'! (an empty chord)", __FILE__, __LINE__);

	std::vector<std::vector<PianoNote>> chordAndItsInversions;
	chordAndItsInversions.push_back(theChordNotes); //first get the starting chord 

	std::vector<PianoNote> currentInvertedChord = theChordNotes;

	/*1st inversion up through the N - 1th inversion (where N is the number of notes in chord)*/
	for (int inversionNumber = 1; inversionNumber < theChordNotes.size(); ++inversionNumber)
	{
		std::string nameOfCurrentFirstNote = currentInvertedChord.at(0).noteName;
		int octaveOfFirstNote = getOctaveOfNote(nameOfCurrentFirstNote);

		for (int i = 1; i < currentInvertedChord.size(); ++i)
		{
			currentInvertedChord.at(i - 1).noteName = currentInvertedChord.at(i).noteName;
			//no change to duration or loudness ... 
		}

		//need to set last note -> ex: if original is C3E3G3A3, last note should become C4 
		std::string newLastNote =
			getShortNameOfNote(nameOfCurrentFirstNote) + std::to_string(octaveOfFirstNote + 1);

		currentInvertedChord.at(currentInvertedChord.size() - 1).noteName = newLastNote; 

		chordAndItsInversions.push_back(currentInvertedChord);
	}

	return chordAndItsInversions; 
}



int PianoChord::getIndexOfBaseNote(const std::string& baseNoteName)
{
	auto iteratorToBaseNote =
		std::find(PianoNote::the88Notes.begin(), PianoNote::the88Notes.end(), baseNoteName);

	return static_cast<int>(std::distance(PianoNote::the88Notes.begin(), iteratorToBaseNote));
}

int PianoChord::getOctaveOfNote(const std::string& noteName)
{
	if (noteName.length() == 2) //ex: C3
	{
		return noteName.at(1) - '0'; //be wary of ASCII-tomfoolery  
	}

	else if (noteName.length() == 3)
	{
		return noteName.at(2) - '0';
	}

	else
	{
		throw MyException("note name length is not 2 or 3??", __FILE__, __LINE__);
	}
}

std::string PianoChord::getShortNameOfNote(const std::string& noteName)
{
	if (noteName.length() == 2)
	{
		return noteName.substr(0, 1); 
	}
	else if (noteName.length() == 3)
	{
		return noteName.substr(0, 2);
	}
	else
	{
		throw MyException("note name length is not 2 or 3??", __FILE__, __LINE__); 
	}
}

PianoChord::PianoChord(const std::string baseNoteName, const ChordType& chordType)
{
	//make sure 88 notes are initialized: 
	PianoNote::initialize(); 

	//make sure baseNote is allowed 
	if (std::find(PianoNote::the88Notes.begin(), PianoNote::the88Notes.end(), baseNoteName)
		== PianoNote::the88Notes.end())
	{
		throw MyException("base note - YOU'RE not in the list!", __FILE__, __LINE__);
	}

	if (chordType == ChordType::Major)
	{
		getMajorChord(baseNoteName); 
	}

	else if (chordType == ChordType::minor)
	{
		//do it!
	}

	else if (chordType == ChordType::majorSixth)
	{
		getMajor6thChord(baseNoteName); 
	}

	else if (chordType == ChordType::minorSeventh)
	{

	}
	//etc. 
}

#pragma endregion