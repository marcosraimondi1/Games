// Include important C++ libraries here
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>

// Make code easier to type with "using namespace"
using namespace sf;

// Prototypes
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// where is the player/branch
enum class side
{
	LEFT,
	RIGHT,
	NONE
};

side branchPositions[NUM_BRANCHES];

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("assets/graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("assets/graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// make bacground trees
	Sprite trees[8];
	for (int i = 0; i < 8; i++)
	{
		trees[i] = *new Sprite();
		trees[i].setTexture(textureTree);
		trees[i].setPosition(-2000, 650);
		trees[i].setScale(0.35, 1);
	}

	// make a bee sprite
	Texture textureBee;
	textureBee.loadFromFile("assets/graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false; // bee currently moving ?
	float beeSpeed = 0.0f;	// bee flying speed

	// make cloud sprites
	Texture textureCloud;
	textureCloud.loadFromFile("assets/graphics/cloud.png");
	const int nClouds = 3;

	Sprite clouds[nClouds];
	bool activeClouds[nClouds];
	float cloudsSpeed[nClouds];

	for (int i = 0; i < nClouds; i++)
	{
		clouds[i] = *new Sprite();
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(0, i * 250);
		activeClouds[i] = false;
		cloudsSpeed[i] = 0.0f;
	}

	// Variables to control time itself
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920) / 2 - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// track if game is paused
	bool paused = true;

	// Draw text
	int score = 0;

	Text messageText;
	Text scoreText;

	// set fonts
	Font font;
	font.loadFromFile("assets/fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	// set actual strings
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// set size and color
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// position text
	FloatRect textRect = messageText.getLocalBounds();

	// now the center of the text is the coordinate (0,0), allowing to center it
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("assets/graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set origin to dead center so it can be spinned without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare player
	Texture texturePlayer;
	texturePlayer.loadFromFile("assets/graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("assets/graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare axe
	Texture textureAxe;
	textureAxe.loadFromFile("assets/graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare flying log
	Texture textureLog;
	textureLog.loadFromFile("assets/graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Log variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control player input
	bool acceptInput = false;

	// Prepare the sounds
	// Chopping sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("assets/sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	// Death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("assets/sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Timeout sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("assets/sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen())
	{

		/*
		****************************************
		Handle the players input
		****************************************
		*/

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 6;

			// Make all branches disappear
			for (int i = 0; i < NUM_BRANCHES; i++)
				branchPositions[i] = side::NONE;

			// hide gravestone
			spriteRIP.setPosition(675, 2000);

			// Move player into position
			spritePlayer.setPosition(580, 720);

			// Create Random Forest
			for (int i = 0; i < 8; i++)
			{
				trees[i].setPosition((rand() % 100) + 350 * i, (rand() % 150) - 200);
				trees[i].setScale(0.20 + (rand() % 5) / 15.5, 1);
			}

			acceptInput = true;
		}

		if (acceptInput)
		{
			// right key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// make sure player is on the right
				playerSide = side::RIGHT;

				// Update score
				score++;
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				// add time
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				// Set log flying to left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

				// play chop sound
				chop.play();
			}
			// left key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// make sure player is on the left
				playerSide = side::LEFT;

				// Update score
				score++;
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				// add time
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				// Set log flying to left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				// play chop sound
				chop.play();
			}
		}
		/*
		****************************************
		Update the scene
		****************************************
		*/

		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// substract from remaining time
			timeRemaining -= dt.asSeconds();

			// resize the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;

				// change the message shown to the player
				messageText.setString("Out of time !!!");

				// Reposition
				FloatRect textRect = messageText.getLocalBounds();

				// now the center of the text is the coordinate (0,0), allowing to center it
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play sound
				outOfTime.play();
			}

			// Setup the bee
			if (!beeActive)
			{
				srand((int)time(0));			 // seed rand
				beeSpeed = (rand() % 200) + 200; // set speed (200-400)

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500; // set height (500-1000)

				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// move bee
				// dt.asSeconds < 1 -> depends on each computer frame rate and acts as an equalizer
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// check border collition
				if (spriteBee.getPosition().x < -100)
					beeActive = false; // new set up in next frame
			}

			// Setup the clouds

			for (int i = 0; i < nClouds; i++)
			{
				if (!activeClouds[i])
				{
					srand((int)time(0) * (10 + 10 * i));
					cloudsSpeed[i] = (rand() % 200) + 20;

					srand((int)time(0) * 10);
					float height = (rand() % (150 + i * 150));

					clouds[i].setPosition(-250, height);
					activeClouds[i] = true;
				}
				else
				{
					clouds[i].setPosition(
						clouds[i].getPosition().x + (cloudsSpeed[i] * dt.asSeconds()),
						clouds[i].getPosition().y);

					// check border collition
					if (clouds[i].getPosition().x > 1920)
						activeClouds[i] = false; // new set up in next frame
				}
			}

			// Update the branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move sprite to the left side
					branches[i].setPosition(610, height);

					// flip to the other side
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);

					// set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// check branch / player collition
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;
				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// hide player
				spritePlayer.setPosition(2000, 660);

				// Change the text on the screen
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				// now the center of the text is the coordinate (0,0), allowing to center it
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// play sound
				death.play();
			}
		}
		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		for (int i = 0; i < nClouds; i++)
			window.draw(clouds[i]);

		for (int i = 0; i < 8; i++)
			window.draw(trees[i]);

		for (int i = 0; i < NUM_BRANCHES; i++)
			window.draw(branches[i]);

		window.draw(spriteTree);

		window.draw(spritePlayer);

		window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
			window.draw(messageText);

		// Show everything we just drew
		window.display();
	}

	return 0;
}

void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// spawn new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r)
	{
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		default:
			// (2,3,4)
			branchPositions[0] = side::NONE;
			break;
	}
}
