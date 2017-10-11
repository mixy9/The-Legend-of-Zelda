#include "main.h"
#include "Repository.h"


Repository::Repository()
{
}

void Repository::loadSoundEffect(string name, string path)
{
	sf::SoundBuffer soundEffect;
	soundEffect.loadFromFile(path);
	auto tempPtr = make_unique<sf::SoundBuffer>(soundEffect);
	soundEffects.emplace(name, move(tempPtr));
}

const unique_ptr<sf::SoundBuffer>& Repository::getSoundEffect(string name)
{
	return soundEffects[name];
}

void Repository::loadTexture(string name, string path)
{
	sf::Texture texture;
	texture.loadFromFile(path);
	auto tempPtr = make_unique<sf::Texture>(texture);
	textures.emplace(name, move(tempPtr));
}

const unique_ptr<sf::Texture>& Repository::getTexture(string name)
{
	return textures[name];
}


Repository::~Repository()
{
}
