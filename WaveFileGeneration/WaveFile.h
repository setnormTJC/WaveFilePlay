//written using http://soundfile.sapp.org/doc/WaveFormat/
#pragma once

#define _USE_MATH_DEFINES //for M_E in music envelope's exponential decay (piano timbre)

#include"PianoNote.h"

//#include"Synthesizer.h"

#include<cmath> 
#include<fstream>



class RiffHeader
{
	/*const */char ChunkID[4] = { 'R', 'I', 'F', 'F' }; //I removed the const in anticipation of reading INPUT files (which will modify this) 
	int ChunkSize{}; //this requires info from `SoundData` below!
	char Format[4] = { 'W', 'A', 'V', 'E' }; 

	friend class WaveFile; 

};

class FormatHeader
{
	/*const */char Subchunk1ID[4] = { 'f', 'm', 't', ' ' }; //'f'm't' ' -> NOTE the space!
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

class SoundSubchunk
{
	char Subchunk2ID[4] = { 'd', 'a', 't', 'a' }; //'d'a't'a 
	int Subchunk2Size{}; //calculated in default constructor (because it requires data from below)


	//NOTE! SIXTEEN-BIT int used for data! Int is required by PCM, 16 is determined by BitsPerSample field 
	std::vector<short> data;

	friend class RiffHeader; 
	friend class WaveFile;
};

class WaveFile
{
	RiffHeader theRiffHeader; 
	FormatHeader theFormatHeader; 
	SoundSubchunk theSoundSubchunk;

	//const int amplitude = 32'767; //pros and cons of making this a member variable - obviously constructing waves with different loudness is good 

	void readRiffHeader(std::ifstream& fin); 
	void readFormatHeader(std::ifstream& fin);
	void readSoundData(std::ifstream& fin);

	void fillDataWithSineWave(const int NumSamples, const int amplitude, const float frequency);
	void fillDataWithSquareWave(const int NumSamples, const int amplitude, const float frequency);
	
	/*
	*  @param position The starting index in the buffer where the chord should be written.
	* @returns maxChordLength -> in the event that some notes of the chord last longer than others
	*/
	int writeChordToBuffer(const std::vector<PianoNote>& chord, std::vector<short>& buffer, int position); 

	void writeSingleNoteToBuffer(const std::vector<short>& singleNoteData, std::vector<short>& buffer, int& writePosition);

	int getMaxNoteLength(const std::vector<PianoNote>& melodyPart);

public: 
	/*I only support Sine and Square, at the moment*/
	enum class WaveType
	{
		Sine,
		Square,
		Saw
	};

	WaveFile() = delete; 
	WaveFile(const int NumSamples, const int amplitude, const float frequency);
	
	/*
	* @param noteName -> ex: C4, Bb7, F#1
	* @param theWaveType -> select from available enums (triangular, sawtooth, etc.)
	*/
	WaveFile(const PianoNote& pianoNote, const WaveType theWaveType);

	/*One constructor to rule them all...
	* @param songNotes-> outer vector is harmony, inner vector CAN be a chord, or it might just be a SINGLE note 
	*/
	WaveFile(const std::vector<std::vector<PianoNote>>& songNotes);

	WaveFile(std::vector<std::vector<PianoNote>> trebleNotes,
		std::vector<std::vector<PianoNote>> bassNotes);

	/*Creates a WaveFile object by reading an input file (whose extension MUST be .wav)*/
	WaveFile(const std::string& inputFileName);

	WaveFile(const WaveFile& other) = delete; 
	WaveFile(const WaveFile&& other) = delete; 

	/*@param filename -> MUST include .WAV*/
	void writeToWaveFile(const std::string& filename);

	/*Anticipate calling this AFTER `WaveFile(const std::vector<std::vector<PianoNote>>& songNotes)`*/
	void addTrack(const std::vector < std::vector<PianoNote>>& newTrack);

#pragma region Audio modification and analysis section 

	/*This method calls std::reverse on `SoundSubchunk's` vector of data (the sound wave)*/
	void reverseAudio();
	/*@param scalingFactor -> if 0.5, halves the amplitude(loudness); if 2.0, doubles, etc.
	Note that this method checks a will not allow scaling to exceed +/- 32,676
	...Also note that "capped out waves" will sound more unpleasant than expected (due to square wavy-ness).
	*/
	void modifyVolume(const float scalingFactor);
	
	std::vector<short> getSoundWave(); 
#pragma endregion
};

/*@param value -> note that I use this for both shorts and ints (shorts are working as intended, so far)*/
void writeLittleEndian(std::ofstream& fout, int value, int byteSize);

/*This function is overloaded for ints and shorts*/
void readLittleEndian(std::ifstream& fin, int& value);

/*Not a particularly intelligent overload*/
void readLittleEndian(std::ifstream& fin, short& value);





