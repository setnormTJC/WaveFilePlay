

#include "WaveFile.h"

#include"MyException.h"

#include<algorithm>

FormatHeader::FormatHeader()
{
	ByteRate = SampleRate * NumChannels * (BitsPerSample / 8); 

	BlockAlign = NumChannels * (BitsPerSample / 8); 
}

WaveFile::WaveFile(const std::string& inputFileName)
{
	if (inputFileName.find(".wav") == std::string::npos) throw MyException("input file MUST be .WAV", __FILE__, __LINE__);

	std::ifstream fin{ inputFileName, std::ios::binary };

	//if (!fin) throw std::exception("input file not found");

	if (!fin) throw MyException("input file not found", __FILE__, __LINE__);

	readRiffHeader(fin);
	readFormatHeader(fin);
	readSoundData(fin);
}

void WaveFile::readRiffHeader(std::ifstream& fin)
{
	//read ChunkID, ChunkSize, and Format:
	fin.read(theRiffHeader.ChunkID, 4);
	readLittleEndian(fin, theRiffHeader.ChunkSize);
	fin.read(theRiffHeader.Format, 4);
}

void WaveFile::readFormatHeader(std::ifstream& fin)
{
	fin.read(theFormatHeader.Subchunk1ID, 4);
	readLittleEndian(fin, theFormatHeader.Subchunk1Size);
	readLittleEndian(fin, theFormatHeader.AudioFormat);
	readLittleEndian(fin, theFormatHeader.NumChannels);

	readLittleEndian(fin, theFormatHeader.SampleRate);
	readLittleEndian(fin, theFormatHeader.ByteRate);
	readLittleEndian(fin, theFormatHeader.BlockAlign);
	readLittleEndian(fin, theFormatHeader.BitsPerSample);

	//a sort of safety measure: 
	if (theFormatHeader.BitsPerSample != 16) throw std::exception("Hey! This WAV file does NOT use 16 bits per sample!");

}

void WaveFile::readSoundData(std::ifstream& fin)
{
	fin.read(theSoundSubchunk.Subchunk2ID, 4);
	readLittleEndian(fin, theSoundSubchunk.Subchunk2Size);

	//Subchunk2Size (in bytes) gives the size of the remainder of the data in the file
	int remainingByteCount = theSoundSubchunk.Subchunk2Size;
	theSoundSubchunk.data.resize(remainingByteCount / 2);
	int index = 0;
	while (index * 2 < remainingByteCount)
	{
		readLittleEndian(fin, theSoundSubchunk.data[index]);
		index++;
	}
}


void WaveFile::writeToWaveFile(const std::string& filename)
{
	if (filename.find(".wav") == std::string::npos)
	{
		throw std::exception("filename MUST include .WAV");
	}
	std::ofstream fout{ filename, std::ios::binary };

	//first, right the RIFF header:
	fout.write(theRiffHeader.ChunkID, 4);
	writeLittleEndian(fout, theRiffHeader.ChunkSize, 4);
	fout.write(theRiffHeader.Format, 4);

	//now the FormatHeader
	fout.write(theFormatHeader.Subchunk1ID, 4);
	writeLittleEndian(fout, theFormatHeader.Subchunk1Size, 4);
	writeLittleEndian(fout, theFormatHeader.AudioFormat, 2); //NOTE the two
	writeLittleEndian(fout, theFormatHeader.NumChannels, 2); //again, NOTE the two
	writeLittleEndian(fout, theFormatHeader.SampleRate, 4);
	writeLittleEndian(fout, theFormatHeader.ByteRate, 4);
	writeLittleEndian(fout, theFormatHeader.BlockAlign, 2);
	writeLittleEndian(fout, theFormatHeader.BitsPerSample, 2);

	//now the SoundData: 
	fout.write(theSoundSubchunk.Subchunk2ID, 4);
	writeLittleEndian(fout, theSoundSubchunk.Subchunk2Size, 4);

	for (int i = 0; i < theSoundSubchunk.data.size(); ++i)
	{
		writeLittleEndian(fout, theSoundSubchunk.data[i], 2);
	}

	fout.close();
}



void writeLittleEndian(std::ofstream& fout, int value, int byteSize)
{
	for (int i = 0; i < byteSize; ++i)
	{
		fout.put(static_cast<char>(value >> (i * 8)));
	}
}

void readLittleEndian(std::ifstream& fin, int& value)
{
	if (sizeof(int) != 4) throw std::exception("size of int is not 4 bytes??");

	char buffer[4];
	fin.read(buffer, 4);
	value = (static_cast<unsigned char>(buffer[0])) |
		(static_cast<unsigned char>(buffer[1]) << 8) |
		(static_cast<unsigned char>(buffer[2]) << 16) |
		(static_cast<unsigned char>(buffer[3]) << 24);


	//Alternative (less readable) approach below: 
	//value = 0; 
	//
	//for (int i = 0; i < byteSize; ++i)
	//{
	//	int tempValue = fin.get();
	//	if (tempValue == EOF) throw std::exception("Unexpected end of file");

	//	//std::cout << std::hex << (tempValue & 0xFF) << " "; 
	//	//print statement for visualization of bitshifting mechanics 

	//	value |= (tempValue & 0xFF) << (i * 8); // bitwise shift for little-endian
	//}

	//std::cout << "\nFinal value: " << std::dec << value << std::endl;
}

