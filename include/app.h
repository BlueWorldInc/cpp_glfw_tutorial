#include <GLFW/glfw3.h>
#include <math.h>
#include <unistd.h>

const unsigned int steps = 100;
const double stepAngle = 3.1415926f * 2.f / steps;

void drawCircle(double red, double green, double blue);
void drawPlanet(double angle, double distance, double diameter, double red, double green, double blue);