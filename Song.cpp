#include "Song.h"
#include<iostream>
#include<windows.h>
#include<cmath>
#include<math.h>
#include<stdlib.h>
#define TWOPI 6.283185307

Song::Song(std::string& NoteString, short bpm, double volume) : _bpm(bpm), _vol(volume) 
{
	int n = NoteString.length();
	double noteDur; // Duration of individual note
	std::string OneNote = "";
	for (int i = 0; i < n; i++) {
		noteDur = 1.f;
		if ((NoteString[i] >= 'A' && NoteString[i] <= 'G') || (NoteString[i] == '#' || NoteString[i] == 'b')) {
			OneNote += NoteString[i];
		}
		else if (NoteString[i] >= '0' && NoteString[i] <= '9') {
			int c = i+1;
			int origI = i;
			while (NoteString[c] != '$') {
				noteDur /= 2;
				c++;
				i++;
			}
			notes.push_back({ OneNote,NoteString[origI] - '0',noteDur});
			OneNote = "";
		}
		else if (NoteString[i] == '.') {
			int c = i + 1;
			while (NoteString[c] != '$') {
				noteDur /= 2;
				c++;
				i++;
			}
			notes.push_back({ ".",0,noteDur});
			OneNote = "";
		}

	}
	loadSoundBuffers();
}

Song::Song(std::ifstream & NoteFile, short bpm, double volume): _bpm(bpm), _vol(volume)
{
	std::string lineOfNotes;
	std::string OneNote;
	double noteDur;
	while (std::getline(NoteFile, lineOfNotes)) {
		for (int i = 0, n = lineOfNotes.size(); i < n; i++) {
			noteDur = 1.f;
			if ((lineOfNotes[i] >= 'A' && lineOfNotes[i] <= 'G') || (lineOfNotes[i] == '#' || lineOfNotes[i] == 'b')) {
				OneNote += lineOfNotes[i];
			}
			else if (lineOfNotes[i] >= '0' && lineOfNotes[i] <= '9') {
				int c = i+1;
				int origI = i;
				while (lineOfNotes[c] == '$' || lineOfNotes[c] == '~') {
					if (lineOfNotes[c] == '$') {
						noteDur /= 2;
					}
					else if (lineOfNotes[c] == '~') {
						noteDur *= 2;
					}
					c++;
					i++;
				}
				notes.push_back({ OneNote,lineOfNotes[origI] - '0',noteDur });
				OneNote = "";
			}
			else if (lineOfNotes[i] == '.') {
				int c = i + 1;
				while (lineOfNotes[c] == '$') {
					noteDur /= 2;
					c++;
				}

				notes.push_back({ ".",0,noteDur});
				i = c;
				OneNote = "";
			}
		}
	}
	loadSoundBuffers();
}

void Song::playSong()
{
	sf::Sound sound;
	const int delayConst =10; // This is the constant you alter to fiddle with delay between notes
	for (int i = 0, n = noteSoundBuffers.size(); i < n; i++) {
		sound.setBuffer(noteSoundBuffers[i]);
		
		sound.play();
		
		Sleep(noteSoundBuffers[i].getSampleCount()/(double)SAMPLE_RATE*1000+delayConst);
	
	}
}

bool Song::transpose(int transposition) // transposition in # of half steps
{
	if (abs(transposition) > 12) {
		std::cout << "ERROR: transposition too big\n";
		return false;
	}
	if (transposition < 0) {
		for (int i = 0, n = notes.size(); i < n; i++) {
			if (notes[i].note.size() > 1) {
				if (notes[i].note[1] == '#') {
					for (int j = 0; j < 12; j++) {
						if (notes[i].note == ChromaticNotesSharp[j]) {
							if (-transposition > j) {
								transposition += j + 1;
								notes[i].octave--;
								notes[i].note = ChromaticNotesSharp[11 + transposition];
								transposition -= j + 1;
							}
							else {
								notes[i].note = ChromaticNotesSharp[j + transposition];
							}
							break;
						}
					}
				}
				else if (notes[i].note[1] == 'b') {
					for (int j = 0; j < 12; j++) {
						if (notes[i].note == ChromaticNotesFlat[j]) {
							if (-transposition > j) {
								transposition += j + 1;
								notes[i].octave--;
								notes[i].note = ChromaticNotesFlat[11 + transposition];
								transposition -= j + 1;
							}
							else {
								notes[i].note = ChromaticNotesFlat[j + transposition];
							}
							break;
						}
					}
				}
			}
			else {
				for (int j = 0; j < 12; j++) {
					if (notes[i].note == ChromaticNotesSharp[j]) {
						if (-transposition > j) {
							transposition += j + 1;
							notes[i].octave--;
							notes[i].note = ChromaticNotesSharp[11 + transposition];
							transposition -= j + 1;
						}
						else {
							notes[i].note = ChromaticNotesSharp[j + transposition];
						}
						break;
					}
				}
			}
		}
		noteSoundBuffers.clear();
		loadSoundBuffers();
	}
	else if(transposition > 0) {
		for (int i = 0, n = notes.size(); i < n; i++) {
			if (notes[i].note.size() > 1) {
				if (notes[i].note[1] == '#') {
					for (int j = 0; j < 12; j++) {
						if (notes[i].note == ChromaticNotesSharp[j]) {
							if (transposition > 11-j) {
								transposition -= 11 - j;
								notes[i].octave++;
								notes[i].note = ChromaticNotesSharp[transposition-1];
								transposition += 11 - j;
							}
							else {
								notes[i].note = ChromaticNotesSharp[j + transposition];
							}
							break;
						}
					}
				}
				else if (notes[i].note[1] == 'b') {
					for (int j = 0; j < 12; j++) {
						if (notes[i].note == ChromaticNotesFlat[j]) {
							if (transposition > 11-j) {
								transposition -= 11 - j;
								notes[i].octave++;
								notes[i].note = ChromaticNotesFlat[transposition-1];
								transposition += 11 - j;
							}
							else {
								notes[i].note = ChromaticNotesFlat[j + transposition];
							}
							break;
						}
					}
				}
			}
			else {
				for (int j = 0; j < 12; j++) {
					if (notes[i].note == ChromaticNotesSharp[j]) {
						if (transposition > 11-j) {
							transposition -= 11 - j;
							notes[i].octave++;
							notes[i].note = ChromaticNotesSharp[transposition - 1];
							transposition += 11 - j;
						}
						else {
							notes[i].note = ChromaticNotesSharp[j + transposition];
						}
						break;
					}
				}
			}
		}
		noteSoundBuffers.clear();
		loadSoundBuffers();
	}
	return true;
}