void readLittleEndian(std::ifstream& fin, short& value)
{
	if (sizeof(short) != 2)
	{
		throw std::exception("short is not 2 bytes on yousa system!?");
	}

	char buffer[2];// = {}; // Initialize to zero in case byteSize < 4
	fin.read(buffer, 2);

	value = static_cast<unsigned char>(buffer[0]) |
		(static_cast<unsigned char>(buffer[1]) << 8);
}


void WaveFile::fillDataWithSineWave(const int NumSamples, const int amplitude, const float frequency)
{
	for (int time = 0; time < NumSamples; ++time)
	{
		theSoundSubchunk.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
	}
}

void WaveFile::fillDataWithSquareWave(const int NumSamples, const int amplitude, const float frequency)
{
	int period = static_cast<int>(theFormatHeader.SampleRate / frequency); // period in samples

	for (int time = 0; time < NumSamples; ++time)
	{
		if ((time / period) % 2 == 0) // Check whether we're in the "positive" half or "negative" half of the period
		{
			theSoundSubchunk.data[time] = amplitude;
		}
		else
		{
			theSoundSubchunk.data[time] = -amplitude;
		}
	}
}

int WaveFile::writeChordToBuffer(const std::vector<PianoNote>& chord, std::vector<short>& buffer, int position)
{
	// Cache all note data once
	std::vector<std::vector<short>> chordWaveData;
	int maxChordLength = 0;

	//cache wave data and get longest duration note in chord: 
	for (const auto& note : chord)
	{
		chordWaveData.push_back(note.getSoundWaveData());
		maxChordLength = std::max(maxChordLength, note.getNumSamples());
	}

	for (int timePoint = 0; timePoint < maxChordLength; ++timePoint)
	{
		short totalAmplitudeFromChordAtTimePoint = 0;
		for (const auto& noteWaveData : chordWaveData)
		{
			if (timePoint < noteWaveData.size())
			{
				totalAmplitudeFromChordAtTimePoint += noteWaveData[timePoint];
			}
		}

		buffer[position + timePoint] = std::clamp(
			static_cast<short>(buffer[position + timePoint] + totalAmplitudeFromChordAtTimePoint),
			static_cast<short>(-25000),
			static_cast<short>(25000)
		);
	}
	return maxChordLength; // Return so the caller knows how far to advance
}

void WaveFile::writeSingleNoteToBuffer(const std::vector<short>& singleNoteData, std::vector<short>& buffer, int& writePosition)
{
	for (int timePoint = 0; timePoint < singleNoteData.size(); ++timePoint)
	{
		buffer[writePosition + timePoint] =
			std::clamp(
				static_cast<short>(buffer[writePosition + timePoint]) + singleNoteData.at(timePoint),
				-25000,
				25000
			);
	}

	writePosition += singleNoteData.size(); // Move to the next note's "time slot"
}

int WaveFile::getMaxNoteLength(const std::vector<PianoNote>& melodyPart)
{
	int maxNoteLength = 0;
	for (const auto& note : melodyPart)
	{
		maxNoteLength = std::max(maxNoteLength, static_cast<int>(note.getNumSamples()));
	}
	return maxNoteLength;
}

std::vector<short> WaveFile::getSoundWave()
{
	return theSoundSubchunk.data; 
}



WaveFile::WaveFile(const int NumSamples, const int amplitude, const float frequency)
{
	theSoundSubchunk.data.resize(NumSamples);

	for (int time = 0; time < NumSamples; ++time)
	{
		//theSoundData.data[time] = amplitude * sin(frequency * time); //not quite right!
		theSoundSubchunk.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
	}

	theSoundSubchunk.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
}


WaveFile::WaveFile(const std::vector<std::vector<PianoNote>>& songNotes)
{
	std::vector<short> soundWaveDataForWaveFile; 
	/*first get total size needed (partially to prevent frequent resizing with pushback)*/
	
	int totalSize = 0; 
	int currentPosition = 0; 

	for (const std::vector<PianoNote>& currentMelodicSegment : songNotes)
	{
		if (!currentMelodicSegment.empty())
		{
			int maxNoteLength = getMaxNoteLength(currentMelodicSegment);
			currentPosition += maxNoteLength; 

			totalSize = currentPosition;
		}
	}

	//resize:
	soundWaveDataForWaveFile.assign(totalSize, static_cast<short>(0));

	int writePosition = 0; //must keep track of the writing position (time point to write data at) 

	//WRITE
	for (const std::vector<PianoNote>& currentMelodicSegment : songNotes)
	{ 
		if (currentMelodicSegment.size() == 1) 
		{
			const std::vector<short>& singleMelodicNoteData = currentMelodicSegment.at(0).getSoundWaveData();
			writeSingleNoteToBuffer(singleMelodicNoteData, soundWaveDataForWaveFile, writePosition);
		}

		else //it is a chord
		{
			int maxChordLength = writeChordToBuffer(currentMelodicSegment, soundWaveDataForWaveFile, writePosition); 
			writePosition += maxChordLength;
		}
	}

	//Update WAV file header info
	theSoundSubchunk.data = soundWaveDataForWaveFile;
	theSoundSubchunk.Subchunk2Size = soundWaveDataForWaveFile.size() * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}

