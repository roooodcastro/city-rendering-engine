#include "Sound.h"

bool Sound::mute = false;

Sound::Sound(void) {
	volume = 128;
	fileName = "";
	chunk = NULL;
}

Sound::Sound(std::string fileName, std::string resourceName) : Resource(resourceName.c_str()) {
	this->fileName = fileName;
	volume = 128;
	chunk = NULL;
}

Sound::Sound(const Sound &copy) {
	this->volume = copy.volume;
	this->fileName = copy.fileName;
	this->chunk = copy.chunk;
}

Sound::~Sound(void) {
}

Sound &Sound::operator=(const Sound &other) {
	this->volume = other.volume;
	this->fileName = other.fileName;
	this->chunk = other.chunk;
	return *this;
}

Sound *Sound::getOrCreate(std::string fileName, std::string resourceName) {
	if (ResourcesManager::resourceExists(resourceName)) {
		return (Sound*) ResourcesManager::getResource(resourceName);
	} else {
		Sound *newSound = new Sound(fileName, resourceName);
		ResourcesManager::addResource(resourceName, newSound);
		return newSound;
	}
}

void Sound::load() {
	chunk = Mix_LoadWAV(fileName.c_str());
	if (chunk == NULL) {
		GameApp::logSDLError(std::cout, "Failed loading sound: " + fileName);
	}
}

void Sound::unload() {
	if (chunk) {
		Mix_FreeChunk(chunk);
		chunk = NULL;
	}
}

int Sound::play() {
	if (!mute) {
		return Mix_PlayChannel(-1, chunk, 0);
	}
	return -1;
}

void Sound::stop(int channel) {
	Mix_HaltChannel(channel);
}

bool Sound::isPlaying(int channel) {
	return (Mix_Playing(channel) == 1) && (Mix_Paused(channel) == 0);
}

void Sound::setVolume(int channel, int volume) {
	if (volume >= 0) {
		Mix_Volume(channel, volume);
	} else {
		Mix_Volume(channel, 0);
	}
}

int Sound::getRealVolume(int channel) {
	return Mix_Volume(channel, -1);
}

void Sound::playSfx(std::string name, int volume) {
	if (ResourcesManager::resourceExists(name) && volume > 0 && !mute) {
		Sound *sound = (Sound*) ResourcesManager::getResource(name);
		Mix_VolumeChunk(sound->getChunk(), volume);
		Mix_PlayChannel(-1, sound->getChunk(), 0);
		sound->volume = volume > 128 ? 128 : volume;
	}
}

void Sound::stopAllSfx() {
	Mix_HaltChannel(-1);
}

void Sound::setMute(bool mute) {
	Sound::mute = mute;
	if (mute) {
		Sound::stopAllSfx();
	}
}