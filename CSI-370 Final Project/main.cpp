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
char quitVariable;
bool gameOver = false;
bool quit = false;
const int dimensions = 10;
int board[dimensions][dimensions];

// string variables used in assembly to print
char invalidFormat[] = "%s\n";
char invalidMessage[] = "Invalid input";

char questionFormat[] = "Enter the %s coordinate of the desired space: ";

// assembly loop math helper variable
int loopMath = 0;

extern "C" void printBoard() {

	toDisplay = "";

	system("cls");
	for (int k = 0; k < dimensions; k++) {
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

extern "C" void generateBoard() {

	cout << "generate board";

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			random = rand() % 5;				// make rareity variable
			if (random == 0) {
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
					if (j > 0 && board[i - 1][j - 1] != 19) board[i - 1][j - 1] += 1;
					// top middle
					if (board[i - 1][j] != 19) board[i - 1][j] += 1;
					// top right
					if (j < dimensions && board[i - 1][j + 1] != 19) board[i - 1][j + 1] += 1;
				}

				// middle left
				if (j > 0 && board[i][j - 1] != 19) board[i][j - 1] += 1;
				// middle right
				if (j < dimensions && board[i][j + 1] != 19) board[i][j + 1] += 1;

				if (i < dimensions) {
					// bottom left
					if (j > 0 && board[i + 1][j - 1] != 19) board[i + 1][j - 1] += 1;
					// bottom middle
					if (board[i + 1][j] != 19) board[i + 1][j] += 1;
					// bottom right
					if (j < dimensions && board[i + 1][j + 1] != 19) board[i + 1][j + 1] += 1;
				}

			}
		}

	}
}

extern "C" void unhideBoard() {

	__asm {

		/*
		for (i = 0; i < dimensions; i++) {
			for (j = 0; j < dimensions; j++) {
				if (board[j][i] > 9) {
					board[j][i] -= 10;
				}
			}
		}
		*/
		
		mov ebx, 0
		outer:
			mov ecx, 0
			inner:
				// clear eax and loopMath (loopMath is a helper variable for doing math
				// with addresses of 2d arrays
				xor eax, eax
				mov loopMath, 0

				mov eax, ecx
				imul eax, 4
				// eax = ecx * 4
				
				mov loopMath, offset board
				add eax, loopMath
				// eax = (address of board) + (ecx * 4)
				
				mov edx, dimensions
				imul edx, ebx
				// edx = dimensions * outerloop counter
				// ex. ebx = 4 makes edx = 4 * (board size of 10) = start at 40
				// THEN add onto 40 by whatever inner loop is at

				imul edx, 4
				add eax, edx
				// eax will now hold the address of each individual cell
				// eax = &board[i][j]

				mov edx, [eax]
				// edx now holds the value of board[i][j]

				// begin if statement ==========
				// if (board[j][i] > 9)
				cmp edx, 9
				// then jump to success
				jg success
				// else
				
				jmp done

				// if
				success :
					// set to unhidden (<= 9)
					// subtract 10 from board[i][j]
					sub edx, 10
					// set board[i][j] = board[i][j]-10
					mov [eax], edx

				// end of if
				done :
				// end if statement ============

				// inner loop increase
				inc ecx
				cmp ecx, dimensions
				jne inner
			// outer loop increase
			inc ebx
			cmp ebx, dimensions
			jne outer
	};
}


extern "C" int validateInput(char *type) {
	// type is either X or Y, used for display purposes
	
	cout << "Enter the " << type << " coordinate of the desired space: " << endl;
	cin >> enteredVal;

	__asm {
		// if (enteredVal < 0 || enteredVal > 9) {
		// 	  cout << "Invalid input" << endl << endl;
		// 	  return -1;
		// } else {
		//    return enteredVal
		// }

		// if (enteredVal < 0)
		cmp enteredVal, 0
		jl success
		// || (enteredVal > 9)
		cmp enteredVal, 9
		jg success
		// else
		// return enteredVal
		mov eax, enteredVal
		jmp done

		// if
		success:
			mov  eax, offset invalidMessage
			push eax
			mov  eax, offset invalidFormat
			push eax
			call printf
			// clean up the stack so that main can exit cleanly
			// use the register ebx for cleanup
			pop ebx
			pop ebx

			// return -1
			mov eax, -1

		// end of if
		done:
		
	};

}



int main() {


	srand(time(NULL));

	while (!quit) {
		generateBoard();
		printBoard();
		while (!gameOver) {
			selectedX = -1;
			while (selectedX == -1) {
				selectedX = validateInput("X");
			}
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

		// TODO : add to assembly
		unhideBoard();
		printBoard();
		cout << endl << "You lose..." << endl;
		cout << "Enter [n] for a new game or any other key to quit" << endl;
		cin >> quitVariable;

		__asm {
			/*
			if (quitVariable == "n") {
				gameOver = false;
			}
			else {
				quit = True
			}
			*/

			// if (quitVariable == "n")
			cmp quitVariable, 110 // 110 is ascii for 'n'
			je success
			// else
			// set quit to True
			mov quit, 1
			jmp done

			// if
			success :
				// set gameOver to False
				mov gameOver, 0

			// end of if
			done:

		};

	}
	return 0;
}
