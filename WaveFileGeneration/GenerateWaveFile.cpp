// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"


int main()
{

	try
	{
	
		std::string baseFilename = "TimpaniC2";
		//wavefile.writeToWaveFile(baseFilename + ".wav");

		FourierTransform ft(baseFilename + ".wav");

		ft.fillTransformDataAndFrequencyMap(); 
	
		ft.writeFTMapToCSV(baseFilename + ".csv");

		std::string callPythonPlottingScript = "python plotTheData.py " + baseFilename + ".csv";
		system(callPythonPlottingScript.c_str());
	}
	
	catch (const MyException& e)
	{
		std::cout << e.what() << "\n";
	}

	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
	
	catch (...)
	{
		std::cout << "Some other? exception type got thrown? \n";
	}
}

