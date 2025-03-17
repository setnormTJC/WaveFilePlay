#include "WaveFile.h"

#include"MyException.h"

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

void WaveFile::fillDataWithPianoWave(const int NumSamples, const int amplitude, const float frequency, const int currentSample)
{
	int attackPhaseSampleCount = NumSamples / 10;  // Arbitrary-ish, rise to peak "fast"
	int newAmplitude = 0; 

	// Apply ADSR envelope and overtones in two phases: attack and decay
	for (int time = 0; time < NumSamples; ++time)
	{
		// ADSR Phase (Attack + Decay)
		simplifiedApplyADSR(newAmplitude, amplitude, attackPhaseSampleCount, time,NumSamples);

		// Fundamental contribution
		theSoundSubchunk.data[currentSample + time] = newAmplitude * sin(2 * M_PI * frequency * time / theFormatHeader.SampleRate);

		//auto overtoneFrequenciesToAmplitudes = PianoNote::mapOvertoneFrequenciesToAmplitudes()
		//for ()

		// Apply overtone contributions
		//applyOvertones(currentSample + time, frequency, newAmplitude, pianoOvertonesToAmplitudeScalingFactors);
	}

}

void WaveFile::simplifiedApplyADSR(int& newAmplitude, int amplitude, int attackPhaseSampleCount, int time, int NumSamples)
{

}

void WaveFile::fillDataWithADSRPianoOvertones(const int NumSamples, const int amplitude, const float frequency)
{
	int attackPhaseSampleCount = NumSamples / 10;  // Arbitrary-ish, rise to peak "fast"
	int kAttack = 300;

	int newAmplitude = 0; // Updated below, of course

	/*N.B. This overtone signature only applies "well" for notes "close" to C4!*/
	std::map<int, float> pianoOvertonesToAmplitudeScalingFactors =
	{
		{1, (1.0 / 3.0)}, // First overtone 
		{2, (1.0 / 4.0)}, // Second overtone
		{3, (1.0 / 5.0)},  // Third overtone
		{4, (1.0 / 6.0)},
		{5, (1.0 / 7.0)}
	};
	

	// Attack phase: amplitude rises
	for (int time = 0; time < attackPhaseSampleCount; ++time)
	{
		applyADSR(newAmplitude, amplitude, attackPhaseSampleCount, time, kAttack, 4, NumSamples);

		// Fundamental contribution
		theSoundSubchunk.data[time] = newAmplitude * sin(2 * M_PI * frequency * time / theFormatHeader.SampleRate);

		// Overtones contribution
		applyOvertones(time, frequency, newAmplitude, pianoOvertonesToAmplitudeScalingFactors);
	}

	// Decay phase: amplitude decays
	for (int time = 0; time < NumSamples - attackPhaseSampleCount; ++time)
	{
		applyADSR(newAmplitude, amplitude, attackPhaseSampleCount, time, kAttack, 4, NumSamples);

		// Fundamental contribution
		theSoundSubchunk.data[attackPhaseSampleCount + time] = newAmplitude * sin(2 * M_PI * frequency * time / theFormatHeader.SampleRate);

		// Overtones contribution
		applyOvertones(attackPhaseSampleCount + time, frequency, newAmplitude, pianoOvertonesToAmplitudeScalingFactors);
	}
}

void WaveFile::fillDataWithADSRPianoOvertones(const int NumSamples, const int amplitude, const float frequency, const int currentSample)
{
	int attackPhaseSampleCount = NumSamples / 10;  // Attack phase: rise to peak "fast"
	int kAttack = 300;

	int newAmplitude = 0;

	// Overtone scaling factors for piano (based on research or Fourier analysis)
	std::map<int, float> pianoOvertonesToAmplitudeScalingFactors =
	{
		{1, (1.0/3.0)}, // First overtone 
		{2, (1.0/4.0)}, // Second overtone
		{3, (1.0/5.0)},  // Third overtone
		{4, (1.0/6.0)},
		{5, (1.0/7.0)}
	};

	int kDecay = 4;
	// Apply ADSR envelope and overtones in two phases: attack and decay
	for (int time = 0; time < NumSamples; ++time)
	{
		// ADSR Phase (Attack + Decay)
		applyADSR(newAmplitude, amplitude, attackPhaseSampleCount, time, kAttack, kDecay, NumSamples);

		// Fundamental contribution
		theSoundSubchunk.data[currentSample + time] = newAmplitude * sin(2 * M_PI * frequency * time / theFormatHeader.SampleRate);

		// Apply overtone contributions
		applyOvertones(currentSample + time, frequency, newAmplitude, pianoOvertonesToAmplitudeScalingFactors);
	}
}

