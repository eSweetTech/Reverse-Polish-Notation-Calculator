/*
	This project was designed and written by Eric Sweet for CS3100.
	The program is designed to be a RPN calculator that allows addition,
	multiplication and exponentiation. If subtraction, letters or division are inputted,
	it continues to run, but displays input errors and expression errors
*/

// Project1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BignumStack.h"
#include "Bignum.h"
#include "Split.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/* Project Tips:

*/
int adderFunction(BignumStack &numberStack);
int exponentiationFunction(BignumStack &numberStack);
int multiplicationFunction(BignumStack &numberStack);
bool isNumeric(const string& input);
void factorialFunction(BignumStack& givenStack);

void leakTest() {
	Bignum test("0123456789");
	BignumStack numberStack;
	for (int i = 0; i < 10; i++) {
		numberStack.push(test);
	}
}

int main()
{
	bool done = 0;
	string input;
	int errorCount = 0;
	string add = "+";
	string subtract = "-";
	string divide = "/";
	string multiply = "*";
	string factorial = "!";
	string exponent = "^";
	vector<string> tokens;
	BignumStack numberStack;
	while (!done) {
		tokens.clear();
		while (numberStack.stackSize() > 0){
			Bignum nullBignum = numberStack.pop();
		}


		cout << "Enter input: ";
		cout.flush();
		//Get user input until a blank line is received
		while (getline(cin, input)) {
			if (input.empty()) {
				break;
			}
			split(input, tokens);
		}

		int tokenSize = tokens.size();

		/*
		If the user does not input any operators or operands, end the program
		If the user inputs operators, loop through them and do the proper if-statement
		*/
		if (tokenSize == 0) {
			done = 1;
		}
		else {
			cout << "User Input: ";
			/*
			Loops through user input and either breaks into a
			operator function or creates a newBignum and adds it
			to the stack.
			If the user uses an invalid operator or inserts characters,
			it reports invalid input and ignores it
			*/
			for (int iterator = 0; iterator < tokenSize; iterator++) {
				if (add.compare(tokens[iterator]) == 0) {
					cout << "+ ";
					errorCount += adderFunction(numberStack);
				}
				else if (multiply.compare(tokens[iterator]) == 0) {
					cout << "* ";
					errorCount += multiplicationFunction(numberStack);
				}
				else if (exponent.compare(tokens[iterator]) == 0) {
					cout << "^ ";
					errorCount += exponentiationFunction(numberStack);
				}
				else if (divide.compare(tokens[iterator]) == 0) {
					cerr << "Invalid Input ";
					errorCount = 1;
				}
				else if (subtract.compare(tokens[iterator]) == 0) {
					cerr << "Invalid Input ";
					errorCount = 1;
				}
				else if (factorial.compare(tokens[iterator]) == 0) {
					cout << "! ";
					factorialFunction(numberStack);
				}
				else {
					if (isNumeric(tokens[iterator]) == 0) {
						cerr << "Invalid Input ";
					}
					else {
						Bignum newBignum(tokens[iterator]);
						numberStack.push(newBignum);
						newBignum.print();
					}
				}
			}
			cout << endl;
			/*
			If an error occured at any math functions,
			or there were not enough operands, output an error message.
			If everything worked properly, output the calculated value.
			*/
			if (errorCount > 0 || numberStack.stackSize() > 1 || numberStack.stackSize() == 0) {
				cout << endl;
				cout << "Answer: ";
				cerr << "Expression Error" << endl;
				errorCount = 0;
			}
			else {
				cout << endl;
				cout << "Answer: ";
				Bignum finalValue = numberStack.pop();
				finalValue.print();
				cout << endl;
			}
			cout << endl;
		}

	}
	/*
	Verifies that there is no memory leak
	*/
	cout << "Before test" << endl;
	system("pause");
	for (int i = 0; i < 100000; i++) {
		leakTest();
	}
	cout << "After test" << endl;
	system("pause");

	system("pause");

	return 0;
}
/*
Verifies the user input is only digits using the isDigit algorithm,
if not, it returns a 1 and the main method displays an input error.
*/
bool isNumeric(const string& userInput) {
	return all_of(userInput.begin(), userInput.end(), ::isdigit);
}
/*
This function takes in a reference to a number stack, and
adds two bignums together. If an error occurs due to the
stack being to small, returns 1, else it returns 0
*/
int adderFunction(BignumStack& numberStack) {
	int stackSize = numberStack.stackSize();
	//verifies that the stack has atleast 2 operands
	if (stackSize < 2) {
		return 1;
	}
	else {
		//get the values from the stack
		Bignum first = numberStack.pop();
		Bignum second = numberStack.pop();
		//new Bignum to hold the added together bignum
		int carry = 0;
		//list to put into the bignum
		string list;
		//gets head of the bignums
		DigitNode *previousFirst = &first.getHead();
		DigitNode *previousSecond = &second.getHead();
		//get to the end of each list
		while (previousFirst->next) {
			previousFirst = previousFirst->next;
		}
		while (previousSecond->next) {
			previousSecond = previousSecond->next;
		}
		//while both list still have previous values
		do {
			/* new value is equal to any carry + firstValue + secondValue
			only uses previousFirst or previousSecond if not null
			*/
			int value = carry + (previousFirst != NULL ? previousFirst->number : 0)
				+ (previousSecond != NULL ? previousSecond->number : 0);
			//if value > 10 we need a carry and to subtract 10 from value
			if (value >= 10) {
				value -= 10;
				carry = 1;
			}
			else {
				//if value is not greater than 10, we dont need a carry
				carry = 0;
			}
			//add value to the string list
			list = to_string(value) + list;
			//iterate to next value of each list if the next is not null
			if (previousFirst != NULL) {
				previousFirst = previousFirst->previous;
			}
			if (previousSecond != NULL) {
				previousSecond = previousSecond->previous;
			}
			//loop if atleast one is not null
		} while (previousFirst != NULL || previousSecond != NULL);
		//if there is still a carry, add it to beginning of list
		if (carry > 0) {
			list = to_string(carry) + list;
		}
		//create new bignum using the list of values
		Bignum newBignum(list);
		//push it to top of stack
		numberStack.push(newBignum);
	}
	// return 0 to show that no error occured
	return 0;
}
/*
This function takes in a reference to a number stack, and
does exponentiates two bignums together. If an error occurs due to the
stack being to small, returns 1, else it returns 0
*/
int exponentiationFunction(BignumStack& numberStack) {
	int stackSize = numberStack.stackSize();
	int errorCount = 0;
	if (stackSize < 2) {
		return 1;
	}
	else {
		/*
		Due to how a RPN calculator works, we know the first value
		off of the stack will always be the exponent, so we pop it off
		and verify that it is not 1 or 0, if it is we return either the base
		or 1 respectfully. If it is not, we loop through and multiply it until the
		value of the exponent is reached
		*/
		Bignum exponent = numberStack.pop();
		Bignum base = numberStack.pop();
		string value = exponent.getValue();
		string one = "1";
		string zero = "0";
		//creates a tempStack to be used in the for loop
		BignumStack tempStack;
		//anything raised to the 1 is itself
		if (one.compare(value) == 0) {
			tempStack.push(base);
		}
		else if (zero.compare(value) == 0) {
			// Anything raised to the 0 is automatically 1
			Bignum newValue("1");
			tempStack.push(newValue);
		}
		else {
			int count = 0;
			string exponentValue;
			DigitNode *exponentList = &exponent.getHead();
			/*
			We were told in the directions for this lab that the exponent
			will always be smalle enough to fit into an integer. This while loop
			and stringstream gets the integer value of the exponent as a string and converts
			it to an integer to be incremented with
			*/
			while (exponentList != NULL) {
				exponentValue = exponentValue + to_string(exponentList->number);
				exponentList = exponentList->next;
			}
			long int compare = 0;
			stringstream check(exponentValue);
			check >> compare;
			/*
			This for loop iterates until the exponent is completed.
			It starts at one because we already push one base to the stack
			It uses the multiplicationFunction because it is multiplied against itself
			*/
			for (int iterator = 0; iterator < compare; iterator++) {
				tempStack.push(base);
			}

		}
		//verifies tempStack only has 1 value
		while (tempStack.stackSize() > 1) {
			errorCount = multiplicationFunction(tempStack);
		}
		/*
		Gets the bignum from the tempStack and puts it back
		on the official stack and then returns 0 since no error occured
		*/
		numberStack.push(tempStack.pop());
	}
	return 0;
}
/*
This function takes in a reference to a number stack, and
multiplies two bignums together. If an error occurs due to the
stack being to small, returns 1, else it returns 0
*/

