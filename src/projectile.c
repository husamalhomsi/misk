// projectile.c - analyze dragless projectile motion on Earth

#include <math.h>
#include <stdio.h>

// Standard acceleration due to gravity
#define GN 9.80665

int main(void) {
  double angle, speed, yspeed;

  fputs("Launch Angle    (deg) = ", stdout);
  scanf("%lg", &angle);
  fputs("Launch Speed    (m/s) = ", stdout);
  scanf("%lg", &speed);

  // Convert the angle from degrees to radians
  angle *= M_PI / 180;
  yspeed = speed * sin(angle);

  printf("Flight Peak     (m)   = %g\n", yspeed * yspeed / (2 * GN));
  printf("Flight Range    (m)   = %g\n", speed * speed * sin(2 * angle) / GN);
  printf("Flight Duration (s)   = %g\n", 2 * yspeed / GN);
}
