// delete string
// delete string

// delete string

// delete string

// ADDDDDDDDDDDDDDDDD

#ifndef IMG_READER_H
#define IMG_READER_H

#include "common.h"
#include <string.h>

#define					BSIZE	2048

struct imgHeader_t
{
	char	sign[4];
	DWORD	numberElement;
};

struct objHeader_t
{
	DWORD	offset;
	DWORD	sizeFile;
	char	nameFile[24]; 
};

class img_reader
{
protected:	
	img_reader(){} // default constructor
	img_reader(const img_reader&){} // copy constructor
public:
	
	img_reader(char* filename);

	char*			imgName;
	DWORD			countFiles;
	objHeader_t*	fileList;

private:
	void getFileList();

public:
	virtual ~img_reader();
};

///////////////////////////////implement////////////////////////////////
img_reader::img_reader(char* filename)
{

	imgName = filename;

	getFileList();

	//_ex( imgName, fileList, 0, countFiles );
	DWORD _start = 0;
	DWORD _end = countFiles;
	
	FILE* imgFile;
	FILE* exportFile;

	if ( fopen_s(&imgFile, imgName, "rb") !=0 ) return;
	
	char* buff;
	objHeader_t * _ptr = fileList; // temp pointer/iterator

	if (_start!=0)
		for(DWORD i=0; i < _start; i++) _ptr++;
		
	DWORD oldOffset = 0;
	for(DWORD i=_start; i < _end; i++)
		{
			// get data	
			fseek(imgFile, (_ptr->offset * BSIZE), SEEK_SET);
			
			oldOffset += _ptr->offset * BSIZE;
			buff = new char[ (_ptr->sizeFile * BSIZE ) ];

			fread(buff, BSIZE, _ptr->sizeFile, imgFile);

			fopen_s(&exportFile, _ptr->nameFile, "wb");
			fwrite(buff, BSIZE, _ptr->sizeFile, exportFile);
			fclose(exportFile);

			_ptr += 1;
			delete[] buff;
		}
	
	fclose(imgFile);

};

void img_reader::getFileList()
{
	FILE* imgFile;

	if ( fopen_s(&imgFile, imgName, "rb") !=0 ) return;

	///////////////////////////////////////////	
	// read header
	imgHeader_t ihdr;

	fread(&ihdr, sizeof(ihdr), 1, imgFile);
		if ( !strcmp(ihdr.sign, "VER2") ) return;

	countFiles = ihdr.numberElement;

	///////////////////////////////////////////
	// read file list to array of headerFiles
	fileList	= new objHeader_t[countFiles];
	

	objHeader_t* _ptr = fileList;	// temp pointer/iterator
	for(DWORD i=0; i < countFiles; i++)
	{
		fread(_ptr, 1, sizeof(objHeader_t), imgFile);	
	
		_ptr += 1;
	}
	fclose(imgFile);
};

img_reader::~img_reader()
{
	delete[] fileList;
};


#endif