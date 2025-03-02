#pragma once
#include <string>
class SongNote
{
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

	/*Allows creation of anonymous objects - as in:
	std::vector<SongNote> amazingGraceNotes =
	{
		{"C3", quarterNoteDurationIn3_4Time,	 SongNote::Loudness::Mezzo}, etc.*/
	SongNote() = default;

	/*
	* @param duration -> in seconds, example: 3/4 time and tempo = 80 bmp implies quarter note duration = (60.0 sec/min)/ (80 bpm) = 0.75 seconds
	*/
	SongNote(const std::string& name, const float durationInSeconds, Loudness amplitude);

};

