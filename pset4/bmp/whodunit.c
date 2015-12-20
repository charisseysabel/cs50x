/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

    // open input file 
    FILE* inputf = fopen(infile, "r");
    if (inputf == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outputf = fopen(outfile, "w");
    if (outputf == NULL)
    {
        fclose(inputf);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read inputf's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inputf);

    // read inputf's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inputf);

    // ensure inputf is (likely) a 24-bit uncompressed BMP 4.0
    // if inputf is a BMP file, will skip to after the if condition.
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outputf);
        fclose(inputf);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outputf);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outputf);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("padding value is %i\n", padding);

    // iterate over inputf's scanlines
    // 'abs' is the 'absolute value' found in stdlib.h
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline/row
        for (int j = 0; j < bi.biWidth; j++)
        {
            
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from inputf
            fread(&triple, sizeof(RGBTRIPLE), 1, inputf);
            
		        //Turn every pure red pixel to white
		        if(triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00 && triple.rgbtRed == 0xff)
		        {
		       		triple.rgbtBlue = 0xff;
		       		triple.rgbtGreen = 0xff;
		        	triple.rgbtRed = 0xff;
		        	fwrite(&triple, sizeof(RGBTRIPLE), 1, outputf);
		        }
				
				//Turns the blue-greenish pixels to white
		        else if(triple.rgbtBlue == 0xf6 && triple.rgbtGreen == 0xf6 && triple.rgbtRed == 0xec)
		        {
		       		triple.rgbtBlue = 0xff;
		       		triple.rgbtGreen = 0xff;
		        	triple.rgbtRed = 0xff;
		        	fwrite(&triple, sizeof(RGBTRIPLE), 1, outputf);
		        }
		        
		        else if(triple.rgbtBlue >= 0xf6 && triple.rgbtGreen >= 0xf6 && triple.rgbtRed >= 0xec)
		        {
		       		triple.rgbtBlue = 0xff;
		       		triple.rgbtGreen = 0xff;
		        	triple.rgbtRed = 0xff;
		        	fwrite(&triple, sizeof(RGBTRIPLE), 1, outputf);
		        }

		        		
				else
				{
					// write RGB triple to outfile
					triple.rgbtBlue = 0xd2;
		       		triple.rgbtGreen = 0xd2;
		        	triple.rgbtRed = 0xe2;
				    fwrite(&triple, sizeof(RGBTRIPLE), 1, outputf);
		        }
            

		
        	
        }

        // skip over padding, if any
        fseek(inputf, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outputf);
        }
    }

    // close infile
    fclose(inputf);

    // close outfile
    fclose(outputf);

    // that's all folks
    return 0;
}
