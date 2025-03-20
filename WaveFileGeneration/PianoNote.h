#pragma once

#include <iostream>
#include<iomanip>
#include<map>
#include<sstream>
#include <unordered_map>


/*The base (and abstract?) class from which PianoNote, Violin?Note, et. al can inherit*/
class Note
{
	//all notes should have pitches, durations, and amplitudes
};

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
class PianoNote : public Note
{
private: 
	
	static bool initialized; 
	
	/*The duration times samplesPerSecond*/
	int totalNumberOfSamples = 0;


	std::vector<short> soundWaveData; 

	std::map<double, short> frequenciesToAmplitudes;


	/***************Private funcs********************************/

	/*Maps the fundamental and its overtones to their amplitudes
	* DEFINITELY needs further tweeking -> at the moment, I am using something like an exponential decay of amplitudes, but overtones don't look perfect
	*/
	void mapFrequenciesToAmplitudes();

	void fillSoundWaveData(); 

	/*See someWaveAnalysis/plots/soundWaves/ADSR-Type1.png for what my "Type1" looks like
	* The sound wave produced by this envelope is at: someWaveAnalysis/wave/A4withHarmonicsAndType1ADSR.wav
	*/
	void applyType1ADSRtoSoundWave(); 

	/*See someWaveAnalysis/plots and wave folder for what "Type 2" looks and sounds like
	* Seems likely that this is the closest I will get to "piano sound" for a bit
	*/
	void applyType2ADSRtoSoundWave(); 

	/*Prevents CLIPPING (underflowing or overflowing short) AND ensures full use of dynamic range*/
	void normalize(); 

	void writeSoundWaveDataToCSVAndPlot(); 

	void writeDFTToCSVAndPlot(); 


public:

	/*Be wary of the summing of multiple harmonics - can cause popping*/
	enum class Loudness : short //int16_t expected by 16 bit WAV
	{
		Silent		=	 0, //ex: for a rest note
		Pianissimo = 32'767 / 10, 
		Piano =		32'767 / 7,
		Mezzo =		32'767 / 5,
		Forte =		32'767 / 3, 
		Fortissimo = 32'767 / 2,
		Fortississimo = (32'767 / 2) + (32'767 / 3)
	};

	std::string noteName = ""; //ex: A4, C#3, Gb4, etc.
	float durationInSeconds = 0.0f; 
	Loudness fundamentalAmplitude = Loudness::Mezzo; 
	static constexpr int samplesPerSecond = 44'100;



	/*These will be restricted to sharps (#), for the moment...N.B. Use PianoNote::initialize() if not creating an object but want to use static member vars of this class!*/
	static std::vector<std::string> the88Notes;

	static std::map<std::string, double, PianoNoteComparator> notesToFrequencies;
	
	PianoNote();

	/*This overload (likely to be deleted later?) sets default loudness to Mezzo*/
	PianoNote(const std::string& noteName, const float durationInSeconds);

	/*@param duration -> in seconds, example: 3/4 time and tempo = 80 bmp implies quarter note duration = (60.0 sec/min)/ (80 bpm) = 0.75 seconds*/
	PianoNote(const std::string& name, const float durationInSeconds, Loudness fundamentalAmplitude);

	//PianoNote(const std::string& name, const float durationInSeconds, Loudness baseNoteAmplitude, ADSR adsrEnvelop, Overtone overtoneSignature);

	/*This initializes the static member vars - the88Notes and notesToFrequencies
	* N.B. A private boolean flag is part of this class that prevents wasting time running through this function more than once. 
	*/
	static void initialize();

	
	std::vector<short> getSoundWaveData() const;

	int getNumSamples() const; 

	friend class PianoChord; 

};

/*Called by NoteComparator and PianoNote - so not a member of either...*/
std::vector<std::string> generateThe88Notes();

class PianoChord
{
private: 
	std::vector<PianoNote> theChordNotes;

	const float duration = 2.0f;
	PianoNote::Loudness fundamentalAmplitude = PianoNote::Loudness::Mezzo;
	int indexOfBaseNote = 0;  


	int getIndexOfBaseNote(const std::string& baseNoteName);
	
	/*Ex: returns 3 for C3, 6 for A#6, etc.*/
	int getOctaveOfNote(const std::string& noteName);

	/*Ex: returns C for C3, A# for A#6, etc.*/
	std::string getShortNameOfNote(const std::string& note);

	void getMajorChord(const std::string& baseNoteName);
	void getMajor6thChord(const std::string& baseNoteName);
	void getMinorChord(const std::string& baseNoteName);
	void getDiminishedChord(const std::string& baseNoteName);
	void getPowerChord(const std::string& baseNoteName);
	void getOctaveChord(const std::string& baseNoteName);

	std::vector<std::string> getChordNoteNames(); 

public: 

	std::vector<PianoNote> getChord(); 
	std::vector<std::vector<PianoNote>> getChordAndItsInversions(); 

	enum class ChordType
	{
		minor = 1, 
		Major = 2, 
		minorSeventh = 3, 
		MajorSixth = 4,
		diminished = 5, 
		PowerChord = 6, //the "power chord" is root + fifth (ex: C, G)
		Octave = 7 //ex: C5, C6 (fairly common to see this)
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
	explicit PianoChord(const std::string baseNoteName, const ChordType& chordType, const float duration, const PianoNote::Loudness fundamentalAmplitude); 

	/*Custom constructor for "atypical" chords - ex: C3, G3, E5, E6 (chord spanning multiple octaves in bass and treble clefs)
	* @param -> the note names, duration (for all), and fundamental amplitude (again, applied to ALL - for now)
	*/
	explicit PianoChord(const std::vector<std::string> noteNames, const float durationOfAll, const PianoNote::Loudness fundamentalAmplitudeOfAll);
	
	/*For notes that begin at the same time but END at different times*/
	explicit PianoChord(const std::vector<PianoNote>& notesWithDifferingDurations);

	PianoChord(const PianoChord& rhs) = delete; 

	PianoChord& operator = (const PianoChord&& rhs) = delete;
};