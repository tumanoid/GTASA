#ifndef DFF_READER_H
#define DFF_READER_H

#include "common.h"
#include "gta_struct.h"
#include "Mesh.h"
#include "Material.h"

typedef struct {
  rw_t	rw;
  char*	data_offset; // offset in main buffer
} section_t;


class DffReader {
  private:
    DffReader(const DffReader&){};

  public:
	 DffReader() : f_buffer(NULL) {};
	 DffReader(char* name) : f_buffer(NULL) {ReadFile(name);};

	 DWORD ReadFile(char* file_name);
	 void CloseFile();
    ~DffReader();	

  public: 	
    char* f_buffer;  // main file buffer in memory
	 
    DWORD clump_count;
    DWORD frame_count;
    DWORD geometry_count;
	DWORD atomic_count;
	 
    int ParseFileRW(char *const);
    int ParseFrame();
    int ParseGeometryList();
	int ParseMaterial();
	int ParseAtomic();

    vector<section_t>   node; // RW headers array
    vector<frame_t>     frame;
    vector<string>      frame_name;
	vector<atomic_t>	atomic;
	vector<Mesh>		mesh;
};

// utilites prototypes
void PrintRW( rw_t& );
DWORD GetSectionID(rw_t rw);


int DffReader::ParseMaterial() {
	char* offset;
	DWORD _num;
	vector<DWORD> mat_count;
	vector<Material> mat_list;

	// get count materials for each geometry
	for (DWORD i=0; i<node.size(); ++i) {
	  if (node[i].rw.sID==MATERIALLIST) {
      offset = node[i+1].data_offset;

        if (offset==NULL) return 1;
      memcpy(&_num, offset, sizeof(_num));
      mat_count.push_back(_num);
    }
  }
	// EQ test. If not equal we have error
	if (mat_count.size()!=mesh.size()) {
		printf("Mesh != MaterialList\n"); 
		return 1;
	}

	// get material array
	for (DWORD i=0; i<node.size(); ++i) {
     if (node[i].rw.sID==MATERIAL) {
      char* _off_text = node[i+1].data_offset;
	  char* _off_refl = NULL;
	  char* _off_spec = NULL;
	   
	  
	  for (int k=1; ;k++) {
	  
		if (node[i+k].rw.sID==0x50e) break;
		if (node[i+k].rw.sID==0x07) break;

		if (node[i+k].rw.sID==0x0253F2FC) _off_refl = node[i+k].data_offset;
		if (node[i+k].rw.sID==0x0253F2F6) _off_spec = node[i+k].data_offset;
	
	  }
        
	  mat_list.push_back( Material(_off_text, _off_spec, _off_refl) );
    }
  }

	// assign materials to mesh
	DWORD it=0;
	//for (DWORD i=0; i<mat_count.size();++i) {
	for (DWORD i=0; i<mesh.size();++i) {
		for (DWORD j=0; j<mat_count[i]; ++j) {
			mesh[i].addMaterial(mat_list[it+j]);
		}
		it+=mat_count[i];
	}

	return 0;
}


int DffReader::ParseAtomic() {
  char* _offset = NULL;
  atomic_t	_atom;
  
  // get geometry count
  for (DWORD i=0; i<node.size(); ++i) {
    if (node[i].rw.sID==ATOMIC) {
      _offset = node[i+1].data_offset;

        if (_offset==NULL) return 1;
      memcpy(&_atom, _offset, sizeof(_atom));
      atomic.push_back(_atom);
    }
  }
  atomic_count = atomic.size();
  return 0;
}


int DffReader::ParseGeometryList() {
  char* _offset = NULL;

  // get geometry count
  for (DWORD i=0; i<node.size(); ++i){
    if (node[i].rw.sID==GEOMETRYLIST) {
	  _offset = node[i+1].data_offset; 
	  break;
    }
  }
	if (_offset==NULL) return 1;
  memcpy(&geometry_count, _offset, sizeof(geometry_count)); _offset+=sizeof(geometry_count);
  
  // create geometry vector
  for (DWORD i=0; i<node.size(); ++i) {	
	if (node[i].rw.sID==GEOMETRY) {
		mesh.push_back(Mesh( node[i+1].data_offset, node[i+1].rw.size ));
	}
  }

  // add face index information from PLG section
  DWORD j=0;
  for (DWORD i=0; i<node.size(); ++i){
    if (node[i].rw.sID==BINMESHPLG) {
		mesh[j].setFaceIndexPLG( node[i].data_offset, node[i].rw.size );
		++j;
    }
  }
  return 0;
} 


