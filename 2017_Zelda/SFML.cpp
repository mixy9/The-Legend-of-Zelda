#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>


using namespace std;

#define BOOMERANG_ROTATION_SPEED	0.2f

void moveTowardPlayer(sf::Sprite& link, sf::Sprite& character)
{
	
	character.move(0.0001f*(link.getPosition() - character.getPosition()));
}


int main()
{
	int window_width = 1000;
	int window_height = 800;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "The Legend of Zelda");


	sf::Texture player;
	player.loadFromFile("Link.png");

	sf::Texture boomerang;
	boomerang.loadFromFile("Boomerang.png");

	sf::Texture enemy;
	enemy.loadFromFile("Blackmage.png");


	

	sf::Sprite link;

	link.setTexture(player);
	link.setPosition(100, 400);
	link.setOrigin(50, 50);
	
	sf::Sprite boomerangSprite;

	boomerangSprite.setTexture(boomerang);
	boomerangSprite.setScale(0.4f, 0.4f);
	boomerangSprite.setOrigin(10, 10);
	boomerangSprite.setPosition(0, 0);

	sf::Sprite blackMage;

	blackMage.setTexture(enemy);
	blackMage.setPosition(800, 800);
	blackMage.setScale(0.2f, 0.2f);

	bool fireActive = 0;
	auto projectileDirection = 0;
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			link.setRotation(0);
			link.move(0, -0.06f);
			
			if (!fireActive)
			{
				projectileDirection = link.getRotation();
			}
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			link.setRotation(180);
			link.move(0, 0.06f);
		
			if (!fireActive)
			{
				projectileDirection = link.getRotation();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			link.setRotation(270);
			link.move(-0.06f, 0);

			if (!fireActive)
			{
				projectileDirection = link.getRotation();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			link.setRotation(90);
			link.move(0.06f, 0);

			if (!fireActive)
			{
				projectileDirection = link.getRotation();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!fireActive)
			{
				boomerangSprite.setPosition(link.getPosition().x - 25, link.getPosition().y);
			}
			fireActive = true;
			
		}
		
		
		if (fireActive)
		{
			if (projectileDirection == 90) { boomerangSprite.move(0.1f, 0); boomerangSprite.rotate(BOOMERANG_ROTATION_SPEED); }
			if (projectileDirection == 0) { boomerangSprite.move(0, -0.1f); boomerangSprite.rotate(BOOMERANG_ROTATION_SPEED); }
			if (projectileDirection == 180) { boomerangSprite.move(0, 0.1f); boomerangSprite.rotate(BOOMERANG_ROTATION_SPEED); }
			if (projectileDirection == 270) { boomerangSprite.move(-0.1f, 0); boomerangSprite.rotate(BOOMERANG_ROTATION_SPEED); }

			if (!(boomerangSprite.getPosition().x < window_width && boomerangSprite.getPosition().x >0 && boomerangSprite.getPosition().y < window_height && boomerangSprite.getPosition().y >0))
			{
				fireActive = false;
				projectileDirection = link.getRotation();
			}
		}
		moveTowardPlayer(link, blackMage);
		window.clear(sf::Color::White);
		window.draw(link);
		window.draw(blackMage);
		if (fireActive) window.draw(boomerangSprite);
		window.display();
	}

	return 0;
}