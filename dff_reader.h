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
#define NODENAME			0x0253F2FE


typedef struct {
	rw_t	rw;
	char*	dataOffset; // offset in main buffer
	//DWORD	parent; // parent ID
} section_t;


class DffReader {
  protected:
	 DffReader(){};
	 DffReader(const DffReader&){};

  public:
	 DffReader(char* file_name);
	~DffReader();	

  private: 	
	 char* file_name; // file name
	 char* f_buffer; // main file buffer
	 
	 DWORD count_clump;
	 DWORD count_frame;
	 
     int parse_fileRW(char *const);
     int parse_frame();
	 
     vector<section_t>   node; // RW sections array
     vector<frame_t>     frame;
	 vector<string>      frame_name;

};

// add prototypes
void printRW( rw_t& );
DWORD getSectionID(rw_t rw);


int DffReader::parse_frame() {
    
	char* _offset = NULL;
	
	// Get frame count
	for (int i=0; i<node.size(); i++){
		if (node[i].rw.sID==FRAMELIST) {
			
			_offset = node[i+1].dataOffset; break;
		}
	}
		if (_offset==NULL) return 1;
	
	memcpy(&count_frame, _offset, sizeof(count_frame) ); _offset+=sizeof(count_frame);
	
	///printf("frame count is %i\n", count_frame); 


	// Get frame_t vector
	frame_t _frame;

	for (int i=0; i<count_frame; i++) {	
		
		memcpy(&_frame, _offset, sizeof(_frame) ); _offset+=sizeof(_frame);
		frame.push_back(_frame);
		
	///printf("pos matrix is %f %f %f\n", _frame.pos.x, _frame.pos.y, _frame.pos.z); 
	}

	///printf("Frame section count is %i\n", frame.size() ); 
	// Get name of frame
	
	int  clump_cnt = 0;
	for (int i=0; i<node.size(); i++){
		
		// only one CLUMP section 
		if (node[i].rw.sID==CLUMP) clump_cnt++;
			if (clump_cnt>1) break;
	
		if (node[i].rw.sID==NODENAME) {
			
			_offset = node[i].dataOffset;
			DWORD size = node[i].rw.size;
			
			char* name = new char[size+1]; //+1 for null terminate
				if (name==NULL) return 1;
			
			memcpy(name, _offset, size);
			name[size]='\0';
	
			frame_name.push_back( string (name) );

	///printf("%s\n", name );
			delete [] name;

		}
	}
	
	if (clump_cnt>1) {
		
		count_clump = 3; // always exist either 1 or 3 CLUMP section
		
		char* _tr = "terminator";
		if (clump_cnt>1) frame_name.push_back( string(_tr) );
	}

	///printf("Frame name count is  %i\n", frame_name.size() );
	///printf("CLUMP count is %i\n", clump_cnt );
	
	return 0;
}

int DffReader::parse_fileRW(char *const buff) {

	if (buff==0) return 1;

	char* _offset = buff;
	section_t tmp_stn;
	rw_t rw;
	
	do {
		
		memcpy(&rw, _offset, sizeof(rw) ); _offset+=sizeof(rw);

		tmp_stn.dataOffset  = _offset;
		tmp_stn.rw			= rw;
		
		/* for testin structures
		if (rw.sID == 0x0120) { 
			printf(">>>  %s \n", file_name); 
			printRW(rw); } */

	    if (( getSectionID(rw) & (CLUMPF|FRAMELISTF|EXTENSIONF|GEOMETRYLISTF|
							      ATOMICF|GEOMETRYF|MATERIALLISTF|MATERIALF|TEXTUREF) )) 
		{
			node.push_back(tmp_stn);
		}
		else 
		{
			node.push_back(tmp_stn);		
			_offset+=rw.size;
		}

	} while (rw.rwID==0x1803FFFF);
	
	// Delete last garbage element
	node.pop_back(); 

	return 0;
}

DffReader::DffReader(char* file_name) {
	FILE *f;

	f_buffer=NULL;
		
		if (file_name==NULL) goto ERR_FILEOPEN;	

	file_name = new char[255];
	strcpy(file_name, file_name);
	
	f = fopen(file_name, "rb");
		if (f==NULL) goto ERR_FILEOPEN;
	
	// Get file size
	fseek(f,0, SEEK_END);
	
	long buff_size = ftell(f);

		if (buff_size==0) goto ERR_READFILESIZE;
	fseek(f,0, SEEK_SET);

	// Allocate memory for buffer
	f_buffer = new char[buff_size];
		if (f_buffer == NULL) goto ERR_MEMORYALLOCATE;
		if (!fread(f_buffer,1,buff_size, f)) goto ERR_READ2BUFFER;
		fclose(f);
	
		if ( parse_fileRW(f_buffer) ) goto ERR_PARSERFILERW;

		parse_frame();

	return;
	// TODO: Set success flag

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

DffReader::~DffReader() {
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