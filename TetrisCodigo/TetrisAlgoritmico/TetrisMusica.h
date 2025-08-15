#ifndef TETRISMUSICA_H_INCLUDED
#define TETRISMUSICA_H_INCLUDED

#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadMusic(const std::string& id, const std::string& filename);
    bool playMusic(const std::string& id, bool loop = true, float volume = 50.f);
    void stopMusic();
    void pauseMusic();
    void setMusicVolume(float v);

    bool loadSound(const std::string& id, const std::string& filename);
    void playSound(const std::string& id, float volume = 80.f);

    void update(float dt);

private:
    sf::Music currentMusic;
    std::string currentMusicId;
    std::map<std::string, std::string> musicFiles;
    std::map<std::string, sf::SoundBuffer> buffers;
    static const int MAX_ACTIVE_SOUNDS = 32;
    sf::Sound activeSounds[MAX_ACTIVE_SOUNDS];
};

#endif // TETRISMUSICA_H_INCLUDED