int multiplicationFunction(BignumStack& numberStack) {
	int stackSize = numberStack.stackSize();
	/*
	If the stackSize is not atleast 2, it returns a 1 causing an Expression Error
	if the stacksize is greater than or equal to 2, it goes through the function by
	popping the values from the stack
	*/
	if (stackSize < 2) {
		return 1;
	}
	else {
		Bignum topValue = numberStack.pop();
		Bignum bottomValue = numberStack.pop();
		string secondValue = bottomValue.getValue();
		string firstValue = topValue.getValue();
		int zeroInt = 0;
		string one = "1";
		string zero = "0";

		//if either bignum is 0 return a bignum of 0
		if (zero.compare(secondValue) == 0 || zero.compare(firstValue) == 0) {
			Bignum newValue("0");
			numberStack.push(newValue);
		}
		//if the first bignum is 1, return the other bignum 
		else if (one.compare(firstValue) == 0) {
			numberStack.push(bottomValue);
		}
		//if the second bignum is 1, return the first bignum
		else if (one.compare(secondValue) == 0) {
			numberStack.push(topValue);
		}
		//if neither bignum is one or zero, go to this statement
		else {
			BignumStack tempStack;
			int errorCount = 0;
			int numberOfZero = 0;
			int carry = 0;
			int iterator = 0;
			string list;
			//node of top and bottom
			DigitNode *top = &topValue.getHead();
			DigitNode *bottom = &bottomValue.getHead();
			//get to the end of each list
			while (top->next) {
				top = top->next;
			}
			while (bottom->next) {
				bottom = bottom->next;
			}
			do {
				//new value is equal to any carry + firstValue * secondValue
				//only uses previousFirst or previousSecond if not null
				int value = carry + ((top != NULL ? top->number : 0)
					* (bottom != NULL ? bottom->number : 0));
				//if value > 10 we need a carry and to get the second digit of value
				if (value >= 10) {
					carry = value / 10;
					value = value % 10;
				}
				else {
					//if value is not greater than 10, we dont need a carry
					carry = 0;
				}
				//add value to the string list
				list = to_string(value) + list;
				//iterate to next value of the top 
				if (top->previous != NULL) {
					top = top->previous;
				}
				//if the next value is null, reset the top to the end, create a bignum out of the list, 
				//iterate bottom to next value, push bignum to temp stack and make list have the amount of zeros the stack has size
				else {
					//add carry to list
					list = to_string(carry) + list;
					//put carry back to zero
					carry = 0;

					while (top->next) {
						top = top->next;
					}
					if (bottom->previous != NULL) {
						bottom = bottom->previous;
					}
					else {
						bottom = NULL;
					}
					Bignum newBignum(list);
					tempStack.push(newBignum);
					numberOfZero++;
					list = "";
					/*
					Add the number of zeroes to the end of the list, that the stack has size
					*/
					while (iterator < numberOfZero) {
						list = to_string(0) + list;
						iterator++;
					}

					iterator = 0;
				}
			} while (bottom != NULL);
			/*
			Multiplication is the addition of multiple lines that have been
			multiplied. this while loop adds them while the tempStack has atleast
			2 operands
			*/

			while (tempStack.stackSize() > 1) {
				errorCount += adderFunction(tempStack);
			}
			numberStack.push(tempStack.pop());
			//BREAKS HERE
			//goes to stack destructor
		}
	}
	return 0;
}
void factorialFunction(BignumStack& givenStack){
	Bignum givenBignum = givenStack.pop();
	string value = givenBignum.getValue();
	BignumStack tempStack;
	long int compare = 0;
	stringstream check(value);
	check >> compare;
	int errorCheck = 0;
	for (int iterator = 0; iterator < compare; iterator++) {
		int value1 = compare - iterator;
		stringstream ss;
		ss << value1;
		string newValue = ss.str();
		Bignum pushedBignum(newValue);
		tempStack.push(pushedBignum);
		if (tempStack.stackSize() > 1) {
			multiplicationFunction(tempStack);
		}
	}
	while (tempStack.stackSize() > 1) {
		multiplicationFunction(tempStack);
	}
	givenStack.push(tempStack.pop());

}