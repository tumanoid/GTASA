#ifndef GTA_STRUCT_H
#define GTA_STRUCT_H
//rw
typedef struct {
	DWORD	sID;  // section id
	DWORD	size; // section size
	DWORD	rwID; // rw id, 0x1803FFFF is GTA:SA
} rw_t;

//clump
typedef struct {
	DWORD atom_num; // linked objects representing number of atomic section - 
	DWORD unwn1; //unknown
	DWORD unwn2; //unknown
} clump_t;

// vector
typedef struct {
	float x,y,z;
} vector_t;

// matrix
typedef struct {
	vector_t raw1;
	vector_t raw2;
	vector_t raw3;
} matrix_t;

// frame info
typedef struct {
	matrix_t rotMatrix;
	vector_t pos;
	DWORD	 parentFrame;
	DWORD	 matrixFlag;
} frame_t;

// geometry info
typedef struct {
	WORD   flag; // Flags
	BYTE   n_uv; // Number of UV coordinates used (GTA default is 1)
	BYTE   unn; // (usually 0 and 1 on PS2 and Xbox)
	DWORD  c_face; // Triangle counter
	DWORD  c_vertex; // Vertex counter
	DWORD  c_subMesh; // SubMesh counter
} geometry_t;

// atomic info
typedef struct {
	DWORD frameIndex;
	DWORD geometryIndex;
	DWORD Unknown1;// Always 5 ?
	DWORD Unknown2;// Always 0 ?
} atomic_t;



#endif //GTA_STRUCT_H