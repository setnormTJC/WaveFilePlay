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

void WaveFile::fillDataWithExponentialDecay(const int NumSamples, const int amplitude, const float frequency)
{
	int attackPhaseSampleCount = NumSamples / 10;  //arbitrary-ish, rise to peak "fast"
	int kAttack = 300; 

	int newAmplitude = 0; //updated below, of course: 

	//see screenshot in "chatGPTNotesOnEvelopes.jpg" in this folder for some details on this magic number 300...
	for (int time = 0; time < attackPhaseSampleCount; ++time)
	{
		//double exponentPart = pow(M_E, -time * kAttack); //just to reduce "complexity" of arithmetic expression

		double exponentPart = pow(M_E, -static_cast<double>(time) / attackPhaseSampleCount * kAttack); 
		//note the "normalization"
		newAmplitude = amplitude * (1 - exponentPart);

		theSoundSubchunk.data[time] = newAmplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
	}

	//int KDecay = 10; //again, see the jpg
	////decay (and sustain?): 
	//for (int time = attackPhaseSampleCount; time < NumSamples; ++time)
	//{
	//	double exponentPart = pow(M_E, -time * KDecay);
	//	theSoundSubchunk.data[time] = amplitude * exponentPart; 
	//}

	int KDecay = 4;

	for (int time = 0; time < NumSamples - attackPhaseSampleCount; ++time)
	{
		double exponentPart = pow(M_E, -static_cast<double>(time) / (NumSamples - attackPhaseSampleCount) * KDecay);
		newAmplitude = amplitude * exponentPart;

		theSoundSubchunk.data[attackPhaseSampleCount + time] = newAmplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);

	}
	
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
			soundWaveValue = scalingFactor * soundWaveValue; 
		}
	}
}

void WaveFile::writeSoundDataToCSV(const std::string& CSVfilename)
{
	if (CSVfilename.find(".csv") == std::string::npos) throw std::exception("modify the filetype to CSV");

	std::ofstream fout(CSVfilename, std::ios::binary); 

	
	int counter = 0; //spreadsheet software is a weanie when plotting > 100'000 points, 
					//so I'm limiting the amount of data written out with this counter
	for (const auto& soundWaveDataPoint : theSoundSubchunk.data)
	{
		if (counter % 10 == 0)
		{
			fout << soundWaveDataPoint << "\n";
		}
		counter++; 
	}
}

std::vector<short> WaveFile::getSoundWave()
{
	return theSoundSubchunk.data; 
}

void WaveFile::writeSoundDataToImagePlot(const std::string& imageFilename)
{
	std::vector<short> soundWave = getSoundWave(); 
	if (soundWave.size() == 0) 
		throw std::exception("Sound wave data is empty - didst thou forget initialization?");

	unsigned int imageWidth, imageHeight; 
	imageWidth = 1'000; 
	imageHeight = 1'000; 

	PlotImage plotImage(imageWidth, imageHeight, Color(ColorEnum::Black));
	
	//PlotImage::plotData expects a map of x,y values (unsurprisingly), so make one: 
	std::map<int, int> timeVersusAmplitude; 
	int timePoint = 0; 
	for (const auto& amplitude : soundWave)
	{
		timeVersusAmplitude.insert({ timePoint, amplitude });
		timePoint++; 
	}

	plotImage.plotData(timeVersusAmplitude, ColorEnum::Green);
	
	plotImage.writeImageFile(imageFilename);
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

WaveFile::WaveFile(const PianoNote& pianoNote, const WaveType theWaveType)
{
	if (PianoNote::notesToFrequencies.find(pianoNote.name) == PianoNote::notesToFrequencies.end())
	{
		throw std::exception("Given note name is not included in the map"); 
		//NOTE: interestingly, this exception is overruled by one that is built in to std::map
	}

	float frequency = PianoNote::notesToFrequencies[pianoNote.name];
	int amplitude = (int)pianoNote.amplitude;
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

	case WaveType::FancyInstrument:
		fillDataWithExponentialDecay(NumSamples, amplitude, frequency); 
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
		int amplitude = static_cast<int>(melodicNotes[i].amplitude);
		
		float frequency;
		if (melodicNotes[i].name != "") //Let this mean SILENCE 
		{
			frequency = PianoNote::notesToFrequencies.at(melodicNotes[i].name);
		}
		else frequency = 0.0f; //0 oscillation should? mean no sound (expect A = 0 from client also for that note)

		for (int time = currentSample; time < currentSample + NumSamples; ++time)
		{
			theSoundSubchunk.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
		}

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
			int amplitude = static_cast<int>(currentHarmonicNote.amplitude);
			float frequency = PianoNote::notesToFrequencies.at(currentHarmonicNote.name);

			theSoundSubchunk.data[time] += amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
		}
	}

	theSoundSubchunk.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);

	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);

}

WaveFile::WaveFile(const std::string& inputFileName)
{
	if (inputFileName.find(".wav") == std::string::npos) throw std::exception("input file MUST be .WAV");

	std::ifstream fin{ inputFileName, std::ios::binary };

	if (!fin) throw std::exception("input file not found");

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

void readLittleEndian(std::ifstream& fin, int & value)
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
