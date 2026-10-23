/*

CREDITS

* Edited by Zolarics
* All rights of the original code go to the respective owners.
* See the license of the repository for further copyright details.

* ESP32 Kit provided by:
	www.elegoo.com 

* Keypad example code provided by:
	Alexander Brevig
	Email: alexanderbrevig@gmail.com
*/

#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdbool.h>
#include <string.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

#define MAX_EQUATION_LENGTH 50

#define OLED_RESET -1
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int SDA_Pin = 21;
int SCL_Pin = 22;

// Define the two-dimensional array on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 18, 19, 23}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27,26,25,33}; //connect to the column pinouts of the keypad

// Initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

double calculated_number = 0.0;
int equation_errored = 0;

char calculator_string[MAX_EQUATION_LENGTH] = "";

bool command_active = false; // Determines if A B C or D should input operators

int isParenthesis(char letter)
{
	int return_value = 0;
        switch(letter)
        {
                case '(':
                        return_value = 1;
                        break;
                case ')':
                        return_value = 1;
                        break;
        }
        return return_value;

}

int isOperator(char letter)
{
	int return_value = 0;
	switch(letter)
	{
		case '+':
			return_value = 1;
			break;
		case '-':
			return_value = 1;
			break;
		case '*':
			return_value = 1;
			break;
		case '/':
			return_value = 1;
			break;
	}
	return return_value;
}

int getNumberFromString(char letter)
{
	int return_value = 0;
	switch(letter)
	{
		case '0':
			return_value = 0;
			break;
		case '1':
			return_value = 1;
			break;
		case '2':
			return_value = 2;
			break;
		case '3':
			return_value = 3;
			break;
		case '4':
			return_value = 4;
			break;
		case '5':
			return_value = 5;
			break;
		case '6':
			return_value = 6;
			break;
		case '7':
			return_value = 7;
			break;
		case '8':
			return_value = 8;
			break;
		case '9':
			return_value = 9;
			break;
	}
	return return_value;
}

int absVal(int number)
{
	int return_value = number;
	if (number < 0 )
	{
		return_value *= -1;
	}
	return return_value;
}
double raiseToPower(int base, int power)
{	
	double return_value = base;
	if (power == 0)
	{
		return_value = 1;
	}
	else
	{
		for (int i = 1; i < absVal(power); i++)
		{	
			printf("exponentiating.. %lf\n",return_value);
			return_value *= base;
			printf("exponentiated. new number is %lf\n",return_value);

		}
		if (power < 0)
		{
			return_value = 1/return_value;
		}
	}

	return return_value;
}

