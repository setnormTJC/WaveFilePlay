// WaveFileGeneration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include"DemoingWaveFile.h"

#include"FourierTransform.h"
#include"MyException.h"
#include"Song.h"
#include <random>


std::vector<std::vector<PianoNote>> getMysterySongVocalTrack(const Song& mysterySong)
{
	auto sixteenth = mysterySong.duration(Song::NoteType::sixteenth);
	auto eighth = mysterySong.duration(Song::NoteType::eighth);
	auto dottedEighth = mysterySong.duration(Song::NoteType::dottedEighth);
	auto quarter = mysterySong.duration(Song::NoteType::quarter);
	auto half = mysterySong.duration(Song::NoteType::half);
	auto dottedHalf = mysterySong.duration(Song::NoteType::dottedHalf);
	auto whole = mysterySong.duration(Song::NoteType::whole);

	using Loudness = PianoNote::Loudness;

	std::vector<std::vector<PianoNote>> measures1Through8; //these are ALL silent 
	std::vector<PianoNote> silentNotes = { PianoNote("A0", whole, Loudness::Silent) };
	for (int i = 1; i <= 8; ++i)
	{
		measures1Through8.push_back(silentNotes);
	}
	/*Note that vocals will never be CHORDS with one singer, but ah well*/

	std::vector<std::vector<PianoNote>> measures9Through12 =
	{
		//measure 9
		{PianoNote("B3", quarter, Loudness::Forte)},
		{PianoNote("B3", quarter, Loudness::Forte)},
		{PianoNote("A3", quarter, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth + quarter, Loudness::Forte)}, //ties (NOT a slur -> slur means play a "phrase" with legato style) into measure 10

		//measure 10 (only rests -> she does not go to A0 :) )
		{PianoNote("A0", quarter + half, Loudness::Silent)},

		//measure 11
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},

		{PianoNote("A3", eighth, Loudness::Forte)}, //fi-
		{PianoNote("B3", quarter, Loudness::Forte)},//-nish
		{PianoNote("A3", eighth + quarter, Loudness::Forte)}, //again, ties into next measure

		//measure 12
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
	};

	std::vector<std::vector<PianoNote>> measures13Through16 =
	{
		//measure 13
		{PianoNote("B3", quarter, Loudness::Forte)}, //breaks
		{PianoNote("E3", eighth, Loudness::Forte)},
		{PianoNote("E3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth + quarter, Loudness::Forte)}, //ties into measure 14
		//14
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		//15
		{PianoNote("A3", quarter, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		//16
		{PianoNote("E3", quarter, Loudness::Forte)},
		{PianoNote("A0", quarter + half, Loudness::Silent)}, //rest
	};

	std::vector<std::vector<PianoNote>> measures17Through20 =
	{
		//measure 17
		{PianoNote("B3", eighth, Loudness::Forte)}, //walk-
		{PianoNote("B3", eighth, Loudness::Forte)}, //-ing
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", quarter, Loudness::Forte)},
		{PianoNote("A3", eighth + quarter, Loudness::Forte)}, //ties
		//18
		{PianoNote("A0", quarter + half, Loudness::Silent)}, //rest
		//19
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("D4", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth + quarter, Loudness::Forte)},
		//20
		{PianoNote("A0", quarter, Loudness::Silent) }, //rest
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)}
	};

	std::vector<std::vector<PianoNote>> measures21Through24 =
	{
		//21
		{PianoNote("B3", quarter, Loudness::Forte)},
		{PianoNote("E3", eighth, Loudness::Forte)},
		{PianoNote("E3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth + quarter, Loudness::Forte)}, //tied
		//22
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		//23
		{PianoNote("A3", quarter, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth + quarter, Loudness::Forte)},//tied
		//24
		{PianoNote("A0", sixteenth, Loudness::Silent)}, //16 rest! fancy rhythm
		{PianoNote("A3", sixteenth, Loudness::Forte)}, //more sixteenths!
		{PianoNote("A3", sixteenth, Loudness::Forte)},
		{PianoNote("B3", sixteenth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)}, //back to eighth
		{PianoNote("E3", eighth, Loudness::Forte)}, //back down LOW
		{PianoNote("E3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)}, //E-minor arpeggio here!
	};

	std::vector<std::vector<PianoNote>> vocalTrack; 
	vocalTrack.insert(vocalTrack.end(), measures1Through8.begin(), measures1Through8.end());
	vocalTrack.insert(vocalTrack.end(), measures9Through12.begin(), measures9Through12.end());
	vocalTrack.insert(vocalTrack.end(), measures13Through16.begin(), measures13Through16.end());
	vocalTrack.insert(vocalTrack.end(), measures17Through20.begin(), measures17Through20.end());
	vocalTrack.insert(vocalTrack.end(), measures21Through24.begin(), measures21Through24.end());

	return vocalTrack;
}

std::vector<std::vector<PianoNote>> getMysterySongTrebleTrack(const Song& mysterySong)
{
	auto sixteenth = mysterySong.duration(Song::NoteType::sixteenth);
	auto eighth = mysterySong.duration(Song::NoteType::eighth);
	auto dottedEighth = mysterySong.duration(Song::NoteType::dottedEighth);
	auto quarter = mysterySong.duration(Song::NoteType::quarter);
	auto half = mysterySong.duration(Song::NoteType::half);
	auto dottedHalf = mysterySong.duration(Song::NoteType::dottedHalf);
	auto whole = mysterySong.duration(Song::NoteType::whole);

	using Loudness = PianoNote::Loudness;

	std::vector<std::vector<PianoNote>> measures1Through4 =
	{
		//first measure:
		{PianoChord({"E5", "E6"}, dottedHalf, Loudness::Mezzo).getChordNotes()},
		{PianoChord({"B4", "B5"}, quarter, Loudness::Mezzo).getChordNotes() },
		//second measure: 
		{PianoChord({"G4", "G5"}, whole, Loudness::Mezzo).getChordNotes()},
		//third:
		{PianoChord({"A4", "A5"}, half, Loudness::Mezzo).getChordNotes()},
		{PianoChord({"G4", "G5"}, quarter, Loudness::Mezzo).getChordNotes()},
		{PianoChord({"F#4", "F#5"}, quarter, Loudness::Mezzo).getChordNotes()},
		//fourth
		{PianoChord({"E4", "E5"}, whole, Loudness::Mezzo).getChordNotes()},
	};

	std::vector<std::vector<PianoNote>> measures5Through8 =
	{
		//5th
		{PianoChord({"E4", "G4", "C5", "E5"}, whole,Loudness::Mezzo).getChordNotes()},
		//6th
		PianoChord({ "B3", "E4", "G4", "A4" }, whole, Loudness::Mezzo).getChordNotes(),
		//7th
		PianoChord ({ "G3", "D4", "F#4" }, whole, Loudness::Mezzo).getChordNotes(),
		//8th
		PianoChord({ "G3", "C4", "E4" }, whole, Loudness::Mezzo).getChordNotes(),
	};

	std::vector<std::vector<PianoNote>> measures9Through12 =
	{
		{PianoNote("G3", whole, Loudness::Forte)},
		{PianoNote("F#3", whole, Loudness::Forte)},
		{PianoNote("G3", whole, Loudness::Forte)},
		{PianoNote("F#3", whole, Loudness::Forte)},
	};

	std::vector<std::vector<PianoNote>> measures13Through16 =
	{
		//13
		{PianoNote("G3", half, Loudness::Forte)},
		{PianoNote("E3", half, Loudness::Forte)},
		//14
		{PianoNote("G3", half, Loudness::Forte)},
		{PianoNote("E3", half , Loudness::Forte)},
		//15
		{PianoNote("A3", whole, Loudness::Forte)},
		//16
		PianoChord({"A3", "C4"}, whole, Loudness::Mezzo).getChordNotes()
	};

	std::vector<std::vector<PianoNote>> measures17Through20 =
	{
		{PianoNote("G3", whole, Loudness::Forte)},
		PianoChord({"A3", "C4"}, whole, Loudness::Mezzo).getChordNotes(),
		{PianoNote("G3", whole, Loudness::Forte)},
		{PianoNote("F#3", whole, Loudness::Forte)},
	};

	std::vector<std::vector<PianoNote>> measures21Through24 =
	{
		//21 (same as measure 13, incidentally)
		{PianoNote("G3", half, Loudness::Forte)},
		{PianoNote("E3", half, Loudness::Forte)},
		//22
		{PianoNote("G3", half, Loudness::Forte)},
		{PianoNote("E3", half, Loudness::Forte)},
		//23
		{PianoNote("A3", whole, Loudness::Forte)},
		//24
		{PianoNote("F#3", whole, Loudness::Forte)},
	};

	std::vector<std::vector<PianoNote>> trebleTrack; 
	//insert measures 1 - 4, then 5 - 8, etc.
	trebleTrack.insert(trebleTrack.end(), measures1Through4.begin(), measures1Through4.end());
	trebleTrack.insert(trebleTrack.end(), measures5Through8.begin(), measures5Through8.end());
	trebleTrack.insert(trebleTrack.end(), measures9Through12.begin(), measures9Through12.end());
	trebleTrack.insert(trebleTrack.end(), measures13Through16.begin(), measures13Through16.end());
	trebleTrack.insert(trebleTrack.end(), measures17Through20.begin(), measures17Through20.end());
	trebleTrack.insert(trebleTrack.end(), measures21Through24.begin(), measures21Through24.end());

	return trebleTrack; 
}

std::vector<std::vector<PianoNote>> getMysterySongBassTrack(const Song& mysterySong)
{
	auto sixteenth = mysterySong.duration(Song::NoteType::sixteenth);
	auto eighth = mysterySong.duration(Song::NoteType::eighth);
	auto dottedEighth = mysterySong.duration(Song::NoteType::dottedEighth); 
	auto quarter = mysterySong.duration(Song::NoteType::quarter); 
	auto whole = mysterySong.duration(Song::NoteType::whole);

	using Loudness = PianoNote::Loudness; 

	std::vector<std::vector<PianoNote>> measures1Through4 =
	{
		//first measure: 
		PianoChord({"G3", "C3"}, whole, Loudness::Forte).getChordNotes(),
		//second measure:
		PianoChord({"E3", "G3"}, whole, Loudness::Forte).getChordNotes(),
		//third:
		PianoChord({"D3", "F#3"}, whole, Loudness::Forte).getChordNotes(),
		//fourth measure (a melody):		
		{PianoNote("B3",eighth, Loudness::Forte)}, //(single-element) vector of type PianoNote
		{PianoNote("C4", eighth, Loudness::Forte)} ,
		{PianoNote("D4", eighth, Loudness::Forte)},
		{PianoNote("C4", eighth, Loudness::Forte)},

		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("C4", eighth, Loudness::Forte)},
		{PianoNote("B3", quarter, Loudness::Forte)}
	};
	std::vector<std::vector<PianoNote>> measures5Through8 =
	{
		/*Measure 5:*/
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)}, //1
		{PianoNote("C2", sixteenth + quarter, Loudness::Forte)}, // and? two
		{PianoNote("C2", eighth, Loudness::Forte)}, //3 
		{PianoNote("C2", eighth, Loudness::Forte)}, //and 
		{PianoNote("C2", quarter, Loudness::Fortissimo)}, //4!

		//Measure 6: 
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("E2", eighth, Loudness::Forte)},  
		{PianoNote("E2", eighth, Loudness::Forte)}, 
		{PianoNote("E2", quarter, Loudness::Fortissimo)}, 

		//7
		{PianoNote("D2", dottedEighth, Loudness::Mezzo) },
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)}, 
		{PianoNote("D2", eighth, Loudness::Forte)},  
		{PianoNote("D2", eighth, Loudness::Forte)},  
		{PianoNote("D2", quarter, Loudness::Fortissimo)},

		//8
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Forte)}, 
		{PianoNote("C2", eighth, Loudness::Forte)},  
		{PianoNote("C2", eighth, Loudness::Forte)}, 
		{PianoNote("C2", quarter,Loudness::Fortissimo)} 
	};

	std::vector<std::vector<PianoNote>> measures9Through12 =
	{
		//9:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", quarter,Loudness::Fortissimo)},
		//10:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},

		//11:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", quarter,Loudness::Fortissimo)},
		//12
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},

	};

	std::vector<std::vector<PianoNote>> measures13Through16 =
	{
		//13
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("C2", eighth, Loudness::Forte)},
		{PianoNote("C2", eighth, Loudness::Forte)},
		{PianoNote("C2", quarter,Loudness::Fortissimo)},
		//14:
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("E2", eighth, Loudness::Forte)},
		{PianoNote("E2", eighth, Loudness::Forte)},
		{PianoNote("E2", quarter,Loudness::Fortissimo)},

		//15:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
		//16
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
	};

	std::vector<std::vector<PianoNote>> measures17Through20 =
	{
		//17
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", quarter,Loudness::Fortissimo)},
		//18:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
		//19:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", eighth, Loudness::Forte)},
		{PianoNote("G2", quarter,Loudness::Fortissimo)},
		//20
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
	};

	std::vector<std::vector<PianoNote>> measures21Through24 =
	{
		//17
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("C2", eighth, Loudness::Forte)},
		{PianoNote("C2", eighth, Loudness::Forte)},
		{PianoNote("C2", quarter,Loudness::Fortissimo)},
		//18:
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("E2", eighth, Loudness::Forte)},
		{PianoNote("E2", eighth, Loudness::Forte)},
		{PianoNote("E2", quarter,Loudness::Fortissimo)},
		//19:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
		//20
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", eighth, Loudness::Forte)},
		{PianoNote("D2", quarter,Loudness::Fortissimo)},
	};

	std::vector<std::vector<PianoNote>> bassTrack;

	bassTrack.insert(bassTrack.end(), measures1Through4.begin(), measures1Through4.end());
	bassTrack.insert(bassTrack.end(), measures5Through8.begin(), measures5Through8.end());
	bassTrack.insert(bassTrack.end(), measures9Through12.begin(), measures9Through12.end());
	bassTrack.insert(bassTrack.end(), measures13Through16.begin(), measures13Through16.end());
	bassTrack.insert(bassTrack.end(), measures17Through20.begin(), measures17Through20.end());
	bassTrack.insert(bassTrack.end(), measures21Through24.begin(), measures21Through24.end());

	return bassTrack;
}

