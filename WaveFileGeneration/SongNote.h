#pragma once
#include <string>
class SongNote
{
public:
	enum class Loudness : int16_t //int16_t expected by 16 bit WAV
	{
		Piano = 32'767 / 4,
		Mezzo = 32'767 / 2,
		Forte = 32'767 / 1
	};

	std::string name; //ex: A4, C#3, Gb4, etc.
	float duration; 
	Loudness amplitude; 
};