double calculateString()
{
	int equation_length = strlen(calculator_string);
	int master_block_array[equation_length][4]; // Contains 4 variables: index of start and index of end parentheis, lowest score, boolean of block.
						    
	/* 
	 * Contains the result of a block. 
	 * Accessed via the respecitve block index. See block_count
	*/ 
	double master_solution_array[equation_length][1]; 
	
	/* Used for incrementing the master_block_array and as a limit on for loop responsible for block calculation
	 * Also responsible for determing what index a block is bound to.
	*/
	int block_count = 0;

	int result_calculated = 0;
	double final_calculated_result = 0.0;

	//printf("evaluating string: %s\n", calculator_string);
	//printf("length is %d, highest index is %d\n", equation_length, equation_length -1);
	
	/*
	Since the calulcation relies on everything going correctly,
	the calculation is wrapped in a do while loop so the process can be killed if the equation ever errors via a break statement
   	*/
	do
	{	
		// Construst blocks
		for (int letter_index=0; letter_index <= (equation_length - 1); letter_index++)
		{
			// printf("on letter: %c\n", calculator_string[letter_index]);
			if (calculator_string[letter_index] == '(')
			{
				int parenthesis_index = letter_index;
				int parenthesis_match = 0;
				int score = 0;
				int lowest_score = 0;
				int operator_on_side = 0;

				// Search for matching parenthesis, starting on the next index

				for (int letter_search=letter_index+1; letter_search <= (equation_length -1); letter_search++)
				{
					switch (calculator_string[letter_search])
					{

						case '(':
							score -= 1;
							lowest_score -=1;
							break;
						case ')':
							score += 1;
							break;

					}

					// Match found, dont loop further
					if (score == 1)
					{
						parenthesis_match = letter_search;
						break;
					}
				}

				// printf("Final Score: %d, Lowest Score: %d\n", score, lowest_score);
				// Escape statement if no match was found
				if (score <= 0)
				{
					equation_errored = 1;
					// printf("equation error: %d\n", equation_errored);
					break;
				}
				// Debug else statement
				/*
				else
				{
					printf("match found at %d\n", parenthesis_match);
				}
				*/

				// Block count is initially 0
				master_block_array[block_count][0] = parenthesis_index; // Index of starting parenthesis
				master_block_array[block_count][1] = parenthesis_match; // Index of matching parenthesis
				master_block_array[block_count][2] = lowest_score; // Determines order in which block is evaluted
				master_block_array[block_count][3] = 0; // Boolean for if the block is solved
				master_solution_array[block_count][0] = 0.0;
					
				block_count += 1;
			}
		}
		
		// The full string itself is a block
		// Score is set to -99 to ensure it is the last block calculated
		master_block_array[block_count][0] = 0;
		master_block_array[block_count][1] = equation_length -1;
		master_block_array[block_count][2] = -99;
		master_block_array[block_count][3] = 0;
		master_solution_array[block_count][0] = 0.0;	

		block_count += 1;

		// Debug for loop
		/*		
		for (int i=0; i < block_count; i++)
		{
			printf("start index: %d\n", master_block_array[i][0]); 
			printf("end index: %d\n", master_block_array[i][1]); 
			printf("lowest score: %d\n", master_block_array[i][2]);
			printf("solved?: %d\n", master_block_array[i][3]); 
		}	
		*/

		// Escape statement if there is an error within the blocks
		// printf("equation errored? %d\n", equation_errored);
		if (equation_errored > 0)
		{
			//printf("Breaking...\n");
			break;
		}
	
			
		/* Operator Array and Operation Solution Array are made global. The calculation effectively builds on another calculation one after another.
		 * This ultimately allows operators to be processed based on what block is currently being analyzed, while preventing them from being re-calculated each time.
		 *
		 * Contains 6 variables: the operator, its weight, its index position, if its been solved, at what point it was solved, and what block it is apart of.
		 * For Index 0: 1 is +, 2 is -, 3 is *, 4 is /
		 * This array is limited to the scope of a block. If a block runs from index 8 to 14, any operators outside of it are not taken into account.
		*/

		// Create operatorArray and operatorSolutionArray with nothing but 0's
		// Solution array is the same as master_solution_array but for operators
		int operatorArray[equation_length][6];
		double operatorSolutionArray[equation_length][1];
		for (int i = 0; i < equation_length; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				operatorArray[i][j] = 0;	

			}
			operatorSolutionArray[i][0] = 0.0;
		}

		//Loop responsible for calculating all of the blocks
		for (int total_blocks_calculated = 0; total_blocks_calculated < block_count; total_blocks_calculated++)
		{	
			// Search for the highest scoring block
			int highest_scoring_block_index = -1234; // This index is then used in the rest of the loop to reference what block is being calculated
			int highest_block_score = -1234;
			for (int block_search_index = 0; block_search_index < block_count; block_search_index++)
			{	
				// printf("---- CURRENTLY SEARCHING: %d\n", search_index);
				// printf("searched lowest_block score: %d\n", master_block_array[search_index][2]);
				// printf("current highest score: %d\n", highest_block_score);	
				// If this block's lowest score is higher than the currently known highest score and the block hasn't already been solved, then mark it as the new highest.
				if ((master_block_array[block_search_index][2] > highest_block_score) && (master_block_array[block_search_index][3] != 1))
				{
					// printf("found new highest score in block: %d\n", search_index);
					highest_block_score = master_block_array[block_search_index][2];
					highest_scoring_block_index = block_search_index;
					// printf("new highest score: %d\n", highest_block_score);

				}				
			}

			int total_operators = 0;

			// Loop from the lowest known index to the highest known index in a block for operators
			// An operator exists IF AND ONLY IF it has a type and weight assigned to it.
			for (int letter_index = master_block_array[highest_scoring_block_index][0]; letter_index <= master_block_array[highest_scoring_block_index][1]; letter_index++)
			{
				
				// printf("on index: %d\n", letter_index);
				// printf("letter is: %c\n", calculator_string[letter_index]);
					
				// Dont re-create operators that have already been solved
				if ((operatorArray[letter_index][3] == 1))
				{
					continue;
				}

				char selected_letter = calculator_string[letter_index];
				switch(selected_letter)
				{
					case '+':
						operatorArray[letter_index][0] = 1; // Operator type
						operatorArray[letter_index][1] = 1; // Operator weight
						break;
					case '-':
						/* negative number check
						 * if to the left of the operator, there is not a number, this means that the minus sign
						 * is making a number negative.
						 * ex:
						 * 12-12: 12 minus 12
						 * 12-(12) 12 minus 12
						 * 12+(-12) 12 plus negative 12
						 *
						 */

						// check if in bounds. If not, then the minus is just making a number negative.
						if ((letter_index - 1) >= 0)
						{
							// check if the letter to the left is an operator or an opening parenthesis
							if ( (isOperator(calculator_string[letter_index - 1]) == 0) && (calculator_string[letter_index-1] != '('))
							{	
								operatorArray[letter_index][0] = 2;
								operatorArray[letter_index][1] = 1;
							}
						}
						break;
					case '*':
						operatorArray[letter_index][0] = 3;
						operatorArray[letter_index][1] = 2;
						break;
					case '/':
						operatorArray[letter_index][0] = 4;
						operatorArray[letter_index][1] = 2;
						break;
					default: // leave type and weight as 0
						break;
				}
				operatorArray[letter_index][2] = letter_index; // Index of operator
				operatorArray[letter_index][3] = 0; // Is the operator solved?
				operatorArray[letter_index][4] = 0; // At what point was the operator solved?
				operatorArray[letter_index][5] = highest_scoring_block_index; // What block is the operator apart of?
				
				if (operatorArray[letter_index][0] != 0)
				{
					total_operators += 1;
					
					/*
					for (int i = 0; i < 4; i++)
					{
						printf("index: %d[%d] = %d\n", letter_index, i, operatorArray[letter_index][i]);
					}
					*/
				}				
			}

			double block_total = 0.0; // total result for the block
		
			// Operator calculation
			for (int total_operators_calculated = 0; total_operators_calculated < total_operators; total_operators_calculated++)
			{
				double operator_total = 0.0;
				double numberArray[2];

				int highest_weighted_operator_index = -1234;
				int highest_operator_weight = -1234;
				for (int operator_search_index = 0; operator_search_index < equation_length; operator_search_index++)
				{	
					//printf("---- CURRENTLY SEARCHING: %d\n", operator_search_index);
					if ((operatorArray[operator_search_index][0] != 0) && (operatorArray[operator_search_index][1] > highest_operator_weight) && (operatorArray[operator_search_index][3] != 1))
					{
						highest_operator_weight = operatorArray[operator_search_index][1];
						highest_weighted_operator_index = operator_search_index;
						//printf("new highest weight: %d from index: %d\n", highest_operator_weight, highest_weighted_operator_index);
					}				
				}

				//printf("[?] starting operator: %c at index: %d\n", calculator_string[highest_weighted_operator_index], highest_weighted_operator_index);

				// using the highest scoring operator index, search to the left. Once it hits another operator, check for that operators index in the operatorArray.
				// If it finds that that operator has been solved, then pull that number from the operator's result via operatorSolutionArray[other_operator_index][0].
				// if it hasn't been solved, then simply convert whatever number is to the right into a full number.
				// Repeat the same for the right.
				// If instead of an operator the calculator comes across a prenthesis, then pull from the master_solution_array after searching for the block index from the master_block_array
				// This can be done by taking the parenthesis index, and matching it to a value from some index in the master_block_array.
				// Once the left and right numbers are calculated, calculate the final result, then store it.
				// Move to the next operator.
				
				/*
				 *
				 *
				 *
				 * There is a problem for when jumping around the equation.
				 * 32-2.1*4+(89.1-2.2/(10/2)+(6-4))-83*123
				 * Taking the following equation, and looking at the 2nd block, the two mini blocks inside of it run into a problem.
				 * Both mini blocks are calculated first, then pemdas is enacted. The problem is that the / goes first, then the -, and then the +.
				 * When it comes time to get the correct numbers, + will pull from an outdated number, it doesnt know that - was the last updated number, and it therefore holds
				 * the correct number.
				 *
				 *
				 * Solution:
				 * Keep track of what operators are within what blocks. Track the operators to the direction being indexed that are apart of the block being calculated.
				 * Check all of the operators in the direction to see if they are already solved. If any of them are unsolved, then this problem can't occur.
				 * If there are 2 or more operators in a given direcrion AND all of the operators in that direction are solved, then this problem can occur
				 * 
				 * When the above scenario occurs, loop through all of the operators in the direction within the bounds of the block.
				 * Check to ensure that the operators are apart of the block being calculated.
				 * While looping through the operators, store when they were solved. The higher this number, the later they were solved.
				 * Grab the value from the highest scoring solved operator. This will gaurentee the latest value from an operation.
				 */


				// Numbers
				for (int calculating_number = 0; calculating_number < 2; calculating_number++)
				{	
					//printf("/ operating over number: %d\n", calculating_number);
					double final_number = 0.0;
					int hit_non_number = 0;
					int decimal_index = -999;
					int started_looking_at = highest_weighted_operator_index;
					int looking_at = highest_weighted_operator_index;
					while(hit_non_number == 0)
					{
						// left is 0, right is 1
						if (calculating_number == 0)
						{
							looking_at--;
						}
						else
						{
							looking_at++;
						}

						//printf("looking at index: %d\n", looking_at);
						// If the index falls out of bounds, dont allow it to loop any further.
						if (looking_at <= -1 || looking_at >= equation_length)
						{	
							//printf("fell out of bounds, breaking [!!!!!!]\n");
							hit_non_number = 1;
						}
						else
						{	
							// exclude minus operators that make numbers negative
							if ( (isOperator(calculator_string[looking_at]) == 1 && operatorArray[looking_at][0] != 0) || isParenthesis(calculator_string[looking_at])  == 1)
							{	
								hit_non_number = 1;
							}

							//printf("checking condition on letter: %c\n", calculator_string[looking_at]);	
						}

						if (calculator_string[looking_at] == '.')
						{
							decimal_index = looking_at;
						}
					}
					//printf("started at index: %d\n", started_looking_at);
					//printf("ended at index: %d\n", looking_at);
					// printf("decimal index: %d\n", decimal_index);
					
					char last_looked_at = calculator_string[looking_at];
					int solution_found = 0;
					// Check if there already is a solution, assuming you are in the bounds.
					if (!(looking_at <= -1 || looking_at >= equation_length))
					{	
						// Check to see if the entire equation to the left or right has been solved
						int checking_direction = started_looking_at;
						int operators_in_current_direction = 0;
						int equation_direction_incomplete = 0;
						// Ensure that the check is greater than the left parenthesis, and less than the right parenthesis
						while((checking_direction > master_block_array[highest_scoring_block_index][0]) && (checking_direction < master_block_array[highest_scoring_block_index][1]))  
						{	
							if (calculating_number == 0)
							{
								checking_direction--;
							}
							else
							{
								checking_direction++;
							}
							
							// If the operator exists in the searched direction
							if (operatorArray[checking_direction][0] != 0 && operatorArray[checking_direction][5] == highest_scoring_block_index)
							{
								operators_in_current_direction += 1;
							}

							// If it is encountered that an operator hasn't been solved, then the equation in that direction is still incomplete.
							if (operatorArray[checking_direction][3] == 0 && operatorArray[checking_direction][0] != 0 && operatorArray[checking_direction][5] == highest_scoring_block_index)
							{
								equation_direction_incomplete = 1;
							}
						}
						//printf("operators in current direction: %d\n",operators_in_current_direction);
						//printf("equation direction incomplete: %d\n",equation_direction_incomplete);	
						
						// If all of the operators in its direction has been solved, find the latest solved operator and use its solution.
						if (operators_in_current_direction >= 2 && equation_direction_incomplete == 0 && solution_found == 0)
						{
							checking_direction = started_looking_at;
							// Ensure that the check is greater than the left parenthesis, and less than the right parenthesis
							while((checking_direction > master_block_array[highest_scoring_block_index][0]) && (checking_direction < master_block_array[highest_scoring_block_index][1]))  
							{	
								if (calculating_number == 0)
								{
									checking_direction--;
								}
								else
								{
									checking_direction++;
								}
								
								// Find the operator that was the latest one solved
								if (operatorArray[checking_direction][4] == total_operators_calculated && operatorArray[checking_direction][5] == highest_scoring_block_index)
								{
									final_number = operatorSolutionArray[checking_direction][0];
									//printf("--! solution found from the latest solved operator %c at index %d\n", calculator_string[checking_direction], checking_direction);	
									solution_found = 1;	
								}
							}	
						}	


						// Looking for operator solutions, assuming the operator is apart of the same block
						int isAnOperator = isOperator(last_looked_at);
						//printf("looking at letter: %c\n", last_looked_at);
						if (isAnOperator == 1 && solution_found == 0)
						{
							if (operatorArray[looking_at][3] == 1 && operatorArray[looking_at][5] == highest_scoring_block_index)
							{
								
								final_number = operatorSolutionArray[looking_at][0];
								//printf("--! solution found from an operator\n");
								solution_found = 1;
							}
						}
						// Looking for block solutions
						int isAParenthesis = isParenthesis(last_looked_at);
						if ((isAParenthesis == 1) && (solution_found == 0))
						{
							for (int block_search=0; block_search < block_count; block_search++)
							{	
								//printf("last looked at index is: %d, block search is on block: %d, with a L_P at %d, and a R_P at %d\n", looking_at, block_search, master_block_array[block_search][0], master_block_array[block_search][1]); 
								// If either of the block's parenthesis indexes match index where the number ended, check if its solved. If it is, pull down the solution
								if ( (master_block_array[block_search][0] == looking_at || master_block_array[block_search][1] == looking_at) && (master_block_array[block_search][3] == 1)  )	
								{
									final_number = master_solution_array[block_search][0];
									//printf("--! solution found from a block\n");
									solution_found = 1;
									break;
								}
							}    					
						}
						
					}
					//printf("// on number: %d solution found? %d\n", calculating_number, solution_found);
					if (solution_found == 0)
					{
						//decimal_index
						//started_looking_at
						//looking_at
						// use started looking at, looking at, and the decimal index values to create a number.	
						// once done, assign that final value to final_number
						/*
						 *
						 * use calculating_number to determine if you need to move the counter left or right. 0 is left, 1 is right.
						 * note that looking at and started looking at end on operators. use the indexes between them.
						 * be careful with indexing the calculator string with out of bounds indexes
						 * LOOKING AT CAN WILL FALL OUT OF BOUNDS ON THE EDGES OF THE EQUATION, BE AWARE.
						 * negative numbers can be checked for by looking for a '-' to the left, and indexing for if the operator exists in the operator array.
						 * ^ if it doesn't, then the minus is meant to negate the number; negation operators arent registered as operators
						 *
						*/
						//printf("started looking at is: %c\n", calculator_string[started_looking_at]);
						//printf("looking at is: %c\n", calculator_string[looking_at]);
						//printf("decimal index is at %d (%c for proof)\n",decimal_index, calculator_string[decimal_index]);
						
						/*
						 * The construction of numbers can be done via the decimal numbering system.
						 * EX: the number 230.23+ =
						 * 2 x 10^2 + 3 x 10^1 + 0 x 10^0 + 2 x 10^-1 + 3 x 10^-2
						 *
						 * started looking at in this case would be at + (6), and looking at would be oob (-1).
						 * use an incrementer calculated by -1 * (-1 * calculating_number): if number is left (0), counter is -1, right (1), counter is 1
						 *
						 * establish a "new number" as 0.0;
						 *
						 * if decimal index isn't present, then follow this calculation:
						 * > using the left number 8621+ as an example
						 * > 4 - (-1) = 5 (index range)
						 * > 5 - 1 = 4 (removing operator total numbers)
						 * > set on_index = started looking at
						 * > for (multiple = 0; multiple = 4; multiple += 1)
						 * > get number from calculator_string[on_index]
						 * > multiply number by 10^multiple and add it to new_number
						 * > on_index -= increment
						 *
						 * if decimal index is present, then follow this calculation:
						 * > using the left number 718.21+ as an example
						 * > 6 - (-1) = 7 (index range)
						 * > 7 - 2 = 5 (removing operator and decimal place total numbers)
						 * > set on_index = started_looking_at
						 * > decimal index = 3
						 * > 3 - 5 = 2 (the degree of decimal place)
						 * > for (multiple = -1 * 2; multiple = 5; multiple += 1)
						 * > if on_index == decimal_index then on_index -= increment
						 * > get number from calculator_string[on_index]
						 * > multiply number by 10 ^ multiple and add it to new_number
						 * > on_index -= increment
						 *
						 * > at the end, check looking_at to see if there is a '-'. If there is, and its not an actual operator, then multiply the final result by -1
						 */
							
						int multiple = 0;
						double new_number = 0.0;
						int index_range = (absVal(started_looking_at - looking_at));
						int on_index = 0;
						if (calculating_number == 0)
						{
							on_index = started_looking_at - 1;
						}
						else
						{
							on_index = looking_at - 1;
						}
							
						if (decimal_index == -999)
						{
							for (int multiple = 0; multiple < index_range - 1; multiple += 1)
							{
								//printf("currently on index: %d, letter is %c, new number is: %lf\n", on_index, calculator_string[on_index], new_number);
								int obtained_number = getNumberFromString(calculator_string[on_index]);
								new_number += (obtained_number * (raiseToPower(10,multiple)));
								on_index -= 1;
								//printf("now on index %d. obtained number was: %d, mutliple was: %d, new number is: %lf\n",on_index, obtained_number,multiple, new_number);

							}
							
						}
						else
						{	
							int numbers_before_decimal = 0; 
							if (calculating_number == 0)
							{
								numbers_before_decimal = absVal(started_looking_at - decimal_index) - 1;
							}
							else
							{
								numbers_before_decimal = absVal(looking_at - decimal_index) - 1;
							}
							int numbers_calculated = 0;	
							//printf("decimal_index: %d, multiple: %d, index_range: %d\n",decimal_index,multiple,index_range);	
							for (int multiple = (numbers_before_decimal * -1); numbers_calculated < (index_range - 2); numbers_calculated += 1)
							{	
								if (on_index == decimal_index)
								{
									on_index -= 1;
								}
								//printf("currently on index: %d, letter is %c, multiple is %d, new number is: %lf\n", on_index, calculator_string[on_index], multiple, new_number);
								int obtained_number = getNumberFromString(calculator_string[on_index]);
								new_number += (obtained_number * (raiseToPower(10,multiple)));
								on_index -= 1;
								multiple += 1;
								//printf("now on index %d. obtained number was: %d, mutliple is now: %d, new number is: %lf\n",on_index, obtained_number,multiple, new_number);


							}
						
						}
						if (calculator_string[on_index + 1] == '-' && operatorArray[on_index + 1][0] == 0)
						{
							new_number *= -1;
						}
						final_number = new_number;
						//printf("[!/ FINAL NUMBER: %lf]\n", final_number);
					}
					numberArray[calculating_number] = final_number;
				}
			
				double left_number = numberArray[0];
				double right_number = numberArray[1];
				//printf("left number: %lf right number: %lf\n",left_number,right_number);				
				switch(operatorArray[highest_weighted_operator_index][0])
				{
					case 1:
						operator_total = left_number + right_number;
						break;
					case 2:
						operator_total = left_number - right_number;
						break;
					case 3:
						operator_total = left_number * right_number;
						break;
					case 4:
						if (right_number == 0.0)
						{	
							equation_errored = 2;
							break;
						}
						operator_total = left_number / right_number;
						break;
				}

				if (equation_errored > 0)
				{
					//printf("Calulcation Errored due to division by 0, breaking out of operator calculation\n");
					break;
				}
			
					
				// Mark as solved	
				operatorArray[highest_weighted_operator_index][3] = 1;
				// Note when it was solved in the process of the operators
				operatorArray[highest_weighted_operator_index][4] = total_operators_calculated + 1;
				// Note solution
				operatorSolutionArray[highest_weighted_operator_index][0] = operator_total;
				//printf("[!] finished operator: %c at index: %d\n", calculator_string[highest_weighted_operator_index], highest_weighted_operator_index);

				if (total_operators_calculated + 1 >= total_operators)
				{
					block_total = operator_total;
				}
			}
		
	
			if (equation_errored > 0)
			{
				//printf("CALCULATION ERROR, ENDING CALCULATION\n");
				break;
			}
		

			// Mark the block as complete
			master_block_array[highest_scoring_block_index][3] = 1;
			// add the result to the solution array
			master_solution_array[highest_scoring_block_index][0] = block_total;
			//printf("!!! finished calulcation for block index: %d that had the score: %d\n", highest_scoring_block_index, highest_block_score);
			//printf("!!! the result was %lf\n", block_total);
			//printf("!!! calculated: %d out of %d total blocks\n", total_blocks_calculated + 1, block_count);

			if (total_blocks_calculated + 1 >= block_count)
			{
				final_calculated_result = block_total;
			}

		}
		// Ends the loop
		result_calculated += 1;
	}
	while (result_calculated == 0);

	return final_calculated_result;
}

