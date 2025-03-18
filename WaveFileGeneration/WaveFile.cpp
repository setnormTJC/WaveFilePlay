

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
	if (inputFileName.find(".wav") == std::string::npos) throw std::exception("input file MUST be .WAV");

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

WaveFile::WaveFile(const PianoNote& pianoNote)
{
	std::vector<short> soundWaveData = pianoNote.getSoundWaveData(); 

	//theSoundSubchunk.data.resize(soundWaveData.size());
	theSoundSubchunk.data = soundWaveData; 
	
	theSoundSubchunk.Subchunk2Size = soundWaveData.size() * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
	
}

WaveFile::WaveFile(const std::vector<std::vector<PianoNote>>& harmonicAndMelodicNotes)
{
	std::vector<short> soundWaveDataForWaveFile; 
	/*first get total size needed (partially to prevent frequent resizing with pushback)*/
	int totalSize = 0; 
	for (const std::vector<PianoNote>& harmonicAndMelodicNote : harmonicAndMelodicNotes)
	{
		if (!harmonicAndMelodicNote.empty())
		{
			totalSize += harmonicAndMelodicNote.at(0).getSoundWaveData().size(); 
		}
	}

	//resize:
	soundWaveDataForWaveFile.assign(totalSize, static_cast<short>(0));

	int writePosition = 0; 

	for (const std::vector<PianoNote>& currentMelodicNote : harmonicAndMelodicNotes)
	{ 
		if (currentMelodicNote.size() == 1) 
		{
			const std::vector<short>& singleMelodicNoteData = currentMelodicNote.at(0).getSoundWaveData(); 

			for (int timePoint = 0; timePoint < singleMelodicNoteData.size(); ++timePoint)
			{
				soundWaveDataForWaveFile[writePosition + timePoint] += singleMelodicNoteData.at(timePoint); 
			}

			writePosition += singleMelodicNoteData.size(); //move on to next note's (or notes') "time slot"
		}

		else //it is a chord -> so ADD up individual note contributions at same time point
		{
			// Cache all note data once
			std::vector<std::vector<short>> chordWaveData;
			for (const PianoNote& note : currentMelodicNote)
			{
				chordWaveData.push_back(note.getSoundWaveData());
			}

			//first, get the duration of the chord - AGAIN, assuming here that all notes of chord have same duration
			//...for now (for simplicity) 
			int totalTimePointsForChord = chordWaveData.at(0).size();

			short safetyNetToPreventPopping = 30'000;

			for (int timePoint = 0; timePoint < totalTimePointsForChord; ++timePoint)
			{
				short totalAmplitudeFromChordAtTimePoint = 0; 
				for (const std::vector<short>& noteWaveData : chordWaveData)
				{
					totalAmplitudeFromChordAtTimePoint += noteWaveData.at(timePoint); 
				}

				soundWaveDataForWaveFile[writePosition + timePoint] = 
					std::clamp(
					static_cast<int>(soundWaveDataForWaveFile[writePosition + timePoint]) + totalAmplitudeFromChordAtTimePoint,
					-30000,
					30000
				);
				//soundWaveDataForWaveFile[writePosition + timePoint] += totalAmplitudeFromChordAtTimePoint;
			}

			writePosition += totalTimePointsForChord;
		}
	}

	theSoundSubchunk.data = soundWaveDataForWaveFile;

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
	int NumSamples = theFormatHeader.SampleRate * pianoNote.durationInSeconds; 

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
		throw std::exception("unsupported wave type");
		break; 
	}

	theSoundSubchunk.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}

WaveFile::WaveFile(const std::vector<PianoNote>& melodicNotes, const WaveType theWaveType)
{
	int totalSamples = 0;
	for (const auto& note : melodicNotes)
	{
		totalSamples += note.durationInSeconds * theFormatHeader.SampleRate;
	}

	// Resize the data vector only once
	theSoundSubchunk.data.resize(totalSamples);

	int currentSample = 0; 
	for (int i = 0; i < melodicNotes.size(); ++i)
	{
		int NumSamples = melodicNotes[i].durationInSeconds * theFormatHeader.SampleRate;
		int amplitude = static_cast<int>(melodicNotes[i].fundamentalAmplitude);
		
		float frequency;
		if (melodicNotes[i].noteName != "") //Let this mean SILENCE 
		{
			frequency = PianoNote::notesToFrequencies.at(melodicNotes[i].noteName);
		}
		else
		{
			frequency = 0.0f;
			amplitude = 0; 
		}

		//fillDataWithADSRPianoOvertones(NumSamples, amplitude, frequency, currentSample); 

		currentSample += NumSamples; 
	}
	
	theSoundSubchunk.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}

WaveFile::WaveFile(const std::vector<PianoNote>& harmonicNotes)
{
	int totalSamples = 0;

	for (int i = 0; i < harmonicNotes.size() - 1; ++i)
	{
		if (harmonicNotes[i].durationInSeconds != harmonicNotes[i + 1].durationInSeconds)
		{
			throw std::exception("FOR NOW, all harmonic notes must have same duration!");
		}
	}

	totalSamples += harmonicNotes[0].durationInSeconds * theFormatHeader.SampleRate;

	theSoundSubchunk.data.resize(totalSamples); 

	for (int time = 0; time < totalSamples; ++time)
	{
		theSoundSubchunk.data[time] = 0; 

		for (const PianoNote& currentHarmonicNote : harmonicNotes)
		{
			int amplitude = static_cast<int>(currentHarmonicNote.fundamentalAmplitude);
			float frequency = PianoNote::notesToFrequencies.at(currentHarmonicNote.noteName);

			//apply ADSR: (fix later)
			//int currentAmplitude = applyADSR(time, totalSamples, amplitude); 

			//theSoundSubchunk.data[time] += currentAmplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);

			//// Add overtones (First, second, and third overtones)
			//theSoundSubchunk.data[time] += currentAmplitude * (1.0/3.0) * sin(2 * M_PI * frequency * 2 * time / theFormatHeader.SampleRate); // First overtone
			//theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 4.0) * sin(2 * M_PI * frequency * 3 * time / theFormatHeader.SampleRate); // Second overtone
			//theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 5.0) * sin(2 * M_PI * frequency * 4 * time / theFormatHeader.SampleRate); // Third overtone

			//for the magic numbers 0.3, 0.2, 0.1, see `pianoOvertonesToAmplitudeScalingFactors` in a function above
		}
	}

	theSoundSubchunk.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

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



