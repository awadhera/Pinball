#include "TGALoader.h"

bool TGALoad(TGA &tgaObj,char *imageFile)
{
	std::ifstream in;
	in.open(imageFile,std::ios::in);
	if(in.fail())
	{
		return false;
	}
	char *imageType = new char[3];
	in.read(imageType,3);
	in.seekg (12, std::ios::cur);
	char *imageInfo = new char[6];
	in.read(imageInfo,6);
	if(imageType[1]!=0 || (imageType[2]!=2 && imageType[3]!=3))
	{
		delete [] imageType;
		delete [] imageInfo;
		in.close();
		return false;
	}
	tgaObj.width = imageInfo[0] + imageInfo[1]*256;
	tgaObj.height = imageInfo[2] + imageInfo[3]*256;
	tgaObj.pixelSizeInBytes = imageInfo[4]/8;
	switch(tgaObj.pixelSizeInBytes)
	{
		case 3:
			tgaObj.cType = RGB;
			break;
		case 4:
			tgaObj.cType = RGBA;
			break;
		default:
			delete [] imageType;
			delete [] imageInfo;
			in.close();
			return false;
	}
	unsigned long imageSize = tgaObj.width * tgaObj.height * tgaObj.pixelSizeInBytes;
	tgaObj.data = new char[imageSize];
	in.read(tgaObj.data,imageSize);
	delete [] imageType;
	delete [] imageInfo;
	in.close();
	return true;
}

