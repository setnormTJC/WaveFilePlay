// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"
#include"Song.h"
#include <random>


void playMysterySong()
{
	constexpr int tempo = 88; //
	auto mysterySongNotes = MusicMaking::getMysterySongNotes(tempo);

	WaveFile wavefile(mysterySongNotes.first); //construct with the treble track first 

	//now add bass track 
	wavefile.addTrack(mysterySongNotes.second);

	std::string filename = "mysterySong.wav";
	wavefile.writeToWaveFile(filename);

	system(filename.c_str());

}

std::vector<std::vector<PianoNote>> getMysterySongTrebleTrack(const Song& mysterySong)
{
	std::vector<std::vector<PianoNote>> trebleTrack =
	{
		//first measure:
		{PianoChord({"E5", "E6"}, mysterySong.duration(Song::NoteType::dottedHalf), PianoNote::Loudness::Mezzo).getChordNotes()},
		{PianoChord({"B4", "B5"}, mysterySong.duration(Song::NoteType::quarter), PianoNote::Loudness::Mezzo).getChordNotes() },
		//second measure: 
		{PianoChord({"G4", "G5"}, mysterySong.duration(Song::NoteType::whole), PianoNote::Loudness::Mezzo).getChordNotes()},
		//third:
		{PianoChord({"A4", "A5"}, mysterySong.duration(Song::NoteType::half), PianoNote::Loudness::Mezzo).getChordNotes()},
		{PianoChord({"G4", "G5"}, mysterySong.duration(Song::NoteType::quarter), PianoNote::Loudness::Mezzo).getChordNotes()},
		{PianoChord({"F#4", "F#5"}, mysterySong.duration(Song::NoteType::quarter), PianoNote::Loudness::Mezzo).getChordNotes()},
		//fourth
		{PianoChord({"E4", "E5"}, mysterySong.duration(Song::NoteType::whole), PianoNote::Loudness::Mezzo).getChordNotes()},

	};

	return trebleTrack; 
}

std::vector<std::vector<PianoNote>> getMysterySongBassTrack(const Song& mysterySong)
{
	auto whole = mysterySong.duration(Song::NoteType::whole);
	auto eighth = mysterySong.duration(Song::NoteType::eighth);
	auto quarter = mysterySong.duration(Song::NoteType::quarter); 

	std::vector<std::vector<PianoNote>> bassTrack =
	{
		//first measure: 
		PianoChord({"G3", "C3"}, whole, PianoNote::Loudness::Forte).getChordNotes(),
		//second measure:
		PianoChord({"E3", "G3"}, whole, PianoNote::Loudness::Forte).getChordNotes(),
		//third:
		PianoChord({"D3", "F#3"}, whole, PianoNote::Loudness::Forte).getChordNotes(),
		//fourth measure (a melody):		
		{PianoNote("B3",eighth, PianoNote::Loudness::Forte)}, //(single-element) vector of type PianoNote
		{PianoNote("C4", eighth, PianoNote::Loudness::Forte)} ,
		{PianoNote("D4", eighth, PianoNote::Loudness::Forte)},
		{PianoNote("C4", eighth, PianoNote::Loudness::Forte)},

		{PianoNote("B4", eighth, PianoNote::Loudness::Forte)},
		{PianoNote("C4", eighth, PianoNote::Loudness::Forte)},
		{PianoNote("B4", quarter, PianoNote::Loudness::Forte)}
	};

	return bassTrack;
}

int main()
{

	try
	{
		constexpr int tempoBPM = 88;
		constexpr int numberOfMeasures = 16;
		constexpr int numberOfTracks = 2;  //for now!
 
		Song mysterySong("mysterySong", tempoBPM, Song::TimeSignature::Common, numberOfMeasures, numberOfTracks);

		std::vector<std::vector<PianoNote>> trebleTrack = getMysterySongTrebleTrack(mysterySong);

		for (const std::vector<PianoNote>& notes : trebleTrack)
			mysterySong.addToTrack(Song::Track::Treble, notes);

		//same for bass notes ... (might use loop if every having > 3 tracks) 
		std::vector<std::vector<PianoNote>> bassTrack = getMysterySongBassTrack(mysterySong);
		for (const std::vector<PianoNote>& notes : bassTrack)
			mysterySong.addToTrack(Song::Track::Bass, notes);

		auto tracks = mysterySong.getTracks();

		WaveFile wavefile(tracks.at(0)); //top track 
		wavefile.addTrack(tracks.at(1)); //bottom track 


		const std::string filename = "fullerMysterySong.wav";
		wavefile.writeToWaveFile(filename);

		system(filename.c_str());
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

