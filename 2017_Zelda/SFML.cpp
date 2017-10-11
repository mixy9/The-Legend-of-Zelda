#include "main.h"
#include "Repository.h"


#define WINDOW_TITLE	"The Legend of Zelda"
#define IMAGE_NAME_LINK			"Link.png"
#define IMAGE_NAME_BOOMERANG	"Boomerang.png"
#define IMAGE_NAME_BLACKMAGE	"Blackmage.png"


namespace movement
{
	float boomerangRotationSpeed = 1000.0f;
	float boomerangMovementSpeed = 800.0f;
	float playerMovementSpeed = 400.0f;
	float enemyMovementSpeed = 1.0f;
	vector<sf::FloatRect*> colliders;
}

namespace filename
{
	string linkTexture = "textures\\Link.png";
	string blackMageTexture = "textures\\Blackmage.png";
	string boomerangTexture = "textures\\Boomerang.png";
	string backgroundTexture = "textures\\test_background.jpg";
	string boomerangThrow = "sound effects\\boomerang.ogg";
	string testMusic = "music\\test.ogg";


}

// experimental npc movement collision detection function. I got it to work with my current obstacles but it may or may not work with obstacles different to the ones used in this example
void moveTowardPlayer(const sf::Sprite& link, sf::Sprite& character, const sf::FloatRect& collider, sf::FloatRect* linkCollider, const sf::Time& delta)
{
	bool collision = false;
	for (auto aCollider : movement::colliders)
	{
		if (*aCollider == collider) continue;

		if (collider.intersects(*aCollider))
		{
			collision = true;

			if (*aCollider == *linkCollider)
			{
				character.move(400.0f*delta.asSeconds(), 400.0f*delta.asSeconds()); //moves out of the way to avoid locking link in place
				break;
			}

			//determines if an obstacle is vertical or horizontal, then determines which end of the obstacle the character is closer to, then moves the character toward that end (hopefully)
			else if (aCollider->height > aCollider->width && abs(character.getPosition().y - aCollider->top) > abs(character.getPosition().y - (aCollider->top + aCollider->height)))
			{
				character.move(0, 0.06f);
			}
			else if (aCollider->height > aCollider->width && abs(character.getPosition().y - aCollider->top) < abs(character.getPosition().y - (aCollider->top + aCollider->height)))
			{
				character.move(0, -0.06f);
			}
			else if (aCollider->height < aCollider->width && abs(character.getPosition().x - aCollider->left) > abs(character.getPosition().x - (aCollider->left + aCollider->width)))
			{
				character.move(0.06f, 0);
			}
			else if (aCollider->height < aCollider->width && abs(character.getPosition().x - aCollider->left) < abs(character.getPosition().x - (aCollider->left + aCollider->width)))
			{
				character.move(-0.06f, 0);
			}
		}
	}

	// if there is no collision the character just follows the player. This way of moving NPCs is probably not good enough and something better will have to be implemented eventually
	if (!collision)
		character.move(movement::enemyMovementSpeed*delta.asSeconds()*(link.getPosition() - character.getPosition()));
}


bool hasCollided(std::vector<sf::FloatRect*> colliders, sf::FloatRect* linkPtr, sf::FloatRect linkCollision)
{
	for (auto aCollider : colliders)
	{
		if (aCollider == linkPtr) continue;
		if (linkCollision.intersects(*aCollider))
			return true;
	}

	return false;
}

