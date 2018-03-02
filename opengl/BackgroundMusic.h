#pragma once

#include <al.h>

class BackgroundMusic {
public:
	// singleton stuff
	static BackgroundMusic* GetInstance();
	static void Destroy();

	// update
	static void Update(float);

	// controls
	static void Switch(bool);
private:
	// singleton stuff
	BackgroundMusic();
	~BackgroundMusic();
	BackgroundMusic(BackgroundMusic const&);
	void operator=(BackgroundMusic const&);

	static BackgroundMusic* instance;

	// music vals
	//     0 = first background track
	//     1 = second background track
	static ALuint source0;
	static ALuint source1;

	static ALuint buffer0;
	static ALuint buffer1;

	static bool track;
	static float volume;
};