#pragma once

#include<iostream> 
#include<fstream> 
#include <vector>
#include<cassert>
#include<array>
#include<algorithm>
#include<map> 
#include<unordered_map>
#include<iomanip> 
#include<filesystem> 

using std::cout, std::ifstream, std::ofstream, std::vector, std::string, std::ios, std::ostream, std::array;
using std::swap, std::map, std::pair, std::setw, std::left;
using std::unordered_map;


class FileHeader
{
	array<char, 2> filetype = { 'B', 'M' };

	unsigned int reserved1And2 = 0x00'00'00'00;
	unsigned int indexOfPixelData = 0x00'00'00'36;

public:
	unsigned int fileSize{};
	FileHeader() = default;

	friend class BMPImage;

};

class InfoHeader
{
	unsigned int infoHeaderSize = 0x00'00'00'28;
	short planes = 0x00'01;

	short bitsPerPixel = 0x00'20; //CAREFUL! not always 32! -> indices 28 - 29 (32 bits - 24 for G,B,R, and 8 for Alp
	unsigned int compressionMethod = 0x00'00'00'00;
	unsigned int sizeOfPixelData{};
	vector<int> remainingHeaderFields =
	{
		0x00'00'00'00,
		0x00'00'00'00,
		0x00'00'00'00,
		0x00'00'00'00
	};

public:
	unsigned int imageWidth = 0; //indices 18 - 21
	unsigned int imageHeight = 0; //indices 22 - 25

	InfoHeader() = default;

	unsigned int getInfoHeaderSize() const;
	unsigned int getSizeOfPixelData() const;

	friend class BMPImage;
};

enum class ColorEnum : unsigned int
{
	//A, R, G, B
	Black = 0xFF'00'00'00,
	White = 0xFF'FF'FF'FF,

	Red = 0xFF'FF'00'00,
	Green = 0xFF'00'FF'00,
	Blue = 0xFF'00'00'FF,

	Yellow = 0xFF'FF'FF'00,
	Cyan = 0xFF'00'FF'FF,
	Magenta = 0xFF'FF'00'FF,
};

struct Color
{
	//should be unsigned because 1) no "negative" colors and 2) having alpha = 255 (FF) is desirable
	unsigned int bgra = 0x00'00'00'00;

	Color() = default;
	Color(unsigned int b, unsigned int g, unsigned int r);
	Color(unsigned int b, unsigned int g, unsigned int r, unsigned int a);
	Color(ColorEnum color);

	unsigned int convertToUnsignedInt();
};

class PixelData
{
public:
	vector<vector<Color>> pixelMatrix;

	PixelData() = default;
};


class BMPImage
{
private:
	void readFileHeaderFromFile(ifstream& fin);
	void readInfoHeaderFromFile(ifstream& fin);
	void readPixelDataFromFile(ifstream& fin);
	void readImageFile(string inputFilename);
private:
	FileHeader fileHeader;
	InfoHeader infoHeader;

public:
	PixelData pixelData;

	BMPImage() = delete;
	BMPImage(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor);
	BMPImage(const string& filepath);

	void setPixelToColor_withThickness(unsigned int x, unsigned int y, const Color& color, unsigned int thickness);

	void writeImageFile(string filename);

};

