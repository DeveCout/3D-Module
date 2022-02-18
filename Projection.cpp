#include "Projection.h"
#include <limits>
#include <iostream>


Projection::Projection(sf::Vector3f const& origin_, sf::Vector3f const& direction_,float const& screenDistance_):origin(origin_),direction(direction_),screenDistance(screenDistance_)
{
	matrix = { 1,0,0,0,1,0,0,0,1 };

	//Rotation de la camera

	std::array<float, 9> rotX{
		1,0,0,
		0,cos(direction.x),-sin(direction.x),
		0,sin(direction.x),cos(direction.x)
	};

	multiply(matrix, rotX);

	std::array<float, 9> rotY{
	cos(direction.y),0,sin(direction.y),
	0,1,0,
	-sin(direction.y),0,cos(direction.y)
	};

	multiply(matrix, rotY);
}

void Projection::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape r(sf::Vector2f(1,1));
	r.setFillColor(sf::Color::White);
	for (size_t i = 0; i < world.size(); i++)
	{
		
		//Si le point est visible, soit z > 0 (car sinon il est hors champs)
		sf::Vector3f p = getRelativePos(world[i]);
		


		if (p.z > 0)
		{
			r.setPosition(sf::Vector2f(p.x * screenDistance / p.z, p.y * screenDistance / p.z));
			r.setSize(sf::Vector2f(POINT_SIZE * screenDistance / p.z, POINT_SIZE * screenDistance / p.z));
			target.draw(r);
		}
		
	}
}

void Projection::setWorld(std::vector<sf::Vector3f> const& world_)
{
	world = world_;
}

sf::Vector3f Projection::getRelativePos(sf::Vector3f const& p) const
{
	sf::Vector3f relativeP = p - origin;

	sf::Vector3f newP;
	
	newP.x = relativeP.x * matrix[0] + relativeP.y * matrix[1] + relativeP.z * matrix[2];
	newP.y = relativeP.x * matrix[3] + relativeP.y * matrix[4] + relativeP.z * matrix[5];
	newP.z = relativeP.x * matrix[6] + relativeP.y * matrix[7] + relativeP.z * matrix[8];

	return newP;
}

sf::Vector2f Projection::getProjectivePos(sf::Vector3f const& p) const
{
	sf::Vector3f np = getRelativePos(p);
	

	return 	sf::Vector2f(np.x * screenDistance / np.z, np.y * screenDistance / np.z);
}

void Projection::multiply(std::array<float, 9>& obj, std::array<float, 9> const& rotation)
{
	std::array<float, 9> newMatrix;
	newMatrix[0] = obj[0] * rotation[0] + obj[1] * rotation[3] + obj[2] * rotation[6];	//check
	newMatrix[1] = obj[0] * rotation[1] + obj[1] * rotation[4] + obj[2] * rotation[7];	//check
	newMatrix[2] = obj[0] * rotation[2] + obj[1] * rotation[5] + obj[2] * rotation[8];	

	newMatrix[3] = obj[3] * rotation[0] + obj[4] * rotation[3] + obj[5] * rotation[6];	
	newMatrix[4] = obj[3] * rotation[1] + obj[4] * rotation[4] + obj[5] * rotation[7];	
	newMatrix[5] = obj[3] * rotation[2] + obj[4] * rotation[5] + obj[5] * rotation[8];

	newMatrix[6] = obj[6] * rotation[0] + obj[7] * rotation[3] + obj[8] * rotation[6];
	newMatrix[7] = obj[6] * rotation[1] + obj[7] * rotation[4] + obj[8] * rotation[7];
	newMatrix[8] = obj[6] * rotation[2] + obj[7] * rotation[5] + obj[8] * rotation[8];
	obj = newMatrix;
}

void Projection::multiply(sf::Vector3f& obj, std::array<float, 9> const& obj2)
{
	sf::Vector3f newVect;
	newVect.x = obj.x * obj2[0] + obj.y * obj2[1] + obj.z * obj2[2];
	newVect.y = obj.x * obj2[3] + obj.y * obj2[4] + obj.z * obj2[5];
	newVect.z = obj.x * obj2[6] + obj.y * obj2[7] + obj.z * obj2[8];


	obj = newVect;
}

