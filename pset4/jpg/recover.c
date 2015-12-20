/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
	//ensure proper usage
	 	if(argc != 1)
	 	{
	 		printf("Usage: ./recover\n");
	 		return 1;
	 	}
	 	
	 //remember arguments, jpgCount, initialize fileName
	 int img = 0;
	 unsigned char block[512];
	 char fileName[10];
	 	
	 // open the card file and make sure it exists
	 FILE* input = fopen("card.raw", "r");
	 if (input == NULL)
	 {
	 	printf("Could not open card.raw\n");
	 	return 2;
	 }
	 
	 /*FILE* output = fopen(fileName, "w");
	 if (output == NULL)
	 {
	 	fclose(input);
	 	return 2;
	 }*/
	 
	 FILE* output = NULL;
	 
	 while(fread(&block, sizeof(block), 1, input) == 1)
	 {
	 	if(block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] == 0xe0 || block[3] == 0xe1))
	 	{
			if(output != NULL)
			{
				fclose(output);
				sprintf(fileName, "%03d.jpg", img);
				img++;
				output = fopen(fileName, "w");
				fwrite(&block, sizeof(block), 1, output);
			}
			else
			{
				output = fopen(fileName, "w");
				sprintf(fileName, "%03d.jpg", img);
				img++;
				output = fopen(fileName, "w");
				fwrite(&block, sizeof(block), 1, output);
				if (output == NULL)
				{
					 fclose(input);
					 return 2;
				}							
			}
	 	}
	 	else
		{
			if(output != NULL)
			{
				fwrite(&block, sizeof(block), 1, output);
			}			
		}
		
	 }
	 	
	 fclose(input);
	 fclose(output);
	 	
	return 0;	
}
