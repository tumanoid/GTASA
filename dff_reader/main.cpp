#include "common.h"
#include "dff_reader.h"

typedef DWORD ERR_STAT;

static DffReader dd;

extern "C" __declspec(dllexport) ERR_STAT OpenDFF(char*);
extern "C" __declspec(dllexport) ERR_STAT CloseDFF();
extern "C" __declspec(dllexport) DWORD getCountFrame(void);
extern "C" __declspec(dllexport) ERR_STAT getFrameList(frame_t*, char* );
extern "C" __declspec(dllexport) DWORD getCountMesh(void);
extern "C" __declspec(dllexport) ERR_STAT getMeshList(char* ,geometry_t* );
extern "C" __declspec(dllexport) ERR_STAT getMesh(DWORD , float*, float*, float*, DWORD*);
extern "C" __declspec(dllexport) DWORD getCountAtomic(void);
extern "C" __declspec(dllexport) ERR_STAT getAtomicList(atomic_t* );
extern "C" __declspec(dllexport) DWORD getCountMaterial(DWORD);
extern "C" __declspec(dllexport) DWORD getMaterial(DWORD num,char*,mat_trace_t*);


ERR_STAT OpenDFF(char* name) {

  return dd.ReadFile(name);	

}


ERR_STAT CloseDFF() {
  dd.CloseFile();
  return 0;
}


DWORD getMaterial(DWORD num,char* mat_name, mat_trace_t* glossy) { // TODO: export color array
  string _mName;

  if (num>dd.mesh.size()) return 9999;

  for (DWORD i=0; i<dd.mesh[num].mat.size() ; ++i) {
	  string str = dd.mesh[num].mat[i].diff_name;
     _mName += str;
     _mName += '+';
	 glossy[i] = dd.mesh[num].mat[i].glossy;
	}
  memcpy(mat_name,_mName.data(),_mName.size());
  return dd.mesh[num].mat.size(); // return count of materials for this geometry
}


DWORD getCountMaterial(DWORD num) {
  
	return dd.mesh[num].mat.size();
}

ERR_STAT getAtomicList(atomic_t* at) {
  
  for (DWORD i=0; i<dd.atomic_count; ++i)
    at[i] = dd.atomic[i];
  return 0;
}


DWORD getCountAtomic(void) { 
  
  return dd.atomic_count;
}


ERR_STAT getMesh(DWORD num, float* vertex,float* normal, float* uv, DWORD* facePLG ) {
  
  if (num>dd.geometry_count) return 1;
	
  DWORD c_vertex  = dd.mesh[num].mesh_info.c_vertex;
  DWORD c_face    = dd.mesh[num].mesh_info.c_face;

  if (!vertex) return 1;
  for (DWORD i=0; i<c_vertex*3; ++i)
	vertex[i]=dd.mesh[num].vertex[i];
  
  // is normal  
  if ( dd.mesh[num].mesh_info.flag&16 )	{
	if (!normal) return 1;
	for (DWORD i=0; i<c_vertex*3; ++i)
	 normal[i]=dd.mesh[num].normal[i];
  }
	
  // UV may be empty
  if ( (dd.mesh[num].mesh_info.flag&4) || (dd.mesh[num].mesh_info.flag&128) ) { 
    for (DWORD i=0; i<c_vertex*2; ++i) 
    uv[i]=dd.mesh[num].uv[i];
  }

  // PLG face
  if (!facePLG) return 1;
  for (DWORD i=0; i<dd.mesh[num].mesh_info.PLG_size; ++i)
	facePLG[i]=dd.mesh[num].facePLG[i];

/*
  Bound info
  bnd.pX = dd.mesh[num].pX;
  bnd.pY = dd.mesh[num].pY;
  bnd.pZ = dd.mesh[num].pZ;
  bnd.radius = dd.mesh[num].Radius;
*/
  return 0;
}


ERR_STAT getMeshList(char* name, geometry_t* mesh_list) {
  
  for (DWORD i=0; i<dd.geometry_count; ++i) 
    mesh_list[i] = dd.mesh[i].mesh_info;
	
  return 0;
}


DWORD getCountMesh(void) { 
  
	return dd.mesh.size();
}


ERR_STAT getFrameList(frame_t* out_frame, char* frame_name) {
  
  for (DWORD i=0; i<dd.frame_count; ++i)
    out_frame[i]=dd.frame[i];

  string _fName;
  for (DWORD i=0; i<dd.frame_count; ++i) {
     string str(dd.frame_name[i]);
     _fName += str;
     _fName += '+';
  }

  memcpy(frame_name,_fName.data(),_fName.size());
  return 0;
}


DWORD getCountFrame(void) { 
  
	return dd.frame.size();
}

/*
int main( int argc, char* argv[]) {

	DffReader dd(argv[1]);
	
	
	//printf("\n\npress 'e' to Exit \n");
	//while (_getch()!='e');

	return 0;

}
*/
