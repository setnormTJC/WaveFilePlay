#pragma once

#include <iostream>
#include<iomanip>
#include<map>
#include<sstream>
#include <unordered_map>


struct PianoNoteComparator
{
private:
	std::unordered_map<std::string, int> noteOrder;

public: 
	bool operator()(const std::string& firstNote, const std::string& secondNote) const;

	PianoNoteComparator();
};

/*"PianoNote" means A0 - C8 (88 notes, in semitone intervals) are included
* N.B. Use PianoNote::initialize() if not creating an object but want to use static member vars of this class!
*/
class PianoNote
{
private: 
	
	static void mapNotesToFrequencies(); 

	static bool initialized; 

public:

	/*Be wary of the summing of multiple harmonics - can cause popping*/
	enum class Loudness : int16_t //int16_t expected by 16 bit WAV
	{
		Silent		=	 0, //ex: for a rest note
		Pianissimo = 32'767 / 10, 
		Piano =		32'767 / 7,
		Mezzo =		32'767 / 5,
		Forte =		32'767 / 3, 
		Fortissimo = 32'767 / 1
	};

	std::string name; //ex: A4, C#3, Gb4, etc.
	float durationInSeconds; 
	Loudness amplitude; 


	/*These will be restricted to sharps (#), for the moment...N.B. Use PianoNote::initialize() if not creating an object but want to use static member vars of this class!*/
	static std::vector<std::string> the88Notes;

	static std::map<std::string, double, PianoNoteComparator> notesToFrequencies;

	/*Allows creation of anonymous objects - as in:std::vector<SongNote> amazingGraceNotes =
	{
		{"C3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo}, etc.*/
	PianoNote();

	/*@param duration -> in seconds, example: 3/4 time and tempo = 80 bmp implies quarter note duration = (60.0 sec/min)/ (80 bpm) = 0.75 seconds*/
	PianoNote(const std::string& name, const float durationInSeconds, Loudness amplitude);

	/*This initializes the static member vars - the88Notes and notesToFrequencies
	* N.B. A private boolean flag is part of this class that prevents wasting time running through this function more than once. 
	*/
	static void initialize();

	/*Needed if using std::find on a vector of PianoNotes - this method just compares the note names on the left and right (hand sides)*/
	//bool operator == (const PianoNote& rhs);

	friend class PianoChord; 

};

/*Called by NoteComparator and PianoNote - so not a member of either...*/
std::vector<std::string> generateThe88Notes();

class PianoChord
{
private: 
	int getIndexOfBaseNote(const std::string& baseNoteName);
	
	/*Ex: returns 3 for C3, 6 for A#6, etc.*/
	int getOctaveOfNote(const std::string& noteName);

	/*Ex: returns C for C3, A# for A#6, etc.*/
	std::string getShortNameOfNote(const std::string& note);

	//std::string getNextNoteNameInChordInversion(const std::string& nextToLastNoteName, const std::string& lastNoteNameWithoutOctave);

	void getMajorChord(const std::string& baseNoteName);
	void getMajor6thChord(const std::string& baseNoteName);

	std::vector<std::string> getChordNoteNames(); 
public: 
	const float duration = 2.0f;
	PianoNote::Loudness loudness = PianoNote::Loudness::Mezzo;


	std::vector<PianoNote> theChordNotes;

	std::vector<std::vector<PianoNote>> getChordAndItsInversions(); 

	enum class ChordType
	{
		minor = 1, 
		Major = 2, 
		minorSeventh = 3, 
		majorSixth = 4
		//etc. as desired 
	};

	/*Intervals in semitone steps - ex: MinorSecond = 1 semitone*/
	enum class Interval
	{
		MinorSecond = 1, 
		MajorSecond = 2,
		MinorThird = 3,
		MajorThird = 4,
		PerfectFourth = 5,
		Tritone = 6,   // Augmented Fourth or Diminished Fifth
		PerfectFifth = 7,
		MinorSixth = 8,
		MajorSixth = 9,
		MinorSeventh = 10,
		MajorSeventh = 11,
		PerfectOctave = 12
	};
	PianoChord() = delete; 
	/*
	* @param chordType -> ex: minor, major, minorSeventh, majorSixth,etc
	*/
	PianoChord(const std::string baseNoteName, const ChordType& chordType); 
};