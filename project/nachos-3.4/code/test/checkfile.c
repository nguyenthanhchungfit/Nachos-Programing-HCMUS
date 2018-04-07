#include "syscall.h"

int main()
{
	SpaceId id_file;
	int f_Success;
	

	f_Success = CreateFile("mydata.txt");
	if(f_Success == -1)
		return 1;
	
	id_file = Open("mydata.txt", 0);

	Write("chung", 5, id_file);
	
	Close(id_file);

	f_Success = CreateFile("mydata.txt");
	if(f_Success == -1)
		return 1;
}
