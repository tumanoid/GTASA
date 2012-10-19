#ifndef MESH_H
#define MESH_H

#include "common.h"
#include "gta_struct.h"
#include "Material.h"

class Mesh {
	private:
		Mesh(){}; // Default constructor
	public:
		Mesh(char* const, DWORD size); // Init constructor
		void addMaterial(Material&);
		void setFaceIndexPLG(char* const, DWORD);
		
	public:
		geometry_t	mesh_info;
		float*		vertex;
		float*		uv;
		float*		uv2;
		float*		normal;
		
		vector<Material>  mat;

		DWORD*		facePLG;
		WORD*		face;
		DWORD*		color;

		float pX, pY, pZ, Radius;
};

void Mesh::addMaterial(Material& m){
	
	mat.push_back(m);
}

void Mesh::setFaceIndexPLG(char* const buff, DWORD size){
	
	facePLG = (DWORD*) buff;
	mesh_info.PLG_size = size>>2; //DWORD
}

Mesh::Mesh(char* const buff, DWORD size) {
	
	if (buff==NULL) return; // TODO: set success flag
	
	vertex=NULL; uv=NULL; uv2=NULL; 
	normal=NULL; face=NULL; facePLG=NULL; color=NULL;
	pX=0; pY=0; pZ=0; Radius=0;

    char* _offset = buff;

	memcpy(&mesh_info, _offset, sizeof(mesh_info)); 
	_offset+=( sizeof(mesh_info) - 4); //hack "PLGsize" field
	
	/////////////////////////////////////////////////
	// Parse mesh info

	// VertexColor list
	if (mesh_info.flag & 8) {
		//TODO: VertexColor parser
		_offset += sizeof(DWORD) * mesh_info.c_vertex;
	}

	// UV list
	if (mesh_info.flag & 128)
	{
		//first UV pair	
		uv = (float*) _offset;
		_offset += sizeof(float) * mesh_info.c_vertex * 2; 
		//second UV pair
		uv2 = (float*) _offset;
		_offset += sizeof(float) * mesh_info.c_vertex * 2; 
		
	   	//if ( !(mesh_info.flag & 4) ) uv;//=uv2;

	} else {
		if (mesh_info.flag & 4) {
		//first UV pair	
		uv = (float*) _offset;
		_offset += sizeof(float) * mesh_info.c_vertex * 2;
		}
	}
	
	//if (mesh_info.n_uv==2) uv = uv2;

	// Index list.
	face = (WORD*) _offset;
	_offset += sizeof(WORD) * mesh_info.c_face * 4; // 4 = index + index + index + index

	// Bound information
	memcpy(&pX, _offset, sizeof(float) ); _offset += sizeof(float); 
	memcpy(&pY, _offset, sizeof(float) ); _offset += sizeof(float); 
	memcpy(&pZ, _offset, sizeof(float) ); _offset += sizeof(float); 
	memcpy(&Radius, _offset, sizeof(float) ); _offset += sizeof(float); 
	// Skip "Has position/Normals"
	_offset += sizeof(DWORD)*2;
	
	//NOTE: Vertex and Normal offset calculate from end of geometry section
	
	// Vertex list
	//vertex = (float*) _offset;
	//_offset += sizeof(float) * mesh_info.c_vertex * 3; // 3 - XYZ coord.
	if (mesh_info.flag & 16) {
		vertex = (float*) (buff+size-( 2 * sizeof(float) * mesh_info.c_vertex * 3) );
	} else
		vertex = (float*) (buff+size-( 1 * sizeof(float) * mesh_info.c_vertex * 3) );

	// Normal
	//	normal = (float*) _offset;
	//	_offset+= sizeof(float) * mesh_info.c_vertex * 3; // 3 - XYZ coord.
	if (mesh_info.flag & 16) {
		normal = (float*) (buff+size-( 1 * sizeof(float) * mesh_info.c_vertex * 3) );
	}
}
#endif //MESH_H