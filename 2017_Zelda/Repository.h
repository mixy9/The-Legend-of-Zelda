#pragma once
#include "main.h"

typedef unique_ptr<sf::Texture> texPtr;
typedef unique_ptr<sf::SoundBuffer> sndPtr;

class Repository
{
private:

	vector<texPtr> textures;
	vector<sndPtr> soundEffects;

public:
	Repository();

	enum soundFlags
	{
		BOOMERANG_THROW = 1 << 0
	};

	enum soundVectorSubscripts
	{
		vBOOMERANG_THROW
	};



	enum textureFlags
	{
		LINK = 1 << 0,
		BLUE_BOOMERANG = 1 << 1,
		BLACK_MAGE = 1 << 2,
		TEST_BACKGROUND = 1 << 3
	};

	enum textureVectorSubscripts
	{
		vLINK,
		vBLUE_BOOMERANG,
		vBLACK_MAGE,
		vTEST_BACKGROUND
	};

	 

	void loadTexture(int flags);
	void loadSoundEffect(int flags);

	const texPtr& getTexture(int flags);
	const sndPtr& getSoundEffect(int flags);


	~Repository();
};

