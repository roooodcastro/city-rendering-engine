/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is the Resource class for Sound effects. It can play sound effects simultaneous.
 * Sounds will be played on random channels, chosen based on availablity. This class offers much less
 * control than the Music class, but is useful to quickly play SFX. This should not be used to play music,
 * especially because this class only plays WAV files, unlike the Music which plays WAV, OGG, MP3, MOD and MIDI.
 *
 * A sound is played on a specific channel, and each channel can only have one sound playing at the same time.
 * The number of channels available is 8.
 */

#pragma once

#include <string>
#include "ResourcesManager.h"
#include "GameApp.h"

class Sound : public Resource {
public:

	Sound(void);
	Sound(std::string fileName, std::string resourceName);
	Sound(const Sound &copy);
	~Sound(void);

	Sound &operator=(const Sound &other);

	/*
	 * Plays the sound. Can be called multiple times before the sound has finished playing.
	 * It returns the channel that the sound has been allocated to. This channel will be
	 * chosen randomly, based on the first available channel.
	 */
	int play();

	/*
	 * Stops the sound on the specified channel.
	 */
	void stop(int channel);

	/* Returns if the sound is currently being played. */
	bool isPlaying(int channel);

	/* Sets the volume for the sound on the specified channel, ranging from 0 to 128 */
	void setVolume(int channel, int volume);
	int getVolume() { return volume; }

	/*
	 * Calculates and returns the actual volume at which the chunk is playing.
	 * Sometimes this can become different from the set value due to internal
	 * functions of the SDL_mixer. If -1 is provided for the channel, it will
	 * return the average value between all channels
	 */
	int getRealVolume(int channel);

	Mix_Chunk *getChunk() { return chunk; }

	/*
	 * Plays a sound effect with the specified volume. Plays the
	 * sound only once. The sound must be preloaded. Also sets the
	 * volume for this sound to the volume specified. If the sound
	 * has not been preloaded, or if the volume is negative,
	 * nothing will happen.
	 */
	static void playSfx(std::string name, int volume);

	/* Stops all sounds that are currently playing in all channels */
	static void stopAllSfx();

	/*
	 * This function tries to retrieve the requested sound from the
	 * resource manager, and if it fails, creates and returns a new sound.
	 */
	static Sound *getOrCreate(std::string fileName, std::string resourceName);

	/* This method should load the specific resource into memory */
	virtual void load();

	/* This method should destroy the resource, unloading and releasing it from memory. This should almost replace the destructor. */
	virtual void unload();
	
	/* Sets the mute flag. If set to true, immediately stops all sounds playing */
	static void setMute(bool mute);

protected:

	/* The volume of the sound, ranging from 0 to 128. Defaults to 128 */
	int volume;
	/* The path to the sound file */
	std::string fileName;
	/* The SDL Chunk to load and play the sound */
	Mix_Chunk *chunk;
	/* Global flag to mute all sounds. When true no sound will be played. Defaults to false */
	static bool mute;
};