//class to load a TGA image file 
//based on Nate Millers tutorial - but cleaned up
//still has a decidedly C flavor
//D. Byrnes , 5/6/02

#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>

#ifndef TGA_H
#define TGA_H

// Error Codes should be visible to all files

// file was not found
const int TGA_FILE_NOT_FOUND = 13;
// color mapped image or image is not RGB uncompressed
const int TGA_BAD_IMAGE_TYPE = 14;
// dimension is not a power of 2
const int TGA_BAD_DIMENSION	= 15;
// image bits is not 24 or 32
const int TGA_BAD_BITS = 16;
// image data could not be loaded
const int TGA_BAD_DATA = 17;

class TGA {

	public:
		
		//everything of interest is done in the read
		TGA() { }
		~TGA() {delete(data); }
		
		// loads a 24 bit or 32 bit targa file and uploads it to memory
		int readTGA(char *fileName);
		
		//public so no accessors are needed
		GLenum format;		//number of channels (3 or 4)
		int width;			//width of image
		int height;			//height of image
		unsigned char * data;	//color data
		
	private:
	
		//these are all helper functions
		
		// make sure the image is ok without scaling -must be a power of 2
		int checkSize (int x);
		
		// reads in RGBA data and returns the same, 32 bit image
		unsigned char *getRGBA (FILE *s, int size);
		
		// reads in RGB data and returns RGB, 24bit image
		unsigned char *getRGB (FILE *s, int size);
		
		// get the image data
		unsigned char *getData (FILE *s, int sz, int iBits);
		
};

#endif