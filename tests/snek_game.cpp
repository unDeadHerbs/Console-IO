#include <algorithm>
#include <unistd.h>

#include "../NCursesConsole.hpp"

#include "snek_game.hpp"

using udh::cio;

Snek::Snek() {
  size = cio.size();
  size.rows -= 2;
  size.cols -= 2;
  body.push_back({size.rows / 2, size.cols / 2}); // Start in the center.

  food = body[0];
  food.col /= 2; // TODO: random

  direction = none;
  alive = true;

  drawWalls();
  updateDisplay();
}

void Snek::drawWalls() {
  cio[0].set(0, '+');
  cio[size.rows + 1].set(0, '+');
  cio[0].set(size.cols + 1, '+');
  cio[size.rows + 1].set(size.cols + 1, '+');
  for (int c = 1; c <= size.cols; c++) {
    cio[0].set(c, '-');
    cio[size.rows + 1].set(c, '-');
  }
  for (int r = 1; r <= size.rows; r++) {
    cio[r].set(0, '|');
    cio[r].set(size.cols + 1, '|');
  }
}

void Snek::updateDisplay() {
  static decltype(Snek::body) old_body;
  for (auto &p : old_body)
    cio[p.row].set(p.col, ' '); // should need to rm 1 or 0
  for (auto &p : body)
    cio[p.row].set(p.col, '#'); // should only need to add one
  if (!alive)
    cio[body[0].row].set(body[0].col, '!');
  cio[food.row].set(food.col, 'a');
  old_body = body;
  cio << std::flush;
  usleep(sleep_time);
}

bool Snek::move(Direction movement_input) {
  // TOOD: return check for body
  if (movement_input != none)
    direction = movement_input;
  switch (direction) {
  case none:
    return true;
  case up:
    body.insert(body.begin(), {body[0].row - 1, body[0].col});
    break;
  case right:
    body.insert(body.begin(), {body[0].row, body[0].col + 1});
    break;
  case down:
    body.insert(body.begin(), {body[0].row + 1, body[0].col});
    break;
  case left:
    body.insert(body.begin(), {body[0].row, body[0].col - 1});
    break;
  }
  if (body[0] == food)
    while (std::find(body.begin(), body.end(), food) !=
           body.end()) { // Generate a new food.
      food.row++;        // TODO: random, this is just lexicographic
      food.col += food.row == size.rows;
      food.row %= size.rows;
      food.col %= size.cols;
    }
  else
    body.pop_back(); // Get longer by one.
  if (body[0].row == 0 || body[0].row == size.rows + 1 || body[0].col == 0 ||
      body[0].col == size.cols + 1)
    alive = false;
  updateDisplay();
  return true;
}
