#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------ Helper Functions ------------------------------------------------
int myStrlen(const char * string){
	int count = 0; 
	while(string[count++] != '\0');
	return count - 1; 
}

/*	Checks if token is in the given string/character list
		- Also gives index where found (for ordering purposes)
*/
int in(char token, const char * charList, int size){
	int i;
	for(i = 0; i < size; ++i) 
		if (token == charList[i]) return i;
	return -1;
}


//------------------------------------------------ Component 1: Regex Preprocessing ------------------------------------------------
/*	Adds explicit concatenation operator ';' into regex.
	Rationale: (why is the code doing what it does?)
		- Order of operations in ascending order: union, concat, closure.
		- Still infix, not postfix
		- maintain precedence between '*' and ';'	
*/
void addExplicitConcatOperator(char * explicitRegex, const char * regex, int regexLen){
	int i, index = 0;
	for(i = 0; i < regexLen - 1; ++i){
		explicitRegex[index++] = regex[i];
		if(in(regex[i], "(|", 2) == -1 && in(regex[i + 1], "*|)", 3) == -1)
			explicitRegex[index++] = ';';
	}
	explicitRegex[index++] = regex[i];
	explicitRegex[index] = '\0';
}



/*	Converts infix expression to postfix
	
*/
void infixToPostfix(char * postfixRegex, const char * explicitRegex, int explicitRegexLen){
	int i, index = 0, stack_pos = 0;
	char operatorStack [explicitRegexLen + 1];

	for(i = 0; i < explicitRegexLen; ++i){
		char token = explicitRegex[i];
		if(in(token, "|;*", 3) != -1){
			while(stack_pos > -1 && operatorStack[stack_pos] != '(' &&
			      in(token, "|;*", 3) < in(operatorStack[stack_pos], "|;*", 3))
				postfixRegex[index++] = operatorStack[stack_pos--];
		
			operatorStack[stack_pos++] = token;
		}

		

		// postfixRegex[i] = explicitRegex[i];

	}
	postfixRegex[i] = '\0';
}


// ------------------------------------------------ Component 2: NDFA Creation ------------------------------------------------




// ------------------------------------------------ Main Function ------------------------------------------------
/*	Takes in a regex from command line and creates a NDFA representation of it
		- argv[1] is the regex 
*/
int main(int argc, char ** argv){
	
//Component 1: Infix to Postfix
	int regexLen = myStrlen(argv[1]);

	char * explicitRegex = (char *) malloc(2 * regexLen * sizeof(char)); 
	addExplicitConcatOperator(explicitRegex, argv[1], regexLen);
	int explicitRegexLen = myStrlen(explicitRegex);
	
	char * postfixRegex = (char *) malloc(explicitRegexLen * sizeof(char) + 1);
	infixToPostfix(postfixRegex, explicitRegex, explicitRegexLen);
	free(explicitRegex);
	printf("%s\n", postfixRegex);
	

//Component 2: NDFA Creation from postfix expression



	// Free resources
	free(postfixRegex);
	
	return 0;
}

