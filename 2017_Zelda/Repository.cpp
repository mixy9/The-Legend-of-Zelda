#include "Repository.h"



Repository::Repository()
{
}

void Repository::loadSoundEffect(int flags)
{
	if (flags & BOOMERANG_THROW)
	{
		sf::SoundBuffer boomThrow;
		boomThrow.loadFromFile("sound effects\\boomerang.ogg");
		auto boomptr = make_unique<sf::SoundBuffer>(boomThrow);
		soundEffects.emplace_back(move(boomptr));
	}
}

const sndPtr& Repository::getSoundEffect(int flags)
{

	if (flags & BOOMERANG_THROW)
	{
		return soundEffects[vBOOMERANG_THROW];
	}
	else return 0;
}
void Repository::loadTexture(int flags)
{
	
	if (flags & LINK)
	{
		sf::Texture link;
		link.loadFromFile("textures\\Link.png");
		auto linkptr = make_unique<sf::Texture>(link);
		textures.emplace_back(move(linkptr));
	}
	
	if (flags & BLUE_BOOMERANG)
	{
		sf::Texture boomerang;
		boomerang.loadFromFile("textures\\Boomerang.png");
		auto boomptr = make_unique<sf::Texture>(boomerang);
		textures.emplace_back(move(boomptr));
	}

	if (flags & BLACK_MAGE)
	{
		sf::Texture enemy;
		enemy.loadFromFile("textures\\Blackmage.png");
		auto enemyptr = make_unique<sf::Texture>(enemy);
		textures.emplace_back(move(enemyptr));
	}
	if (flags & TEST_BACKGROUND)
	{
		sf::Texture testBackground;
		testBackground.loadFromFile("textures\\test_background.jpg");
		auto bgrndPtr = make_unique<sf::Texture>(testBackground);
		textures.emplace_back(move(bgrndPtr));
	}
}

const texPtr& Repository::getTexture(int flags)
{

	if (flags & LINK)
	{
		return textures[vLINK];
	}

	if (flags & BLUE_BOOMERANG)
	{
		return textures[vBLUE_BOOMERANG];
	}

	if (flags & BLACK_MAGE)
	{
		return textures[vBLACK_MAGE];
	}
	if (flags & TEST_BACKGROUND)
	{
		return textures[vTEST_BACKGROUND];
	}
	else return 0;
}


Repository::~Repository()
{
}
