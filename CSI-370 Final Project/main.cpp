/*
	Wes Beard - wesley.beard@mymail.champlain.edu
	Michael Leonard - michael.leonard@mymail.champlain.edu
	CSI-370
	12/14/2020
	Final Project
*/

/*
========== Template while loop ==========

eax = 0;
while(eax < dimensions) {
	eax++;
}

__asm {
	mov eax, 0	// used for example
	while_loop :
		cmp eax, dimensions
		jge while_done
		inc eax	// your code goes here
		jmp while_loop
	while_done :
};
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
int selectedX;
int selectedY;
int minesCount;
int random;
int currentRandom;
int selectedIndex;
string toDisplay;
char quitVariable;
bool gameOver = false;
bool quit = false;
const int dimensions = 10;
int dimensionsMinusOne = dimensions - 1;
int board[dimensions][dimensions];

int bombChance = 10;		// the higher the number, the less bombs there will be

// string variables used in assembly to print
char invalidFormat[] = "%s\n";
char invalidMessage[] = "Invalid input";

char endOfGameMessage[] = "\nYou lose\nEnter [n] for a new game or any other key to quit\n";

// helper variable for doing math while trying to access array positions in nested for loops
int loopMath = 0;
int loopMath2 = 0;

extern "C" void printBoard() {

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

extern "C" void newRandom() {

	currentRandom = rand() % bombChance;

	__asm {
		// return currentRandom
		mov eax, currentRandom
	};
}

extern "C" void generateBoard() {
	
	//for (i = 0; i < dimensions; i++) {
	//	for (j = 0; j < dimensions; j++) {
	//		// set bombs on the board (anything >9 is hidden, so the bombs are hidden by default)
	//		random = rand() % 5;
	//							
	//		if (random == 0) {
	//			board[i][j] = 19;
	//		}
	//		else {
	//			board[i][j] = 10;
	//		}
	//	}
	//}


	__asm {
		// set eax to 0
		xor eax, eax

		mov ebx, 0
		outer :
			mov ecx, 0
			inner :

				// save ecx before calling rand()
				// rand() overwrites ecx for some reason
				mov loopMath, ecx

				xor eax, eax
				// get new random number
				call newRandom
				// put return value of newRandom into currentRandom
				mov currentRandom, eax
				mov ecx, loopMath
				
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

	/*
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
	*/
		
	__asm {

		mov ebx, 0
		number_placement_outer :
			mov ecx, 0
				number_placement_inner :

			// get the board[i][j] vvvvvvvv
				xor eax, eax	// clear eax and loopMath
				mov loopMath, 0

				mov eax, ecx 
				imul eax, 4		// eax = ecx * 4

				mov loopMath, offset board
				add eax, loopMath	// eax = (address of board) + (ecx * 4)

				mov edx, dimensions
				imul edx, ebx	// edx = dimensions * outerloop counter

				imul edx, 4

				add eax, edx	// eax = &board[i][j]	
				mov edx, [eax]	// edx = board[i][j]
					

			// inner nested loop code vvvvv
				// if (x > 10)
				cmp edx, 19
				je is_bomb_success
				// else goes here
				// ...
				jmp is_bomb_done

				// if goes here
				// ...
				is_bomb_success:

					// if the current cell is a bomb, code for adding all the numbers will go here vvvvvvvvvvvvvvvvvvvvv

					// order to place numbers:
					// 1 2 3
					// 4   5
					// 6 7 8
				
					
					// add to the top left square if possible
					// if (j > 0 && board[i - 1][j - 1] != 19) board[i - 1][j - 1] += 1;
					mov loopMath, 0	// just incase if-statement quits first, make sure it doesn't add to eax
					cmp ecx, 0
					jle top_left_done	// if (j <= 0), quit
					cmp ebx, 0
					jle top_left_done   // if (i <= 0), quit
					// eax - dimensions * 4 - (4)
					mov loopMath2, eax	// put old eax into loopMath2
					mov eax, dimensions
					imul eax, 4
					add eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					sub eax, loopMath	// eax = (dimensions * 4 - 1) AKA (loopMath)
					// eax should now equal &board[i - 1][j - 1]
					mov edx, [eax]	// edx = board[i - 1][j - 1]
					cmp edx, 19
					je top_left_done	// if (board[i - 1][j - 1] == 19), quit

					// both are true, so do...
					inc [eax]			// increment that cell by 1
						
					top_left_done:
					add eax, loopMath	// bring eax back to it's orignal position


					// top middle
					// if (board[i - 1][j] != 19) board[i - 1][j] += 1;
					mov loopMath, 0
					cmp ebx, 0
					jle top_middle_done
					mov loopMath2, eax
					mov eax, dimensions
					imul eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					sub eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je top_middle_done

					inc [eax]			// increment that cell by 1

					top_middle_done:
					add eax, loopMath	// bring eax back to it's orignal position



					// top right
					// if (j < dimensions && board[i - 1][j + 1] != 19) board[i - 1][j + 1] += 1;
					mov loopMath, 0
					cmp ebx, 0
					jle top_right_done   // if (i <= 0), quit
					cmp ecx, dimensionsMinusOne
					jge top_right_done
					mov loopMath2, eax
					mov eax, dimensions
					imul eax, 4
					sub eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					sub eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je top_right_done

					inc [eax]			// increment that cell by 1
						
					top_right_done:
					add eax, loopMath	// bring eax back to it's orignal position
					
					

					
					// middle left
					// if (j > 0 && board[i][j - 1] != 19) board[i][j - 1] += 1;
					mov loopMath, 0
					cmp ecx, 0
					jle middle_left_done
					mov loopMath2, eax
					mov eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					sub eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je middle_left_done

					inc [eax]			// increment that cell by 1

					middle_left_done:
					add eax, loopMath	// bring eax back to it's orignal position
					
						
					// middle right
					// if (j < dimensions && board[i][j + 1] != 19) board[i][j + 1] += 1;
					mov loopMath, 0
					cmp ecx, dimensionsMinusOne
					jge middle_right_done
					mov loopMath2, eax
					mov eax, -4
					mov loopMath, eax
					mov eax, loopMath2
					sub eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je middle_right_done

					inc[eax]			// increment that cell by 1

					middle_right_done:
					add eax, loopMath	// bring eax back to it's orignal position

					


					// bottom left
					// if (j > 0 && board[i + 1][j - 1] != 19) board[i + 1][j - 1] += 1;
					mov loopMath, 0
					cmp ebx, dimensionsMinusOne
					jge bottom_left_done
					cmp ecx, 0
					jle bottom_left_done
					mov loopMath2, eax
					mov eax, dimensions
					imul eax, 4
					sub eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					add eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je bottom_left_done

					inc [eax]			// increment that cell by 1

					bottom_left_done:
					sub eax, loopMath	// bring eax back to it's orignal position
					
					
					// bottom middle
					// if (board[i + 1][j] != 19) board[i + 1][j] += 1;
					mov loopMath, 0
					cmp ebx, dimensionsMinusOne
					jge bottom_middle_done
					mov loopMath2, eax
					mov eax, dimensions
					imul eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					add eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je bottom_middle_done

					inc [eax]			// increment that cell by 1

					bottom_middle_done:
					sub eax, loopMath	// bring eax back to it's orignal position
					

					// bottom right
					// if (j < dimensions && board[i + 1][j + 1] != 19) board[i + 1][j + 1] += 1;
					mov loopMath, 0
					cmp ebx, dimensionsMinusOne
					jge bottom_right_done
					cmp ecx, dimensionsMinusOne
					jge bottom_right_done
					mov loopMath2, eax
					mov eax, dimensions
					imul eax, 4
					add eax, 4
					mov loopMath, eax
					mov eax, loopMath2
					add eax, loopMath
					mov edx, [eax]
					cmp edx, 19
					je bottom_right_done

					inc[eax]			// increment that cell by 1

					bottom_right_done:
					sub eax, loopMath	// bring eax back to it's orignal position



				// end of if-statement
				is_bomb_done:

			// inner loop increase
			inc ecx
			cmp ecx, dimensions
			jne number_placement_inner
		// outer loop increase
		inc ebx
		cmp ebx, dimensions
		jne number_placement_outer
	};
		
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
				// if (board[i][j] > 10)	// this will show all numbers and X's but not 0's
				cmp edx, 10
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


