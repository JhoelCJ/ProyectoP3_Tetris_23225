#include "TetrisMusica.h"
#include <iostream>

AudioManager::AudioManager() : currentMusicId("") {}

AudioManager::~AudioManager() {
    stopMusic();
}

bool AudioManager::loadMusic(const std::string& id, const std::string& filename) {
    musicFiles[id] = filename;
    return true;
}

bool AudioManager::playMusic(const std::string& id, bool loop, float volume) {
    auto it = musicFiles.find(id);
    if (it == musicFiles.end()) {
        std::cerr << "AudioManager::playMusic -> music id no registrado: " << id << std::endl;
        return false;
    }

    const std::string& path = it->second;
    if (!currentMusic.openFromFile(path)) {
        std::cerr << "AudioManager::playMusic -> ERROR al abrir archivo: " << path << std::endl;
        return false;
    }
    currentMusic.setLoop(loop);
    currentMusic.setVolume(volume);
    currentMusic.play();
    currentMusicId = id;
    std::cout << "AudioManager::playMusic -> reproduciendo id=" << id << " path=" << path << std::endl;
    return true;
}

void AudioManager::stopMusic() {
    if (currentMusic.getStatus() != sf::Music::Stopped) currentMusic.stop();
    currentMusicId.clear();
}

void AudioManager::pauseMusic() {
    if (currentMusic.getStatus() == sf::Music::Playing) currentMusic.pause();
}

void AudioManager::setMusicVolume(float v) {
    currentMusic.setVolume(v);
}

bool AudioManager::loadSound(const std::string& id, const std::string& filename) {
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(filename)) {
        std::cerr << "AudioManager::loadSound -> no se pudo abrir: " << filename << std::endl;
        return false;
    }
    buffers[id] = buf;
    return true;
}

void AudioManager::playSound(const std::string& id, float volume) {
    auto it = buffers.find(id);
    if (it == buffers.end()) {
        std::cerr << "AudioManager::playSound -> sound id no cargado: " << id << std::endl;
        return;
    }

    int slot = -1;
    for (int i = 0; i < MAX_ACTIVE_SOUNDS; ++i) {
        if (activeSounds[i].getStatus() != sf::Sound::Playing) {
            slot = i;
            break;
        }
    }

    if (slot == -1) slot = 0;

    activeSounds[slot].setBuffer(it->second);
    activeSounds[slot].setVolume(volume);
    activeSounds[slot].play();
}

void AudioManager::update(float) {}
