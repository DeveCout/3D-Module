#pragma once
#include <vector>
#include <array>
#include "Projection.h"
#include "Matrix.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Plane
{
	std::array<sf::Vector3f,3> points;
	sf::Color color;

public :
	Plane(sf::Vector3f const& p1, sf::Vector3f const& p2, sf::Vector3f const& p3);
	void drawWith(sf::RenderTarget& target,Projection const& p,sf::Vector3f const& light = sf::Vector3f(0,0,0));
	void transform(std::array<float, 9> const& matrix);
	void setColor(sf::Color const& newColor);
};

