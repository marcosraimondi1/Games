#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	Vector2f m_Position; // where is the player

	Sprite m_Sprite;

	Texture m_Texture;

	Vector2f m_Resolution; // screen resolution

	IntRect m_Arena; // size of current arena

	int m_TileSize; // size of each tile of the arena

	// direction player is currently moving
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	int m_Health; // current health

	int m_MaxHealth; // max health

	Time m_LastHit; // when was the player last hit

	float m_Speed; // speed in pixels per seconds

public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	void resetPlayerStats(); // called at the end of every game
	bool hit(Time timeHit);	 // handle the player getting hit by a zombie
	Time getLastHitTime();	 // how long ago was the player last hit
	FloatRect getPosition(); // where is the player
	Vector2f getCenter();	 // where is the center of the player
	float getRotation();	 // angle the player is facing to
	Sprite getSprite();		 // send copy of the sprite

	// move player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// stop player moving in specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void update(float elapsedTime, Vector2i mousePosition);

	void upgradeSpeed();  // boost speed
	void upgradeHealth(); // give health to player

	void increaseHealthLevel(int amount); // increase max health

	int getHealth(); // get current health

};