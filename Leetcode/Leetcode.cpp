#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Vec2.h"

int main(int argc, char* argv[]) {
  Game g("config.txt");
  g.run();

  return 0;
}
