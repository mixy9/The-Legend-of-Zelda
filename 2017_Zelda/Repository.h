#pragma once
#include "main.h"

typedef unique_ptr<sf::Texture> texPtr;

class Repository
{
private:

	vector<texPtr> textures;
public:
	Repository();

	enum textureFlags
	{
		LINK = 1 << 0,
		BLUE_BOOMERANG = 1 << 1,
		BLACK_MAGE = 1 << 2
	};

	enum textureVectorSubscripts
	{
		vLINK,
		vBLUE_BOOMERANG,
		vBLACK_MAGE
	};

	 

	void loadTexture(int flags);

	const texPtr& getTexture(int flags);


	~Repository();
};