int DffReader::ParseFrame() {
	char* _offset = NULL;
	
	// get frame count
	for (DWORD i=0; i<node.size(); i++){
		if (node[i].rw.sID==FRAMELIST) {
			_offset = node[i+1].data_offset; 
			break;
		}
	}
		if (_offset==NULL) return 1;
	memcpy(&frame_count, _offset, sizeof(frame_count) ); _offset+=sizeof(frame_count);
	
	// get frame_t vector
	frame_t _frame;
	for (DWORD i=0; i<frame_count; i++) {	
		memcpy(&_frame, _offset, sizeof(_frame) ); _offset+=sizeof(_frame);
		frame.push_back(_frame);
	}
	// get name of frame
	for (DWORD i=0; i<node.size(); i++){
      if (node[i].rw.sID==NODENAME) {
	    _offset = node[i].data_offset;
		DWORD size = node[i].rw.size;
			
		char* name = new char[size+1]; //+1 for null terminate
		  if (name==NULL) return 1;
			
		memcpy(name, _offset, size);
		name[size]='\0';
		frame_name.push_back( string (name) );
		delete [] name;
	  }
	}

	//if (clump_count>1) frame_name.push_back( string("terminator") );

	if ( frame_count!=frame_name.size() )
		while (frame_count!=frame_name.size() ) frame_name.push_back("noname");

	return 0;
}


int DffReader::ParseFileRW(char *const buff) {
	char* _offset = buff;
	section_t tmp_stn;
	rw_t rw;

	clump_count = 0;
	if (buff==0) return 1;

	do {
		memcpy(&rw, _offset, sizeof(rw) ); _offset+=sizeof(rw);
		tmp_stn.data_offset  = _offset;
		tmp_stn.rw			 = rw;
		// only one clump section
		if (rw.sID==CLUMP) clump_count++;
			if (clump_count>1) return 0;

	    if (( GetSectionID(rw) & (CLUMPF|FRAMELISTF|EXTENSIONF|GEOMETRYLISTF|
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

	// delete last garbage element
	node.pop_back(); 
	return 0;
}

int LogPrint(char* st, int numErr){
		FILE* f=fopen("parse.log","a");
		if (numErr==0) fprintf(f,"%s\n",st);
		else fprintf(f,"%s  :NUMERR %i\n",st, numErr);
		fclose(f);

		return numErr;
}

void DffReader::CloseFile() {
	
		delete [] f_buffer; 
	    
		clump_count = 0;
		frame_count = 0;
		geometry_count = 0;
		atomic_count = 0;
		
		node.clear();
		frame.clear();
		frame_name.clear();
		atomic.clear();
		mesh.clear();

}

DWORD DffReader::ReadFile(char* f_name) {
	FILE *f;

		clump_count = 0;
		frame_count = 0;
		geometry_count = 0;
		atomic_count = 0;
		
		node.clear();
		frame.clear();
		frame_name.clear();
		atomic.clear();
		mesh.clear();
		
		LogPrint(f_name,0);

	if (f_name==NULL) return LogPrint("Emty file name",1);;	

	f = fopen(f_name, "rb");
		if (f==NULL) return LogPrint("File not found",2);; 
	
	// get file size
	fseek(f,0, SEEK_END);
	long buff_size = ftell(f);
		
	fseek(f,0, SEEK_SET);

	// allocate memory and read to buffer
	f_buffer = new char[buff_size];
	
	size_t tmp = fread(f_buffer,1,buff_size, f);
	
	fclose(f); // BUG
	//_fcloseall(); // ???

	if (buff_size==0) return LogPrint("Zero buff size",3);;
	if (tmp==0) return LogPrint("Zero byte read",5);;
	

	// parse elements
		if ( ParseFileRW(f_buffer) ) return LogPrint("ParseFileRW",20);
			//LogPrint("ParseFileRW\n");
		if ( ParseFrame() ) return LogPrint("ParseFrame",21);
			//LogPrint("ParseFrame\n");
		if ( ParseGeometryList() ) return LogPrint("ParseGeometryList",22);
			//LogPrint("ParseGeometryList\n");
		if ( ParseMaterial() ) return LogPrint("ParseMaterial",23);
			//LogPrint("ParseMaterial\n");
		if ( ParseAtomic() ) return LogPrint("ParseAtomic",24);
			//LogPrint("ParseAtomic\n");

#if 0		
		FILE* fl=fopen("parse.log","a");
		fprintf(fl,"Node-%i Frame-%i f_name-%i Atom-%i Mesh-%i\n", node.size(), frame.size(), frame_name.size(), atomic.size(), mesh.size() );
		fclose(fl);
#endif

	return 0;
}


DffReader::~DffReader() {
	if (f_buffer!=NULL) delete [] f_buffer;
}


//////////////////////////
// utilites
//////////////////////////
void printRW( rw_t& hdr) {
	printf("RW :\n");
	printf(" rwID %X\n", hdr.sID);
	printf(" size %i\n", hdr.size);
	if (hdr.rwID==0x1803FFFF) printf("GTA:SA\n");
		else printf("Unknown game %X\n", hdr.rwID);
	printf("\n");
}


DWORD GetSectionID(rw_t rw){
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
#endif//DFF_READER_H