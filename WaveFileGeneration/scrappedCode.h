#pragma once

//for (const std::vector<PianoNote>& harmonicAndMelodicNote : harmonicAndMelodicNotes)
//{
//	if (harmonicAndMelodicNote.size() == 1)
//	{
//		for (const PianoNote& currentHarmonicOrSingleNote : harmonicAndMelodicNote)
//		{
//			std::vector<short> currentSoundWaveData = currentHarmonicOrSingleNote.getSoundWaveData(); 

//			for (const short amplitudeAtTimePoint : currentSoundWaveData)
//			{
//				soundWaveDataForWaveFile.push_back(amplitudeAtTimePoint);
//			}
//		}
//	}

//	else //melodic note is also harmonic 
//	{

//	}
//}
// 
//WaveFile::WaveFile(const PianoNote& pianoNote)
//{
//	std::vector<short> soundWaveData = pianoNote.getSoundWaveData(); 
//
//	//theSoundSubchunk.data.resize(soundWaveData.size());
//	theSoundSubchunk.data = soundWaveData; 
//	
//	theSoundSubchunk.Subchunk2Size = soundWaveData.size() * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
//
//	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
//	
//}

//WaveFile::WaveFile(const std::vector<PianoNote>& melodicNotes, const WaveType theWaveType)
//{
//	int totalSamples = 0;
//	for (const auto& note : melodicNotes)
//	{
//		totalSamples += note.durationInSeconds * theFormatHeader.SampleRate;
//	}
//
//	// Resize the data vector only once
//	theSoundSubchunk.data.resize(totalSamples);
//
//	int currentSample = 0;
//	for (int i = 0; i < melodicNotes.size(); ++i)
//	{
//		int NumSamples = melodicNotes[i].durationInSeconds * theFormatHeader.SampleRate;
//		int amplitude = static_cast<int>(melodicNotes[i].fundamentalAmplitude);
//
//		float frequency;
//		if (melodicNotes[i].noteName != "") //Let this mean SILENCE 
//		{
//			frequency = PianoNote::notesToFrequencies.at(melodicNotes[i].noteName);
//		}
//		else
//		{
//			frequency = 0.0f;
//			amplitude = 0;
//		}
//
//		//fillDataWithADSRPianoOvertones(NumSamples, amplitude, frequency, currentSample); 
//
//		currentSample += NumSamples;
//	}
//
//	theSoundSubchunk.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
//
//	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
//
//}
//
//WaveFile::WaveFile(const std::vector<PianoNote>& harmonicNotes)
//{
//	int totalSamples = 0;
//
//	for (int i = 0; i < harmonicNotes.size() - 1; ++i)
//	{
//		if (harmonicNotes[i].durationInSeconds != harmonicNotes[i + 1].durationInSeconds)
//		{
//			throw std::exception("FOR NOW, all harmonic notes must have same duration!");
//		}
//	}
//
//	totalSamples += harmonicNotes[0].durationInSeconds * theFormatHeader.SampleRate;
//
//	theSoundSubchunk.data.resize(totalSamples);
//
//	for (int time = 0; time < totalSamples; ++time)
//	{
//		theSoundSubchunk.data[time] = 0;
//
//		for (const PianoNote& currentHarmonicNote : harmonicNotes)
//		{
//			int amplitude = static_cast<int>(currentHarmonicNote.fundamentalAmplitude);
//			float frequency = PianoNote::notesToFrequencies.at(currentHarmonicNote.noteName);
//
//			//apply ADSR: (fix later)
//			//int currentAmplitude = applyADSR(time, totalSamples, amplitude); 
//
//			//theSoundSubchunk.data[time] += currentAmplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
//
//			//// Add overtones (First, second, and third overtones)
//			//theSoundSubchunk.data[time] += currentAmplitude * (1.0/3.0) * sin(2 * M_PI * frequency * 2 * time / theFormatHeader.SampleRate); // First overtone
//			//theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 4.0) * sin(2 * M_PI * frequency * 3 * time / theFormatHeader.SampleRate); // Second overtone
//			//theSoundSubchunk.data[time] += currentAmplitude * (1.0 / 5.0) * sin(2 * M_PI * frequency * 4 * time / theFormatHeader.SampleRate); // Third overtone
//
//			//for the magic numbers 0.3, 0.2, 0.1, see `pianoOvertonesToAmplitudeScalingFactors` in a function above
//		}
//	}
//
//	theSoundSubchunk.Subchunk2Size = totalSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
//
//	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
//
//}
