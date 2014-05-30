#include "Profiler.h"

std::map<int, ProfilingTimer*> *Profiler::profilingTimers = nullptr;
bool Profiler::initialized = false;

ProfilingTimer *Profiler::getTimer(int id) {
    if (initialized) {
        auto it = profilingTimers->find(id);
        if (it != profilingTimers->end()) {
            return it->second;
        }
    }
    return nullptr;
}

void Profiler::addProfilingTimer(ProfilingTimer *timer) {
    if (initialized && timer != nullptr && profilingTimers->find(timer->id) == profilingTimers->end()) {
        profilingTimers->insert(std::pair<int, ProfilingTimer*>(timer->id, timer));
    }
}

void Profiler::removeProfilingTimer(int id) {
    if (initialized) {
        profilingTimers->erase(id);
    }
}

void Profiler::startProfiler() {
    if (!initialized) {
        profilingTimers = new std::map<int, ProfilingTimer*>();
        initialized = true;
    }
}

void Profiler::stopProfiler() {
    if (initialized) {
        profilingTimers->clear();
        delete profilingTimers;
        profilingTimers = nullptr;
        initialized = false;
    }
}