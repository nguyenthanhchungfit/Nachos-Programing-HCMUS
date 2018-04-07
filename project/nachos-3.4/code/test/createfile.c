#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	int stdin;
	char fileName[MAX_LENGTH];
	PrintString("\n\t\t\t-----TAO FILE-----\n\n");
	PrintString(" - Nhap ten file tu stdin (Open(stdin)): ");
	
	stdin = Open("stdin", 2); // Goi ham Open mo file stdin nhap vao ten file
	if (stdin != -1)
	{
		int len = Read(fileName, MAX_LENGTH, stdin); // Goi ham Read doc ten file vua nhap
		
		if(len <= 1)
			PrintString(" -> Ten file khong hop le!!!\n\n");
		else
		{
			if (CreateFile(fileName) == 0) // Goi ham CreateFile
			{
				PrintString(" -> Create file thanh cong.\n\n");
			}
		}
		Close(stdin); // Goi ham Close de dong stdin
	}
	
	//Test CreateFile bang ReadString truoc, sau do thanh cong thi dung stdin doc Console
	
	/*ReadString(fileName, MAX_LENGTH);
	
	if(strlen(fileName)== 0 || strlen(fileName)== 1)
		fileName = "text.txt";
	
	if (CreateFile(fileName) == 0) // Goi ham CreateFile de tao file. Tra ve 0 neu thanh cong
	{
		PrintString("Create file thanh cong.");
	}
	else
	{
		PrintString("Loi khi tao file.");
	}*/
	
	return 0;
}
