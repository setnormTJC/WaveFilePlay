#include "PianoNote.h"


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


