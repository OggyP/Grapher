class parabola
{
public:
	float currentPoints[3][2];
	int colour[3] = { rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55 };
	int colourPoint[3] = { rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55 };
	float getYval(float x)
	{
		float result = currentPoints[0][1] * ((x - currentPoints[1][0]) / (currentPoints[0][0] - currentPoints[1][0]) * (x - currentPoints[2][0]) / (currentPoints[0][0] - currentPoints[2][0]));
		result += currentPoints[1][1] * ((x - currentPoints[0][0]) / (currentPoints[1][0] - currentPoints[0][0]) * (x - currentPoints[2][0]) / (currentPoints[1][0] - currentPoints[2][0]));
		result += currentPoints[2][1] * ((x - currentPoints[0][0]) / (currentPoints[2][0] - currentPoints[0][0]) * (x - currentPoints[1][0]) / (currentPoints[2][0] - currentPoints[1][0]));
		return result;
	}
};