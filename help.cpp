#include <iostream>
#include <math.h>

int main () {
  double x = 0.1f;
  double y = 0.0f;

  double theta = 0;
  double angularIncrement = M_PI/3;

  while (fabs(x) < 1.5 && fabs(y) < 1.0) {

    double MA = theta + M_PI_2;
    double NLA = theta + angularIncrement;

    double x0 = x;
    double y0 = y;

    x = (y0 - tan(MA)*x0)/(tan(NLA) - tan(MA));

    y = tan(NLA)*x;

    std::cout << '(' << x << ", " << y << ")\n";
    theta += angularIncrement;

  }

  std::cout.flush();

  return 0;
}