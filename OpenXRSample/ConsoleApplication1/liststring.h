#pragma once
#ifndef LISTSTRING_H
#define LISTSTRING_H

#include <stdio.h>
#include <stdlib.h>

#define STRING struct string
#define String STRING *
#define NODE struct node

struct node
{
	char character;
	NODE* next;
	NODE* prev;
};

struct string
{
	NODE* head;
	NODE* tail;
	int count;
};

String createString();									  //create a string
char sgetChar(String str, int pos);						  //Get the character at a given position in the string
void stringCat(String str1, String str2);				  //Concatenate two strings
void freeString(String str);							  //Free the string memory
void addChar(String str, char character);				  //Add a character to the end of the string
void addCharIndex(String str, char character, int index); //add a character at a specific index
char* putString(String str);							  //Return a character array from a string
void printString(String str);							  //Print the string
void scanString(String str, char* input);				  //scan contents of character array into string
void stringGets(String str);							  //read console into string
int stringLength(String str);							  //length of the string
void clearString(String str);							  //emptys the contents of the string
void deleteChar(String str, int index);					  //delete character
void deleteCharValue(String str, char value);			  //delete a specific character from a string
String splitString(String str, int index);				  //split the string in half at a certain index
void stringToUpper(String str);							  //converts string to upper case
void scanStringFirst(String str, char* input);			  //Appends to start of string
int stringComp(String str1, String str2);				  //check equality of two strings
void stringCopy(String str1, String str2);

int stringLength(String str)
{
	return str->count;
}

char sgetChar(String str, int pos)
{
	if (str->count < pos)
	{
		//printf("ERROR: REQUESTED POSITION OUTSIDE RANGE OF STRING");
		return (char)NULL;
	}
	if (str->head == NULL)
	{
		return (char)NULL;
	}
	NODE* n = str->head;
	for (int i = 0; i < pos; i++)
	{
		n = n->next;
	}
	if (n == NULL)
	{
		return (char)NULL;
	};
	return (char)n->character;
}

//Create the String and return the pointer to memory
String createString()
{
	String str;
	if ((str = (STRING*)malloc((sizeof(STRING)))))
	{
		str->head = NULL;
		str->tail = NULL;
		str->count = 0;
	}
	else
	{
		//printf("COULD NOT ALLOCATE MEMORY FOR STRING");
		return NULL;
	}
	return str;
}

void stringCat(String destination, String source)
{
	NODE* n = source->head;
	for (int i = 0; i < source->count; i++)
	{
		addChar(destination, n->character);
		n = n->next;
	}
}

//Free all the memory being used by the string
void freeString(String str)
{
	clearString(str);
	free(str->tail);
	free(str);
}

//Add a character to the String
void addChar(String str, char character)
{
	NODE* ch;
	if (!(ch = (NODE*)malloc(sizeof(NODE))))
	{
		printf("ERROR: UNABLE TO ALLOCATE SPACE FOR CHARACTER\n");
		return;
	}
	ch->next = NULL;
	ch->character = character;
	ch->prev = NULL;
	if (str->count > 0)
	{
		ch->prev = str->tail;
		str->tail->next = ch;
		str->tail = ch;
		str->count++;
	}
	else
	{
		str->head = ch;
		str->tail = ch;
		str->count++;
	}
}

void addCharIndex(String str, char character, int index)
{
	if (index < 0 || index > str->count)
	{
		printf("Error: requested index to add character outside of string range\n");
	}
	NODE* ch;
	NODE* prev = NULL;
	NODE* next = str->head;
	if (!(ch = (NODE*)malloc(sizeof(NODE))))
	{
		printf("ERROR: UNABLE TO ALLOCATE SPACE FOR CHARACTER\n");
		return;
	}
	for (int i = 0; i < index; i++)
	{
		prev = next;
		next = next->next;
	}
	if (prev == str->tail)
	{
		ch->next = NULL;
		ch->prev = prev;
		str->tail = ch;
		prev->next = ch;
	}
	else if (next == str->head)
	{
		ch->next = next;
		ch->prev = NULL;
		str->head = ch;
		next->prev = ch;
	}
	else
	{
		ch->next = next;
		ch->prev = prev;
		prev->next = ch;
		next->prev = ch;
	}
	if (str->count == 0)
	{
		addChar(str, character);
	}
	ch->character = character;
	str->count++;
}

//Moves string into character array, size of char array must be defined
char* putString(String str)
{
	char* temp = (char*)malloc(str->count + 1);
	NODE* tempnode = str->head;
	int i = 0;
	while (tempnode != NULL)
	{
		temp[i] = tempnode->character;
		tempnode = tempnode->next;
		i++;
	}
	temp[i] = '\0';
	return temp;
}

