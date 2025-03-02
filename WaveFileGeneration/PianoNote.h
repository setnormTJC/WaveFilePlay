#pragma once

#include <iostream>
#include<iomanip> 
#include<sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


struct PianoNoteComparator
{
private:
	std::unordered_map<std::string, int> noteOrder;

public: 
	bool operator()(const std::string& firstNote, const std::string& secondNote) const;

	PianoNoteComparator();
};

/*"PianoNote" means A0 - C8 (88 notes, in semitone intervals) are included*/
class PianoNote
{
private: 

	void mapNotesToFrequencies(); 


public:
	enum class Loudness : int16_t //int16_t expected by 16 bit WAV
	{
		Piano = 32'767 / 4,
		Mezzo = 32'767 / 2,
		Forte = 32'767 / 1
	};

	std::string name; //ex: A4, C#3, Gb4, etc.
	float durationInSeconds; 
	Loudness amplitude; 

	/*These will be restricted to sharps (#), for the moment*/
	std::vector<std::string> the88Notes; 
	std::map<std::string, double, PianoNoteComparator> notesToFrequencies;


	/*Allows creation of anonymous objects - as in:
	std::vector<SongNote> amazingGraceNotes =
	{
		{"C3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo}, etc.*/
	PianoNote();

	/*
	* @param duration -> in seconds, example: 3/4 time and tempo = 80 bmp implies quarter note duration = (60.0 sec/min)/ (80 bpm) = 0.75 seconds
	*/
	PianoNote(const std::string& name, const float durationInSeconds, Loudness amplitude);

};

/*Called by NoteComparator and PianoNote - so not a member of either...*/
std::vector<std::string> generateThe88Notes();