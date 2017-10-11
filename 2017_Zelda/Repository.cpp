#include "main.h"
#include "Repository.h"


Repository::Repository()
{
}

void Repository::loadSoundEffect(string path)
{
	sf::SoundBuffer soundEffect;
	soundEffect.loadFromFile(path);
	soundEffects.emplace(path, move(make_unique<sf::SoundBuffer>(soundEffect)));
}

const unique_ptr<sf::SoundBuffer>& Repository::getSoundEffect(string name)
{
	return soundEffects[name];
}

void Repository::loadTexture(string path)
{
	sf::Texture texture;
	texture.loadFromFile(path);
	textures.emplace(path, move(make_unique<sf::Texture>(texture)));
}

const unique_ptr<sf::Texture>& Repository::getTexture(string name)
{
	return textures[name];
}


Repository::~Repository()
{
}
