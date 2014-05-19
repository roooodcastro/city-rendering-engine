/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class reads and writes configuration options from simple text files.
 * Its main purpose if provide easy configuration setups for the game, in a non hard-coded way.
 *
 * Each configuration will be saved on separated lines, with each line consisting of the name
 * of the configuration and the value, with an equal sign joining them.
 */

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <functional> 
#include <cctype>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Common.h"

class ConfigurationManager {
public:

	~ConfigurationManager(void);

	static ConfigurationManager *getInstance();

	/*
	 * Reads a value from the configuration file. The name of the
	 * configuration and a default value must be provided. If the
	 * configuration doesn't exists, the default value will be returned.
	 */
	int readInt(std::string configName, int defaultValue);
	bool readBool(std::string configName, bool defaultValue);
	float readFloat(std::string configName, float defaultValue);
	std::string readString(std::string configName, std::string defaultValue);

	/*
	 * Writes a value in the configuration file. The name of the configuration
	 * and the value must be provided. If the configuration already exists, it
	 * will be overriden by the new value. Returns true if the operation succeeded
	 */
	void writeConfig(std::string configName, int value);
	void writeConfig(std::string configName, bool value);
	void writeConfig(std::string configName, float value);
	void writeConfig(std::string configName, std::string value);

	/* Returns true if the configuration exists in the file */
	bool configExists(std::string configName);

protected:

	ConfigurationManager(void);

	/* The singleton instance of this class */
	static ConfigurationManager *instance;

	/* The name of the configuration file. Defaults to 'settings.cfg' */
	std::string configFileName;
};