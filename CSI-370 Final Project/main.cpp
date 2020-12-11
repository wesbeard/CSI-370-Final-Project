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
int spaceX;
int spaceY;
int minesCount;
string toDisplay;
bool gameOver = false;
const int dimensions = 10;
int board[dimensions][dimensions];

void printBoard() {

	board[0][0] = 10;

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
				toDisplay = to_string(board[i][j]);
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
			int random = rand() % 10;
			if (random == 9) {
				board[i][j] = 9;
			}
			else {
				board[i][j] = 0;
			}
		}
	}

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			if (board[i][j] == 9) {

				// 1 2 3
				// 4   5
				// 6 7 8

				if (i > 0) {
					if(j > 0 && board[i-1][j-1] != 9) board[i - 1][j - 1] += 1;
					if(board[i - 1][j] != 9) board[i - 1][j] += 1;
					if(j < dimensions && board[i - 1][j + 1] != 9) board[i - 1][j + 1] += 1;
				}

				if(j > 0 && board[i][j - 1] != 9) board[i][j - 1] += 1;
				if(j < dimensions && board[i][j + 1] != 9) board[i][j + 1] += 1;

				if (i < dimensions) {
					if(j > 0 && board[i + 1][j - 1] != 9) board[i + 1][j - 1] += 1;
					if(board[i + 1][j] != 9) board[i + 1][j] += 1;
					if(j < dimensions && board[i + 1][j + 1] != 9) board[i + 1][j + 1] += 1;
				}

			}

		}
	}
 }

bool validateInput() {

	cout << "Enter the X coordinate of the desired space: " << endl;
	cin >> spaceX;

	if (spaceX < 0 || spaceX > 9) {
		cout << "Invalid input" << endl << endl;
		return false;
	}

	cout << "Enter the Y coordinate of the desired space: " << endl;
	cin >> spaceY;

	if (spaceY < 0 || spaceY > 9) {
		cout << "Invalid input" << endl << endl;
		return false;
	}

	return true;
}

int main() {

	srand(time(NULL));
	generateBoard();
	printBoard();
	while (!gameOver) {
		if (validateInput()) {
			printBoard();
		}
	}

	return 0;
}