int main()
{
	int window_width = 1000;
	int window_height = 800;



	sf::RenderWindow window(sf::VideoMode(window_width, window_height), WINDOW_TITLE);

	sf::Clock clock;
	sf::Time delta;


	//Initialize and load the resource repository

	Repository repository;

	repository.loadTexture(filename::linkTexture);
	repository.loadTexture(filename::blackMageTexture);
	repository.loadTexture(filename::boomerangTexture);
	repository.loadTexture(filename::backgroundTexture);
	repository.loadSoundEffect(filename::boomerangThrow);
	
	



	//create the two obstacles used for this example
	sf::RectangleShape wall(sf::Vector2f(600, 15));
	wall.setFillColor(sf::Color(0, 0, 0));
	wall.setPosition(200, 400);

	sf::RectangleShape wall2(sf::Vector2f(15, 600));
	wall2.setFillColor(sf::Color(0, 0, 0));
	wall2.setPosition(500, 100);

	//collision boxes for the walls
	sf::FloatRect wallCollision = wall.getGlobalBounds();
	sf::FloatRect* wallPtr = &wallCollision;
	sf::FloatRect wall2Collision = wall2.getGlobalBounds();
	sf::FloatRect* wall2Ptr = &wall2Collision;


	//create Link, the player character
	sf::Sprite link;
	link.setTexture(*repository.getTexture(filename::linkTexture));
	link.setPosition(100, 400);
	link.setOrigin(50, 50);

	//collision box for link
	sf::FloatRect linkCollision = link.getGlobalBounds();
	sf::FloatRect* linkPtr = &linkCollision;


	//create the boomerang
	sf::Sprite boomerangSprite;
	boomerangSprite.setTexture(*repository.getTexture(filename::boomerangTexture));
	boomerangSprite.setScale(0.4f, 0.4f);
	boomerangSprite.setOrigin(10, 10);
	boomerangSprite.setPosition(0, 0);


	sf::Sprite background;
	background.setTexture(*repository.getTexture(filename::backgroundTexture));

	//collision box for the boomerang
	sf::FloatRect boomerangCollision = boomerangSprite.getGlobalBounds();

	//sound effects for the boomerang
	sf::Sound boomerangThrow;
	
	boomerangThrow.setBuffer(*repository.getSoundEffect(filename::boomerangThrow));

	sf::Music musicTest;
	musicTest.openFromFile(filename::testMusic);

	//create the example enemy
	sf::Sprite blackMage;
	blackMage.setTexture(*repository.getTexture(filename::blackMageTexture));
	blackMage.setPosition(800, 800);
	blackMage.setScale(0.2f, 0.2f);

	//collision box for the enemy
	sf::FloatRect blackMageCollision = blackMage.getGlobalBounds();
	sf::FloatRect* blackMagePtr = &blackMageCollision;

	//everyting on the screen that can block movement goes in the colliders vector
	movement::colliders.push_back(blackMagePtr);
	movement::colliders.push_back(linkPtr);
	movement::colliders.push_back(wallPtr);
	movement::colliders.push_back(wall2Ptr);

	//some variables used in the while loop below
	bool projectileActive = 0;
	auto projectileDirection = 0;
	auto lastValidPosition = link.getPosition();
	bool collision = false;

	/* Main Game Loop */
	while (window.isOpen())
	{
		/* Update */


		delta = clock.restart();

		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Handle movement
		{
			//move up
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				link.setRotation(0);

				//check for collision against every collider on the screen
				if (!hasCollided(movement::colliders, linkPtr, linkCollision))
					lastValidPosition = link.getPosition();
				else
					link.setPosition(lastValidPosition); //in case of collision move to last position where there was no collision

				link.move(0, -movement::playerMovementSpeed*delta.asSeconds()); //after we resolved any collisions we can attempt to move again

				if (!projectileActive) //without this part the projectile was changing direction mid flight :D
				{
					projectileDirection = (int)link.getRotation();
				}
			}

			//move down
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				link.setRotation(180);

				if (!hasCollided(movement::colliders, linkPtr, linkCollision))
					lastValidPosition = link.getPosition();
				else link.setPosition(lastValidPosition);
				link.move(0, movement::playerMovementSpeed*delta.asSeconds());

				if (!projectileActive)
				{
					projectileDirection = (int)link.getRotation();
				}
			}

			//move left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{

				link.setRotation(270);

				if (!hasCollided(movement::colliders, linkPtr, linkCollision))
					lastValidPosition = link.getPosition();
				else
					link.setPosition(lastValidPosition);

				link.move(-movement::playerMovementSpeed*delta.asSeconds(), 0);



				if (!projectileActive)
				{
					projectileDirection = (int)link.getRotation();
				}
			}

			//move right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				link.setRotation(90);

				if (!hasCollided(movement::colliders, linkPtr, linkCollision))
					lastValidPosition = link.getPosition();
				else
					link.setPosition(lastValidPosition);

				link.move(movement::playerMovementSpeed*delta.asSeconds(), 0);

				if (!projectileActive)
				{
					projectileDirection = (int)link.getRotation();
				}
			}
		}

		//fires a projectile
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!projectileActive)
			{
				boomerangSprite.setPosition(link.getPosition().x - 25, link.getPosition().y);
				boomerangThrow.play();
			}

			projectileActive = true;
		}

		//handles the projectile's flight
		//NOTE: when we move to having actual animations and stop rotating link,
		// we will have to determine the projectile direction some other way.
		// Curently it's based off link's rotation.
		if (projectileActive)
		{
			switch (projectileDirection)
			{
			case 90:
				{
					boomerangSprite.move(movement::boomerangMovementSpeed*delta.asSeconds(), 0);
					boomerangSprite.rotate(movement::boomerangRotationSpeed*delta.asSeconds());
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
			case 0:
				{
					boomerangSprite.move(0, -movement::boomerangMovementSpeed*delta.asSeconds());
					boomerangSprite.rotate(movement::boomerangRotationSpeed*delta.asSeconds());
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}

				}
				break;
			case 180:
				{
					boomerangSprite.move(0, movement::boomerangMovementSpeed*delta.asSeconds());
					boomerangSprite.rotate(movement::boomerangRotationSpeed*delta.asSeconds());
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
			case 270:
				{
					boomerangSprite.move(-movement::boomerangMovementSpeed*delta.asSeconds(), 0);
					boomerangSprite.rotate(movement::boomerangRotationSpeed*delta.asSeconds());
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
		/*	case 45:
				{
					boomerangSprite.move(movement::boomerangMovementSpeed / 1.5f, -movement::boomerangMovementSpeed / 1.5f);
					boomerangSprite.rotate(movement::boomerangRotationSpeed);
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
			case 315:
				{
					boomerangSprite.move(-movement::boomerangMovementSpeed / 1.5f, -movement::boomerangMovementSpeed / 1.5f);
					boomerangSprite.rotate(movement::boomerangRotationSpeed);
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
			case 135:
				{
					boomerangSprite.move(movement::boomerangMovementSpeed / 1.5f, movement::boomerangMovementSpeed / 1.5f);
					boomerangSprite.rotate(movement::boomerangRotationSpeed);
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;
			case 225:
				{
					boomerangSprite.move(-movement::boomerangMovementSpeed / 1.5f, movement::boomerangMovementSpeed / 1.5f);
					boomerangSprite.rotate(movement::boomerangRotationSpeed);
					for (auto aCollider : movement::colliders)
					{
						if (aCollider == linkPtr) continue;
						if (boomerangCollision.intersects(*aCollider))
						{
							projectileActive = false;
						}
					}
				}
				break;*/
			default:
				break;
			}
			if (boomerangSprite.getPosition().x > 800 && boomerangSprite.getPosition().y < 190)
			{
				if(musicTest.getStatus()!= sf::SoundSource::Status::Playing)
				musicTest.play();
			}

			if (!(boomerangSprite.getPosition().x < window_width
				&& boomerangSprite.getPosition().x > 0
				&& boomerangSprite.getPosition().y < window_height
				&& boomerangSprite.getPosition().y > 0))
			{
				projectileActive = false;
				projectileDirection = (int)link.getRotation();
			}
		}

		moveTowardPlayer(link, blackMage, blackMageCollision, &linkCollision, delta);

		//window.clear always goes first
		window.clear(sf::Color::White);

		//collision boxes have to update positions every cycle to follow their sprites
		linkCollision = link.getGlobalBounds();
		blackMageCollision = blackMage.getGlobalBounds();
		boomerangCollision = boomerangSprite.getGlobalBounds();

		/* Render */
		
		//draw everything
		{
			window.draw(background);
			window.draw(link);
			window.draw(blackMage);
			window.draw(wall);
			window.draw(wall2);
			

			if (projectileActive)
				window.draw(boomerangSprite);
		}

		/* Switch front-buffer with back-buffer */
		//.display() always goes last
		window.display();

	} //end of main game loop

	return 0;
}
