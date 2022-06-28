#include "./Classes/Player/Player.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

int main()
{

	// states of the game
	enum class State
	{
		PAUSED,
		LEVELING_UP,
		GAME_OVER,
		PLAYING
	};

	State state = State::GAME_OVER; // start game in GAME_OVER state

	// Get screen resolution
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	// Create SFML window
	RenderWindow window(
		VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	// create SFML main View
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;		// clock
	Time gameTimeTotal; // how long has the player been active

	Vector2f mouseWorldPositition;	// where the mouse in relation to world coordinates
	Vector2i mouseScreenPositition; // where the mouse in relation to screen coordinates

	Player player; // player instance
	IntRect arena; // boundaries of arena

	// game loop
	while (window.isOpen())
	{
		/*
		************
		Handle Input
		************
		*/
		// Handle events by polling
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause game
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
					state = State::PAUSED;
				// Restart game if paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset clock so there isnt a frame jump
					clock.restart();
				}
				// Start a new game if GAME_OVER
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
					state = State::LEVELING_UP;

				if (state == State::PLAYING)
				{}
			}
		}

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// Handle WASD while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of keys
			if (Keyboard::isKeyPressed(Keyboard::W))
				player.moveUp();
			else
				player.stopUp();
			if (Keyboard::isKeyPressed(Keyboard::S))
				player.moveDown();
			else
				player.stopDown();
			if (Keyboard::isKeyPressed(Keyboard::D))
				player.moveRight();
			else
				player.stopRight();
			if (Keyboard::isKeyPressed(Keyboard::A))
				player.moveLeft();
			else
				player.stopLeft();
		}

		// Handle LEVELING up state
		if (state == State::LEVELING_UP)
		{
			if (Keyboard::isKeyPressed(Keyboard::Num1))
				state = State::PLAYING;
			if (Keyboard::isKeyPressed(Keyboard::Num2))
				state = State::PLAYING;
			if (Keyboard::isKeyPressed(Keyboard::Num3))
				state = State::PLAYING;
			if (Keyboard::isKeyPressed(Keyboard::Num4))
				state = State::PLAYING;
			if (Keyboard::isKeyPressed(Keyboard::Num5))
				state = State::PLAYING;
			if (Keyboard::isKeyPressed(Keyboard::Num6))
				state = State::PLAYING;
			if (state == State::PLAYING)
			{
				// Prepare the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				int tileSize = 50;

				// Spawn player in middle
				player.spawn(arena, resolution, tileSize);

				// Reset the clock
				clock.restart();
			}
		}

		/*
		************
		UPDATE THE FRAME
		************
		*/
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();

			// Update the total game time
			gameTimeTotal += dt;

			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPositition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPositition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player
			mainView.setCenter(player.getCenter());
		}

		/*
		************
		Draw the scene
		************
		*/
		if (state == State::PLAYING)
		{
			window.clear();
			// set the mainview to be displayed in the window
			// and draw everything related to it
			window.setView(mainView);

			// Draw the player
			window.draw(player.getSprite());
		}
		if (state == State::LEVELING_UP)
		{
		}
		if (state == State::PAUSED)
		{
		}
		if (state == State::GAME_OVER)
		{
		}
		window.display();

		return 0;
	}
