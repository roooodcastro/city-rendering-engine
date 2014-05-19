/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is the Resource class for Music tracks. It will play music for the game, that can be looped or not.
 * There's also a volume control that allows to change the volume of the music, and a fadeIn and fadeOut effect when
 * starting or stopping the music.
 */

#pragma once

#include <string>
#include "ResourcesManager.h"
#include "GameApp.h"

class Music : public Resource {
public:

	Music(void);
	Music(std::string fileName, std::string resourceName);
	Music(const Music &copy);
	~Music(void);

	Music &operator=(const Music &other);

	/*
	 * Plays or resumes the music. If fadeInMillis is higher than 0,
	 * it will apply a fade-in effect to the music. If the music is currently
	 * paused, fadeInMillis will have no effect.
	 */
	void play(int fadeInMillis);

	/*
	 * Paused the music, enabling it to be resumed later with play().
	 * FadeIn and fadeOut will have no effect while pausing and resuming music.
	 */
	void pause();
	/*
	 * Stops the music. If play() is called after, the music will resume from the start.
	 * If fadeOutMillis is higher than 0, it will apply a fade-out effect to the music.
	 */
	void stop(int fadeOutMillis);

	/* Returns if the music is currently being played. */
	bool isPlaying();
	/* Returns of the music is currently paused. If the music is stopped, this will return false */
	bool isPaused();

	/* Sets the volume for the music, ranging from 0 to 128 */
	void setVolume(int volume);
	int getVolume() { return volume; }
	/*
	 * Calculates and returns the actual volume at which the mixer is playing.
	 * Sometimes this can become different from the set value due to internal
	 * functions of the SDL_mixer.
	 */
	int getRealVolume();

	/*
	 * Sets if the music will loop or not. If the music is already playing, it needs
	 * to be stopped and restarted to apply the new loop setting.
	 */
	void setLoop(bool loop) { this->loop = loop; }
	bool isLooping() { return loop; }

	Mix_Music *getMixer() { return mixer; }

	/*
	 * This function tries to retrieve the requested music from the
	 * resource manager, and if it fails, creates and returns a new music.
	 */
	static Music *getOrCreate(std::string fileName, std::string resourceName);

	/* This method should load the specific resource into memory */
	virtual void load();

	/* This method should destroy the resource, unloading and releasing it from memory. This should almost replace the destructor. */
	virtual void unload();

	/* Sets the mute flag. If set to true, immediately stops the music playing, if any */
	static void setMute(bool mute);

protected:

	/* If true, the music will loop continuously until pause() or stop() are called. Defaults to true */
	bool loop;
	/* The volume of the music, ranging from 0 to 128. Defaults to 128 */
	int volume;
	/* The path to the music file */
	std::string fileName;
	/* The SDL Mixer to load and play the music */
	Mix_Music *mixer;
	/* Global flag to mute the music. When true no music will be played. Defaults to false */
	static bool mute;
};