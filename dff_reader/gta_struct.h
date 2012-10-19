#ifndef GTA_STRUCT_H
#define GTA_STRUCT_H

// GTASA node rwID
#define CLUMP				0x10
#define ATOMIC				0x14
#define FRAMELIST			0x0E
#define GEOMETRYLIST		0x1A
#define GEOMETRY			0x0F
#define MATERIALLIST		0x08
#define MATERIAL			0x07
#define TEXTURE				0x06
#define EXTENSION			0x03

// Flags
#define CLUMPF				1
#define ATOMICF				2
#define FRAMELISTF			4
#define GEOMETRYLISTF		8
#define GEOMETRYF			16
#define MATERIALLISTF		32
#define MATERIALF			64
#define TEXTUREF			128
#define EXTENSIONF			256

//GTASA leaf rwID
#define STRUCT				0x01
#define NODENAME			0x0253F2FE
#define GEOMETRY			0x0F
#define BINMESHPLG			0x50E
#define ATOMIC				0x14


// RW
typedef struct {
	DWORD	sID;  // section id
	DWORD	size; // section size
	DWORD	rwID; // rw id, 0x1803FFFF is GTA:SA
} rw_t;

// Clump
typedef struct {
	DWORD atom_num; // linked objects representing number of atomic section - 
	DWORD unwn1; // Number of Light. Not support in GTA:SA
	DWORD unwn2; // Number of Camera. Not support in GTA:SA
} clump_t;

// Vector
typedef struct {
	float x,y,z;
} vector_t;

// Matrix
typedef struct {
	vector_t raw1;
	vector_t raw2;
	vector_t raw3;
} matrix_t;

// Frame info
typedef struct {
	matrix_t rotMatrix;
	vector_t pos;
	DWORD	 parentFrame;
	DWORD	 matrixFlag;
} frame_t;

// Geometry info
typedef struct {
	WORD   flag; // Flags
	BYTE   n_uv; // Number of UV coordinates used (GTA default is 1)
	BYTE   unn; // (usually 0 and 1 on PS2 and Xbox)
	DWORD  c_face; // Triangle counter
	DWORD  c_vertex; // Vertex counter
	DWORD  c_subMesh; // SubMesh counter
	DWORD  PLG_size; // hack;
} geometry_t;

// Atomic info
typedef struct {
	DWORD frameIndex;
	DWORD geometryIndex;
	DWORD Unknown1;// Always 5 ?
	DWORD Unknown2;// Always 0 ?
} atomic_t;

// bound box/sphere info
typedef struct {
	float pX;
	float pY;
	float pZ;
	float radius;
} bound_t;

// material info
typedef struct {
	DWORD flag;// 0x00000004 - reflect/ 0x00000001 - specular
	DWORD diffuse_color;
	DWORD reflect_color;
	float ref_amount;
	float spec_amount;
	float r,g,b; // reflect RGB
} mat_trace_t;

#endif //GTA_STRUCT_H