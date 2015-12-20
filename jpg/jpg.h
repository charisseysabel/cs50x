/*
*
* jpg.h
*
* Computer Science 50
* Problem Set 4
* Stores a block of data (512 bytes at a time)
*
*/

typedef struct
{
	unsigned int	signature;
	unsigned char	restOfData[508];
} __attribute__((__packed__))
BLOCKDATA;
