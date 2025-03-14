// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"



int main()
{

	try
	{
		//FourierTransform::transform1SecondOf100Hz(); 
		//const std::string filename = "C4E4G4B4.wav";
		//expected max frequencies: 262, 330, 392, 494

		//const std::string filename = "me.wav";
		// 
		//const std::string filename = "d4VirtualPiano.wav";


		//std::vector<double> transform = FourierTransform::getTransformOfWaveFile(filename);
		//MusicMaking::chatGPTTriesLaFilleAuxCheveuxDeLin(); 


		WaveFile waveFile(PianoNote("C4", 2.0, PianoNote::Loudness::Forte), WaveFile::WaveType::FancyInstrument);

		const char* waveFilename = "C4_withExponentialDecayEnvelope.wav";

		waveFile.writeToWaveFile(waveFilename);
		
		std::string plotImageFilename = "plotImage.bmp";

		system(waveFilename);

		//waveFile.writeSoundDataToImagePlot(plotImageFilename);

		//system(plotImageFilename.c_str());

		//system(filename); 
	}
	
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