//Play a mathematical function, which must be a combination of sines and cosines with net amplitude 1
Song::Song(short(*func)(double), int time)
{
	std::vector<sf::Int16> samples;
	for (int i = 0; i < time * SAMPLE_RATE; i++) {
		samples.push_back(func(i));
	}
	sf::SoundBuffer buff;
	buff.loadFromSamples(&samples[0], time * SAMPLE_RATE, 1, SAMPLE_RATE);
	noteSoundBuffers.push_back(buff);
}

Song::~Song()
{
}

// generate the value of a sine wave of frequency 'freq'(Hz) amplitude 'amplitude' at time 'time'(1/SAMPLE_RATEths of a second) 
short Song::SineWave(double time, double freq, double amplitude)
{
	short result;
	double tpc = SAMPLE_RATE / freq; // Ticks per cycle (samples per second)
	double cycles = time / tpc; // Total cycles
	double rad = cycles * TWOPI; // radian to be sined
	short amp = amplitude * 32767; // amplitude
	result = amp * sin(rad); 
	return result;
}

//Get a sound buffer from a note, given its value and duration
sf::SoundBuffer Song::GetSoundBufferFromNote(Note note, double beats)
{
	double freq = 440;//		Set default note to a4
	int n = 4;//				
	while (n != note.octave) { // change freq to match the A note of the notes' octave
		if (n > note.octave) {
			freq /= 2;
			n--;
		}
		else {
			freq *= 2;
			n++;
		}
	}
	std::vector<sf::Int16> samples; // Raw amplitude array
	double time = (60/ (double)_bpm)*beats; // Length of the tone in seconds

	std::string noteLetter = note.note;
	//if the note is a '.' it indicates a rest and therefore we push silence into our soundbuffer, should probably polish this, is very memory inefficient
	if (noteLetter == ".") {
		for (int i = 0; i < time * SAMPLE_RATE; i++) { // SAMPLE_RATE samples per second
			samples.push_back(0);
		}
	}
	else {
		int expNumerator = 0; // the n in 2^(n/12)
		if (noteLetter.length() < 2) {
			for (int i = 0; i < 12; i++) {
				if (ChromaticNotesSharp[i] == noteLetter) {
					expNumerator = i; // Find the right exponent and set it
					break;
				}
			}
		}
		else {
			if (noteLetter[1] == '#') {
				for (int i = 0; i < 12; i++) {
					if (ChromaticNotesSharp[i] == noteLetter) {
						expNumerator = i;
						break;
					}
				}
			}
			else {
				for (int i = 0; i < 12; i++) {
					if (ChromaticNotesFlat[i] == noteLetter) {
						expNumerator = i;
						break;
					}
				}
			} 
		}
		double exponent = (double)expNumerator / 12;
		freq = freq * pow(2.f, exponent);// frequency by 2^n/12


		for (int i = 0; i < time * SAMPLE_RATE; i++) {
			samples.push_back(SineWave(i, freq, _vol));
		}
	}
	sf::SoundBuffer buffer;
	buffer.loadFromSamples(&samples[0], samples.size(), 1, SAMPLE_RATE);
	return buffer;
}

//Loads sound buffers into memory  
bool Song::loadSoundBuffers()
{
	if (notes.size() == 0) {
		std::cout << "ERROR: notes array empty \n";
		return false;
	}
	for (int i = 0, n = notes.size(); i < n; i++) {
		noteSoundBuffers.push_back(GetSoundBufferFromNote(notes[i],notes[i].duration));
	}
	return true;
}

//Overloaded operator to output a note
std::ostream & operator<<(std::ostream & stream, Song::Note note)
{
	stream << note.note << note.octave; 
	return stream;
}