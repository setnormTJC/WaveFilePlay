#include "BMPImage.h"

unsigned int InfoHeader::getInfoHeaderSize() const
{
	return infoHeaderSize;
}

unsigned int InfoHeader::getSizeOfPixelData() const
{
	return sizeOfPixelData;
}

Color::Color(unsigned int b, unsigned int g, unsigned int r)
{
	bgra = (b << 0) | (g << 8) | (r << 16) | (0xFF << 24); // Set alpha to 255
}


Color::Color(unsigned int b, unsigned int g, unsigned int r, unsigned int a)
{
	bgra =
		(
			b << 0 |
			g << 8 |
			r << 16 |
			a << 24
			);
}

Color::Color(ColorEnum color)
	:bgra((unsigned int)color)
{

}

unsigned int Color::convertToUnsignedInt()
{
	return bgra;
}

void BMPImage::readFileHeaderFromFile(ifstream& fin)
{
	char filetype[2];
	fin.read(filetype, 2); //NOTE: fin.GET() appends null terminator! (\0)
	fileHeader.filetype.at(0) = filetype[0];
	fileHeader.filetype.at(1) = filetype[1];

	char filesize[4];
	fin.read(filesize, 4);

	//DETAILED approach without bitshifting and bitwise OR: 
	//auto first = (unsigned char)filesize[0];
	//auto second= (unsigned char)filesize[1];
	//auto third = (unsigned char)filesize[2];
	//auto fourth = (unsigned char)filesize[3];
	//cout << first + (second * pow(2, 8)) + (third*pow(2, 16)) + (fourth*pow(2, 24)) << "\n";

	//equivalently (faster, probably): 
	fileHeader.fileSize =
		(
			(unsigned char)filesize[0] << 0 |
			(unsigned char)filesize[1] << 8 |
			(unsigned char)filesize[2] << 16 |
			(unsigned char)filesize[3] << 24
			);
	char reserved1And2[4];
	fin.read(reserved1And2, 4);

	fileHeader.reserved1And2 = (
		(unsigned char)reserved1And2[0] << 0 |
		(unsigned char)reserved1And2[1] << 8 |
		(unsigned char)reserved1And2[2] << 16 |
		(unsigned char)reserved1And2[3] << 24
		);

	char indexOfPixelData[4];
	fin.read(indexOfPixelData, 4);
	fileHeader.indexOfPixelData = (
		(unsigned char)indexOfPixelData[0] << 0 |
		(unsigned char)indexOfPixelData[1] << 8 |
		(unsigned char)indexOfPixelData[2] << 16 |
		(unsigned char)indexOfPixelData[3] << 24
		);
}

