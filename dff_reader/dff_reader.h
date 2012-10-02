#ifndef DFF_READER_H
#define DFF_READER_H
#include "common.h"

// section types

#define	rwDATA				1
#define	rwSTRING			2
#define	rwEXTENSION			3
#define	rwTEXTURE			6
#define	rwMATERIAL			7
#define	rwMATERIALLIST		8

#define	rwFRAMELIST			14
#define	rwGEOMETRY			15
#define	rwCLUMP				16

#define	rwATOMIC			20
#define	rwGEOMETRYLIST		26

#define	rwMATERIALSPLIT		1294
#define	rwFRAME				39056126


typedef float vector2[2];

typedef float vector3[3];

typedef uint  face3_t[3];

typedef struct 
{
	DWORD	sectionType;
	DWORD	sectionSize;
	DWORD	RWversionID

} rw_hdr_t;

typedef struct 
{
	WORD	tcount;
	WORD	smt;
} rw_struct_t;

typedef struct
{
	float pos[3];
	float norm[3];
	float uv[2];

} point_t;

class dffReader
{
public:
	dffReader();
	~dffReader();

public:
	// data
	face3_t* face;
	vector3* vertex;
	vector3* normal;
	vector2* uv;
	
	// counters
	uint	cFace;
	uint	cVertex;
	uint	cNormal;
	uint	cUV;
};



#endif //DFF_READER_H