//Prints a String from start to finish
void printString(String str)
{
	NODE* n = str->head;
	while (n != NULL)
	{
		printf("%c", n->character);
		n = n->next;
	}
}

//scans array of characters into string
void scanString(String str, char* input)
{
	int i = 0;
	while (input[i] != '\0')
	{
		addChar(str, input[i]);
		i++;
	}
}

//Gets entire input stream from stdio until newline character
void stringGets(String str)
{
	char c = getchar();

	while (c != '\n')
	{
		addChar(str, c);
		c = getchar();
	}
}

void clearString(String str)
{
	NODE* n;
	NODE* del;
	n = str->head;
	while (n != NULL)
	{
		del = n;
		n = n->next;
		free(del);
	}
	str->count = 0;
	str->head = NULL;
	str->tail = NULL;
}

void deleteChar(String str, int index)
{
	if ((str->count - 1) < index)
	{
		printf("ERROR: Attempted to Acess element outside of string bounds\n");
		return;
	};
	NODE* n = str->head, * prevLoc, * nextLoc;
	for (int i = 0; i < index; i++)
	{
		n = n->next;
	}
	if (str->count == 1)
	{
		clearString(str);
		return;
	}
	if (n == str->head)
	{
		str->head = n->next;
		prevLoc = n;
		n = n->next;
		n->prev = NULL;
		free(prevLoc);
		str->count--;
		return;
	}
	if (n == str->tail)
	{
		str->tail = n->prev;
		prevLoc = n->prev;
		prevLoc->next = NULL;
		free(n);
		str->count--;
		return;
	}
	prevLoc = n->prev;
	nextLoc = n->next;
	prevLoc->next = nextLoc;
	nextLoc->prev = prevLoc;
	str->count--;
	free(n);
	return;
}
void deleteCharValue(String str, char value)
{
	NODE* n = str->head, * nextLoc;
	int i = 0;
	if (n == NULL)
	{
		return;
	};
	while (n != NULL)
	{
		//for (int i = 0; i < str->count; i++) {
		if (n->character == value && n != NULL)
		{
			nextLoc = n->next;
			deleteChar(str, i);
			n = nextLoc->next;
		}
		else
		{
			if (n != NULL)
			{
				n = n->next;
			}
		}
		i++;
		if (i == str->count - 1 && value == str->tail->character)
		{
			deleteChar(str, str->count - 1);
			return;
		}
	}
	//}
}

String splitString(String str, int index)
{
	String newString = createString();
	NODE* n = str->head;
	if (str->count - 2 < index)
	{
		printf("ERROR: requested position is outside of string range (string must be larger than 1 character)\n");
		return NULL;
	}
	for (int i = 0; i < index; i++)
	{
		n = n->next;
	}
	newString->head = n->next;
	newString->count = str->count - index - 1;
	newString->head->prev = NULL;
	str->tail = n;
	str->tail->next = NULL;
	str->count = index + 1;
	return newString;
}

void stringToUpper(String str)
{
	NODE* n = str->head;
	while (n != NULL)
	{
		if ((n->character > 96) && (n->character < 123))
		{
			n->character = (n->character - 32);
		}
		n = n->next;
	}
}

void scanStringFirst(String str, char* input)
{
	int i = 0;
	while (input[i] != '\0')
	{
		addCharIndex(str, input[i], i);
		i++;
	}
}

int stringComp(String str1, String str2)
{
	if (stringLength(str1) != stringLength(str2))
	{
		return 0;
	};
	for (int i = 0; i < stringLength(str1); i++)
	{
		if (sgetChar(str1, i) != sgetChar(str2, i))
		{
			return 0; //false
		}
	}
	return 1; //true
}

void stringCopy(String str1, String str2)
{
	char* temp;
	temp = putString(str2);
	scanString(str1, temp);
	free(temp);
}


/*int main(void) {
	String str;
	str = createString();
	String str2;
	//addChar(str2, 'a');
	//addChar(str2, 's');
	//addChar(str2, 'k');
	printString(str);
	char altstr[39] = "this is a string I'm not sure how long";
	//char* altstr2;
	scanString(str, altstr);	//Scan contents of altstr char array into string
	//altstr2 = putString(str); //Scan contents of String str into altstr2
	//freeString(str);		    //Free the string
	//clearString(str);			//Clears the String contents emptying the string
	//stringCat(str, str2);		//Concatonates 2 strings
	//printf("%c", sgetChar(str, 40));	//prints the given character from a string
	//printf("%d", stringLength(str));	//returns string length
	//printString(str);					//Prints a string
	deleteCharValue(str, 'g');
	//printString(str);
	str2 = splitString(str, 8);
	deleteChar(str2, 3);
	printString(str);
	printf("\n");
	printString(str2);
	//printString(str2);
}*/
#endif // !liststring.h
