#include "common.h"
#include "img_reader.h"
#include "dff_reader.h"


extern "C" __declspec(dllexport) int foo(void*, long);

int foo(void* a, long b){
	return 0;
}

/*
int main( int argc, char* argv[]) {

	DffReader dd(argv[1]);
	
	
	vector<int> a[5];
	for (int j=0; j<5; j++)
		for (int i=0; i<100; i++)
			a[j].push_back(i);

	for (int j=0; j<5; j++)
		for (int i=0; i<100; i++)
			printf("%i | ", a[j][i]);

	//printf("\n\npress 'e' to Exit \n");
	//while (_getch()!='e');

	return 0;

}
*/