void Projection::multiply(std::array<float, 9> & obj, float const& a)
{
	obj[0] *= a;
	obj[1] *= a;
	obj[2] *= a;
	obj[3] *= a;
	obj[4] *= a;
	obj[5] *= a;
	obj[6] *= a;
	obj[7] *= a;
	obj[8] *= a;
}

std::array<float, 9> Projection::getInvMatrix(std::array<float, 9> const& mat)
{

	float detMatrix = mat[0] * mat[4] * mat[8] +
		mat[3] * mat[7] * mat[2] +
		mat[1] * mat[5] * mat[6] - (
			mat[6] * mat[4] * mat[2] +
			mat[3] * mat[1] * mat[8] +
			mat[0] * mat[7] * mat[5]);

	std::array<float, 9> tM = {
	mat[0],
	mat[3],
	mat[6],
	mat[1],
	mat[4],
	mat[7],
	mat[2],
	mat[5],
	mat[8]
	};
	std::array<float, 9> invMatrix =
	{
		tM[4] * tM[8] - tM[5] * tM[7],
		-(tM[3] * tM[8] - tM[6] * tM[5]),
		tM[3] * tM[7] - tM[6] * tM[4],
		-(tM[1] * tM[8] - tM[2] * tM[7]),
		tM[0] * tM[8] - tM[2] * tM[6],
		-(tM[0] * tM[7] - tM[1] * tM[6]),
		tM[1] * tM[5] - tM[2] * tM[4],
		-(tM[0] * tM[5] - tM[2] * tM[3]),
		tM[0] * tM[4] - tM[1] * tM[3]
	};

	multiply(invMatrix, 1 / detMatrix);
	return invMatrix;
}

void Projection::rotateCamera(sf::Vector3f const& rotation)
{
	direction.x += rotation.x;
	direction.y += rotation.y;
	direction.z += rotation.z;
	
	refreshCamera();
}

void Projection::moveCamera(sf::Vector3f const& movement)
{
	sf::Vector3f nV = movement;
	multiply(nV, invMatrix);
	origin += nV;
}

void Projection::moveScreenDistance(float const& d)
{
	screenDistance += d;
	if (screenDistance < 0) screenDistance = 0;
}

sf::Vector3f Projection::getDirection() const
{
	return direction;
}

std::string Projection::toString() const
{
	std::string str ="----P3D Description--- : \n Position : " + std::to_string(origin.x) +"/"+ std::to_string(origin.y) +"/"+ std::to_string(origin.z)+"\n";
	str += "Screen Distance : " + std::to_string(screenDistance);
	return str;
}

void Projection::refreshCamera()
{

	std::array<float, 9> rotX{
	1,0,0,
	0,cos(direction.x),-sin(direction.x),
	0,sin(direction.x),cos(direction.x)
	};


	std::array<float, 9> rotY{
	cos(direction.y),0,sin(direction.y),
	0,1,0,
	-sin(direction.y),0,cos(direction.y)
	};


	matrix = { 1,0,0,0,1,0,0,0,1 };

	multiply(matrix, rotX);
	multiply(matrix, rotY);

	invMatrix = getInvMatrix(matrix);
}