int main()
{

	try
	{
		constexpr int tempoBPM = 88;
		constexpr int numberOfMeasures = 16;
		constexpr int numberOfTracks = 3;  
 
		Song mysterySong("mysterySong", tempoBPM, Song::TimeSignature::Common, numberOfMeasures, numberOfTracks);

		/*Making a lambda (per AI suggestion):*/
		auto loadTrack = [&](Song::Track trackType, const std::vector<std::vector<PianoNote>>& notes)
			{
				for (const auto& noteSet : notes)
				{
					mysterySong.addToTrack(trackType, noteSet);
				}
			};

		loadTrack(Song::Track::Vocal, getMysterySongVocalTrack(mysterySong));
		loadTrack(Song::Track::Treble,	getMysterySongTrebleTrack(mysterySong));
		loadTrack(Song::Track::Bass,	 getMysterySongBassTrack(mysterySong));

		//checking for misalignment problems: 
		auto trackDurations = mysterySong.getTrackDurations(); 
		auto trackNames = mysterySong.getTrackNames(); 
		for (int i = 0; i < trackDurations.size(); ++i)
		{
			std::cout << trackNames[i] << " lasts for : " << trackDurations[i] << " seconds\n";
		}

		auto tracks = mysterySong.getTracks();
		
		/*Making the WaveFile object*/
		WaveFile wavefile(tracks.at(0)); //vocal 
		wavefile.addTrack(tracks.at(1)); //treble 
		wavefile.addTrack(tracks.at(2)); //bass

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