void BMPImage::readInfoHeaderFromFile(ifstream& fin)
{
	char infoHeaderSize[4];
	fin.read(infoHeaderSize, 4);
	infoHeader.infoHeaderSize =
		(
			(unsigned char)infoHeaderSize[0] << 0 |
			(unsigned char)infoHeaderSize[1] << 8 |
			(unsigned char)infoHeaderSize[2] << 16 |
			(unsigned char)infoHeaderSize[3] << 24
			);


	/*a "safety check" here:*/
	if (infoHeader.infoHeaderSize != 40)
	{
		//cout << "Hey! Listen!\n"; 
		//cout << "Info header size is not 40! - it is " << infoHeader.infoHeaderSize << "\n";
		//std::cin.get(); 
	}


	char imageWidth[4];
	fin.read(imageWidth, 4);
	infoHeader.imageWidth =
		(
			(unsigned char)imageWidth[0] << 0 |
			(unsigned char)imageWidth[1] << 8 |
			(unsigned char)imageWidth[2] << 16 |
			(unsigned char)imageWidth[3] << 24
			);

	char imageHeight[4];
	fin.read(imageHeight, 4);
	infoHeader.imageHeight =
		(
			(unsigned char)imageHeight[0] << 0 |
			(unsigned char)imageHeight[1] << 8 |
			(unsigned char)imageHeight[2] << 16 |
			(unsigned char)imageHeight[3] << 24
			);

	char planes[2];
	fin.read(planes, 2);
	infoHeader.planes =
		(
			(unsigned char)planes[0] << 0 |
			(unsigned char)planes[1] << 8
			);

	char bitsPerPixel[2];
	fin.read(bitsPerPixel, 2);
	infoHeader.bitsPerPixel =
		(
			(unsigned char)bitsPerPixel[0] << 0 |
			(unsigned char)bitsPerPixel[1] << 8
			);

	/*another "safety check" here:*/
	if (infoHeader.bitsPerPixel != 32)
	{
		//cout << "Hey! Listen!\n";
		//cout << "bitsPerPixel is not 32 (gbra)! - it is " << infoHeader.bitsPerPixel << "\n";
		//std::cin.get();
	}


	char compressionMethod[4];
	fin.read(compressionMethod, 4);
	infoHeader.compressionMethod =
		(
			(unsigned char)compressionMethod[0] << 0 |
			(unsigned char)compressionMethod[1] << 8 |
			(unsigned char)compressionMethod[2] << 16 |
			(unsigned char)compressionMethod[3] << 24
			);

	char sizeOfPixelData[4];
	fin.read(sizeOfPixelData, 4);
	infoHeader.sizeOfPixelData =
		(
			(unsigned char)sizeOfPixelData[0] << 0 |
			(unsigned char)sizeOfPixelData[1] << 8 |
			(unsigned char)sizeOfPixelData[2] << 16 |
			(unsigned char)sizeOfPixelData[3] << 24
			);


	assert(infoHeader.remainingHeaderFields.size() == 4); //useless assertion? 

	for (int i = 0; i < infoHeader.remainingHeaderFields.size(); ++i)
	{
		char remainingHeaderFields[4];
		fin.read(remainingHeaderFields, 4);

		infoHeader.remainingHeaderFields.at(i) =
			(
				(unsigned char)remainingHeaderFields[0] << 0 |
				(unsigned char)remainingHeaderFields[1] << 8 |
				(unsigned char)remainingHeaderFields[2] << 16 |
				(unsigned char)remainingHeaderFields[3] << 24
				);

	}
}

