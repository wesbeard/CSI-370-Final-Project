/*
	Wes Beard - wesley.beard@mymail.champlain.edu
	Michael Leonard - michael.leonard@mymail.champlain.edu
	CSI-370
	12/14/2020
	Final Project
*/

#include <iostream>

using namespace std;

bool gameOver = false;
int  dimensions = 10;
int  spaceX;
int  spaceY;

void printBoard() {
	cout << "Printing board..." << endl;
}

void generateBoard() {
	cout << "Generating board..." << endl;
 }

void validateInput() {
	cout << "Enter the X coordinate of the desired space: " << endl;
	cin >> spaceX;
	cout << "Enter the Y coordinate of the desired space: " << endl;
	cin >> spaceY;
}

int main() {

	generateBoard();
	while (!gameOver) {
		printBoard();
		validateInput();
	}

	return 0;
}