void WaveFile::addTrack(const std::vector<std::vector<PianoNote>>& newTrack)
{
	/*First, confirm that newTrack has same duration as "top" track (the one passed to constructor)*/
	int totalSize = 0;
	int currentPosition = 0;

	for (const std::vector<PianoNote>& currentMelodicSegment : newTrack)
	{
		if (!currentMelodicSegment.empty())
		{
			int maxNoteLength = getMaxNoteLength(currentMelodicSegment);
			currentPosition += maxNoteLength;
			totalSize = currentPosition;
		}
	}

	int currentTrackDuration = static_cast<int>(theSoundSubchunk.data.size());
	int newTrackDuration = totalSize; 

	std::vector<short> soundWaveDataForWaveFile = theSoundSubchunk.data; //get the "top" track

	//asdf

	/*Padding business*/
		// Define a threshold for perceptible misalignment in samples (e.g., 5ms worth of samples)
	const int misalignmentThreshold = static_cast<int>(0.005 * 44'100);  // 5ms threshold, converted to samples

	// Check if the misalignment is "perceptibly large"
	if (std::abs(newTrackDuration - currentTrackDuration) > misalignmentThreshold)
	{
		throw MyException("Sample misalignment between tracks is perceptibly large. "
			"Check that all tracks are correctly aligned.",
			__FILE__, __LINE__);
	}
	
	
	if (newTrackDuration < currentTrackDuration)
	{
		// Add padding (silent data) to the new track's buffer
		int numberOfSamplesToTrim = currentTrackDuration - newTrackDuration; 

		soundWaveDataForWaveFile.erase
			(soundWaveDataForWaveFile.end() - numberOfSamplesToTrim,
			soundWaveDataForWaveFile.end());

	}

	else if (newTrackDuration > currentTrackDuration)
	{
		int paddingRequired = newTrackDuration - currentTrackDuration;

		soundWaveDataForWaveFile.insert(soundWaveDataForWaveFile.end(), paddingRequired, static_cast<short>(0));
	}

	int writePosition = 0; 

	//WRITE
	for (const std::vector<PianoNote>& currentMelodicSegment : newTrack)
	{
		if (currentMelodicSegment.size() == 1)
		{
			const std::vector<short>& singleMelodicNoteData = currentMelodicSegment.at(0).getSoundWaveData();
			
			//writeSingleNoteToBuffer DOES seem to be successfully ADDING to previous content at writePosition!
			writeSingleNoteToBuffer(singleMelodicNoteData, soundWaveDataForWaveFile, writePosition);
		}

		else //it is a chord
		{
			int maxChordLength = writeChordToBuffer(currentMelodicSegment, soundWaveDataForWaveFile, writePosition);
			//writeChordToBuffer DOES seem to be successfully ADDING to previous content at writePosition!
			writePosition += maxChordLength;
		}
	}

	theSoundSubchunk.data = soundWaveDataForWaveFile; // The sound data with added padding and/or second track

	// Update Info header (because padding MIGHT have been added)
	theSoundSubchunk.Subchunk2Size = soundWaveDataForWaveFile.size() * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}



WaveFile::WaveFile(const PianoNote& pianoNote, const WaveType theWaveType)
{
	if (PianoNote::notesToFrequencies.find(pianoNote.noteName) == PianoNote::notesToFrequencies.end())
	{
		throw std::exception("Given note name is not included in the map"); 
		//NOTE: interestingly, this exception is overruled by one that is built in to std::map
	}

	float frequency = static_cast<float>(PianoNote::notesToFrequencies[pianoNote.noteName]);
	int amplitude = (int)pianoNote.fundamentalAmplitude;
	int NumSamples = static_cast<int>(theFormatHeader.SampleRate * pianoNote.durationInSeconds);

	theSoundSubchunk.data.resize(NumSamples);

	switch (theWaveType)
	{
	case WaveType::Sine:
		fillDataWithSineWave(NumSamples, amplitude, frequency); 
		break; 

	case WaveType::Square:
		fillDataWithSquareWave(NumSamples, amplitude, frequency); 
		break; 

	default: 
		throw MyException("unsupported wave type", __FILE__, __LINE__);
		break; 
	}

	theSoundSubchunk.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}


void WaveFile::reverseAudio()
{
	//ain't nothin' to it but to do it...
	std::reverse(theSoundSubchunk.data.begin(), theSoundSubchunk.data.end());
}

void WaveFile::modifyVolume(const float scalingFactor)
{
	for (auto& soundWaveValue : theSoundSubchunk.data)
	{
		if (scalingFactor * soundWaveValue <= SHRT_MAX
			||
			scalingFactor * soundWaveValue >= SHRT_MIN)
		{
			soundWaveValue = static_cast<short>(scalingFactor * soundWaveValue);
		}
	}
}



