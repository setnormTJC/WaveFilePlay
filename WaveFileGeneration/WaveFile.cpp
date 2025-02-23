#include "WaveFile.h"

FormatHeader::FormatHeader()
{
	ByteRate = SampleRate * NumChannels * (BitsPerSample / 8); 

	BlockAlign = NumChannels * (BitsPerSample / 8); 

}

void WaveFile::fillDataWithSineWave(const int NumSamples, const int amplitude, const float frequency)
{
	for (int time = 0; time < NumSamples; ++time)
	{
		theSoundData.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
	}
}

void WaveFile::fillDataWithSquareWave(const int NumSamples, const int amplitude, const float frequency)
{
 
	int period = static_cast<int>(theFormatHeader.SampleRate / frequency); // period in samples

	for (int time = 0; time < NumSamples; ++time)
	{
		if ((time / period) % 2 == 0) // Check whether we're in the "positive" half or "negative" half of the period
		{
			theSoundData.data[time] = amplitude;
		}
		else
		{
			theSoundData.data[time] = -amplitude;
		}
	}
}

WaveFile::WaveFile()
{

	const int NumSamples = (theFormatHeader.SampleRate/2) * notesToFrequencies.size(); 
	//makes each note in notesToFrequencies sound for 0.5 seconds 

	theSoundData.data.resize(NumSamples);

	constexpr float amplitude = 32'767;   

	auto it = notesToFrequencies.begin(); 

	float frequency = notesToFrequencies.at(notesToFrequencies.begin()->first); //Hz

	for (int time = 0; time < NumSamples; ++time)
	{
		if (time % (theFormatHeader.SampleRate / 2) == 0)
		{
			if (it != notesToFrequencies.end()) //safety check
			{
				//update frequency to next note: 
				frequency = it->second; 
				++it; 
			}

		}

		theSoundData.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);

	}
		
	theSoundData.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundData.Subchunk2Size); 
}

WaveFile::WaveFile(const int NumSamples, const int amplitude, const float frequency)
{
	theSoundData.data.resize(NumSamples);

	for (int time = 0; time < NumSamples; ++time)
	{
		//theSoundData.data[time] = amplitude * sin(frequency * time); //not quite right!
		theSoundData.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
	}

	theSoundData.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundData.Subchunk2Size);
}

WaveFile::WaveFile(const std::string& noteName, const WaveType theWaveType, const float durationInSeconds)
{
	if (notesToFrequencies.find(noteName) == notesToFrequencies.end())
	{
		throw std::exception("Given note name is not included in the map"); 
		//NOTE: interestingly, this exception is overruled by one that is built in to std::map
	}

	float frequency = notesToFrequencies[noteName];
	int amplitude = 32'767;
	int NumSamples = theFormatHeader.SampleRate * durationInSeconds; 

	theSoundData.data.resize(NumSamples);

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

	theSoundData.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundData.Subchunk2Size);

}

WaveFile::WaveFile(const std::vector<SongNote>& notes, const WaveType theWaveType)
{
	int totalSamples = 0;
	for (const auto& note : notes)
	{
		totalSamples += note.duration * theFormatHeader.SampleRate;
	}

	// Resize the data vector only once
	theSoundData.data.resize(totalSamples);

	int currentSample = 0; 
	for (int i = 0; i < notes.size(); ++i)
	{
		int NumSamples = notes[i].duration * theFormatHeader.SampleRate;
		int amplitude = static_cast<int>(notes[i].amplitude);
		float frequency = notesToFrequencies.at(notes[i].name);

		for (int time = currentSample; time < currentSample + NumSamples; ++time)
		{
			theSoundData.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
		}

		currentSample += NumSamples; 
	}
	
	//CUTE little reverse: 
	std::reverse(theSoundData.data.begin(), theSoundData.data.end());

	//drop the volume by a factor of 2: 
	for (int i = 0; i < theSoundData.data.size(); ++i)
	{
		theSoundData.data[i] = theSoundData.data[i] / 2; 
	}

	theSoundData.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundData.Subchunk2Size);

}

void WaveFile::writeToFile(const std::string& filename)
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
	fout.write(theSoundData.Subchunk2ID, 4);
	writeLittleEndian(fout, theSoundData.Subchunk2Size, 4); 

	for (int i = 0; i < theSoundData.data.size(); ++i)
	{
		writeLittleEndian(fout, theSoundData.data[i], 2);
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
