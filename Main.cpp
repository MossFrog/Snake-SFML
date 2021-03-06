#include "Main.h"

using namespace std;

void updateRender(snakeSection & actualSection);
void teleportCheck(sf::Vector2f & snakeHeadPos);
void randomFoodSpawn(snakeFood & mainFood);
bool foodCheck(snakeFood & mainFood, snakeSection & snakeHead);
void extendSnake(snakeSection & snakeHead, vector<snakeSection> & snakeBody);

int main()
{
	//-- Disable keyrepeat events to make the game tap oriented --//
	//-- Limit the framerate to prevent screen tearing (although this does cause dirty pixels) --//
	sf::RenderWindow mainWindow(sf::VideoMode(800, 600), "Snake - SFML", sf::Style::Close);
	mainWindow.setFramerateLimit(120);
	mainWindow.setKeyRepeatEnabled(false);


	//-- Definitions Section --//
	int gameState = 1;
	int score = 0;
	//-- Clock Cycle in miliseconds --//
	int clockCycle = 400;
	string mainDirection = "East";

	vector<snakeSection> snakeBody;
	bool addSection = false;
	bool moveEnable = true;

	//-- Declaring and loading the font into the software --//
	sf::Font mainFont;

	if (!mainFont.loadFromFile("ProFontWindows.ttf"))
	{
	}
	else
	{
		cout << "Loaded 'ProFontWindows.ttf'" << endl;
	}

	//-- Text and UI elements --//
	sf::Text scoreText;
	scoreText.setFont(mainFont);
	scoreText.setCharacterSize(35);
	scoreText.setPosition(10, 10);
	scoreText.setString("Score: " + itoa(score));
	scoreText.setColor(sf::Color(255, 255, 255, 100));


	//-- rectangleShapes used as a default renders for each snakeSection --//
	sf::RectangleShape defaultRect;
	defaultRect.setSize(sf::Vector2f(20, 20));
	defaultRect.setOrigin(10, 10);

	sf::RectangleShape defaultInnerRect;
	defaultInnerRect.setSize(sf::Vector2f(18, 18));
	defaultInnerRect.setOrigin(9, 9);
	defaultInnerRect.setFillColor(sf::Color::Black);

	//-- Definition of the snake "head" --//
	snakeSection snakeHead;
	snakeHead.position = sf::Vector2f(410, 310);
	snakeHead.render = defaultRect;
	snakeHead.innerRender = defaultInnerRect;

	//-- Food's random spawn position --//
	RandGen newGen;

	//-- Definition of the snake's "food" --//
	snakeFood mainFood;
	mainFood.render.setSize(sf::Vector2f(6, 6));
	mainFood.render.setOrigin(3, 3);
	mainFood.render.setPosition(sf::Vector2f(newGen.RandInt(40)*20 + 10, newGen.RandInt(30)*20 + 10));
	mainFood.position = mainFood.render.getPosition();


	//-- Clocks and timers --//
	sf::Clock moveTimer;
	moveTimer.restart();

	//------------------------//

	while (mainWindow.isOpen())
	{
		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mainWindow.close();

			if (event.type == sf::Event::KeyPressed)
			{

			}
		}

		//-- Extend the snake if addSection is true --//
		if (addSection)
		{
			extendSnake(snakeHead, snakeBody);
			addSection = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && mainDirection != "South" && moveEnable)
		{
			mainDirection = "North";
			moveEnable = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && mainDirection != "North" && moveEnable)
		{
			mainDirection = "South";
			moveEnable = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && mainDirection != "East" && moveEnable)
		{
			mainDirection = "West";
			moveEnable = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && mainDirection != "West" && moveEnable)
		{
			mainDirection = "East";
			moveEnable = false;
		}

		//-- Update the snakeHead depending on the direction --//
		if (moveTimer.getElapsedTime().asMilliseconds() > clockCycle)
		{
			snakeHead.prevPosition = snakeHead.position;

			if (mainDirection == "East")
			{
				snakeHead.position.x += 20;
			}

			else if (mainDirection == "West")
			{
				snakeHead.position.x -= 20;
			}

			else if (mainDirection == "South")
			{
				snakeHead.position.y += 20;
			}

			else if (mainDirection == "North")
			{
				snakeHead.position.y -= 20;
			}

			//-- Handle Teleportation --//
			teleportCheck(snakeHead.position);

			//-- Update the snakeBody --//
			if (snakeBody.size() > 0)
			{
				snakeBody[0].prevPosition = snakeBody[0].position;
				snakeBody[0].position = snakeHead.prevPosition;
				updateRender(snakeBody[0]);
			}

			for (int i = 1; i < snakeBody.size(); i++)
			{
				snakeBody[i].prevPosition = snakeBody[i].position;
				snakeBody[i].position = snakeBody[i - 1].prevPosition;
				updateRender(snakeBody[i]);
			}

			//-- Check if the food has been eaten --//
			if (foodCheck(mainFood, snakeHead))
			{
				randomFoodSpawn(mainFood);
				if (clockCycle >= 100)
				{
					clockCycle -= 50;
				}

				//-- Extend the SnakeBody by 1 unit --//
				addSection = true;
			}

			moveTimer.restart();
			moveEnable = true;
		}

		//-- Get the Mouse position to check button relativity --//
		sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);

		//-- Update all textual and UI elements --//
		scoreText.setString("Score: " + itoa(score));

		//-- Update all the rendered elements --//
		updateRender(snakeHead);

		mainWindow.clear(sf::Color::Black);


		mainWindow.draw(snakeHead.render);
		mainWindow.draw(snakeHead.innerRender);

		for (int i = 0; i < snakeBody.size(); i++)
		{
			mainWindow.draw(snakeBody[i].render);
			mainWindow.draw(snakeBody[i].innerRender);
		}

		mainWindow.draw(mainFood.render);

		mainWindow.draw(scoreText);

		mainWindow.display();
	}

	return 0;
}

