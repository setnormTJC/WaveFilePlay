#define _USE_MATH_DEFINES //for M_PI in Fourier Transform 

#include "FourierTransform.h"
#include"MyException.h"

FourierTransform::FourierTransform(const std::string& waveFilename)
{

	WaveFile waveFile(waveFilename);

	soundWave = waveFile.getSoundWave();

	//AI recommends NORMALIZING sound wave data before submitting for FT analysis: 
	
	// Find the maximum absolute value in the sound wave
	short maxAmplitude = *std::max_element(soundWave.begin(), soundWave.end(),
		[](short a, short b) { return std::abs(a) < std::abs(b); });

	if (maxAmplitude == 0) return; // Avoid division by zero

	for (auto& sample : soundWave) {
		sample = static_cast<short>((sample * 32767) / maxAmplitude);
	}

}

void FourierTransform::fillTransformDataAndFrequencyMap()
{
	std::cout << "Generating FT from minFrequency = " << minFrequency << " hz to maxFrequency = "
		<< maxFrequency << " hz.\n";

	const int N = soundWave.size();

	//const int binCount = N / 2; // FT result is symmetric, only need the first half

	std::vector<double> realPartOfTransform(maxFrequency - minFrequency, 0.0); //the second arg inits all elements to 0.0
	std::vector<double> imagPartOfTransform(maxFrequency - minFrequency, 0.0);

	//use the approach below for possibly more accurate FT
	//std::vector<double> realPartOfTransform(binCount, 0.0); //the second arg inits all elements to 0.0
	//std::vector<double> imagPartOfTransform(binCount, 0.0);


	for (int k = 0; k < realPartOfTransform.size(); ++k) //k is each frequency value in the transform 
	{
		for (int n = 0; n < N - 1; ++n) //n is each time point in sound wave 
		{
			//double window = 0.5 * (1 - cos(2 * M_PI * n / (N - 1))); // Hanning window
			double angle = 2 * M_PI * k * n / N;

			realPartOfTransform[k] += soundWave[n] * cos(angle);
			imagPartOfTransform[k] -= soundWave[n] * sin(angle);
		}
	}

	//std::vector<double> transform(maxFrequency - minFrequency, 0.0);
	//theTransformData.resize(maxFrequency - minFrequency); 
	theTransformData.assign(maxFrequency - minFrequency, 0.0);

	for (int k = 0; k < theTransformData.size(); ++k)
	{
		theTransformData[k] = sqrt(realPartOfTransform[k] * realPartOfTransform[k] +
			imagPartOfTransform[k] * imagPartOfTransform[k]);

		//also fill the map: 
		frequenciesToAmplitudes.insert({ k * samplingRate / N, theTransformData[k] });
		//frequenciesToAmplitudes.insert({ k, theTransformData[k] }); //WRONG!!
	}

}

void FourierTransform::displayFTPlot(const std::string& bitmapFilename) const
{
	if (bitmapFilename.find(".bmp") == string::npos)
		throw MyException("plot file name MUST include .bmp", __FILE__, __LINE__);

	PlotImage plot(1'000, 1'000, Color(ColorEnum::Black));

	//std::map<int, int> subMap; 
	//auto it = frequenciesToAmplitudes.begin();

	//for (int i = 0; i < 1000 && it != frequenciesToAmplitudes.end(); ++i, ++it) {
	//	subMap.insert(*it); // Insert each element into subMap
	//}
	
	plot.plotData(frequenciesToAmplitudes, ColorEnum::Cyan);

	plot.writeImageFile(bitmapFilename); 

	system(bitmapFilename.c_str());
	
}

void FourierTransform::writeFTMapToCSV(const std::string& csvFilename) const
{
	if (csvFilename.find(".csv") == std::string::npos)
		throw MyException("filename must contain .csv", __FILE__, __LINE__);

	std::ofstream fout(csvFilename); 

	for (const auto& [frequency, amplitude] : frequenciesToAmplitudes)
	{
		fout << frequency << "," << amplitude << "\n";

		if (amplitude > 1000) //just for temporary visualization 
		{
			//std::cout << frequency << "," << amplitude << "\n";
		}
	}

	fout.close(); 


}