extern "C" int validateInput(int typeCode) {
	// type is either X or Y, used for display purposes
	char type;
	if (typeCode == 0) {
		type = 'X';
	}
	else {
		type = 'Y';
	}

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

	while(!quit) {

	__asm {

		/* CODE BEING REPLACED:
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

			unhideBoard();
			printBoard();
			cout << endl << "You lose..." << endl;
			cout << "Enter [n] for a new game or any other key to quit" << endl;
		}
		*/

		//generate and print the board on new game
		call generateBoard
		call printBoard

		game_over_loop :
			// if gameOver is true then end loop
			cmp gameOver, 1
			je game_over_done

				// validate input for the x coordinate	
				mov eax, -1
				x_loop:
					cmp eax, -1
					jne x_done

					// parameter 0 = x
					mov eax, 0
					push eax
					call validateInput
					pop ebx

					jmp x_loop
				x_done:
				mov selectedX, eax

				// validate input for the y coordinate	
				mov eax, -1
				y_loop :
					cmp eax, -1
					jne y_done

					// parameter 1 = y
					mov eax, 1
					push eax
					call validateInput
					pop ebx

					jmp y_loop
				y_done :
				mov selectedY, eax

				// get 2d array
				mov eax, offset board

				mov ecx, selectedX
				imul ecx, 4
				mov selectedX, ecx

				mov ecx, selectedY
				imul ecx, 4
				mov selectedY, ecx

				mov ecx, selectedY
				imul ecx, dimensions

				add	ecx, selectedX
				add eax, ecx
				mov ebx, [eax]

				cmp ebx, 9
				jg check_true
					jmp check_done
						
					check_true :
						// subtract 10 from selectedIndex if greater than 9
						sub [eax], 10
						sub ebx, 10
						jmp check_done
				// end of if-statement
				check_done :

				call printBoard

				// if selected space is a bomb then end the game
				mov eax, selectedIndex
				cmp ebx, 9
				je loss_true
					jmp loss_done

					loss_true:
						// if game is over then set gameOver to true
						mov gameOver, 1
						jmp loss_done
				loss_done:
					
			jmp game_over_loop
		game_over_done :

		call unhideBoard
		call printBoard

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
		done :

	};
	}

	return 0;
}
