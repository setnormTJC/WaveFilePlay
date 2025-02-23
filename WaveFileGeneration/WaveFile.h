//written using http://soundfile.sapp.org/doc/WaveFormat/
#pragma once


#include"SongNote.h"

#include<cmath> 
#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

//N.B. ONLY compares notes in the range C3 - C5
struct NoteComparator
{
	bool operator()(const std::string& firstNote, const std::string& secondNote) const
	{
		static const std::unordered_map<std::string, int> noteOrder =
		{
			{"C3", 0}, {"C#3", 1}, {"D3", 2}, {"D#3", 3}, {"E3", 4}, {"F3", 5},
			{"F#3", 6}, {"G3", 7}, {"G#3", 8}, {"A3", 9}, {"A#3", 10}, {"B3", 11},
			{"C4", 12}, {"C#4", 13}, {"D4", 14}, {"D#4", 15}, {"E4", 16}, {"F4", 17},
			{"F#4", 18}, {"G4", 19}, {"G#4", 20}, {"A4", 21}, {"A#4", 22}, {"B4", 23},
			{"C5", 24}
		};

		return noteOrder.at(firstNote) < noteOrder.at(secondNote); 
	}
};


class RiffHeader
{
	const char ChunkID[4] = { 'R', 'I', 'F', 'F' };
	int ChunkSize; //this requires info from `SoundData` below!
	const char Format[4] = { 'W', 'A', 'V', 'E' }; 

	friend class WaveFile; 

};

class FormatHeader
{
	const char Subchunk1ID[4] = { 'f', 'm', 't', ' ' }; //'f'm't' ' -> NOTE the space!
	int Subchunk1Size = 0x00'00'00'10; //NOTE the endianness - 16 (base 10) means PCM (pulse-code modulation) 
	short AudioFormat = 0x00'01; //1 means PCM ... using short because AudioFormat field has 2 Byte size
	short NumChannels = 0x00'01; //1 means MONO - stick to that for now ... 
	int SampleRate = 0x00'00'AC'44; //= AC'44  ~= 44,100 (Hz) in base 10;
	
	int ByteRate; //gets calculated in DEFAULT constructor of this class (because a member var. below is needed) 
	short BlockAlign; //also calculated in DEFAULT constructor 
	short BitsPerSample = 0x00'10; //16 bits is typical for sound quality ... 

	FormatHeader(); 

	friend class WaveFile;
};

class SoundData
{
	const char Subchunk2ID[4] = { 'd', 'a', 't', 'a' }; //'d'a't'a 
	int Subchunk2Size; //calculated in default constructor (because it requires data from below)
	
	//NOTE! SIXTEEN-BIT int used for data! Int is required by PCM, 16 is determined by BitsPerSample field 
	std::vector<int16_t> data; 

	friend class RiffHeader; 
	friend class WaveFile;
};

class WaveFile
{
	RiffHeader theRiffHeader; 
	FormatHeader theFormatHeader; 
	SoundData theSoundData;

	//const int amplitude = 32'767; //pros and cons of making this a member variable - obviously constructing waves with different loudness is good 

	/*The notes and their frequencies in Hz from C3 to C5*/
	std::map<std::string, float, NoteComparator> notesToFrequencies = 
	{
		{"C3", 130.81f}, {"C#3", 138.59f}, {"D3", 146.83f}, {"D#3", 155.56f}, {"E3", 164.81f}, {"F3", 174.61f},
		{"F#3", 185.00f}, {"G3", 196.00f}, {"G#3", 207.65f}, {"A3", 220.00f}, {"A#3", 233.08f}, {"B3", 246.94f},
		{"C4", 261.63f}, {"C#4", 277.18f}, {"D4", 293.66f}, {"D#4", 311.13f}, {"E4", 329.63f}, {"F4", 349.23f},
		{"F#4", 369.99f}, {"G4", 392.00f}, {"G#4", 415.30f}, {"A4", 440.00f}, {"A#4", 466.16f}, {"B4", 493.88f},
		{"C5", 523.25f}
	};

	void fillDataWithSineWave(const int NumSamples, const int amplitude, const float frequency);
	void fillDataWithSquareWave(const int NumSamples, const int amplitude, const float frequency);

public: 
	/*Default constructor sets NumSamples to 44'100, frequency = 440.0 Hz, and amplitude 1.0*/
	enum class WaveType
	{
		Sine,
		Triangular,
		Square,
		Saw,
		FancyInstrument // more ambitious -> model the Attack, Sustain, Release, Decay envelope curve thing
	};

	/*plays C3 -> C5 with sin wave and half second duration*/
	WaveFile(); 
	WaveFile(const int NumSamples, const int amplitude, const float frequency);
	
	/*
	* @param noteName -> ex: C4, Bb7, F#1
	* @param theWaveType -> select from available enums (triangular, sawtooth, etc.)
	*/
	WaveFile(const std::string& noteName, const WaveType theWaveType, const float durationInSeconds);
	
	/*The fanciest constructor overload (so far)*/
	WaveFile(const std::vector<SongNote>& notes, const WaveType theWaveType);

	/*MUST include .WAV*/
	void writeToFile(const std::string& filename); 
};


/*N.B. This is an "auxillary"/nonmember function*/
void writeLittleEndian(std::ofstream& fout, int value, int byteSize);