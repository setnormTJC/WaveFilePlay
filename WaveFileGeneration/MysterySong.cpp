#include "MysterySong.h"
#include"WaveFile.h"


std::vector<std::vector<PianoNote>> MysterySong::getMysterySongVocalTrack(const Song& mysterySong)

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

	std::vector<std::vector<PianoNote>> measures25Through28 =
	{
		//25
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		{PianoNote("G3", quarter, Loudness::Forte)},
		{PianoNote("G3", quarter, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth + quarter, Loudness::Fortissimo)}, //ties
		//26
		{PianoNote("E4", quarter, Loudness::Fortissimo)}, //HIGHEST vocal note so far!
		{PianoNote("D4", quarter, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth + quarter, Loudness::Forte)}, //ties
		//27
		{PianoNote("A3", dottedEighth, Loudness::Forte)}, //begin fancy vocal rhythm
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		//28
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("A3", sixteenth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)}
	};

	std::vector<std::vector<PianoNote>> measures29Through32 =
	{
		//29
		{PianoNote("E3", quarter, Loudness::Mezzo)}, //"pops" if forte (also, add some vocal variation)
		{PianoNote("G3", quarter, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth + quarter, Loudness::Forte)}, //ties
		//30
		{PianoNote("E4", quarter, Loudness::Fortissimo)}, //again, highest note so far 
		{PianoNote("E4", eighth, Loudness::Fortissimo)},
		{PianoNote("B3", quarter, Loudness::Forte)},
		{PianoNote("A3", eighth + dottedEighth, Loudness::Forte)}, //quite interesting rhythmic tie
		//31
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},

		//32
		{PianoNote("A3", dottedEighth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
	};

	std::vector<std::vector<PianoNote>> measures33Through36 =
	{
		//33
		{PianoNote("F#3", quarter, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)}, //vocal leap!
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},

		{PianoNote("E4", eighth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("F#4", eighth, Loudness::Forte)},
		{PianoNote("G4", eighth + quarter, Loudness::Fortissimo)}, //HIGHEST note so far -> ties 
		//34
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},

		{PianoNote("E4", eighth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("E4", sixteenth, Loudness::Forte)},
		{PianoNote("F#4", eighth, Loudness::Forte)},
		{PianoNote("A4", eighth, Loudness::Fortissimo)}, //even higher!
		//35
		{PianoNote("F#4", quarter, Loudness::Forte)},
		{PianoNote("A0", quarter + sixteenth, Loudness::Silent)}, //REST - quarter + sixteenth
		{PianoNote("F#4", sixteenth, Loudness::Forte)}, //you 
		{PianoNote("F#4", sixteenth, Loudness::Forte)}, //like 
		{PianoNote("E4", sixteenth, Loudness::Forte)}, //your 
		{PianoNote("F#4", eighth, Loudness::Forte)}, //girls 
		{PianoNote("F#4", eighth, Loudness::Forte)}, //in- 
		//36
		{PianoNote("F#4", eighth, Loudness::Forte)}, //SAY-
		{PianoNote("E4", eighth + quarter, Loudness::Forte)},
		{PianoNote("A0", half, Loudness::Forte)}, //rest (well-deserved)
	};

	std::vector<std::vector<PianoNote>> measures37Through41 =
	{
		//37
		{PianoNote("A0", quarter, Loudness::Silent)}, //more well-deserved rest :) 
		{PianoNote("G3", quarter, Loudness::Forte)},
		{PianoNote("G3", eighth, Loudness::Forte)},
		{PianoNote("A3", quarter, Loudness::Forte)},
		{PianoNote("B3", eighth + quarter, Loudness::Forte)}, //ties
		//38
		{PianoNote("E4", eighth, Loudness::Forte)},
		{PianoNote("E4", eighth, Loudness::Forte)},
		{PianoNote("D4", quarter, Loudness::Forte)},
		{PianoNote("B3", eighth, Loudness::Forte)},
		{PianoNote("A3", eighth + quarter, Loudness::Forte)}, //ties
		//39
		{PianoNote("A0", quarter + dottedEighth, Loudness::Silent)}, //another interesting rest rhythm
		{PianoNote("G3", sixteenth, Loudness::Mezzo)}, //quieting for drama
		{PianoNote("A3", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G3", sixteenth, Loudness::Mezzo)},
		//40
		{PianoNote("A3", quarter, Loudness::Forte)},
		{PianoNote("A0", quarter + eighth, Loudness::Silent)},
		{PianoNote("A3", sixteenth, Loudness::Forte)},
		{PianoNote("G3", sixteenth, Loudness::Forte)},
		{PianoNote("A3", sixteenth + sixteenth, Loudness::Forte)}, //ties
		{PianoNote("E3", sixteenth, Loudness::Forte)}, //to
		{PianoNote("E3", sixteenth + quarter, Loudness::Forte)}, //die  (ties)
		//41
		{PianoNote("A0", quarter + half, Loudness::Silent)}
	};

	std::vector<std::vector<PianoNote>> vocalTrack;
	vocalTrack.insert(vocalTrack.end(), measures1Through8.begin(), measures1Through8.end());
	vocalTrack.insert(vocalTrack.end(), measures9Through12.begin(), measures9Through12.end());
	vocalTrack.insert(vocalTrack.end(), measures13Through16.begin(), measures13Through16.end());
	vocalTrack.insert(vocalTrack.end(), measures17Through20.begin(), measures17Through20.end());
	vocalTrack.insert(vocalTrack.end(), measures21Through24.begin(), measures21Through24.end());
	vocalTrack.insert(vocalTrack.end(), measures25Through28.begin(), measures25Through28.end());

	vocalTrack.insert(vocalTrack.end(), measures29Through32.begin(), measures29Through32.end());
	vocalTrack.insert(vocalTrack.end(), measures33Through36.begin(), measures33Through36.end());
	vocalTrack.insert(vocalTrack.end(), measures37Through41.begin(), measures37Through41.end());

	return vocalTrack;
}

std::vector<std::vector<PianoNote>> MysterySong::getMysterySongTrebleTrack(const Song& mysterySong)

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
		PianoChord({ "G3", "D4", "F#4" }, whole, Loudness::Mezzo).getChordNotes(),
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

	std::vector<std::vector<PianoNote>> measures25Through28 =
	{
		//25
		PianoChord(
			{
				PianoNote("G3", dottedHalf, Loudness::Mezzo),
				PianoNote("E3", quarter, Loudness::Piano),
				PianoNote("C4", dottedHalf, Loudness::Piano) //pretty complex bit in the sheet music ... 
			}
		).getChordNotes(),
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
			//26
			PianoChord({"E3", "G3"}, whole, Loudness::Mezzo).getChordNotes(),
			//27
			{PianoNote("F#3", whole, Loudness::Mezzo)},
			//28
			PianoChord({"E3", "C4"}, whole, Loudness::Mezzo).getChordNotes()
	};

	std::vector<std::vector<PianoNote>> measures29Through32 = measures25Through28;
	//measures 29 - 32 are the same as 25 - 29!

	std::vector<std::vector<PianoNote>> measures33Through36 =
	{
		//33
		PianoChord({"G3", "C4"}, dottedHalf, Loudness::Mezzo).getChordNotes(),
		//reducing volume for benefit of drawing attention to vocal track at this measure 
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		//34
		PianoChord({"G3", "B3"}, dottedHalf, Loudness::Mezzo).getChordNotes(),
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		//35
		PianoChord({"A3", "D4"}, half, Loudness::Mezzo).getChordNotes(),
		PianoChord({"F#3", "A3"}, quarter, Loudness::Mezzo).getChordNotes(),
		PianoChord({"F#3", "A3"}, quarter, Loudness::Mezzo).getChordNotes(),
		//36
		PianoChord({"G3", "C4"}, whole, Loudness::Mezzo).getChordNotes()
	};

	std::vector<std::vector<PianoNote>> measures37Through41 =
	{
		//37
		PianoChord({"E3", "C4"}, whole, Loudness::Mezzo).getChordNotes(),
		//38
		{PianoNote("G3", whole, Loudness::Mezzo)},
		//39
		PianoChord({"F#3", "D4"}, whole, Loudness::Mezzo).getChordNotes(),
		//40
		PianoChord({"G3", "C4"}, half, Loudness::Mezzo).getChordNotes(),
		{PianoNote("C4", quarter, Loudness::Mezzo)},
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		//41
		{PianoNote("A0", whole, Loudness::Silent) }
	};

	std::vector<std::vector<PianoNote>> trebleTrack;
	//insert measures 1 - 4, then 5 - 8, etc.
	trebleTrack.insert(trebleTrack.end(), measures1Through4.begin(), measures1Through4.end());
	trebleTrack.insert(trebleTrack.end(), measures5Through8.begin(), measures5Through8.end());
	trebleTrack.insert(trebleTrack.end(), measures9Through12.begin(), measures9Through12.end());
	trebleTrack.insert(trebleTrack.end(), measures13Through16.begin(), measures13Through16.end());
	trebleTrack.insert(trebleTrack.end(), measures17Through20.begin(), measures17Through20.end());
	trebleTrack.insert(trebleTrack.end(), measures21Through24.begin(), measures21Through24.end());
	trebleTrack.insert(trebleTrack.end(), measures25Through28.begin(), measures25Through28.end());

	trebleTrack.insert(trebleTrack.end(), measures29Through32.begin(), measures29Through32.end());
	trebleTrack.insert(trebleTrack.end(), measures33Through36.begin(), measures33Through36.end());
	trebleTrack.insert(trebleTrack.end(), measures37Through41.begin(), measures37Through41.end());

	return trebleTrack;
}

std::vector<std::vector<PianoNote>> MysterySong::getMysterySongBassTrack(const Song& mysterySong)

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
		PianoChord({"G3", "C3"}, whole, Loudness::Mezzo).getChordNotes(),
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
		{PianoNote("C2", sixteenth + quarter, Loudness::Mezzo)}, // and? two
		{PianoNote("C2", eighth, Loudness::Mezzo)}, //3 
		{PianoNote("C2", eighth, Loudness::Mezzo)}, //and 
		{PianoNote("C2", quarter, Loudness::Mezzo)}, //4!

		//Measure 6: 
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", quarter, Loudness::Mezzo)},

		//7
		{PianoNote("D2", dottedEighth, Loudness::Mezzo) },
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter, Loudness::Mezzo)},

		//8
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", quarter,Loudness::Mezzo)}
	};

	std::vector<std::vector<PianoNote>> measures9Through12 =
	{
		//9:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", quarter,Loudness::Mezzo)},
		//10:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},

		//11:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", quarter,Loudness::Mezzo)},
		//12
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},

	};

	std::vector<std::vector<PianoNote>> measures13Through16 =
	{
		//13
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", quarter,Loudness::Mezzo)},
		//14:
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", quarter,Loudness::Mezzo)},

		//15:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
		//16
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
	};

	std::vector<std::vector<PianoNote>> measures17Through20 =
	{
		//17
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", quarter,Loudness::Mezzo)},
		//18:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
		//19:
		{PianoNote("G2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("G2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", eighth, Loudness::Mezzo)},
		{PianoNote("G2", quarter,Loudness::Mezzo)},
		//20
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
	};

	std::vector<std::vector<PianoNote>> measures21Through24 =
	{
		//17
		{PianoNote("C2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("C2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", eighth, Loudness::Mezzo)},
		{PianoNote("C2", quarter,Loudness::Mezzo)},
		//18:
		{PianoNote("E2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("E2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", eighth, Loudness::Mezzo)},
		{PianoNote("E2", quarter,Loudness::Mezzo)},
		//19:
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
		//20
		{PianoNote("D2", dottedEighth, Loudness::Mezzo)},
		{PianoNote("D2", sixteenth + quarter, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", eighth, Loudness::Mezzo)},
		{PianoNote("D2", quarter,Loudness::Mezzo)},
	};

	std::vector<std::vector<PianoNote>> measures25Through28 =
	{
		//25
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + quarter, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, quarter, Loudness::Mezzo).getChordNotes(),
		//26
		PianoChord("E2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//27
		PianoChord("D2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//28
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + quarter, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, quarter, Loudness::Mezzo).getChordNotes(),
	};

	std::vector<std::vector<PianoNote>> measures29Through32 = measures25Through28;
	//As in the treble clef, measures 29 - 32 are the same as 25 - 29!

	std::vector<std::vector<PianoNote>> measures33Through36 =
	{
		//33
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//34
		PianoChord("E2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//35
		PianoChord("D2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//36
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
	};

	std::vector<std::vector<PianoNote>> measures37Through41 =
	{
		//37
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//38
		PianoChord("E2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("E2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//39
		PianoChord("D2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, sixteenth + quarter + eighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("D2", PianoChord::ChordType::Octave, eighth + quarter, Loudness::Mezzo).getChordNotes(),
		//40
		PianoChord("C2", PianoChord::ChordType::Octave, dottedEighth, Loudness::Mezzo).getChordNotes(),
		PianoChord("C2", PianoChord::ChordType::Octave, sixteenth + quarter + quarter, Loudness::Mezzo).getChordNotes(),
		{PianoNote("A0", quarter, Loudness::Silent)}, //rest
		//41 (note the odd!) 
		{PianoNote("A0", whole, Loudness::Silent) }
	};

	std::vector<std::vector<PianoNote>> bassTrack;

	bassTrack.insert(bassTrack.end(), measures1Through4.begin(), measures1Through4.end());
	bassTrack.insert(bassTrack.end(), measures5Through8.begin(), measures5Through8.end());
	bassTrack.insert(bassTrack.end(), measures9Through12.begin(), measures9Through12.end());
	bassTrack.insert(bassTrack.end(), measures13Through16.begin(), measures13Through16.end());
	bassTrack.insert(bassTrack.end(), measures17Through20.begin(), measures17Through20.end());
	bassTrack.insert(bassTrack.end(), measures21Through24.begin(), measures21Through24.end());
	bassTrack.insert(bassTrack.end(), measures25Through28.begin(), measures25Through28.end());

	bassTrack.insert(bassTrack.end(), measures29Through32.begin(), measures29Through32.end());
	bassTrack.insert(bassTrack.end(), measures33Through36.begin(), measures33Through36.end());
	bassTrack.insert(bassTrack.end(), measures37Through41.begin(), measures37Through41.end());

	return bassTrack;
}

void MysterySong::getAndPlayFullSong()
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
	loadTrack(Song::Track::Treble, getMysterySongTrebleTrack(mysterySong));
	loadTrack(Song::Track::Bass, getMysterySongBassTrack(mysterySong));

	//checking for misalignment problems: 
	auto trackDurations = mysterySong.getTrackDurations();
	auto trackNames = mysterySong.getTrackNames();
	for (int i = 0; i < trackDurations.size(); ++i)
	{
		std::cout << trackNames[i] << " lasts for : " << trackDurations[i] << " seconds\n";
	}

	/*Optional, for funsies*/
	auto minAndMaxFrequencies = mysterySong.findMinAndMaxFrequenciesInSong(); 
	std::cout << "Min frequency: " << minAndMaxFrequencies.first << "\n";
	std::cout << "Max frequency: " << minAndMaxFrequencies.second << "\n";

	auto tracks = mysterySong.getTracks();

	/*Making the WaveFile object*/
	WaveFile wavefile(tracks.at(0)); //vocal 
	wavefile.addTrack(tracks.at(1)); //treble 
	wavefile.addTrack(tracks.at(2)); //bass

	const std::string filename = "fullerMysterySong.wav";
	wavefile.writeToWaveFile(filename);

	system(filename.c_str());
}
