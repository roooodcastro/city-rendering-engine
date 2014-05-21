#include "ConfigurationManager.h"

ConfigurationManager *ConfigurationManager::instance = NULL;

ConfigurationManager::ConfigurationManager(void) {
    configFileName = "settings.cfg";
}

ConfigurationManager::~ConfigurationManager(void) {
}

ConfigurationManager *ConfigurationManager::getInstance() {
    if (instance == NULL) {
        instance = new ConfigurationManager();
    }
    return instance;
}

bool ConfigurationManager::readBool(std::string configName, bool defaultValue) {
    if (configExists(configName)) {
        std::string value = readString(configName, "true");
        return value == "true";
    }
    return defaultValue;
}

int ConfigurationManager::readInt(std::string configName, int defaultValue) {
    if (configExists(configName)) {
        std::string value = readString(configName, "0");
        return atoi(value.c_str());
    }
    return defaultValue;
}

float ConfigurationManager::readFloat(std::string configName, float defaultValue) {
    if (configExists(configName)) {
        std::string value = readString(configName, "0");
        return (float) atof(value.c_str());
    }
    return defaultValue;
}

std::string ConfigurationManager::readString(std::string configName, std::string defaultValue) {
    if (configExists(configName)) {
        std::ifstream file(configFileName);
        if (!file.is_open()) {
            std::cout << "ERROR: Cannot open the configuration file: " << configFileName << std::endl;
            return false;
        }
        std::vector<std::string> lines;
        char lineBuffer[200];
        while(!file.eof()) {
            file.getline(lineBuffer, 200);
            lines.emplace_back(lineBuffer);
        }
        for (unsigned i = 0; i < lines.size(); i++) {
            std::string line = lines[i];
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            std::vector<std::string> splitted = split(line, '=');
            if (splitted[0].compare(configName) == 0) {
                return std::string(splitted[1]);
            }
        }
    }
    return defaultValue;
}

void ConfigurationManager::writeConfig(std::string configName, int value) {
    std::ostringstream convert;
    convert << value;
    writeConfig(configName, (std::string) convert.str());
}

void ConfigurationManager::writeConfig(std::string configName, bool value) {
    writeConfig(configName, (std::string) (value ? "true" : "false"));
}

void ConfigurationManager::writeConfig(std::string configName, float value) {
    std::ostringstream convert;
    convert << value;
    writeConfig(configName, (std::string) convert.str());
}

void ConfigurationManager::writeConfig(std::string configName, std::string value) {
    // Read the config file
    std::fstream file(configFileName);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot open the configuration file: " << configFileName << std::endl;
        return;
    }
    std::vector<std::string> lines;
    char lineBuffer[200];
    while(!file.eof()) {
        file.getline(lineBuffer, 200);
        lines.emplace_back(lineBuffer);
    }
    int lineNum = -1;
    // Find the line of the configuration, if it exists
    for (unsigned i = 0; i < lines.size(); i++) {
        std::string line = lines[i];
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        std::vector<std::string> splitted = split(line, '=');
        if (splitted.size() >= 2) {
            if (splitted[0].compare(configName) == 0) {
                lineNum = i;
            }
        }
    }
    // Change the config value
    if (lineNum >= 0) {
        lines[lineNum] = configName + "=" + value;
    } else {
        lines.emplace_back(configName + "=" + value);
    }
    // Now recreate the file and rewrite its contents
    file.close();
    file.open(configFileName, std::ofstream::out | std::ofstream::trunc);
    for (unsigned i = 0; i < lines.size(); i++) {
        if (lines[i].size() > 0) {
            file << lines[i] << std::endl;
        }
    }
}

bool ConfigurationManager::configExists(std::string configName) {
    std::ifstream file(configFileName);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot open the configuration file: " << configFileName << std::endl;
        return false;
    }
    std::vector<std::string> lines;
    char lineBuffer[200];
    while(!file.eof()) {
        file.getline(lineBuffer, 200);
        lines.emplace_back(lineBuffer);
    }
    for (unsigned i = 0; i < lines.size(); i++) {
        std::string line = lines[i];
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        std::vector<std::string> splitted = split(line, '=');
        if (splitted.size() > 0) {
            if (splitted[0].compare(configName) == 0) {
                return true;
            }
        }
    }
    return false;
}

void ConfigurationManager::setConfigFileName(std::string configFileName) {
    getInstance()->configFileName = configFileName;
}