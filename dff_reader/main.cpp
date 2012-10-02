#include "dff_reader.h"

#include <iostream>

extern size_t readFile(char const *name, BYTE *data);

int main(int argc, char *argv[] )

{
	BYTE* a = new BYTE[];
	
	size_t b = readFile(argv[1], a);
	std::cout << "SIZE of array" <<  b << std::endl;
	
	int d; std::cin >> d;


	delete[] a;

	return 0;
}