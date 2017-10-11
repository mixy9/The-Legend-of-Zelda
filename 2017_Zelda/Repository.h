#pragma once
#include "main.h"
#include "Repository.h"
#include <unordered_map>

class Repository
{
private:

	unordered_map<string, unique_ptr<sf::Texture>> textures;
	unordered_map<string, unique_ptr<sf::SoundBuffer>> soundEffects;

public:
	Repository();

	void loadTexture(string path);
	void loadSoundEffect(string path);

	const unique_ptr<sf::Texture>& getTexture(string name);
	const unique_ptr<sf::SoundBuffer>& getSoundEffect(string);


	~Repository();
};

