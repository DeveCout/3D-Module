#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#define POINT_SIZE 1

class Projection : public sf::Drawable
{
	/// <summary>
	/// Position de la camera BASE CARTHESIENNE
	/// </summary>
	sf::Vector3f origin;
	
	/// <summary>
	/// Représente les roration permettant d'obtenir la camera
	/// </summary>
	sf::Vector3f direction;
	sf::Vector3f directionNormalize;

	std::array<float,9> matrix;
	std::array<float,9> invMatrix;

	float screenDistance;
	std::vector<sf::Vector3f> world;

public :

	Projection(sf::Vector3f const& origin_,sf::Vector3f const& direction_,float const& screenDistance_);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setWorld(std::vector<sf::Vector3f> const& world_);
	sf::Vector3f getRelativePos(sf::Vector3f const& p) const;
	sf::Vector2f getProjectivePos(sf::Vector3f const& p) const;
	static void multiply(std::array<float, 9>& obj, std::array<float, 9> const& obj2);
	static void multiply(sf::Vector3f& obj, std::array<float, 9> const& obj2);
	static void multiply(std::array<float, 9> & obj, float const& a);
	static std::array<float, 9> getInvMatrix(std::array<float,9> const& mat);
	void rotateCamera(sf::Vector3f const& roation);
	void moveCamera(sf::Vector3f const& movement);
	void moveScreenDistance(float const& d);
	sf::Vector3f getDirection()const;
	std::string toString()const;
	void refreshCamera();
	void drawSphere(std::vector<sf::Vector3f> const& sphere,sf::Vector3f const& lighting,sf::RenderTarget& target, bool const& canSeeThrough = true)const;
	void drawSphere(std::vector<sf::Vector3f> const& sphere,std::vector<sf::Vector3f> const& lightings,sf::RenderTarget& target, bool const& canSeeThrough = true)const;
	sf::Vector3f getOrigin()const;
};

float scalarProduct(sf::Vector3f const& a,sf::Vector3f const& b);
float getNorm(sf::Vector3f const& a);