#include "Naquadah.h"

Naquadah *Naquadah::instance = nullptr;

Naquadah::Naquadah(void) {
}

Naquadah::~Naquadah(void) {
}

Naquadah *Naquadah::getInstance() {
	return instance;
}

void Naquadah::initialize(std::string configFilePath) {

}