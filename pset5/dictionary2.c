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
			return 26;
		}		
		else if(isalpha(letter))
		{
			int temp = tolower(letter) - 'a';
			return temp;
		}
		return 0;
		
	}

	int wordCount = 0;
	
	// unload function
	void freeNode(node* uNode, node* backupNode);

/**
 * Returns true if word is in dictionary else false.
 */
	bool check(const char* word)
	{
		// TODO
		// gain access to dictionary in memory.
		node* check_ptr = malloc(sizeof(node));
		check_ptr = root_ptr;
		
		// loop through each letter of the input
		for(int cur = 0, wordLength = strlen(word); cur < wordLength; cur++)
		{	// disable case sensitivity
			int checkIndex = idxConvrt(word[cur]);
			
			if(check_ptr->children[checkIndex] != NULL)
			{
				check_ptr = check_ptr->children[checkIndex];
			}
			/*else if(check_ptr->children[checkIndex] == NULL)
			{
				if(check_ptr->is_word == true)
				{
					return true;
				}
				else
				{
					return false;
				}
			}*/
		}
		if(check_ptr->is_word != true)
		{
			return false;
		}
		else
			check_ptr = root_ptr;
			return true;
	}




/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
	bool load(const char* dictionary)
	{
		// TODO
		// set up pointers
			root_ptr = malloc(sizeof(node));
			node* temp_ptr = root_ptr;
			
		// open file and check for null
			FILE* textFile = fopen(dictionary, "r");
			if(textFile == NULL)
			{
				printf("Could not open %s\n", dictionary);
				return false;
			}
			// iterate through file until feof
			while(!feof(textFile))
			{
				fscanf(textFile, "%s", maxWord);
				// loop through ever letter in the word
				for(int l = 0, strlenOfWord = strlen(maxWord); l < strlenOfWord; l++)
				{
					if(temp_ptr->children[idxConvrt(maxWord[l])] == NULL)
					{
						node* new_node = malloc(sizeof(node));
						temp_ptr->children[idxConvrt(maxWord[l])] = new_node;
						temp_ptr = new_node;	
					}
					else if(temp_ptr->children[idxConvrt(maxWord[l])] != NULL)
					{
						temp_ptr = temp_ptr->children[idxConvrt(maxWord[l])];
						temp_ptr->children[idxConvrt(maxWord[l])] = NULL;
					}	

				}
				temp_ptr->is_word = true;
				temp_ptr = root_ptr;
				wordCount++;
			}
			
		
		//close file
		fclose(textFile);
		return true;
		
	}
	




/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    int* total = &wordCount;
    return* total;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    node* unloader = malloc(sizeof(node));
    node* temp = malloc(sizeof(node));
    
    unloader = root_ptr;
    temp = root_ptr;
    int rootIndex = 0;
    
    
    while(rootIndex < 27)
    {
    	if(unloader->children[rootIndex] != NULL)
    	{
			while(unloader->children[rootIndex] != NULL)
			{

				temp = unloader;
				unloader = unloader->children[rootIndex];
				freeNode(unloader, temp);
			}
    	}	
    	
    	if(unloader->children[rootIndex] == NULL)
    	{
    		unloader = temp;
    		free(unloader->children[rootIndex]);
    	}
    	rootIndex++;
    }
    


   
    
    
    return true;
}

void freeNode(node* uNode, node* backupNode)
{
	int rootIndex = 0;
	
	/*if(uNode == NULL)
	{
		return;
	}*/
	
	while(rootIndex < 27)
    {
    	while(uNode->children[rootIndex] != NULL)
    	{
    		backupNode = uNode;
    		uNode = uNode->children[rootIndex];
			freeNode(uNode, backupNode);
    	}
    	if(uNode->children[rootIndex] == NULL)
    	{
    		free(uNode->children[rootIndex]);
    		uNode = backupNode;			
    	}
    	rootIndex++;
    }
    if(uNode == NULL)
    {
    	return;
    }
    
}


