void Projection::drawSphere(std::vector<sf::Vector3f> const& sphere, sf::Vector3f const& lighting, sf::RenderTarget& target, bool const& canSeeThrough) const
{
	sf::RectangleShape r(sf::Vector2f(1, 1));
	r.setFillColor(sf::Color::White);

	float luminosity = 0;

	for (size_t i = 1; i < sphere.size(); i++)
	{
		sf::Vector3f vectPoint = sphere[i] - sphere[0];
		float normeVectPoint =getNorm(vectPoint);
		vectPoint.x /= normeVectPoint;
		vectPoint.y /= normeVectPoint;
		vectPoint.z /= normeVectPoint;

		sf::Vector3f directionVect(0,0,100);
		multiply(directionVect, invMatrix);

		if (canSeeThrough || scalarProduct(vectPoint, directionVect) < 0)
		{
			sf::Vector3f p = getRelativePos(sphere[i]);

			if (p.z > 0)
			{
				r.setPosition(sf::Vector2f(p.x * screenDistance / p.z, p.y * screenDistance / p.z));
				r.setSize(sf::Vector2f(POINT_SIZE * screenDistance / p.z, POINT_SIZE * screenDistance / p.z));

				sf::Vector3f vectLight = sphere[i] - lighting;
				float normeVectLight = getNorm(vectLight);
				vectLight.x /= normeVectLight;
				vectLight.y /= normeVectLight;
				vectLight.z /= normeVectLight;

				luminosity = -scalarProduct(vectLight, vectPoint);

				if (luminosity > 0)
				{
					r.setFillColor(sf::Color(luminosity * 255, luminosity * 255, luminosity * 255, 255));
					target.draw(r);
				}
			}

		}
	}
	sf::Vector3f p = getRelativePos(lighting);
	if (p.z > 0)
	{
		r.setPosition(sf::Vector2f(p.x * screenDistance / p.z, p.y * screenDistance / p.z));
		r.setSize(sf::Vector2f(POINT_SIZE * screenDistance / p.z, POINT_SIZE * screenDistance / p.z));
		r.setFillColor(sf::Color::Red);
		target.draw(r);
	}
}

void Projection::drawSphere(std::vector<sf::Vector3f> const& sphere, std::vector<sf::Vector3f> const& lightings, sf::RenderTarget& target,bool const& canSeeThrough) const
{
	sf::RectangleShape r(sf::Vector2f(1, 1));
	r.setFillColor(sf::Color::White);

	float luminosity = 0;

	for (size_t i = 1; i < sphere.size(); i++)
	{
		sf::Vector3f vectPoint = sphere[i] - sphere[0];
		float normeVectPoint = getNorm(vectPoint);
		vectPoint.x /= normeVectPoint;
		vectPoint.y /= normeVectPoint;
		vectPoint.z /= normeVectPoint;

		sf::Vector3f directionVect = sphere[i] - origin;

		if (canSeeThrough || scalarProduct(vectPoint, directionVect) < 0)
		{
			sf::Vector3f p = getRelativePos(sphere[i]);

			if (p.z > 0)
			{
				r.setPosition(sf::Vector2f(p.x * screenDistance / p.z, p.y * screenDistance / p.z));
				r.setSize(sf::Vector2f(POINT_SIZE * screenDistance / p.z, POINT_SIZE * screenDistance / p.z));
				luminosity = 0;
				for (size_t j = 0; j < lightings.size(); j++)
				{
					sf::Vector3f vectLight = sphere[i] - lightings[j];
					float normeVectLight = getNorm(vectLight);
					vectLight.x /= normeVectLight;
					vectLight.y /= normeVectLight;
					vectLight.z /= normeVectLight;

					float potentialLuminosity = -scalarProduct(vectLight, vectPoint);

					if(luminosity < potentialLuminosity)

					luminosity = potentialLuminosity;
				}


				if (luminosity > 0)
				{
					r.setFillColor(sf::Color(luminosity * 255, luminosity * 255, luminosity * 255, 255));
					target.draw(r);
				}
			}

		}
	}
	for (size_t i = 0; i < lightings.size(); i++)
	{
		sf::Vector3f p = getRelativePos(lightings[i]);
		if (p.z > 0)
		{
			r.setPosition(sf::Vector2f(p.x * screenDistance / p.z, p.y * screenDistance / p.z));
			r.setSize(sf::Vector2f(POINT_SIZE * screenDistance / p.z, POINT_SIZE * screenDistance / p.z));
			r.setFillColor(sf::Color::Red);
			target.draw(r);
		}
	}
}

sf::Vector3f Projection::getOrigin() const
{
	return origin;
}

float scalarProduct(sf::Vector3f const& a, sf::Vector3f const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float getNorm(sf::Vector3f const& a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
