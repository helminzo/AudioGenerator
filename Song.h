#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>
#include<fstream>
#include<vector>



class Song
{
private:
	typedef struct Note {
		std::string note;
		short octave;
		double duration;
	}Note;
public:
	//For songs
	Song(std::string& NoteString, short bpm, double volume);
	Song(std::ifstream& NoteFile, short bpm, double volume);
	void playSong();
	bool transpose(int transposition);

	//For functions
	Song(short(*func)(double), int time);
	~Song();
	
private:
	const int SAMPLE_RATE = 44100;
	short _bpm;
	double _vol;
	std::vector<Note> notes;
	std::vector<sf::SoundBuffer> noteSoundBuffers;
	std::string ChromaticNotesSharp[12] = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	std::string ChromaticNotesFlat[12] = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };
	std::string circleOfFifths[12] = { "F", "C", "G", "D", "A", "E", "B", "Gb", "Db", "Ab", "Eb", "Bb" };


	friend std::ostream& operator<<(std::ostream& stream, Note note); 
	short SineWave(double time, double freq, double amplitude);
	sf::SoundBuffer GetSoundBufferFromNote(Note note, double beats);
	bool loadSoundBuffers();
	
};

