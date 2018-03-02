#include "BackgroundMusic.h"

#include "Framework.h"
#include <iostream>

void* load(char *fname, long *bufsize);

BackgroundMusic* BackgroundMusic::instance = nullptr;

ALuint BackgroundMusic::source0;
ALuint BackgroundMusic::source1;

ALuint BackgroundMusic::buffer0;
ALuint BackgroundMusic::buffer1;

bool BackgroundMusic::track = false;
float BackgroundMusic::volume = 1;

BackgroundMusic* BackgroundMusic::GetInstance() {
	if (instance == nullptr) {
		instance = new BackgroundMusic();
	}
	return instance;
}

void BackgroundMusic::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


BackgroundMusic::BackgroundMusic() {
	// buffers
	alGenBuffers(1, &buffer0);
	{
		long dataSize;
		const ALvoid* data = load("Background_0.raw", &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffer0, AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}

	alGenBuffers(1, &buffer1);
	{
		long dataSize;
		const ALvoid* data = load("Background_1.raw", &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffer1, AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}

	// sources
	alGenSources(1, &source0);
	alSourcef(source0, AL_PITCH, 1.);
	alSourcef(source0, AL_GAIN, 0);
	alSource3f(source0, AL_POSITION, -250, 0, 50);
	alSource3f(source0, AL_VELOCITY, 0., 0., 0.);
	alSourcef(source0, AL_MAX_DISTANCE, 1.);
	alSourcei(source0, AL_LOOPING, AL_TRUE);
	alSourcei(source0, AL_BUFFER, buffer0);

	alGenSources(1, &source1);
	alSourcef(source1, AL_PITCH, 1.);
	alSourcef(source1, AL_GAIN, 0);
	alSource3f(source1, AL_POSITION, -250, 0, 50);
	alSource3f(source1, AL_VELOCITY, 0., 0., 0.);
	alSourcef(source1, AL_MAX_DISTANCE, 1.);
	alSourcei(source1, AL_LOOPING, AL_TRUE);
	alSourcei(source1, AL_BUFFER, buffer1);

	// play
	alSourcePlay(source0);
	alSourcePlay(source1);

	fflush(stderr);
}

BackgroundMusic::~BackgroundMusic() {
	alDeleteSources(1, &source0);
	alDeleteSources(1, &source1);

	alDeleteBuffers(1, &buffer0);
	alDeleteBuffers(1, &buffer1);
}


void BackgroundMusic::Update(float dt) {
	float delta = dt * 0.5f;

	// sweet crossfade action
	if (track) {
		if (volume > 0) {
			volume -= delta;
			if (volume < 0) volume = 0;
		}
	} else {
		if (volume < 1) {
			volume += delta;
			if (volume > 1) volume = 1;
		}
	}

	alSourcef(source0, AL_GAIN, volume);
	alSourcef(source1, AL_GAIN, 1 - volume);
}

void BackgroundMusic::Switch(bool b) {
	track = b;
}