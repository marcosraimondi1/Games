#include "classes/Ball/Ball.hpp"
#include "classes/Bat/Bat.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <sstream>

int main()
{
	// Set up
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Pong", Style::Fullscreen);

	int score = 0;
	int lives = 3;

	// Create bat
	Bat bat(1920 / 2, 1080 - 20);

	// Create ball
	Ball ball(1920 / 2, 0);

	// Create HUD
	Font font;
	font.loadFromFile("assets/fonts/DS-DIGIT.ttf");
	Text hud;
	hud.setFont(font);
	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	// Sounds
	SoundBuffer bounceBuffer;
	bounceBuffer.loadFromFile("assets/sound/chop.wav");
	Sound bounce;
	bounce.setBuffer(bounceBuffer);

	SoundBuffer gameoverBuffer;
	gameoverBuffer.loadFromFile("assets/sound/death.wav");
	Sound gameover;
	gameover.setBuffer(gameoverBuffer);

	// Clock
	Clock clock;

	while (window.isOpen())
	{
		/*
		****************************
		****************************
		Handle the player input
		****************************
		*/

		// handle close
		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// move bat
		if (Keyboard::isKeyPressed(Keyboard::Left))
			bat.moveLeft();
		else
			bat.stopLeft();

		if (Keyboard::isKeyPressed(Keyboard::Right))
			bat.moveRight();
		else
			bat.stopRight();

		/*
		*****************************
		*****************************
		Update the bat, the ball and the HUD
		*****************************
		*/
		// Update the delta time
		Time dt = clock.restart();

		// Update Bat
		bat.update(dt);

		// Update Ball
		ball.update(dt);

		// Update Hud
		std::stringstream ss;
		ss << "Score: " << score << " Lives: " << lives;
		hud.setString(ss.str());

		// Check collitions
		// BOTTOM
		if (ball.getPosition().top > window.getSize().y)
		{
			// reverse the ball direction
			ball.reboundBottom();

			// Remove a life
			lives--;

			// Check for zero lives
			if (lives < 1)
			{
				// reset score
				score = 0;
				// reset lives
				lives = 3;
				gameover.play();
			}
		}
		// TOP
		if (ball.getPosition().top < 0)
		{
			ball.reboundBatOrTop();
			bounce.play();
		}

		// SIDES
		if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x)
		{
			ball.reboundSides();
			bounce.play();
		}

		// BAT
		if (ball.getPosition().intersects(bat.getPosition()))
		{
			ball.reboundBatOrTop();
			bounce.play();
			score++;
		}

		/*
		*****************************
		*****************************
		Draw the bat, the ball and the HUD
		*****************************
		*/

		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.display();
	}

	return 0;
}