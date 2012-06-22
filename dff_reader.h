#include "common.h"
#include "gta_struct.h"


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


// add prototypes
void printRW( rw_t& );

typedef struct {
	DWORD	rwID; // this section ID
	DWORD	size; // size in byte
	char*	data; // offset in main buffer
	DWORD	parent; // parent ID
} section_t;


class dff_reader {
 protected:
	 dff_reader(){};
	 dff_reader(const dff_reader&){};

 public:
	 dff_reader(char* file_name);
	~dff_reader();	

 private: 	
	 char* f_buffer; // main file buffer
	 int  parse_fileRW(char *const);
	 
	 //raw
	 vector<section_t>   node; // RW sections array
	 
	 //vector<section_t>   clump;
	 vector<frame_t>   frame;
	 //vector<section_t>   geometry;
	 //vector<section_t>	 material;
	 //vector<section_t>	 texture;
	 //vector<section_t>	 atomic;
};

DWORD getSectionID(rw_t rw);

long lvl = 0;

DWORD clump_cnt;



int dff_reader::parse_fileRW(char *const buff) {

	if (buff==0) return 1;

	char* ofst = buff;
	section_t tmp_stn;
	rw_t rw;
	
	do {
		
		memcpy(&rw, ofst, sizeof(rw_t) ); ofst+=sizeof(rw_t);
		
		tmp_stn.data = ofst;
		tmp_stn.rwID = rw.sID;
		tmp_stn.size = rw.size;
		tmp_stn.parent = 0;
		//printRW(rw);

	    if (( getSectionID(rw) & (CLUMPF|FRAMELISTF|EXTENSIONF|GEOMETRYLISTF|
							   ATOMICF|GEOMETRYF|MATERIALLISTF|MATERIALF|TEXTUREF) )) 
		{
			node.push_back(tmp_stn);
		}
		else 
		{
			node.push_back(tmp_stn);		
			ofst+=rw.size;
		}

	} while (rw.rwID==0x1803FFFF);
	
	node.pop_back(); // delete last garbage element

	return 0;
}

dff_reader::dff_reader(char* file_name) {
	FILE *f;
	f_buffer=0;

		if (file_name==0) goto ERR_FILEOPEN;
	
	f = fopen(file_name, "rb");
		if (f==0) goto ERR_FILEOPEN;
	
	// Get file size
	fseek(f,0, SEEK_END);
	long buff_size = ftell(f);
		if (buff_size==0) goto ERR_READFILESIZE;
	fseek(f,0, SEEK_SET);

	// Allocate memory for buffer
	f_buffer = new char[buff_size];
		if (f_buffer == 0) goto ERR_MEMORYALLOCATE;
		if (!fread(f_buffer,1,buff_size, f)) goto ERR_READ2BUFFER;
		fclose(f);
	
		if ( parse_fileRW(f_buffer) ) goto ERR_PARSERFILERW;

		
	//	for (int i=0; i<node.size(); i++)
	//		printf("%X | ", node[i].rwID);

	return;
	// TODO: set success flag

ERR_FILEOPEN: 
	printf("ERR_FILEOPEN\n"); return;
ERR_READFILESIZE:
	printf("ERR_READFILESIZE\n"); return;
ERR_MEMORYALLOCATE:
	printf("ERR_MEMORYALLOCATE\n"); return;
ERR_READ2BUFFER:
	printf("ERR_READ2BUFFER\n"); return;
ERR_PARSERFILERW:
	printf("ERR_PARSERFILERW\n"); return;
}

dff_reader::~dff_reader() {
	if (f_buffer) delete [] f_buffer;
}

//////////////////////////////////////////////////////////
// utils
void printRW( rw_t& hdr) {
	printf("RW :\n");
	printf(" rwID %X\n", hdr.sID);
	printf(" size %i\n", hdr.size);
	if (hdr.rwID==0x1803FFFF) printf("GTA:SA\n");
		else printf("Unknown game %X\n", hdr.rwID);
	printf("\n");
}

DWORD getSectionID(rw_t rw){
	switch (rw.sID) {
			case CLUMP: return CLUMPF;
			case ATOMIC:return ATOMICF;
			case FRAMELIST: return FRAMELISTF;
			case EXTENSION: return EXTENSIONF;
			case GEOMETRYLIST: return GEOMETRYLISTF;
			case GEOMETRY: return GEOMETRYF;
			case MATERIALLIST: return MATERIALLISTF;
			case MATERIAL: return MATERIALF;
		break;
	}
	return 0;

}
/*
switch (rw.sID) {
			case CLUMP: lvl=0; break;
			case ATOMIC: lvl=0; break;
			case FRAMELIST: lvl=1; break;
			case EXTENSION: lvl=3; break;
			case GEOMETRYLIST: lvl=1; break;
			case GEOMETRY: lvl=2; break;
			case MATERIALLIST: lvl=3; break;
			case MATERIAL: lvl=4; break;

		   break;

	   }
	   */