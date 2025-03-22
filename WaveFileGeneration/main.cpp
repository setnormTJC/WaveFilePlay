// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"
#include"MysterySong.h"
#include"Song.h"
#include <random>



int main()
{

	try
	{
		MysterySong::getAndPlayFullSong(); 
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

