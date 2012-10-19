#ifndef MATERIAL_H
#define MATERIAL_H
#include "common.h"
#include "gta_struct.h"


class Material
{
private:
	Material(){};
public:
	Material(char* const, char* const , char* const);
	~Material(void){};
public:
	string diff_name;
	DWORD  num_texture;
	mat_trace_t glossy;
};


Material::Material(char* const buff,char* const specular,char* const reflect) {
	rw_t rw;
	char* offset = buff;
		if (!offset) return;
		
	float tmp_level;
	memset(&glossy,0,sizeof(glossy));

	if (specular) {
	  memcpy(&tmp_level, specular, sizeof(tmp_level));
	  glossy.flag = 0x1;
	  glossy.spec_amount = tmp_level;
	}
	
	if (reflect) {
	  memcpy(&tmp_level, reflect, sizeof(tmp_level));
	  glossy.r = tmp_level;
	  
	  memcpy(&tmp_level, reflect+4, sizeof(tmp_level));
	  glossy.g = tmp_level;
	  
	  memcpy(&tmp_level, reflect+8, sizeof(tmp_level));
	  glossy.b = tmp_level;
	  
	  memcpy(&tmp_level, reflect+16, sizeof(tmp_level));
	  glossy.ref_amount = tmp_level;

	  glossy.flag = glossy.flag | 0x2;  
	}

	diff_name = "NODIFFUSE";

	offset+=sizeof(DWORD); // skip unkwn
	
	DWORD  _rgba;
	memcpy(&_rgba, offset, sizeof(_rgba)); offset+=sizeof(_rgba);
	glossy.diffuse_color = _rgba;

	offset+=sizeof(DWORD); // skip unkwn
	
	memcpy(&num_texture, offset, sizeof(num_texture)); offset+=sizeof(num_texture);
	if (num_texture==0) return;

    offset+=3*sizeof(DWORD); // skip unkwn
	offset+=sizeof(rw_t); // skip Texture header
	offset+=sizeof(rw_t); // skip Flag header
	offset+=sizeof(DWORD); // skip flag data
	memcpy(&rw, offset, sizeof(rw)); offset+=sizeof(rw); // read String header

	// read diffuse name
	char _buff[32]; 
	memcpy(_buff, offset, rw.size ); offset+=rw.size;
	diff_name = string(_buff);
#if 0
	FILE* f = fopen("log.txt","a");
	fprintf(f,"-- %s  \n",diff_name.data());
	fprintf(f,">> %X  %X\n",specular, reflect); 
	fprintf(f,">>FLAG %X \n",glossy.flag ); 
	fprintf(f,">> spec= %f ref= %f\n", glossy.spec_amount, glossy.ref_amount); 
	fprintf(f,">>---------------------\n"); 
	fclose(f);
#endif
}
#endif //MATERIAL_H