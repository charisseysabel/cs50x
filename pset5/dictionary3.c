/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"
/**
 *	DECLARATIONS
 */
	// node struct
	typedef struct node
	{
		bool is_word;
		struct node* children[27];
	} node;
	
	node* root_ptr = NULL;
	
	// max length for character array + \0
	char maxWord[LENGTH + 1];
	
	// function to convert current letter to corresponding index place
	int idxConvrt(const char letter)
	{
		if(letter == '\'')
		{
			return 27;
		}		
		else if(isalpha(letter))
		{
			int temp = tolower(letter) - 'a';
			return temp;
		}
		return 0;
		
	}
	
	// variable to count how many words entered memory
	int wordCount = 0;
	
	// unload function
	bool freeNode(node* uNode);

/**
 * Returns true if word is in dictionary else false.
 */
	bool check(const char* word)
	{
		// TODO
		node* temp_ptr = root_ptr;		
		// loop through each letter of the input
		for(int cur = 0, wordLength = strlen(word); cur < wordLength; cur++)
		{	// disable case sensitivity
			int checkIndex = idxConvrt(word[cur]);
			
			if(temp_ptr->children[checkIndex] != NULL)
			{
				temp_ptr = temp_ptr->children[checkIndex];
			}
			else if(temp_ptr->children[checkIndex] == NULL)
			{
				return false;
			}
		}
		return temp_ptr->is_word;
	}




/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
	bool load(const char* dictionary)
	{
		// TODO
		// set up pointers
			root_ptr = malloc(sizeof(node));
			node* temp_ptr = malloc(sizeof(node));
			temp_ptr = root_ptr;
			
		// open file and check for null
			FILE* textFile = fopen(dictionary, "r");
			if(textFile == NULL)
			{
				printf("Could not open %s\n", dictionary);
				return false;
			}
			// iterate through file until feof
			while(fscanf(textFile, "%s", maxWord) == 1)
			{
				//fscanf(textFile, "%s", maxWord);
				wordCount++;
				// loop through ever letter in the word
				for(int l = 0; l < strlen(maxWord); l++)
				{	// disable case sensitivity
					int checkIndex = idxConvrt(maxWord[l]);
					
					if(temp_ptr->children[checkIndex] == NULL)
					{
						node* new_node = malloc(sizeof(node));
						temp_ptr->children[checkIndex] = new_node;
						temp_ptr = new_node;	
					}
					else if(temp_ptr->children[checkIndex] != NULL)
					{
						temp_ptr = temp_ptr->children[checkIndex];
						//temp_ptr->children[idxConvrt(maxWord[l])] = NULL;
					}	
				}
				temp_ptr->is_word = true;
				temp_ptr = root_ptr;
			}
			fclose(textFile);
			//temp_ptr = root_ptr;
			//free(temp_ptr);
			return true;	
	}
	




/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    //int* total = &wordCount;
    return wordCount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return freeNode(root_ptr);
}

bool freeNode(node* uNode)
{
	for(int i = 0; i < 27; i++)
	{
		if(uNode->children[i] != NULL)
		{
			freeNode(uNode->children[i]);
		}
	}
	free(uNode);
	
	return true;
	
}

























