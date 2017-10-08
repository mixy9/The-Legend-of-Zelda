#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>


using namespace std;

namespace movement
{
	float boomerangRotationSpeed = 0.2f;
	float boomerangMovementSpeed = 0.1f;
	float playerMovementSpeed = 0.06f;
	float enemyMovementSpeed = 0.0001f;
	vector<sf::FloatRect*> colliders;
}
	
	// experimental npc movement collision detection function. I got it to work with my current obstacles but it may or may not work with obstacles different to the ones used in this example
	void moveTowardPlayer(const sf::Sprite& link, sf::Sprite& character, const sf::FloatRect& collider, sf::FloatRect* linkCollider)
	{
	
		bool collision = false;
		for (auto a : movement::colliders)
		{			
			if (*a == collider) continue;
			if (collider.intersects(*a))
			{
				collision = true;
				if (*a == *linkCollider)
				{
					character.move(0.1, 0.1); //moves out of the way to avoid locking link in place
					break;
				}

				//determines if an obstacle is vertical or horizontal, then determines which end of the obstacle the character is closer to, then moves the character toward that end (hopefully)
				else if (a->height > a->width && abs(character.getPosition().y - a->top) > abs(character.getPosition().y - (a->top + a->height)))
				{
					character.move(0, 0.06f);
				}
				else if (a->height > a->width && abs(character.getPosition().y - a->top) < abs(character.getPosition().y - (a->top + a->height)))
				{
					character.move(0, -0.06f);
				}
				else if (a->height < a->width && abs(character.getPosition().x - a->left) > abs(character.getPosition().x - (a->left + a->width)))
				{
					character.move(0.06f, 0);
				}
				else if (a->height < a->width && abs(character.getPosition().x - a->left) < abs(character.getPosition().x - (a->left + a->width)))
				{
					character.move(-0.06f, 0);
				}
			}
			
		}
		// if there is no collision the character just follows the player. This way of moving NPCs is probably not good enough and something better will have to be implemented eventually
		if (!collision)character.move(movement::enemyMovementSpeed*(link.getPosition() - character.getPosition()));
		
			
	}
	




