#include "SongNote.h"

SongNote::SongNote(const std::string& name, const float durationInSeconds, Loudness amplitude)
	:name(name), durationInSeconds(durationInSeconds), amplitude(amplitude)
{

}
