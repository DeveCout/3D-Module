#include "Plane.h"

Plane::Plane(sf::Vector3f const& p1, sf::Vector3f const& p2, sf::Vector3f const& p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	color = sf::Color::White;
}


void Plane::drawWith(sf::RenderTarget& target, Projection const& p,sf::Vector3f const& light)
{
	
	sf::Vertex projection[] =
	{
		sf::Vertex(p.getProjectivePos(points[0]),color),
		sf::Vertex(p.getProjectivePos(points[1]),color),
		sf::Vertex(p.getProjectivePos(points[2]),color)
	};
	target.draw(projection,3,sf::Triangles);
}

void Plane::transform(std::array<float, 9> const& matrix)
{
	points[0] = Matrix::multiply(matrix, points[0]);
	points[1] = Matrix::multiply(matrix, points[1]);
	points[2] = Matrix::multiply(matrix, points[2]);
}

void Plane::setColor(sf::Color const& newColor)
{
	color = newColor;
}