int main()
{
	int window_width = 1000;
	int window_height = 800;

	

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "The Legend of Zelda");

	//load needed textures
	sf::Texture player;
	player.loadFromFile("Link.png");

	sf::Texture boomerang;
	boomerang.loadFromFile("Boomerang.png");

	sf::Texture enemy;
	enemy.loadFromFile("Blackmage.png");


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
	link.setTexture(player);
	link.setPosition(100, 400);
	link.setOrigin(50, 50);

	//collision box for link
	sf::FloatRect linkCollision = link.getGlobalBounds();
	sf::FloatRect* linkPtr = &linkCollision;
	
	
	//create the boomerang
	sf::Sprite boomerangSprite;
	boomerangSprite.setTexture(boomerang);
	boomerangSprite.setScale(0.4f, 0.4f);
	boomerangSprite.setOrigin(10, 10);
	boomerangSprite.setPosition(0, 0);

	//collision box for the boomerang
	sf::FloatRect boomerangCollision = boomerangSprite.getGlobalBounds();

	//create the example enemy
	sf::Sprite blackMage;
	blackMage.setTexture(enemy);
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

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		//move up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
		{
			link.setRotation(0);		

			//check for collision against every collider on the screen
			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue; //this line prevents link from colliding with himself
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition(); 
			else link.setPosition(lastValidPosition); //in case of collision move to last position where there was no collision
			link.move(0, -movement::playerMovementSpeed); //after we resolved any collisions we can attempt to move again

			if (!projectileActive) //without this part the projectile was changing direction mid flight :D
			{
				projectileDirection = link.getRotation();
			}

		}
		//move down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			link.setRotation(180);

			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(0, movement::playerMovementSpeed);

			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}
		//move left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			
			link.setRotation(270);		

			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					link.setPosition(lastValidPosition);
					break;
				}
				else collision = false;
			}
			if(!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(-movement::playerMovementSpeed, 0);
			


			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}

		//move right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			link.setRotation(90);
			
			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(movement::playerMovementSpeed, 0);

			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}

		//move up right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			link.setRotation(45);
			

			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(movement::playerMovementSpeed / 1.41f, -movement::playerMovementSpeed / 1.41f); //without dividing by 1.41 diagonal move speed would be faster than up-down/left right.
																									  //number 1.41 chosen because it feels like the movement speeds are the same in all directions
			if (!projectileActive)																	 //and because it's the square root of 2 and it makes it look like i was doing some math when i absolutely was not
			{
				projectileDirection = link.getRotation();
			}
		}

		//move up left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			link.setRotation(315);
		

			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(-movement::playerMovementSpeed / 1.41f, -movement::playerMovementSpeed / 1.41f);

			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}
		//move down right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			link.setRotation(135);
		

			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(movement::playerMovementSpeed / 1.41f, movement::playerMovementSpeed / 1.41f);

			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}

		//move down left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			link.setRotation(225);
		
			
			for (auto a : movement::colliders)
			{
				if (a == linkPtr) continue;
				if (linkCollision.intersects(*a))
				{
					collision = true;
					break;
				}
				else collision = false;

			}
			if (!collision)lastValidPosition = link.getPosition();
			else link.setPosition(lastValidPosition);
			link.move(-movement::playerMovementSpeed / 1.41f, movement::playerMovementSpeed / 1.41f);

			if (!projectileActive)
			{
				projectileDirection = link.getRotation();
			}
		}
	
		//fires a projectile
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (!projectileActive)
			{
				boomerangSprite.setPosition(link.getPosition().x - 25, link.getPosition().y);
			}
			projectileActive = true;

		}

		//handles the projectile's flight
		//NOTE: when we move to having actual animations and stop rotating link, we will have to determine the projectile direction some other way. Curently it's based off link's rotation.
		if (projectileActive)
		{
			if (projectileDirection == 90)
			{
				boomerangSprite.move(movement::boomerangMovementSpeed, 0); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 0)
			{
				boomerangSprite.move(0, -movement::boomerangMovementSpeed); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}

			}
			if (projectileDirection == 180)
			{
				boomerangSprite.move(0, movement::boomerangMovementSpeed); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 270)
			{
				boomerangSprite.move(-movement::boomerangMovementSpeed, 0); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 45)
			{
				boomerangSprite.move(movement::boomerangMovementSpeed/1.5f, -movement::boomerangMovementSpeed/1.5f); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 315)
			{
				boomerangSprite.move(-movement::boomerangMovementSpeed / 1.5f, -movement::boomerangMovementSpeed / 1.5f); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 135)
			{
				boomerangSprite.move(movement::boomerangMovementSpeed / 1.5f, movement::boomerangMovementSpeed / 1.5f); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}
			if (projectileDirection == 225)
			{
				boomerangSprite.move(-movement::boomerangMovementSpeed / 1.5f, movement::boomerangMovementSpeed / 1.5f); boomerangSprite.rotate(movement::boomerangRotationSpeed);
				for (auto a : movement::colliders)
				{
					if (a == linkPtr) continue;
					if (boomerangCollision.intersects(*a)) { projectileActive = false; }
				}
			}

			if (!(boomerangSprite.getPosition().x < window_width && boomerangSprite.getPosition().x >0 && boomerangSprite.getPosition().y < window_height && boomerangSprite.getPosition().y >0))
			{
				projectileActive = false;
				projectileDirection = link.getRotation();
			}
		}

		moveTowardPlayer(link, blackMage, blackMageCollision, &linkCollision); 

		//window.clear always goes first
		window.clear(sf::Color::White);

		//collision boxes have to update positions every cycle to follow their sprites
		linkCollision		= link.getGlobalBounds(); 
		blackMageCollision	= blackMage.getGlobalBounds();
		boomerangCollision	= boomerangSprite.getGlobalBounds();

		//draw everything
		window.draw(link);
		window.draw(blackMage);
		window.draw(wall);
		window.draw(wall2);
		if (projectileActive) window.draw(boomerangSprite);

		//.display() always goes last
		window.display();

	}//end of main while loop
	
	return 0;

}