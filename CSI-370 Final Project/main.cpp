/*
	Wes Beard - wesley.beard@mymail.champlain.edu
	Michael Leonard - michael.leonard@mymail.champlain.edu
	CSI-370
	12/14/2020
	Final Project
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

int i;
int j;
int k;
int enteredVal;
int selectedX, selectedY;
int minesCount;
int random;
string toDisplay;
string quitVariable;
bool gameOver = false;
bool quit = false;
const int dimensions = 10;
int board[dimensions][dimensions];

void printBoard() {

	toDisplay = "";

	system("cls");
	for (k = 0; k < dimensions; k++) {
		cout << "  " << k << " ";
	}
	cout << endl;
	for (i = 0; i < dimensions; i++) {
		for (k = 0; k < dimensions; k++) {
			cout << "+---";
		}
		cout << "+" << endl;
		for (j = 0; j < dimensions; j++) {
			if (board[i][j] > 9) {
				toDisplay = " ";
			}
			else {
				// display an X for the bombs
				if (board[i][j] == 9) {
					toDisplay = "X";
				}
				else {
					toDisplay = to_string(board[i][j]);
				}
			}
			cout << "| " << toDisplay << " ";
		}
		cout << "| " << i << endl;
	}
	for (k = 0; k < dimensions; k++) {
		cout << "+---";
	}
	cout << "+" << endl << endl;
}

void generateBoard() {

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			random = rand() % 10;
			if (random == 9) {
				board[i][j] = 19;
			}
			else {
				board[i][j] = 10;
			}
		}
	}

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			if (board[i][j] == 19) {

				// 1 2 3
				// 4   5
				// 6 7 8

				if (i > 0) {
					// top left
					if(j > 0 && board[i-1][j-1] != 19) board[i - 1][j - 1] += 1;
					// top middle
					if(board[i - 1][j] != 19) board[i - 1][j] += 1;
					// top right
					if(j < dimensions && board[i - 1][j + 1] != 19) board[i - 1][j + 1] += 1;
				}

				// middle left
				if(j > 0 && board[i][j - 1] != 19) board[i][j - 1] += 1;
				// middle right
				if(j < dimensions && board[i][j + 1] != 19) board[i][j + 1] += 1;

				if (i < dimensions) {
					// bottom left
					if(j > 0 && board[i + 1][j - 1] != 19) board[i + 1][j - 1] += 1;
					// bottom middle
					if(board[i + 1][j] != 19) board[i + 1][j] += 1;
					// bottom right
					if(j < dimensions && board[i + 1][j + 1] != 19) board[i + 1][j + 1] += 1;
				}

			}
		}

	}
 }

void unhideBoard() {
	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			// if board is hidden at this position (> 9)
			if (board[j][i] > 9) {
				// set to unhidden (<= 9)
				board[j][i] -= 10;
			}
		}
	}
}

int validateInput(string type) {
	// type is either X or Y, used for display purposes

	cout << "Enter the " << type << " coordinate of the desired space: " << endl;
	cin >> enteredVal;

	if (enteredVal < 0 || enteredVal > 9) {
		cout << "Invalid input" << endl << endl;
		return -1;
	}
	
	return enteredVal;
}

int main() {

	// seed the random number generator
	srand(time(NULL));

	while (!quit) {

		// initialize board
		generateBoard();
		// print first instance of board
		printBoard();

		while (!gameOver) {
			// start with invalid x, loop until x is valid
			selectedX = -1;
			while (selectedX == -1) {
				selectedX = validateInput("X");
			}
			// start with invalid y, loop until y is valid
			selectedY = -1;
			while (selectedY == -1) {
				selectedY = validateInput("Y");
			}

			if (board[selectedY][selectedX] > 9) {
				// any number > 10 is hidden,
				// make the board[y][x] unhidden if it's not already
				board[selectedY][selectedX] -= 10;
			}

			printBoard();

			if (board[selectedY][selectedX] == 9) {
				gameOver = true;
			}

		}

		unhideBoard();
		printBoard();
		cout << endl << "You lose..." << endl;
		cout << "Enter [n] for a new game or any other key to quit" << endl;
		cin >> quitVariable;

		if (quitVariable == "n") {
			gameOver = false;
		}
		else {
			return 0;
		}
	}
	return 1;

}