void setup() 
{
	Serial.begin(115200);

  Wire.begin(SDA_Pin, SCL_Pin);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
      Serial.println(F("SSD1306 allocation failed"));
            for(;;);
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
	display.print("awaiting input");
	display.display();
}

// Appends the provided letter to the active equation string
void appendString(char to_append)
{
    char appending_string[2] = {to_append,'\0'};
    strcat(calculator_string, appending_string);
}

// Removes one letter from the active equation string
void subtractString()
{
	calculator_string[strlen(calculator_string)-1] = '\0'; 
}

// Toggles command functions should be activated
void toggleCommand()
{
	command_active = (command_active != true);
	if(command_active)
	{
		appendString('^');
	}
	else
	{
		subtractString();
	}
}

// Clears the active equation string
void clearString()
{
  for(int i = (strlen(calculator_string) - 1); i >= 0; i--)
  {
   subtractString();
  }
}

void loop() 
{
	char customKey = customKeypad.getKey();
  
	if (customKey)
	{
    //Serial.println(customKey);  
	  calculated_number = 0.0;	
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    
     /* 
     Serial.println("-----");
     Serial.println(strlen(calculator_string) >= (MAX_EQUATION_LENGTH - 2));
    
     Serial.println((command_active == false && (customKey != 'A' && customKey != 'B' && customKey != '#')));
    
     Serial.println((command_active == true && (customKey != '#' && customKey != '*')));
     Serial.println("-----");
     */

		// Don't allow input if the string is too long except if removing a letter or solving the equation.
		// Enable exceptions for command letters since # appends an '^' to the string
		if (
        strlen(calculator_string) >= (MAX_EQUATION_LENGTH - 2)
        &&
        (
         (command_active == false && (customKey != 'A' && customKey != 'B' && customKey != '#'))
         ||
         (command_active == true && (customKey != '#' && customKey != '*'))
        )
       )
		{
      display.clearDisplay();
      display.setCursor(25,25);
      display.print("Too Long!");
      display.display();
      delay(500);
		}
		else
		{
			switch(customKey)
			{	
				/*
					Letter Options:
					A -> Attempt to solve
					B -> Backspace (remove last letter)
					C -> Place (
					D -> Place )
					* -> Place .

					Command Options:
					#* -> Clear string
					#A -> Place +
					#B -> Place -
					#C -> Place *
					#D -> Place /
				*/
				case '#': 
            toggleCommand();
					  break;
				case '*':
					if(command_active == true)
					{
						toggleCommand();
            
            for(int i = (strlen(calculator_string) - 1); i >= 0; i--)
            {
              subtractString();
            }
          	display.clearDisplay();
            display.setCursor(25,25);
					  display.print("strings cleared");
            display.display();
            delay(500);

					}
					else
					{
						appendString('.');
					}
					break;

				case 'A':
					if(command_active == true)
					{
						toggleCommand();
						appendString('+');		
					}
					else // Attempt to solve equation
					{
            equation_errored = 0;
            calculated_number = calculateString();
            if (equation_errored == 1)
            {
              display.clearDisplay();
              display.setCursor(0,25);
              display.print("error: invalid parenthesis");
              display.display();
              delay(2000);
            }
            if (equation_errored == 2)
            {
              display.clearDisplay();
              display.setCursor(0,25);
              display.print("error: division by 0");
              display.display();
              delay(2000);
            }
					}
					break;
				case 'B':
					if(command_active == true)
					{	
						toggleCommand();
						appendString('-'); 
					}       
					else // Remove last character
					{
						subtractString();	
					}
					break;
				 case 'C':
					if(command_active == true)
					{
						toggleCommand();
						appendString('*');
					}
					else
					{	
						appendString('(');
					}
					break;
				case 'D':
					if(command_active == true)
					{
						toggleCommand();
						appendString('/');
					}
					else
					{
						appendString(')');
					}
					break;
				default:
					appendString(customKey);
					break;
			}
		}	
		display.clearDisplay();
		display.setCursor(0,0);
		display.print(calculator_string);
		display.setCursor(0,35);
		display.print(calculated_number);
		display.display();
	}
}
