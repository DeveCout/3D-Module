#include "Projection1D.h"

Projection1D::Projection1D(sf::Vector2f const& origin_, float const& angle_, sf::Vector2f const& direction_, float const& screenDistance_) : origin(origin_), angle(angle_), direction(direction_),screenDistance(screenDistance_)
{
}

void Projection1D::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Vector2f subConeBranch1 = direction;
	sf::Vector2f subConeBranch2 = direction;
	
	rotateVect(subConeBranch1, angle);
	rotateVect(subConeBranch2, -angle);

	sf::Vertex screen[]
	{
		sf::Vertex(sf::Vector2f(origin.x + screenDistance,tan(angle) * screenDistance),sf::Color::Red),
		sf::Vertex(sf::Vector2f(origin.x + screenDistance,tan(angle) * -screenDistance),sf::Color::Red)
	};

	sf::CircleShape c(4);
	float xp = screenDistance;//Distance entre l'origine est l'ecran

	c.setPosition(sf::Vector2f(origin.x + screenDistance - 2, (xp * point.y / point.x) - 2));
	c.setFillColor(sf::Color(255, 0, 0, 100));


	subConeBranch1.x *= 300;
	subConeBranch1.y *= 300;
	subConeBranch2.x *= 300;
	subConeBranch2.y *= 300;

	sf::Vertex cone[]
	{
		sf::Vertex(sf::Vector2f(origin.x + subConeBranch1.x,origin.y + subConeBranch1.y)),
		sf::Vertex(sf::Vector2f(origin.y,origin.y),sf::Color::Red),
		sf::Vertex(sf::Vector2f(origin.x + subConeBranch2.x,origin.y + subConeBranch2.y))
	};

	sf::Vertex pointRay[]
	{
		sf::Vertex(origin,sf::Color::Green),
		sf::Vertex(point,sf::Color::Green)
	};

	sf::Vertex directionLine[]
	{
		sf::Vertex(origin),
		sf::Vertex(sf::Vector2f(100,direction.y * screenDistance))
	};


	target.draw(c);
	target.draw(screen, 2, sf::LinesStrip);
	target.draw(cone, 3, sf::LinesStrip);
	target.draw(pointRay, 2, sf::LinesStrip);
	target.draw(directionLine, 2, sf::LinesStrip);
}

void Projection1D::setPoint(sf::Vector2f const& p)
{
	point = p;
}

void rotateVect(sf::Vector2f& vect,float const& angle)
{
	float x = vect.x;
	float y = vect.y;
	vect.x = x * cos(angle) - y * sin(angle);
	vect.y = x * sin(angle) + y * cos(angle);
}