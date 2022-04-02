#include <iostream>
#include <conio.h>	 // for input
#include <windows.h> // for sleeping
using namespace std;

// D E F I N I T I O N S
enum eDirection
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// C L A S S E S
class Point
{
private:
	int position[2];
	char rep;

public:
	Point(int x = 0, int y = 0, char c = '.')
	{
		position[0] = x < 0 ? 0 : x;
		position[1] = y < 0 ? 0 : y;
		rep = c;
	}

	void setPosition(int x, int y)
	{
		position[0] = x;
		position[1] = y;
	}

	void setRep(char c)
	{
		rep = c;
	}

	int *getPosition()
	{
		return position;
	}

	bool equals(Point p)
	{
		return (p.getPosition()[0] == position[0] && p.getPosition()[1] == position[1]);
	}

	char toString() { return rep; }
};

class Snake
{
private:
	Point head;
	Point tail[100];
	int nTail;
	eDirection dir;
	int speed;
	void processInput();
	int movement_range[2]; // maxX, maxY

public:
	Snake(int = 0, int = 0, int = 1, int = 20, int = 20);

	void setSpeed(int);
	void setDirection(eDirection);

	Point getHead();
	int getNTail();
	Point *getTail();

	void eat();
	void move();
	bool isDead();
};

class Map
{
private:
	Snake snake;
	Point fruit;
	int score;
	int width;
	int height;

	// Setters
	void setHeight(int);
	void setWidth(int);

public:
	Map(int = 20, int = 20);

	// Getters
	int getScore();
	int getWidth();
	int getHeight();

	bool eating_fruit();
	string toString();
	bool next_state();
};

class Game
{
private:
	Map map;

public:
	Game();

	void play();
};

// M A I N

int main()
{
	Game snake_game = *new Game();
	snake_game.play();
	return EXIT_SUCCESS;
}

// M E T H O D S
Game::Game()
{
	map = *new Map(50, 25);
}

void Game::play()
{
	while (map.next_state())
	{
		system("cls");

		cout << map.toString();

		if (map.getScore() > 900)
			break;
		Sleep(25);
	}

	cout << endl
		 << "G A M E   O V E R" << endl
		 << "SCORE: " << map.getScore() << endl;
}

Map::Map(int w, int h)
{
	score = 0;
	snake = *new Snake(w / 2, h / 2, 1, w, h);

	fruit = *new Point(rand() % w, rand() % h, 'F');
	setWidth(w);
	setHeight(h);
}

int Map::getScore()
{
	return score;
}

bool Map::next_state()
{
	if (eating_fruit())
	{
		snake.eat();
		score += 10;
		fruit.setPosition(rand() % getWidth(), rand() % getHeight());
	}

	snake.move();

	if (snake.isDead())
		return 0;
	// return 0;

	return 1;
}

void Map::setHeight(int h)
{
	height = h > 0 ? h : 0;
}

void Map::setWidth(int w)
{
	width = w > 0 ? w : 0;
}

int Map::getHeight()
{
	return height;
}

int Map::getWidth()
{
	return width;
}

bool Map::eating_fruit()
{
	return fruit.equals(snake.getHead());
}

string Map::toString()
{
	string map = "";

	// top border
	for (int i = 0; i < getWidth() + 2; i++)
		map += "#";

	map += "\n";

	// MAP
	Point p = *new Point();
	for (int i = 0; i < getHeight(); i++)
	{
		for (int j = 0; j <= getWidth(); j++)
		{
			if (j == 0 || j == getWidth())
				map += "#";

			p.setPosition(j, i);

			if (p.equals(fruit))
				map += fruit.toString();
			else if (p.equals(snake.getHead()))
				map += snake.getHead().toString();
			else
			{
				bool printSpace = true;
				Point *tail = snake.getTail();

				for (int k = 0; k < snake.getNTail(); k++)
				{
					if (p.equals(tail[k]))
					{
						map += tail[k].toString();
						printSpace = false;
					}
				}

				if (printSpace)
					map += " ";
			}
		}
		map += "\n";
	}

	// bottom border
	for (int i = 0; i < getWidth() + 2; i++)
		map += "#";

	map += "\n";
	return map;
}

Snake::Snake(int startX, int startY, int startSpeed, int maxX, int maxY)
{
	head = *new Point(startX, startY, 'O');

	setDirection(STOP);
	setSpeed(startSpeed);

	nTail = 0;

	movement_range[0] = maxX;
	movement_range[1] = maxY;

	for (int i = 0; i < 100; i++)
	{
		tail[i] = *new Point();
	}
}

int Snake::getNTail()
{
	return nTail;
}

Point *Snake::getTail()
{
	return tail;
}

void Snake::setSpeed(int s)
{
	speed = s < 0 ? 0 : s;
}

void Snake::setDirection(eDirection d)
{
	switch (d)
	{

	case RIGHT:
		dir = (dir != LEFT) ? d : dir;
		break;

	case LEFT:
		dir = (dir != RIGHT) ? d : dir;
		break;

	case UP:
		dir = (dir != DOWN) ? d : dir;
		break;

	case DOWN:
		dir = (dir != UP) ? d : dir;
		break;

	default:
		dir = STOP;
		setSpeed(0);
		break;
	}
}

Point Snake::getHead()
{
	return head;
}

void Snake::eat()
{
	tail[nTail].setRep('o');
	nTail++;
}

void Snake::processInput()
{
	if (_kbhit()) // if no char pressed, returns 0
	{
		switch (_getch())
		{
		case 'a':
			setDirection(LEFT);
			break;
		case 'd':
			setDirection(RIGHT);
			break;
		case 's':
			setDirection(DOWN);
			break;
		case 'w':
			setDirection(UP);
			break;
		case 'p':
			system("PAUSE");
			break;
		default:
			break;
		}
	}
}

void Snake::move()
{

	processInput();

	int *hPos = head.getPosition();

	int pHead[2] = {hPos[0], hPos[1]}; // save previous head position

	// move head
	switch (dir)
	{
	case RIGHT:
		head.setPosition(hPos[0] + speed, hPos[1]);
		break;

	case LEFT:
		head.setPosition(hPos[0] - speed, hPos[1]);
		break;

	case UP:
		head.setPosition(hPos[0], hPos[1] - speed);
		break;

	case DOWN:
		head.setPosition(hPos[0], hPos[1] + speed);
		break;

	default:
		break;
	}

	// CHECK MOVEMENT RANGE
	if (hPos[0] <= 0)
		head.setPosition(movement_range[0] - 1, hPos[1]);

	if (hPos[0] >= movement_range[0])
		head.setPosition(0, hPos[1]);

	if (hPos[1] < 0)
		head.setPosition(hPos[0], movement_range[1] - 1);

	if (hPos[1] >= movement_range[1])
		head.setPosition(hPos[0], 0);

	// move tail
	if (nTail == 0 || dir == STOP)
		return;

	Point pTail = tail[0];
	tail[0].setPosition(pHead[0], pHead[1]);

	Point pTail2;

	for (int i = 1; i < nTail; i++)
	{
		pTail2 = tail[i];
		tail[i] = pTail;
		pTail = pTail2;
	}
}

bool Snake::isDead()
{
	for (int i = 0; i < nTail; i++)
	{
		if (head.equals(tail[i]))
			return true;
	}

	return false;
}
