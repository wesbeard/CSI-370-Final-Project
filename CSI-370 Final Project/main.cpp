/*
	Wes Beard - wesley.beard@mymail.champlain.edu
	Michael Leonard - michael.leonard@mymail.champlain.edu
	CSI-370
	12/14/2020
	Final Project
*/


/*
========== Template print message ==========

Variables declared at top of program:
char invalidFormat[] = "%s\n";	// %s means string
char invalidMessage[] = "Invalid input";

printf(invalidFormat, invalidMessage)

__asm {
	mov  eax, offset invalidMessage
	push eax
	mov  eax, offset invalidFormat
	push eax
	// print invalidMessage
	call printf
	// clean up the stack
	// use unused register ebx for cleanup
	pop ebx
	pop ebx
};
*/


/*

========== Template if-statement ==========
int x = 9
if (x > 10) {
		return 1;
	} else {
		return 0;
	}
}

Note: if the function has NO CODE after the assembly including a return statement,
it will return whatever is in eax

__asm {
	// if (x > 10)
	cmp x, 10
	jg success
	// else goes here
	// ...
	mov eax, 0	// (part of example code)
	jmp done

	// if goes here
	// ...
	jmp done
	success :
		mov eax, 1	// (part of example code)

	// end of if-statement
	done :

};

*/

/*
========== Template nested for loop ==========
for (i = 0; i < dimensions; i++) {
	for (j = 0; j < dimensions; j++) {
		eax++
	}
}

__asm {
	// set eax to 0 (part of example code)
	xor eax, eax

	mov ebx, 0
	outer :
		mov ecx, 0
		inner :
			
			// increment eax (this is where your code goes)
			inc eax

		// inner loop increase
		inc ecx
		cmp ecx, dimensions
		jne inner
	// outer loop increase
	inc ebx
	cmp ebx, dimensions
	jne outer
};

*/

/*
========== Template 2d array access ==========

Note: this is to be used within a nested for loop

At the end of this code, you will have board[i][j]
	The value of board[i][j] is in edx
	The reference to board[i][j] is in eax
		(setting board[i][j] = 9 would look like "mov [eax], 9"

__asm {
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
};
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
int currentRandom;
string toDisplay;
char quitVariable;
bool gameOver = false;
bool quit = false;
const int dimensions = 10;
int board[dimensions][dimensions];

// string variables used in assembly to print
char invalidFormat[] = "%s\n";
char invalidMessage[] = "Invalid input";

char endOfGameMessage[] = "\nYou lose\nEnter [n] for a new game or any other key to quit\n";

// helper variable for doing math while trying to access array positions in nested for loops
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

extern "C" void newRandom() {
	cout << "New random ";
	currentRandom = rand() % 5;
}

extern "C" void generateBoard() {
	
	//for (i = 0; i < dimensions; i++) {
	//	for (j = 0; j < dimensions; j++) {
	//		// set bombs on the board (anything >9 is hidden, so the bombs are hidden by default)
	//		random = rand() % 5;				// maybe make a rareity variable (so we can change the percentage chance easily)
	//											// I TEMPORARILY made the bomb placement a 1/5 chance to make testing easier ***********************************
	//		if (random == 0) {
	//			board[i][j] = 19;
	//		}
	//		else {
	//			board[i][j] = 10;
	//		}
	//	}
	//}

	cout << "foobar1";

	__asm {
		// set eax to 0
		xor eax, eax

		mov ebx, 0
		outer :
		mov ecx, 0
			inner :

				mov eax, ecx
				// get new random number
				call newRandom
				mov ecx, eax

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
				
				// if (x == 0)
				cmp currentRandom, 0
				je success
				// else goes here
				mov [eax], 10
				jmp done
				
				success:
				// if goes here
				mov[eax], 19

				// end of if-statement
				done :

			// inner loop increase
			inc ecx
			cmp ecx, dimensions
			jne inner
			// outer loop increase
			inc ebx
			cmp ebx, dimensions
			jne outer
	};

	cout << "foobar2";

	for (i = 0; i < dimensions; i++) {
		for (j = 0; j < dimensions; j++) {
			// place all numbers on the board
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


extern "C" int validateInput(string type) {
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
			// print invalidMessage
			call printf
			// clean up the stack
			// use register ebx for cleanup
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

			if(board[selectedY][selectedX] == 9) {
				gameOver = true;
			}

		}


		__asm {

			// show entire board and let user restart if they'd like

			/*
			unhideBoard();
			printBoard();
			cout << endl << "You lose..." << endl;
			cout << "Enter [n] for a new game or any other key to quit" << endl;
			*/

			call unhideBoard;
			call printBoard;

			mov  eax, offset endOfGameMessage
			push eax
			// print endOfGameMessage
			call printf
			// clean up the stack
			// use unused register ebx for cleanup
			pop ebx

		};

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