void BMPImage::readPixelDataFromFile(ifstream& fin)
{
	if (infoHeader.bitsPerPixel == 32)
	{
		pixelData.pixelMatrix.resize(infoHeader.imageHeight,
			vector<Color>(infoHeader.imageWidth));	//CAREFUL to resize as a TWO-d array - NOT 1D!

		for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
		{
			for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
			{
				char bgra[4];
				fin.read(bgra, 4);

				if (fin.fail())
					//fin.fail gets set to true if, for example, ... the `row` counter variable goes too far
					//ex: 	for (int row = 0; row < infoHeader.imageHeight + 1; ++row)
				{
					cout << "Error: Attempted to read beyond the end of the file at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

				Color currentPixelColor
				{ (unsigned int)(unsigned char)bgra[0], (unsigned int)(unsigned char)bgra[1] ,
					(unsigned int)(unsigned char)bgra[2] , (unsigned int)(unsigned char)bgra[3] };
				if (col < infoHeader.imageWidth)
				{
					pixelData.pixelMatrix.at(row).at(col) = currentPixelColor;
				}

				else
				{
					cout << "Error: Attempted to access out-of-bounds pixel at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

			}
		}

		int lastThingInFile = fin.get(); //should be -1, I think

		//confirm that the end of the file was reached:
		if (!fin.eof())
		{
			cout << "Hey!\nListen\n EOF was not reached? Is there more pixel data? \n";
			std::cin.get();
		}
	}

	else
	{
		// Calculate the number of bytes per pixel based on the bit depth
		int bytesPerPixel = infoHeader.bitsPerPixel / 8;

		// Calculate the padding bytes per row (each row is padded to a multiple of 4 bytes)
		int paddingBytes = (4 - (infoHeader.imageWidth * bytesPerPixel) % 4) % 4;

		//cout << "not 32 bits per pixel\n";
		pixelData.pixelMatrix.resize(infoHeader.imageHeight,
			vector<Color>(infoHeader.imageWidth));	//CAREFUL to resize as a TWO-d array - NOT 1D!

		for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
		{
			for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
			{
				char bgr[3];
				fin.read(bgr, 3);

				if (fin.fail())
					//fin.fail gets set to true if, for example, ... the `row` counter variable goes too far
					//ex: 	for (int row = 0; row < infoHeader.imageHeight + 1; ++row)
				{
					cout << "Error: Attempted to read beyond the end of the file at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

				Color currentPixelColor
				{ (unsigned int)(unsigned char)bgr[0], (unsigned int)(unsigned char)bgr[1] ,
					(unsigned int)(unsigned char)bgr[2] };
				if (col < infoHeader.imageWidth)
				{
					pixelData.pixelMatrix.at(row).at(col) = currentPixelColor;
				}

				else
				{
					cout << "Error: Attempted to access out-of-bounds pixel at row " << row << ", col " << col << ".\n";
					std::cin.get();
					return;
				}

			}
			// Skip the padding bytes at the end of each row
			fin.ignore(paddingBytes);

		}

		//int counter = 0; 
		//while (!fin.eof())
		//{
			//counter++; 
		int lastThingInFile = fin.get(); //should be -1, I think
		//}

		//confirm that the end of the file was reached:
		if (!fin.eof())
		{
			cout << "Hey!\nListen\n EOF was not reached? Is there more pixel data? \n";
			std::cin.get();
		}
	}
}

void BMPImage::readImageFile(string inputFilename)
{
	ifstream fin{ inputFilename, ios::binary };

	if (!fin)
	{
		cout << "File " << inputFilename << " not found.\n";
		std::cin.get();
		return;
	}

	//first read the file header info: 
	readFileHeaderFromFile(fin);

	//now read info header: 
	readInfoHeaderFromFile(fin);

	readPixelDataFromFile(fin);

	fin.close();
}

void BMPImage::writeImageFile(string filename)
{
	ofstream fout{ filename, ios::binary };

	//first comes the 14-byte file header: 
	fout.write(reinterpret_cast<const char*>(fileHeader.filetype.data()), 2); //no sizeof here, since filetype is a pointer
	fout.write(reinterpret_cast<const char*>(&fileHeader.fileSize), sizeof(fileHeader.fileSize));
	fout.write(reinterpret_cast<const char*>(&fileHeader.reserved1And2), sizeof(fileHeader.reserved1And2));
	fout.write(reinterpret_cast<const char*>(&fileHeader.indexOfPixelData), sizeof(fileHeader.indexOfPixelData));

	//next, the 40-byte info header: 
	fout.write(reinterpret_cast<const char*>(&infoHeader.infoHeaderSize), sizeof(infoHeader.infoHeaderSize));
	fout.write(reinterpret_cast<const char*>(&infoHeader.imageWidth), sizeof(infoHeader.imageWidth));
	fout.write(reinterpret_cast<const char*>(&infoHeader.imageHeight), sizeof(infoHeader.imageHeight));
	fout.write(reinterpret_cast<const char*>(&infoHeader.planes), sizeof(infoHeader.planes));
	fout.write(reinterpret_cast<const char*>(&infoHeader.bitsPerPixel), sizeof(infoHeader.bitsPerPixel));
	fout.write(reinterpret_cast<const char*>(&infoHeader.compressionMethod), sizeof(infoHeader.compressionMethod));
	fout.write(reinterpret_cast<const char*>(&infoHeader.sizeOfPixelData), sizeof(infoHeader.sizeOfPixelData));

	for (const auto& remainingHeaderField : infoHeader.remainingHeaderFields)
	{
		fout.write(reinterpret_cast<const char*>(&remainingHeaderField), sizeof(remainingHeaderField));
	}

	// Calculate the number of bytes per pixel based on the bit depth
	int bytesPerPixel = infoHeader.bitsPerPixel / 8;

	// Calculate the padding bytes per row (each row is padded to a multiple of 4 bytes)
	int paddingBytes = (4 - (infoHeader.imageWidth * bytesPerPixel) % 4) % 4;

	//now the pixel data: 
	for (unsigned int row = 0; row < infoHeader.imageHeight; ++row)
	{
		for (unsigned int col = 0; col < infoHeader.imageWidth; ++col)
		{
			if (infoHeader.bitsPerPixel == 32)
			{


				unsigned int colorOfCurrentPixel = pixelData.pixelMatrix.at(row).at(col).convertToUnsignedInt();

				fout.write(reinterpret_cast<const char*>(&colorOfCurrentPixel), sizeof(colorOfCurrentPixel));
			}

			else if (infoHeader.bitsPerPixel == 24)
			{
				unsigned int colorOfCurrentPixel = pixelData.pixelMatrix.at(row).at(col).convertToUnsignedInt();
				char rgb[3] = {
					static_cast<char>((colorOfCurrentPixel >> 0) & 0xFF),
					static_cast<char>((colorOfCurrentPixel >> 8) & 0xFF),
					static_cast<char>((colorOfCurrentPixel >> 16) & 0xFF)
				};
				fout.write(rgb, 3);
			}

			else
			{
				cout << "Hey! Neither 32 nor 24 bits per pixel? What is this file?\n";
				std::cin.get();
			}
		}

		// Write the padding bytes at the end of each row
		char padding[3] = { 0, 0, 0 };
		fout.write(padding, paddingBytes);
	}

	fout.close();
}

BMPImage::BMPImage(unsigned int imageWidth, unsigned int imageHeight, const Color& fillColor)
{
	infoHeader.imageWidth = imageWidth;
	infoHeader.imageHeight = imageHeight;
	infoHeader.sizeOfPixelData = imageWidth * imageHeight * (infoHeader.bitsPerPixel / 8);
	//NOTE: fileheader size should always be 14 (I think) 
	fileHeader.fileSize = 14 + infoHeader.getInfoHeaderSize() + infoHeader.sizeOfPixelData;

	//fill pixelData with given fill color:
	for (unsigned int row = 0; row < imageHeight; ++row)
	{
		vector<Color> currentRow;
		for (unsigned int col = 0; col < imageWidth; ++col)
		{
			currentRow.push_back(fillColor);
		}
		pixelData.pixelMatrix.push_back(currentRow);
	}
}

BMPImage::BMPImage(const string& filepath)
{
	readImageFile(filepath);
}

void BMPImage::setPixelToColor_withThickness(unsigned int x, unsigned int y, const Color& color, unsigned int thickness)
{
	swap(x, y); //image coordinates "naturally" flip x and y

	// Ensure the center pixel is within bounds
	if (x >= infoHeader.imageWidth || y >= infoHeader.imageHeight)
	{
		cout << "Error: Center pixel out of bounds.\n";
		return;
	}

	//center pixel 
	pixelData.pixelMatrix[x][y] = color;


	if (thickness > 1)
	{
		//neighbors within thickness:
		for (int row = -1 * thickness; row <= (int)thickness; ++row) //goofy cast
		{
			for (int col = -1 * thickness; col < (int)thickness; ++col)
			{
				int newX = x + col;
				int newY = y + row;

				// Ensure the new coordinates are within bounds
				if (newX >= 0
					&& newX < static_cast<int>(infoHeader.imageWidth)
					&& newY >= 0
					&& newY < static_cast<int>(infoHeader.imageHeight))
				{
					pixelData.pixelMatrix[newX][newY] = color;
				}

			}
		}
	}
}
