#include "Song.h"

Song::Song(const std::string& songName, const int tempoBPM, const TimeSignature timeSignature, const int numberOfMeasures
	, const int numberOfTracks)
	:songName(songName), tempoBPM(tempoBPM), timeSignature(timeSignature), numberOfMeasures(numberOfMeasures)
{
	mapNoteTypesToDurations(tempoBPM, timeSignature); 

	tracks.resize(numberOfTracks); 

}

float Song::duration(const NoteType noteType) const 
{
	if (noteTypesToDurations.find(noteType) == noteTypesToDurations.end())
		throw MyException("note type (ex: quarterNote) not found", __FILE__, __LINE__);
	return noteTypesToDurations.at(noteType);
}



void Song::mapNoteTypesToDurations(const int tempoBPM, const TimeSignature timeSignature)
{
	float secondsPerMinute = 60.0f;
	float secondsPerBeat = secondsPerMinute / tempoBPM;
	/*the duration of the note type*/
	float halfDuration, quarterDuration, eighthDuration;

	switch (timeSignature)
	{
	case TimeSignature::Common:
	case TimeSignature::Triple:
	case TimeSignature::Duple:
		quarterDuration = secondsPerBeat; //bit of a goofy way to do this ... but ah well 
		halfDuration = quarterDuration * 2.0f; 
		eighthDuration = quarterDuration / 2.0f; 
		break;

	case TimeSignature::Cut: 
		halfDuration = secondsPerBeat;
		quarterDuration = halfDuration / 2.0f; 
		eighthDuration = halfDuration / 4.0f; 
		break; 

	case TimeSignature::Compound6over8:
		eighthDuration = secondsPerBeat;
		quarterDuration = eighthDuration * 2.0f; 
		halfDuration = eighthDuration * 4.0f; 
		break; 

	default: 
		throw MyException("Unsupported time signature", __FILE__, __LINE__);
	}

	noteTypesToDurations.insert({ NoteType::sixteenth, eighthDuration/2.0f });
	noteTypesToDurations.insert({ NoteType::dottedSixteenth, eighthDuration / 1.5f});
	noteTypesToDurations.insert({ NoteType::eighth, eighthDuration });
	noteTypesToDurations.insert({ NoteType::dottedEighth, eighthDuration * 1.5f });

	noteTypesToDurations.insert({ NoteType::quarter, quarterDuration });
	noteTypesToDurations.insert({ NoteType::dottedQuarter, quarterDuration * 1.5f });

	noteTypesToDurations.insert({ NoteType::half, halfDuration });
	noteTypesToDurations.insert({ NoteType::dottedHalf, halfDuration * 1.5f });
	noteTypesToDurations.insert({ NoteType::whole, halfDuration * 2.0f });
}


std::vector<std::vector<std::vector<PianoNote>>> Song::getTracks()
{
	return tracks; 
}

void Song::addToTrack(const Song::Track track, const std::vector<PianoNote>& dataToAdd) 
												//changed from vector<PianoChord>
{
	std::vector<std::vector<PianoNote>>& trackMeasures = tracks[track];

	trackMeasures.push_back(dataToAdd);

	/*Less important map stuff below*/
	switch (track) //switching tracks (like a choo-choo train)
	{
	case Track::Vocal:
		trackNumbersToNames[static_cast<int>(track)] = "Vocal track";
		break;

	case Track::Treble:
		trackNumbersToNames[static_cast<int>(track)] = "Treble track";
		break;

	case Track::Bass:
		trackNumbersToNames[static_cast<int>(track)] = "Bass track";
		break;

	default: 
		throw MyException("attempted to add unsupported track type", __FILE__, __LINE__);
	}
}

std::vector<float> Song::getTrackDurations()
{
	std::vector<float> durations; 
	for (const auto& notes : tracks)
	{
		float totalDuration = 0.0f;
		for (const auto& thing1 : notes) //good variable name here
		{
			totalDuration += thing1.at(0).durationInSeconds;
		}
		durations.push_back(totalDuration); 
	}
	return durations; 
}

std::vector<std::string> Song::getTrackNames()
{
	if (trackNumbersToNames.size() == 0)
		throw MyException("track numbers to names map is empty!", __FILE__, __LINE__);

	std::vector<std::string> trackNames; 
	for (const auto& [trackNumber, trackName] : trackNumbersToNames)
	{
		trackNames.push_back(trackName);
	}

	return trackNames;
}





