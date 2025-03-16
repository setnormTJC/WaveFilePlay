#include "PianoNote.h"

#include"MyException.h"

#include<algorithm>

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
	return noteOrder.at(firstNote) < noteOrder.at(secondNote);
}


/*Using: https://en.wikipedia.org/wiki/Piano_key_frequencies*/
void PianoNote::mapNotesToFrequencies()
{
	//Be wary! floats here give C8 frequency around 3950 Hz (should be 4186)
	double twelfthRootOf2 = (float)pow(2, (1.0 / 12));
	/*A (the?) base tuning pitch (Hertz)*/
	double A4Frequency = 440.0; 

	for (int n = 0; n < the88Notes.size(); ++n)
	{
		notesToFrequencies[the88Notes[n]] = (float)(A4Frequency * pow(twelfthRootOf2, (n - 48)));
	}

	for (const auto& [note, frequency] : notesToFrequencies)
	{
		std::cout << std::left << std::setw(5) << note;

		std::cout << std::fixed << std::setprecision(1); //display frequencies to nearest 0.1 Hz (may change this later) 
		std::cout << std::left << std::setw(7) << frequency;
		std::cout << "\n";
	}

}

PianoNote::PianoNote()
{
	//the88Notes = generateThe88Notes(); 
	//mapNotesToFrequencies(); 

	initialize(); 
}

PianoNote::PianoNote(const std::string& name, const float durationInSeconds, Loudness amplitude)
	:name(name), durationInSeconds(durationInSeconds), amplitude(amplitude)
{
	//the88Notes = generateThe88Notes();
	//mapNotesToFrequencies();

	initialize(); 
}

void PianoNote::initialize()
{
	if (initialized) return; // Ensure this runs only once
	initialized = true;

	the88Notes = generateThe88Notes();

	double twelfthRootOf2 = pow(2, (1.0 / 12));
	double A4Frequency = 440.0;

	for (int n = 0; n < the88Notes.size(); ++n)
	{
		notesToFrequencies[the88Notes[n]] = A4Frequency * pow(twelfthRootOf2, (n - 48));
	}
}

//bool PianoNote::operator==(const PianoNote& rhs)
//{
//	return this->name == rhs.name; 
//}

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

	//for (const std::string& note : the88Notes)
	//{
	//	std::cout << note << "\n";
	//}

	return the88Notes; 
}

//std::string PianoChord::getNextNoteNameInChordInversion(const std::string& nextToLastNoteName, const std::string& lastNoteNameWithoutOctave)
//{
//	auto iteratorToNextToLast = std::find()
//}

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
		chordNoteNames.push_back(note.name);
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
		std::string nameOfCurrentFirstNote = currentInvertedChord.at(0).name;
		int octaveOfFirstNote = getOctaveOfNote(nameOfCurrentFirstNote);

		for (int i = 1; i < currentInvertedChord.size(); ++i)
		{
			currentInvertedChord.at(i - 1).name = currentInvertedChord.at(i).name;
			//no change to duration or loudness ... 
		}

		//need to set last note -> ex: if original is C3E3G3A3, last note should become C4 
		std::string newLastNote =
			getShortNameOfNote(nameOfCurrentFirstNote) + std::to_string(octaveOfFirstNote + 1);

		currentInvertedChord.at(currentInvertedChord.size() - 1).name = newLastNote; 

		chordAndItsInversions.push_back(currentInvertedChord);
	}

	return chordAndItsInversions; 
}



int PianoChord::getIndexOfBaseNote(const std::string& baseNoteName)
{
	auto iteratorToBaseNote =
		std::find(PianoNote::the88Notes.begin(), PianoNote::the88Notes.end(), baseNoteName);

	return (std::distance(PianoNote::the88Notes.begin(), iteratorToBaseNote));
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
