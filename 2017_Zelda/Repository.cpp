#include "Repository.h"



Repository::Repository()
{
}


void Repository::loadTexture(int flags)
{
	
	if (flags & LINK)
	{
		sf::Texture link;
		link.loadFromFile("Link.png");
		auto linkptr = make_unique<sf::Texture>(link);
		textures.emplace_back(move(linkptr));
	}
	
	if (flags & BLUE_BOOMERANG)
	{
		sf::Texture boomerang;
		boomerang.loadFromFile("Boomerang.png");
		auto boomptr = make_unique<sf::Texture>(boomerang);
		textures.emplace_back(move(boomptr));
	}

	if (flags & BLACK_MAGE)
	{
		sf::Texture enemy;
		enemy.loadFromFile("Blackmage.png");
		auto enemyptr = make_unique<sf::Texture>(enemy);
		textures.emplace_back(move(enemyptr));
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
}


Repository::~Repository()
{
}
