#pragma once
#include"Song.h"


namespace MysterySong
{
	std::vector<std::vector<PianoNote>> getMysterySongVocalTrack(const Song& mysterySong);
	
	std::vector<std::vector<PianoNote>> getMysterySongTrebleTrack(const Song& mysterySong);

	std::vector<std::vector<PianoNote>> getMysterySongBassTrack(const Song& mysterySong);

	void getAndPlayFullSong(); 
}
