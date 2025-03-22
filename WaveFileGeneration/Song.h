#pragma once

#include"MyException.h"
#include"PianoNote.h"

class Song
{
public:
	enum class TimeSignature
	{
		Common, //4/4 -> second 4 means counting quarter notes, first 4 means 4 quarter notes per measure
		Triple, //3/4 -> 3 quarter notes make up one measure 
		Duple, //2/4 -> 2 quarter notes " " "
		Cut, //2/2 -> 2 HALF notes per measure 
		Compound6over8 //six EIGHTH notes per measure 
	};

	enum class NoteType
	{
		sixteenth,
		dottedSixteenth,
		eighth,
		dottedEighth,
		quarter, 
		dottedQuarter,
		half,
		dottedHalf,
		whole, 
	};

	enum Track
	{
		Vocal,
		Treble, 
		Bass
	};

	explicit Song(const std::string& songName, const int tempoBPM, const TimeSignature timeSignature, const int numberOfMeasures, const int numberOfTracks);

	Song(const Song& rhs) = delete; 
	Song& operator = (const Song&& rhs) = delete; 

	/* @returns the duration (in seconds) of the note*/
	float duration(const NoteType noteType) const;


	std::vector<std::vector<std::vector<PianoNote>>> getTracks(); 

	void addToTrack(const Song::Track track, const std::vector<PianoNote>& dataToAdd);

	std::vector<float> getTrackDurations(); 
	/*ex: track 0 is vocals, track 1 is treble, etc.*/
	std::vector<std::string> getTrackNames(); 

	/*For funsies*/
	std::pair<float, float> findMinAndMaxFrequenciesInSong(); 

private: 
	std::string songName; 
	int tempoBPM; 
	TimeSignature timeSignature; 
	/*How to put this to good use?*/
	int numberOfMeasures; 

	/*Maps "common" notes to their durations - Ex: "quarter" note at 60 bpm tempo gets mapped to 1.0f seconds*/
	std::map<NoteType, float> noteTypesToDurations; 

	/*Ex: treble, bass, vocal, guitar, drum, etc., etc.*/
	std::vector<std::vector<std::vector<PianoNote>>> tracks; 

	std::map<int, std::string> trackNumbersToNames;

	/*Only called by the constructor*/
	void mapNoteTypesToDurations(const int tempoBPM, const TimeSignature timeSignature);

};

