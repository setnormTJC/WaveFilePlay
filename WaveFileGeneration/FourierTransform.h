#pragma once

#include "WaveFile.h"

class FourierTransform
{
private: 

	std::vector<short> soundWave;
	std::vector<double> theTransformData;
	/*N.B. the amplitudes originally calculated by FT are doubles -> I am casting to INTs here*/
	std::map<int, int> frequenciesToAmplitudes;

	static constexpr int minFrequency = 0; //20 Hz is around the lowest frequency audible to humans - setting this to 0 to avoid "shifting": 
	static constexpr int maxFrequency = 5'000; //A "little" higher than C8 -> probably an okay cutoff 

	static constexpr float samplingRate = 44'100.0f;

public: 
	FourierTransform() = delete; 
	/*fills the private member var `soundWave` by calling `WaveFile` constructor with waveFilename*/
	explicit FourierTransform(const std::string& waveFilename);
	explicit FourierTransform(std::vector<short>& soundWave);
	FourierTransform(const FourierTransform& other) = delete; 

	FourierTransform(FourierTransform&& other) = delete;

	void fillTransformDataAndFrequencyMap(); 

	void writeFTMapToCSV(const std::string& csvFilename) const; 


};

