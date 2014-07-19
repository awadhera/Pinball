#ifndef TGALOADER_H
#define TGALOADER_H

#include<fstream>
#include<iostream>
#include <GL/glew.h>

enum TGAcolorType { RGB, RGBA };
struct TGA
{
	TGA()
	{
		data = (char *)0;
		width = 0;
		height = 0;
		pixelSizeInBytes = 0;
	}
	TGAcolorType cType;
	int TexID;
	char *data;
	int width;
	int height;
	int pixelSizeInBytes;
};

bool TGALoad(TGA &tgaObj,char *imageFile);

#endif