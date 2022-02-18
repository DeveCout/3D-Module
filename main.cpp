#include <SFML/Graphics.hpp>
#include <iostream>
#include "Projection1D.h"
#include "Projection.h"
#include "Plane.h"

#define PI 3.1415926535

std::vector<sf::Vector3f> createSphere(unsigned int const& meridian, unsigned int const& tropical, float const& radius = 100);
std::vector<sf::Vector3f> createRandomSphere(unsigned int const& nbPoint,float const& radius = 100);
std::vector<sf::Vector3f> createCube(float const& sideSize, unsigned int const& dotPerSide);

std::vector<sf::Vector3f> createSphere(unsigned int const& meridian,unsigned int const& tropical,float const& radius)
{
	std::vector<sf::Vector3f> sphere;
	sphere.push_back(sf::Vector3f(0,0,0));//The central vect to Calcul the "Lighting"
	sphere.push_back(sf::Vector3f(0,radius,0)); //Pole
	sphere.push_back(sf::Vector3f(0,-radius,0));
	float const angleRatioMer = PI / meridian;
	float const angleRatioTro = PI / tropical;
	for (size_t i = 0; i < tropical; i++)
	{
		for (size_t j = 0; j < meridian * 2; j++)
		{
			sf::Vector3f pos;
			float localRadius = sin(angleRatioTro * i) * radius;

			pos.x = sin(angleRatioMer * j) * localRadius;
			pos.y = cos(angleRatioMer * j) * localRadius;

			pos.z = cos(angleRatioTro * i) * radius;
			sphere.push_back(pos);
		}
	}
	return sphere;
}

std::vector<sf::Vector3f> createRandomnSphere(unsigned int const& nbPoint,float const& radius)
{
	std::vector<sf::Vector3f> sphere;
	sphere.push_back(sf::Vector3f(0, 0, 0));//The central vect to Calcul the "Lighting"
	sf::Vector3f pos;

	for (size_t a = 0; a < nbPoint; a++)
	{

		int i = rand();
		int j = rand();

		sf::Vector3f pos;
		float localRadius = sin(i) * radius;

		pos.x = sin(j) * localRadius;
		pos.y = cos(j) * localRadius;

		pos.z = cos(i) * radius;
		sphere.push_back(pos);
	}
	return sphere;
}

std::vector<sf::Vector3f> createCube(float const& sideSize, unsigned int const& dotPerSide)
{
	std::vector<sf::Vector3f> cube = std::vector<sf::Vector3f>();
	
	float step = sideSize / dotPerSide;
	float maxRange = sideSize - step;
	for (float y = 0; y < maxRange; y += step)
	{
		for (float x = 0; x < maxRange; x += step)
		{
			for (float z = 0; z < maxRange; z += step)
			{
				if (x == 0 || y == 0 || z == 0 || x > maxRange-step+1 || y > maxRange- step+1 || z > maxRange- step+1)
					cube.push_back(sf::Vector3f(x, y, z));
			}
		}
	}
	return cube;
}


int main()
{
	//------------------------------------------ TEST------------------------------

	Plane p1 = Plane(sf::Vector3f(0, 0, 0), sf::Vector3f(100, 0, 100), sf::Vector3f(100, 0, 0));
	Plane ground = Plane(sf::Vector3f(-3000, 1000, -3000), sf::Vector3f(0, 1000, 3000), sf::Vector3f(3000, 1000, -3000));
	ground.setColor(sf::Color(100,100,100,255));
	Plane p2 = Plane(sf::Vector3f(0, 0, 0), sf::Vector3f(100, 0, 100), sf::Vector3f(0, 0, 100));

	std::vector<sf::Vector3f> world = createSphere(80,30);
	std::vector<sf::Vector3f> lights;
	lights.push_back(sf::Vector3f(120, 0, 0));
	lights.push_back(sf::Vector3f(0, 0, 0));

	//------------------------------------------------------------------------

	sf::RenderWindow rWindow(sf::VideoMode(900, 900), "3D");
	sf::View view = sf::View(sf::Vector2f(0,0),sf::Vector2f(900,900));
	rWindow.setView(view);

	sf::Event event;	
	Projection p3D = Projection(sf::Vector3f(-150,0,0),sf::Vector3f(0,PI*1.5,0),500);

	sf::Vector2i mousePos;

	//-----------------------MOUSE--------------------

	//---------------------MAP---------------------------

	float angle = 0.1;


	p3D.setWorld(world);

	//---------------------TIME MANAGEMENT-----------------

	sf::Clock clock;
	sf::Time dt;
	
	sf::Vector3f move;

	while (rWindow.isOpen())
	{
		while (rWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)rWindow.close();

		}

		mousePos = sf::Mouse::getPosition(rWindow);

		dt = clock.restart();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			move.z = 100;
		}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			move.z = -100;
		}
		else
			move.z = 0;


		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{ 
			move.x = -100;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			move.x = 100;
		}
		else
			move.x = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			move.y = -100;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			move.y = 100;
		}
		else
			move.y = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
		{
			p3D.moveScreenDistance(dt.asSeconds() * 10);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		{
			p3D.moveScreenDistance(-dt.asSeconds() * 10);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			p3D.rotateCamera(sf::Vector3f(0, dt.asSeconds() * -0.7, 0));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			p3D.rotateCamera(sf::Vector3f(0, dt.asSeconds() * 0.7, 0));

		}

		std::array<float, 9> rotY{
		cos(angle * dt.asSeconds()),0,sin(angle * dt.asSeconds()),
		0,1,0,
		-sin(angle * dt.asSeconds()),0,cos(angle * dt.asSeconds())
		};

		std::array<float, 9> rotX{
		1,0,0,
		0,cos(angle * dt.asSeconds()),-sin(angle * dt.asSeconds()),
		0,sin(angle * dt.asSeconds()),cos(angle * dt.asSeconds())
		};

		std::array<float, 9> rotXSuperFast{
		1,0,0,
		0,cos(angle * dt.asSeconds() * 10),-sin(angle * dt.asSeconds() * 10),
		0,sin(angle * dt.asSeconds() * 10),cos(angle * dt.asSeconds() * 10)
		};

		std::array<float, 9> rotYSuperFast{
		cos(angle * dt.asSeconds() * 10),0,sin(angle * dt.asSeconds() * 10),
		0,1,0,
		-sin(angle * dt.asSeconds() * 10),0,cos(angle * dt.asSeconds() * 10)
		};

		for (size_t i = 0; i < lights.size(); i++)
		{
			Projection::multiply(lights[i], rotYSuperFast);
			Projection::multiply(lights[i], rotXSuperFast);
		}

		p3D.setWorld(world);
		p3D.moveCamera(sf::Vector3f(move.x * dt.asSeconds(),move.y * dt.asSeconds(),move.z * dt.asSeconds()));
		p3D.refreshCamera();
		lights[1] = p3D.getOrigin();

		p3D.drawSphere(world, lights, rWindow);
		rWindow.display();
		rWindow.clear();
	}












	return 0;
}