void updateRender(snakeSection & actualSection)
{
	actualSection.render.setPosition(actualSection.position);
	actualSection.innerRender.setPosition(actualSection.position);
}

void teleportCheck(sf::Vector2f & snakeHeadPos)
{
	if (snakeHeadPos.x > 800)
	{
		snakeHeadPos.x = 10;
	}

	else if (snakeHeadPos.x < 0)
	{
		snakeHeadPos.x = 790;
	}

	else if (snakeHeadPos.y < 0)
	{
		snakeHeadPos.y = 590;
	}

	else if (snakeHeadPos.y > 600)
	{
		snakeHeadPos.y = 10;
	}
}

void randomFoodSpawn(snakeFood & mainFood)
{
	RandGen newGenerator;
	mainFood.render.setPosition(sf::Vector2f(newGenerator.RandInt(40) * 20 + 10, newGenerator.RandInt(30) * 20 + 10));
	mainFood.position = mainFood.render.getPosition();
}

bool foodCheck(snakeFood & mainFood, snakeSection & snakeHead)
{
	if (mainFood.position == snakeHead.position)
	{
		return true;
	}

	return false;
}

void extendSnake(snakeSection & snakeHead, vector<snakeSection> & snakeBody)
{
	snakeSection newSection;
	newSection = snakeHead;

	if (snakeBody.size() == 0)
	{ 
		newSection.position = snakeHead.prevPosition;
		newSection.innerRender.setPosition(snakeHead.prevPosition);
		newSection.render.setPosition(snakeHead.prevPosition);
	}

	else
	{
		newSection.position = snakeBody[snakeBody.size() - 1].prevPosition;
		newSection.innerRender.setPosition(newSection.position);
		newSection.render.setPosition(newSection.position);
	}

	snakeBody.push_back(newSection);
}