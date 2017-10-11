#pragma once
#include "main.h"
#include "Repository.h"

class Repository
{
private:

	unordered_map<string, unique_ptr<sf::Texture>> textures;
	unordered_map<string, unique_ptr<sf::SoundBuffer>> soundEffects;

public:
	Repository();

	void loadTexture(string name, string path);
	void loadSoundEffect(string name, string path);

	const unique_ptr<sf::Texture>& getTexture(string name);
	const unique_ptr<sf::SoundBuffer>& getSoundEffect(string);


	~Repository();
};

