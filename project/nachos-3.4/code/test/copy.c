#include "syscall.h"
#define MAX_LENGTH 32

int main()
{
	//OpenFileId cua file nguon va file dich
	int srcFileId;
	int destFileId;
	//Do dai file
	int fileSize;
	int i; //Index for loop
	char c; //Ky tu de in ra
	char source[MAX_LENGTH];
	char dest[MAX_LENGTH];
	
	PrintString("\n\t\t\t-----SAO CHEP FILE-----\n\n");
	PrintString(" - Nhap ten file nguon: ");
	ReadString(source, MAX_LENGTH); // Goi ham ReadString de doc vao ten file nguon
	
	PrintString(" - Nhap ten file dich: ");
	ReadString(dest, MAX_LENGTH); // Goi ham ReadString de doc vao ten file dich
	srcFileId = Open(source, 1); // Goi ham Open de mo file nguon
	
	if (srcFileId != -1) //Kiem tra mo file thanh cong
	{
		//Tao file moi voi ten la chuoi luu trong "dest"
		destFileId = CreateFile(dest);
		Close(destFileId);
		
		destFileId = Open(dest, 0); // Goi ham Open de mo file dich
		if (destFileId != -1) //Kiem tra mo file thanh cong
		{
			// Seek den cuoi file nguon de lay duoc do dai noi dung file nguon (fileSize)
			fileSize = Seek(-1, srcFileId);
			
			//Chuan bi sao chep
			Seek(0, srcFileId); // Seek den dau file nguon
			Seek(0, destFileId); // Seek den dau file dich
			i = 0;
			
			// Vong lap chay tu dau file nguon den het file nguon
			for (; i < fileSize; i++) 
			{
				Read(&c, 1, srcFileId); //Doc tung ki tu cua file nguon
				Write(&c, 1, destFileId); //Ghi vao file dich
			}
			
			PrintString(" -> Copy thanh cong.\n\n");
			Close(destFileId); // Goi ham Close de dong file dich
		}
		else
		{
			PrintString(" -> Tao file dich khong thanh cong!!!\n\n");
		}
		
		Close(srcFileId); // Goi ham Close de dong file nguon
	}
	else
	{
		PrintString("Loi khi mo file");
	}
	
	return 0;
}

