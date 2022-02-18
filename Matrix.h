#pragma once
#include <array>
#include <SFML/System.hpp>

class Matrix
{
	
	public :

	static std::array<float,9> multiply (std::array<float,9> const& a, std::array<float, 9> const& b);
	static sf::Vector3f multiply(std::array<float,9> const& a, sf::Vector3f const& b);
};

