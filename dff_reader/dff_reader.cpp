#include "dff_reader.h"

dffReader::dffReader()
{
	face   = new face3_t[];
	vertex = new vector3[];
	normal = new vector3[];
	uv	   = new vector2[];
}

dffReader::~dffReader()
{
	delete[] face;
	delete[] vertex;
	delete[] normal;
	delete[] uv;
}