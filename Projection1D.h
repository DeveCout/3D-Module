#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Projection1D : public sf::Drawable
{
	sf::Vector2f origin;
	float angle;
	sf::Vector2f direction;
	float screenDistance;
	sf::Vector2f point;

public : 

	Projection1D(sf::Vector2f const& origin_ ,float const& angle_,sf::Vector2f const&  direction_, float const& screenDistance_);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setPoint(sf::Vector2f const& p);
};

void rotateVect(sf::Vector2f& vect,float const& angle);