#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <vector>
#include <string>
#include "randgen.h"
#include "strutils.h"

//-- Struct for storing sections of the Snake (Blocks) --//
struct snakeSection
{
	sf::Vector2f position;
	sf::RectangleShape render;
	sf::RectangleShape innerRender;
	sf::Vector2f prevPosition;
};

struct snakeFood
{
	sf::Vector2f position;
	sf::RectangleShape render;
};