void WaveFile::applyADSR(int& newAmplitude, int amplitude, int attackPhaseSampleCount, int time, int kAttack, int kDecay, int NumSamples)
{
	int attackTime = attackPhaseSampleCount; 
	int decayTime = (attackPhaseSampleCount * 10) / 2;    // Adjust the decay time as per your requirements
	int sustainLevel = amplitude / 4;    // Sustain level (can be a fraction of the amplitude)

	if (time < attackPhaseSampleCount)
	{
		double exponentPart = pow(M_E, -static_cast<double>(time) / attackPhaseSampleCount * kAttack);
		newAmplitude = amplitude * (1 - exponentPart);  // Attack phase (rise)
	}
	//else 
	//{
	//	double exponentPart = pow(M_E, -static_cast<double>(time - attackPhaseSampleCount) / (NumSamples - attackPhaseSampleCount) * kDecay);
	//	newAmplitude = amplitude * exponentPart;  // Decay phase (fall)
	//}

	else if (time < attackTime + decayTime)
	{
		double exponentPart = pow(M_E, -static_cast<double>(time - attackPhaseSampleCount) / (NumSamples - attackPhaseSampleCount) * kDecay);
		newAmplitude = amplitude * exponentPart;  // Decay phase (fall)
	}


	else
	{
		newAmplitude = sustainLevel; 
	}
}

int WaveFile::applyADSR(int time, int totalSamples, int amplitude)
{
	int attackTime = totalSamples / 10;  // Adjust the attack time as per your requirements
	int decayTime = totalSamples / 2;    // Adjust the decay time as per your requirements
	int sustainLevel = amplitude / 4;    // Sustain level (can be a fraction of the amplitude)

	// Attack phase: Linear rise
	if (time < attackTime)
	{
		return static_cast<int>(amplitude * (static_cast<float>(time) / attackTime));
	}
	// Decay phase: Linear decay to sustain level
	else if (time < attackTime + decayTime)
	{
		return static_cast<int>(amplitude - (amplitude - sustainLevel) * (static_cast<float>(time - attackTime) / decayTime));
	}
	// Sustain phase: Constant sustain level
	else
	{
		return sustainLevel;
	}
}


void WaveFile::applyOvertones(int time, float frequency, int amplitude, std::map<int, float>& pianoOvertonesToAmplitudeScalingFactors)
{
	// Frequencies for overtones (could be stored in a map for easier adjustments)
	std::vector<float> overtoneMultipliers = { 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };

	// Loop through each overtone and apply the contribution
	for (int i = 0; i < overtoneMultipliers.size(); ++i)
	{
		int overtoneFrequency = frequency * overtoneMultipliers[i];
		theSoundSubchunk.data[time] += amplitude * pianoOvertonesToAmplitudeScalingFactors[i + 1] *
			sin(2 * M_PI * overtoneFrequency * time / theFormatHeader.SampleRate);
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

WaveFile::WaveFile(const PianoNote& pianoNote, const WaveType theWaveType)
{
	if (PianoNote::notesToFrequencies.find(pianoNote.noteName) == PianoNote::notesToFrequencies.end())
	{
		throw std::exception("Given note name is not included in the map"); 
		//NOTE: interestingly, this exception is overruled by one that is built in to std::map
	}

	float frequency = static_cast<float>(PianoNote::notesToFrequencies[pianoNote.noteName]);
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

	case WaveType::Piano:
		fillDataWithADSRPianoOvertones(NumSamples, amplitude, frequency); 
		//fillDataWithPianoWave(NumSamples, amplitude, frequency); 
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
		if (melodicNotes[i].noteName != "") //Let this mean SILENCE 
		{
			frequency = PianoNote::notesToFrequencies.at(melodicNotes[i].noteName);
		}
		else
		{
			frequency = 0.0f;
			amplitude = 0; 
		}

		fillDataWithADSRPianoOvertones(NumSamples, amplitude, frequency, currentSample); 

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
			float frequency = PianoNote::notesToFrequencies.at(currentHarmonicNote.noteName);

			//apply ADSR: (fix later)
			int currentAmplitude = applyADSR(time, totalSamples, amplitude); 

			theSoundSubchunk.data[time] += currentAmplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);

			// Add overtones (First, second, and third overtones)
			theSoundSubchunk.data[time] += currentAmplitude * (1.0/3.0) * sin(2 * M_PI * frequency * 2 * time / theFormatHeader.SampleRate); // First overtone
			theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 4.0) * sin(2 * M_PI * frequency * 3 * time / theFormatHeader.SampleRate); // Second overtone
			theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 5.0) * sin(2 * M_PI * frequency * 4 * time / theFormatHeader.SampleRate); // Third overtone

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



