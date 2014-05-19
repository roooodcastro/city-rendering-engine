#include "Music.h"

bool Music::mute = false;

Music::Music(void) {
	loop = true;
	volume = 128;
	fileName = "";
	mixer = NULL;
}

Music::Music(std::string fileName, std::string resourceName) : Resource(resourceName.c_str()) {
	this->fileName = fileName;
	loop = true;
	volume = 128;
	mixer = NULL;
}

Music::Music(const Music &copy) {
	this->loop = copy.loop;
	this->volume = copy.volume;
	this->fileName = copy.fileName;
	this->mixer = copy.mixer;
}

Music::~Music(void) {
}

Music &Music::operator=(const Music &other) {
	this->loop = other.loop;
	this->volume = other.volume;
	this->fileName = other.fileName;
	this->mixer = other.mixer;
	return *this;
}

Music *Music::getOrCreate(std::string fileName, std::string resourceName) {
	if (ResourcesManager::resourceExists(resourceName)) {
		return (Music*) ResourcesManager::getResource(resourceName);
	} else {
		Music *newMusic = new Music(fileName, resourceName);
		ResourcesManager::addResource(resourceName, newMusic);
		return newMusic;
	}
}

void Music::load() {
	mixer = Mix_LoadMUS(fileName.c_str());
	if (mixer == NULL) {
		GameApp::logSDLError(std::cout, "Failed loading music: " + fileName);
	}
}

void Music::unload() {
	if (mixer) {
		Mix_FreeMusic(mixer);
		mixer = NULL;
	}
}

void Music::play(int fadeInMillis) {
	if (!mute) {
		if (isPaused()) {
			Mix_ResumeMusic();
		} else {
			// Calculates the number of times the music will play. -1 is infinite.
			int loops = loop ? -1 : 1;
			if (fadeInMillis > 0) {
				Mix_FadeInMusic(mixer, loops, fadeInMillis);
			} else {
				Mix_PlayMusic(mixer, loops);
			}
		}
	}
}

void Music::pause() {
	if (isPlaying()) {
		Mix_PauseMusic();
	}
}

void Music::stop(int fadeOutMillis) {
	if (fadeOutMillis > 0) {
		Mix_FadeOutMusic(fadeOutMillis);
	} else {
		Mix_HaltMusic();
	}
}

bool Music::isPlaying() {
	return (Mix_PlayingMusic() == 1) && (Mix_PausedMusic() == 0);
}

bool Music::isPaused() {
	return (Mix_PlayingMusic() == 1) && (Mix_PausedMusic() == 1);
}

void Music::setVolume(int volume) {
	if (volume >= 0) {
		Mix_VolumeMusic(volume);
	} else {
		Mix_VolumeMusic(0);
	}
}

int Music::getRealVolume() {
	return Mix_VolumeMusic(-1);
}

void Music::setMute(bool mute) {
	Music::mute = mute;
	if (mute) {
		Mix_HaltMusic();
	}
}