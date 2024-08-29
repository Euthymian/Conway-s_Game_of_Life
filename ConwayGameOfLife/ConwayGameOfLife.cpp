#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameConfig.h"
using namespace std;

sf::Color COLOR_BACKGROUND = sf::Color(10, 10, 10, 255);
sf::Color COLOR_GRID = sf::Color(50, 50, 50, 255);
sf::Color COLOR_DIE_NEXT = sf::Color(170, 170, 170, 255);
sf::Color COLOR_ALIVE_NEXT = sf::Color(255, 255, 255, 255);

sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int rows = SCREEN_HEIGHT / CELL_SIZE;
int cols = SCREEN_WIDTH / CELL_SIZE;

int** getLife() {
	int** cells = new int* [rows];
	for (int i = 0; i < rows; i++) {
		cells[i] = new int[cols];
		for (int j = 0; j < cols; j++) {
			cells[i][j] = 0;
		}
	}
	return cells;
}

void deleteLife(int** life) {
	for (int i = 0; i < rows; i++) {
		delete[] life[i];
	}
	delete[] life;
}

int sumOfAlive(int** life, int row, int col) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		int newRow = row + dy[i], newCol = col + dx[i];
		if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
			if (life[newRow][newCol] == 1) {
				sum++;
			}
		}
	}
	return sum;
}

int** updateLife(sf::RenderWindow* window, int** life, bool running) {
	int** updatedLife = getLife();

	window->clear(COLOR_GRID);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int sum = sumOfAlive(life, i, j);
			if (life[i][j] == 1) {
				cell.setFillColor(COLOR_ALIVE_NEXT);
				if (!running) updatedLife[i][j] = life[i][j];
			}
			else
				cell.setFillColor(COLOR_BACKGROUND);

			if (running) {
				if (life[i][j] == 1) {
					if (sum < 2 || sum > 3) {
						cell.setFillColor(COLOR_DIE_NEXT);
					}
					else {
						updatedLife[i][j] = 1;
						cell.setFillColor(COLOR_ALIVE_NEXT);
					}
				}
				else {
					if (sum == 3) {
						updatedLife[i][j] = 1;
						cell.setFillColor(COLOR_ALIVE_NEXT);
					}
				}
			}

			cell.setPosition(j * CELL_SIZE + 1, i * CELL_SIZE + 1);
			window->draw(cell);
		}
	}

	return updatedLife;
}

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH + 1, SCREEN_HEIGHT + 1), GAME_TITLE);
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);

	int** life = getLife();
	bool running = false;
	bool pressedMouseLeft = false;

	life = updateLife(window, life, running);
	window->display();

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				running = false;
				deleteLife(life);
				window->close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.scancode == sf::Keyboard::Scan::Space) {
					running = !running;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					pressedMouseLeft = true;
					sf::Vector2i pos = sf::Vector2i(sf::Mouse::getPosition(*window).x / CELL_SIZE, sf::Mouse::getPosition(*window).y / CELL_SIZE);
					life[pos.y][pos.x] = 1;
					life = updateLife(window, life, running);
					window->display();
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					pressedMouseLeft = false;
				}
			}
			if (event.type == sf::Event::MouseMoved && pressedMouseLeft) {
				sf::Vector2i pos = sf::Vector2i(sf::Mouse::getPosition(*window).x / CELL_SIZE, sf::Mouse::getPosition(*window).y / CELL_SIZE);
				life[pos.y][pos.x] = 1;
				life = updateLife(window, life, running);
				window->display();
			}
		}

		if (running) {
			life = updateLife(window, life, running);
			window->display();
		}
	}

	return 0;
}