#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "board.hpp"
#include "board_ui.hpp"
using namespace std;
int main(int argc, char** argv) {
  int cols = 30, lines = 20, bombs = 10;
  srand(time(NULL));

  if (argc == 2) {
    lines = cols = atoi(argv[1]);
    bombs = lines * cols / 10 + 1;
  } else if (argc == 3) {
    lines = atoi(argv[1]);
    cols = atoi(argv[2]);
    bombs = lines * cols / 10 + 1;
  } else if (argc > 3) {
    lines = atoi(argv[1]);
    cols = atoi(argv[2]);
    bombs = atoi(argv[3]);
  }

  // init game logic and UI
  Board board(cols, lines, bombs);
  BoardUI boardUI = BoardUI(cols, lines);
  // init window
  auto ws = boardUI.getWindowSize();
  sf::RenderWindow window(sf::VideoMode(ws.x, ws.y), "Mine game!");

  window.setFramerateLimit(120);
  while (window.isOpen()) {
    auto cb = board.charBoard();
    auto tb = boardUI.setTextBoard(cb);

    auto m = sf::Mouse::getPosition(window);
    sf::Vector2i mCell = boardUI.getMouseCell(m);

    if (board.onBound(mCell.x, mCell.y)) {
      boardUI.updateColor(cb, mCell);
    }

    window.clear();
    window.draw(boardUI.getTiles(), 4 * lines * cols, sf::Quads);
    for (int i = 0; i < lines * cols; ++i) {
      window.draw(tb[i]);
    }
    window.display();

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          board.discoverCell(mCell);
        } else if (event.mouseButton.button == sf::Mouse::Right) {
          board.flagToggle(mCell);
        }
      }
    }
  }

  return 0;
}