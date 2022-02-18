#include "Matrix.h"

std::array<float, 9> Matrix::multiply(std::array<float, 9> const& a, std::array<float, 9> const& b)
{
	std::array<float, 9> newMatrix;
	newMatrix[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];	//check
	newMatrix[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];	//check
	newMatrix[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];

	newMatrix[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
	newMatrix[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
	newMatrix[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];

	newMatrix[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
	newMatrix[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
	newMatrix[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];
	return newMatrix;
}

sf::Vector3f Matrix::multiply(std::array<float, 9> const& a, sf::Vector3f const& b)
{
	sf::Vector3f newP;

	newP.x = b.x * a[0] + b.y * a[1] + b.z * a[2];
	newP.y = b.x * a[3] + b.y * a[4] + b.z * a[5];
	newP.z = b.x * a[6] + b.y * a[7] + b.z * a[8];

	return newP;
}
