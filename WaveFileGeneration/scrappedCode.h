#pragma once

/*The notes and their frequencies in Hz from C3 to C5*/
//std::map<std::string, float, NoteComparator> notesToFrequencies = 
//{
//	{"C3", 130.81f}, {"C#3", 138.59f}, {"D3", 146.83f}, {"D#3", 155.56f}, {"E3", 164.81f}, {"F3", 174.61f},
//	{"F#3", 185.00f}, {"G3", 196.00f}, {"G#3", 207.65f}, {"A3", 220.00f}, {"A#3", 233.08f}, {"B3", 246.94f},
//	{"C4", 261.63f}, {"C#4", 277.18f}, {"D4", 293.66f}, {"D#4", 311.13f}, {"E4", 329.63f}, {"F4", 349.23f},
//	{"F#4", 369.99f}, {"G4", 392.00f}, {"G#4", 415.30f}, {"A4", 440.00f}, {"A#4", 466.16f}, {"B4", 493.88f},
//	{"C5", 523.25f}
//};


/*plays C3 -> C5 with sin wave and half second duration*/
//WaveFile::WaveFile()
//{
//
//	const int NumSamples = (theFormatHeader.SampleRate / 2) * notesToFrequencies.size();
//	//makes each note in notesToFrequencies sound for 0.5 seconds 
//
//	theSoundSubchunk.data.resize(NumSamples);
//
//	constexpr float amplitude = 32'767;
//
//	auto it = notesToFrequencies.begin();
//
//	float frequency = notesToFrequencies.at(notesToFrequencies.begin()->first); //Hz
//
//	for (int time = 0; time < NumSamples; ++time)
//	{
//		if (time % (theFormatHeader.SampleRate / 2) == 0)
//		{
//			if (it != notesToFrequencies.end()) //safety check
//			{
//				//update frequency to next note: 
//				frequency = it->second;
//				++it;
//			}
//
//		}
//
//		theSoundSubchunk.data[time] = amplitude * sin(2 * 3.141592 * frequency * time / theFormatHeader.SampleRate);
//
//	}
//
//	theSoundSubchunk.Subchunk2Size = NumSamples * theFormatHeader.NumChannels * (theFormatHeader.BitsPerSample / 8);
//
//	theRiffHeader.ChunkSize = 4 + (8 + theFormatHeader.Subchunk1Size) + (8 + theSoundSubchunk.Subchunk2Size);
//}
