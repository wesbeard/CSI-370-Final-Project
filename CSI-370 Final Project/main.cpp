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

using namespace std;

int i;
int j;
int spaceX;
int spaceY;
int minesCount;
bool gameOver = false;
const int dimensions = 10;
int board[dimensions][dimensions];

void printBoard() {

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			cout << " - " << board[i][j];
		}
		cout << endl;
	}
}

void generateBoard() {

	cout << "Generating board..." << endl;

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
			minesCount = 0;
			if (board[i][j] == 9) {
				if (board[i - 1][j - 1] != 9) {
					board[i - 1][j - 1]++;
				}
				if (board[i - 1][j + 1] != 9) {
					cout << board[i - 1][j + 1] << endl;
					board[i - 1][j + 1]++;
				}
				if (board[i + 1][j - 1] != 9) {
					board[i + 1][j - 1]++;
				}
				if (board[i + 1][j + 1] != 9) {
					board[i + 1][j + 1]++;
				}
				if (board[i][j + 1] != 9) {
					board[i][j + 1]++;
				}
				if (board[i][j - 1] != 9) {
					board[i][j - 1]++;
				}
				if (board[i - 1][j] != 9) {
					board[i - 1][j]++;
				}
				if (board[i + 1][j] != 9) {
					board[i + 1][j]++;
				}
			}

			/*if (board[i][j] == 0) {
				if (board[i - 1][j - 1] == 9) {
					minesCount++;
				}
				if (board[i - 1][j + 1] == 9) {
					minesCount++;
				}
				if (board[i + 1][j + 1] == 9) {
					minesCount++;
				}
				if (board[i + 1][j - 1] == 9) {
					minesCount++;
				}
				if (board[i][j - 1] == 9) {
					minesCount++;
				}
				if (board[i][j + 1] == 9) {
					minesCount++;
				}
				if (board[i + 1][j] == 9) {
					minesCount++;
				}
				if (board[i - 1][j] == 9) {
					minesCount++;
				}
				board[i][j] = minesCount;
			}*/
		}
	}
 }

bool validateInput() {

	cout << "Enter the X coordinate of the desired space: " << endl;
	cin >> spaceX;
	cout << "Enter the Y coordinate of the desired space: " << endl;
	cin >> spaceY;
	return false;
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
