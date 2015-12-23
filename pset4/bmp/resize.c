/*
* 	resize.c
*	Computer Science 50: Problem set 4
*
*	Resizes a BMP file.
*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
	//ensure proper usage
	if(argc != 4)
	{
		printf("Usage: ./resize n infile outfile\n");
		return 1;
	}
	
	//remember arguments
	int n = atoi(argv[1]);
	char* infile = argv[2];
	char* outfile = argv[3];
	
	
	//ensure 'n' is correct
	if (n < 0 || n > 100)
	{
		printf("Please enter a positive int between 1 and 100.\n");
		return 2;
	}
	
	//open input file, read it & ensure it exists.
	FILE* inputf = fopen(infile, "r");
		if (inputf == NULL)
		{
			printf("Could not open '%s'.\n", infile);
			return 3;
		}
	
	//open output file & ensure it exists.
	FILE* outputf = fopen(outfile, "w");
		if(outputf == NULL)
		{
			fclose(inputf);
			fprintf(stderr, "Could not create '%s'.\n", outfile);
			return 4;	
		}

	/*
	*	HEADERS
	*/	
		//read infile's BITMAPFILEHEADER
		BITMAPFILEHEADER bf;
			fread(&bf, sizeof(BITMAPFILEHEADER), 1, inputf);
		
		//read infile's BITMAINFOHEADER
		BITMAPINFOHEADER bi;
			fread(&bi, sizeof(BITMAPINFOHEADER), 1, inputf);
	
		// ensure infile is a 24-bit uncompressed BMP file image.
		if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40
			|| bi.biBitCount != 24 || bi.biCompression != 0)
			{
				fclose(outputf);
				fclose(inputf);
				fprintf(stderr, "Unsupported file format.\n");
				return 5;
			}
	
			// Copy original bf & bi into 'new' headers
			BITMAPFILEHEADER newBf = bf;
			BITMAPINFOHEADER newBi = bi;
				//update height & width ; padding ; biSize ; bfSize
				newBi.biWidth = bi.biWidth * n;
				newBi.biHeight = bi.biHeight * n;				
				
				int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
				int newPadding = (4 - (newBi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
					
				newBi.biSizeImage = (newBi.biWidth  * sizeof(RGBTRIPLE) + newPadding) * abs(newBi.biHeight);
				newBf.bfSize = newBi.biSizeImage + 54;
					
				// write 'new headers' into outputf
				fwrite(&newBf, sizeof(BITMAPFILEHEADER), 1, outputf);
				fwrite(&newBi, sizeof(BITMAPINFOHEADER), 1, outputf);

	/*
	*	SCANLINES
	*/						
		// iterate over infile's scanlines
		for(int rows = 0; rows < abs(bi.biHeight); rows++)
		{
			// iterate through the same scanline 'n' times
			for(int vCount = 0; vCount < n; vCount++)
			{
				// iterate through every pixel in the current scanline/row
				for(int pxCount = 0; pxCount < bi.biWidth; pxCount++)
				{
					// create a temporary storage for each RGB colors
					RGBTRIPLE triple;
					
					fread(&triple, sizeof(RGBTRIPLE), 1, inputf);
					
					// write the pixels 'n' times
					for(int morePixels = 0; morePixels < n; morePixels++)
					{
						fwrite(&triple, sizeof(RGBTRIPLE), 1, outputf);
					}				
				}
				for(int addPad = 0; addPad < newPadding; addPad++)
					fputc(0x00, outputf);	
				
				//return to beginning of scanline
				if(vCount < n - 1)
					fseek(inputf, -(bi.biWidth) * sizeof(RGBTRIPLE), SEEK_CUR);				
			}			
			// skip over padding, if any
			fseek(inputf, oldPadding, SEEK_CUR);										
		}				
		
		// When everything is done...
		// close infile
		fclose(inputf);
		
		//close outfile
		fclose(outputf);
		
		return 0;

}
















