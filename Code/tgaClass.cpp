//implementation of tgaClass
//Note : the C file and memory operations are very efficient and so were
//not translated to C++ operations
#include "tgaClass.h"

using namespace std;

// make sure the image is ok without scaling - must be a power of 2
int TGA :: checkSize (int x)
{
	if (x == 2	 || x == 4 || 
		x == 8	 || x == 16 || 
		x == 32  || x == 64 ||
		x == 128 || x == 256 || x == 512)
		return 1;
	else return 0;
}
// reads in RGBA data and returns the same, 32 bit image with 4 channels
unsigned char * TGA :: getRGBA (FILE *fname, int size)
{
	unsigned char *rgba;	//the image data
	unsigned char temp;		//for reformatting data
	int bread;				//size of buffer read from file
	int i;

	rgba = (unsigned char *) malloc (size * 4); // allocate memory for image

	//  no memory allocated?
	if (rgba == NULL)
		return 0;

	bread = fread (rgba, sizeof (unsigned char), size * 4, fname); 
	
	// make sure that all of the data is where it should be
	if (bread != size * 4)
	{
		free (rgba);
		return 0;
	}
	for (i = 0; i < size * 4; i += 4 )
	{
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i + 2] = temp;
	}

	format = GL_RGBA;
	return rgba;
}
// reads in RGB data and returns RGB, 24bit image
unsigned char * TGA :: getRGB (FILE *fname, int size)
{
	unsigned char *rgb;	//the image data
	unsigned char temp;	//for reformatting data
	int bread;			//size of buffer read from file
	int i;

	rgb = (unsigned char *) malloc (size * 3); // allocate memory for image
	
	//  no memory allocated?
	if (rgb == NULL)
		return 0;

	bread = fread (rgb, sizeof (unsigned char), size * 3, fname); // read in the image data

	// make sure that all of the data is where it should be
	if (bread != size * 3)
	{
		// Bytes read != Bytes requested
		free (rgb);
		return 0;
	}

	for (i = 0; i < size * 3; i += 3)
	{
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}
	
	format = GL_RGB;

	return rgb;
}

// get the image data
unsigned char * TGA :: getData (FILE *fname, int sz, int iBits)
{
	if (iBits == 32)
		return getRGBA (fname, sz);
	else if (iBits == 24)
		return getRGB (fname, sz);
    return NULL;
}


// loads a 24bit or 32 bit targa file and uploads it to memory
int TGA :: readTGA (char *name)
{
	unsigned char type[4];
	unsigned char info[7];
	
	int imageBits, size;
	FILE *fname;
	
	data = NULL;
	
	// Could not open the file!
	if (!(fname = fopen (name, "r+bt")))
		return TGA_FILE_NOT_FOUND;

	fread (&type, sizeof (char), 3, fname); // read in colormap info and image type, byte 0 ignored
	fseek (fname, 12, SEEK_SET);			// seek past the header and useless info
	fread (&info, sizeof (char), 6, fname);

	// make sure that the colormap is 0 and type is 2, unmapped RGB
	if (type[1] != 0 || type[2] != 2)
		return TGA_BAD_IMAGE_TYPE;

	width = info[0] + info[1] * 256; // image width
	height = info[2] + info[3] * 256; // image height
	imageBits =	info[4]; // pixel depth

	size = width * height; // image size

	// Dimension imageWidth x imageHeight is not a valid dimension!!
	if (!checkSize (width) || !checkSize (height))
		return TGA_BAD_DIMENSION;

	// Image bits != Supported bits 
	if (imageBits != 32 && imageBits != 24)
		return TGA_BAD_BITS;

	// get the image data
	data = getData (fname, size, imageBits);
	
	fclose (fname);

	// something went wrong
	if (data == NULL)
		return TGA_BAD_DATA;

	return 1;
}

