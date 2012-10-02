#include "common.h"

///////////////////////////////////////////////////
// Read data from disk. Return size of data
//
size_t readFile(char const *name, BYTE *data)
{
		
	FILE* f = fopen(name, "rb");
		if (!f) return 0;
	
	// get file size
	fseek(f, 0, SEEK_END);
	size_t const size = ftell(f);
	fseek(f, 0, SEEK_SET);

	//read data
	size_t const result = fread(data, sizeof(BYTE), size, f);
		if (!result) return 0;
	
	fclose(f);
	return result;
};