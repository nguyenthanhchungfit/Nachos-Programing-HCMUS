// exception.cc 
/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM			/
// 	1512034 Nguyen Dang Binh		/
// 	1512042 Nguyen Thanh Chung		/
// 	1512123 Hoang Ngoc Duc			/
/////////////////////////////////////////////////




//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.



#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32 // Do dai quy uoc cho file name

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	Cac quy dinh su dung thanh ghi:
//
// 	   1. Thanh ghi r2: Chua ma lenh syscall va ket qua thuc hien o syscall se tra nguoc ve
// 	   2. Thanh ghi r4: Tham so thu nhat
//	   3. Thanh ghi r5: Tham so thu hai
//	   4. Thanh ghi r6: Tham so thu ba	   
//	   5. Thanh ghi r7: Tham so thu tu
//
// 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// Doi thanh ghi Program counter cua he thong ve sau 4 byte de tiep tuc nap lenh
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
   	machine->WriteRegister(PrevPCReg, counter);
    	counter = machine->ReadRegister(NextPCReg);
    	machine->WriteRegister(PCReg, counter);
   	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAddr, int limit)
{
	int i; //chi so index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}


// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}


// Ham xu ly ngoai le runtime Exception va system call
void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

	// Bien toan cuc cho lop SynchConsole
	
	
	//Bat dau
	switch (which) {
	case NoException:
		return;

	case PageFaultException:
		DEBUG('a', "\n No valid translation found");
		printf("\n\n No valid translation found");
		interrupt->Halt();
		break;

	case ReadOnlyException:
		DEBUG('a', "\n Write attempted to page marked read-only");
		printf("\n\n Write attempted to page marked read-only");
		interrupt->Halt();
		break;

	case BusErrorException:
		DEBUG('a', "\n Translation resulted invalid physical address");
		printf("\n\n Translation resulted invalid physical address");
		interrupt->Halt();
		break;

	case AddressErrorException:
		DEBUG('a', "\n Unaligned reference or one that was beyond the end of the address space");
		printf("\n\n Unaligned reference or one that was beyond the end of the address space");
		interrupt->Halt();
		break;

	case OverflowException:
		DEBUG('a', "\nInteger overflow in add or sub.");
		printf("\n\n Integer overflow in add or sub.");
		interrupt->Halt();
		break;

	case IllegalInstrException:
		DEBUG('a', "\n Unimplemented or reserved instr.");
		printf("\n\n Unimplemented or reserved instr.");
		interrupt->Halt();
		break;

	case NumExceptionTypes:
		DEBUG('a', "\n Number exception types");
		printf("\n\n Number exception types");
		interrupt->Halt();
		break;

	case SyscallException:
		switch (type){

		case SC_Halt:
			// Input: Khong co
			// Output: Thong bao tat may
			// Chuc nang: Tat HDH
			DEBUG('a', "\nShutdown, initiated by user program. ");
			printf("\nShutdown, initiated by user program. ");
			interrupt->Halt();
			return;

		case SC_ReadInt:
		{
		// Input: K co
                    // Output: Tra ve so nguyen doc duoc tu man hinh console.
                    // Chuc nang: Doc so nguyen tu man hinh console.
                    char* buffer;
                    int MAX_BUFFER = 255;
                    buffer = new char[MAX_BUFFER + 1];
                    int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);// doc buffer toi da MAX_BUFFER ki tu, tra ve so ki tu doc dc
                    int number = 0; // so luu ket qua tra ve cuoi cung
						
                    /* Qua trinh chuyen doi tu buffer sang so nguyen int */
			
                    // Xac dinh so am hay so duong                       
                    bool isNegative = false; // Gia thiet la so duong.
                    int firstNumIndex = 0;
                    int lastNumIndex = 0;
                    if(buffer[0] == '-')
                    {
                        isNegative = true;
                        firstNumIndex = 1;
                        lastNumIndex = 1;                        			   		
                    }
                    
                    // Kiem tra tinh hop le cua so nguyen buffer
                    for(int i = firstNumIndex; i < numbytes; i++)					
                    {
                        if(buffer[i] == '.') /// 125.0000000 van la so
                        {
                            int j = i + 1;
                            for(; j < numbytes; j++)
                            {
				// So khong hop le
                                if(buffer[j] != '0')
                                {
                                    printf("\n\n The integer number is not valid");
                                    DEBUG('a', "\n The integer number is not valid");
                                    machine->WriteRegister(2, 0);
                                    IncreasePC();
                                    delete buffer;
                                    return;
                                }
                            }
                            // la so thoa cap nhat lastNumIndex
                            lastNumIndex = i - 1;				
                            break;                           
                        }
                        else if(buffer[i] < '0' && buffer[i] > '9')
                        {
                            printf("\n\n The integer number is not valid");
                            DEBUG('a', "\n The integer number is not valid");
                            machine->WriteRegister(2, 0);
                            IncreasePC();
                            delete buffer;
                            return;
                        }
                        lastNumIndex = i;    
                    }			
                    
                    // La so nguyen hop le, tien hanh chuyen chuoi ve so nguyen
                    for(int i = firstNumIndex; i<= lastNumIndex; i++)
                    {
                        number = number * 10 + (int)(buffer[i] - 48); 
                    }
                    
                    // neu la so am thi * -1;
                    if(isNegative)
                    {
                        number = number * -1;
                    }
                    machine->WriteRegister(2, number);
                    IncreasePC();
                    delete buffer;
                    return;		
		}

		case SC_PrintInt:
		{	
		    // Input: mot so integer
                    // Output: khong co 
                    // Chuc nang: In so nguyen len man hinh console
                    int number = machine->ReadRegister(4);
		    if(number == 0)
                    {
                        gSynchConsole->Write("0", 1); // In ra man hinh so 0
                        IncreasePC();
                        return;    
                    }
                    
                    /*Qua trinh chuyen so thanh chuoi de in ra man hinh*/
                    bool isNegative = false; // gia su la so duong
                    int numberOfNum = 0; // Bien de luu so chu so cua number
                    int firstNumIndex = 0; 
			
                    if(number < 0)
                    {
                        isNegative = true;
                        number = number * -1; // Nham chuyen so am thanh so duong de tinh so chu so
                        firstNumIndex = 1; 
                    } 	
                    
                    int t_number = number; // bien tam cho number
                    while(t_number)
                    {
                        numberOfNum++;
                        t_number /= 10;
                    }
    
		    // Tao buffer chuoi de in ra man hinh
                    char* buffer;
                    int MAX_BUFFER = 255;
                    buffer = new char[MAX_BUFFER + 1];
                    for(int i = firstNumIndex + numberOfNum - 1; i >= firstNumIndex; i--)
                    {
                        buffer[i] = (char)((number % 10) + 48);
                        number /= 10;
                    }
                    if(isNegative)
                    {
                        buffer[0] = '-';
			buffer[numberOfNum + 1] = 0;
                        gSynchConsole->Write(buffer, numberOfNum + 1);
                        delete buffer;
                        IncreasePC();
                        return;
                    }
		    buffer[numberOfNum] = 0;	
                    gSynchConsole->Write(buffer, numberOfNum);
                    delete buffer;
                    IncreasePC();
                    return;        			
					
		}

		case SC_ReadChar:
		{

			//Input: Khong co
			//Output: Duy nhat 1 ky tu (char)
			//Cong dung: Doc mot ky tu tu nguoi dung nhap
			int maxBytes = 255;
			char* buffer = new char[255];
			int numBytes = gSynchConsole->Read(buffer, maxBytes);

			if(numBytes > 1) //Neu nhap nhieu hon 1 ky tu thi khong hop le
			{
				printf("Chi duoc nhap duy nhat 1 ky tu!");
				DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
				machine->WriteRegister(2, 0);
			}
			else if(numBytes == 0) //Ky tu rong
			{
				printf("Ky tu rong!");
				DEBUG('a', "\nERROR: Ky tu rong!");
				machine->WriteRegister(2, 0);
			}
			else
			{
				//Chuoi vua lay co dung 1 ky tu, lay ky tu o index = 0, return vao thanh ghi R2
				char c = buffer[0];
				machine->WriteRegister(2, c);
			}

			delete buffer;
			//IncreasePC(); // error system
			//return;
			break;
		}

		case SC_PrintChar:
		{
			// Input: Ki tu(char)
			// Output: Ki tu(char)
			// Cong dung: Xuat mot ki tu la tham so arg ra man hinh
			char c = (char)machine->ReadRegister(4); // Doc ki tu tu thanh ghi r4
			gSynchConsole->Write(&c, 1); // In ky tu tu bien c, 1 byte
			//IncreasePC();
			break;

		}

		case SC_ReadString:
		{
			// Input: Buffer(char*), do dai toi da cua chuoi nhap vao(int)
			// Output: Khong co
			// Cong dung: Doc vao mot chuoi voi tham so la buffer va do dai toi da
			int virtAddr, length;
			char* buffer;
			virtAddr = machine->ReadRegister(4); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
			length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
			buffer = User2System(virtAddr, length); // Copy chuoi tu vung nho User Space sang System Space
			gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
			System2User(virtAddr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
			delete buffer; 
			IncreasePC(); // Tang Program Counter 
			return;
			//break;
		}

		case SC_PrintString:
		{
			// Input: Buffer(char*)
			// Output: Chuoi doc duoc tu buffer(char*)
			// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh
			int virtAddr;
			char* buffer;
			virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
			int length = 0;
			while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
			gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
			delete buffer; 
			//IncreasePC(); // Tang Program Counter 
			//return;
			break;
		}

		case SC_CreateFile:
		{
			// Input: Dia chi tu vung nho user cua ten file
			// Output: -1 = Loi, 0 = Thanh cong
			// Chuc nang: Tao ra file voi tham so la ten file
			int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call ...");
			DEBUG('a', "\n Reading virtual address of filename");

			virtAddr = machine->ReadRegister(4); //Doc dia chi cua file tu thanh ghi R4
			DEBUG('a', "\n Reading filename.");
			
			//Sao chep khong gian bo nho User sang System, voi do dang toi da la (32 + 1) bytes
			filename = User2System(virtAddr, MaxFileLength + 1);
			if (strlen(filename) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				//IncreasePC();
				//return;
				break;
			}
			
			if (filename == NULL)  //Neu khong doc duoc
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				delete filename;
				//IncreasePC();
				//return;
				break;
			}
			DEBUG('a', "\n Finish reading filename.");
			
			if (!fileSystem->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
			{
				//Tao file that bai
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				delete filename;
				//IncreasePC();
				//return;
				break;
			}
			
			//Tao file thanh cong
			machine->WriteRegister(2, 0);
			delete filename;
			//IncreasePC(); //Day thanh ghi lui ve sau de tiep tuc ghi
			//return;
			break;
		}

		case SC_Open:
		{
			// Input: arg1: Dia chi cua chuoi name, arg2: type
			// Output: Tra ve OpenFileID neu thanh, -1 neu loi
			// Chuc nang: Tra ve ID cua file.
	 
			//OpenFileID Open(char *name, int type)
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
			int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
			char* filename;
			filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
			//Kiem tra xem OS con mo dc file khong
			
			// update 4/1/2018
			int freeSlot = fileSystem->FindFreeSlot();
			if (freeSlot != -1) //Chi xu li khi con slot trong
			{
				if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
				{
					
					if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
					{
						machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
					}
				}
				else if (type == 2) // xu li stdin voi type quy uoc la 2
				{
					machine->WriteRegister(2, 0); //tra ve OpenFileID
				}
				else // xu li stdout voi type quy uoc la 3
				{
					machine->WriteRegister(2, 1); //tra ve OpenFileID
				}
				delete[] filename;
				break;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file return -1
			
			delete[] filename;
			break;
		}

		case SC_Close:
		{
			//Input id cua file(OpenFileID)
			// Output: 0: thanh cong, -1 that bai
			int fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
			if (fid >= 0 && fid <= 14) //Chi xu li khi fid nam trong [0, 14]
			{
				if (fileSystem->openf[fid]) //neu mo file thanh cong
				{
					delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
					fileSystem->openf[fid] = NULL; //Gan vung nho NULL
					machine->WriteRegister(2, 0);
					break;
				}
			}
			machine->WriteRegister(2, -1);
			break;
		}

		case SC_Read:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte read thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Doc file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the read vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the read vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop doc file stdout (type quy uoc la 3) thi tra ve -1
			{
				printf("\nKhong the read file stdout.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop doc file stdin (type quy uoc la 2)
			if (fileSystem->openf[id]->type == 2)
			{
				// Su dung ham Read cua lop SynchConsole de tra ve so byte thuc su doc duoc
				int size = gSynchConsole->Read(buf, charcount); 
				System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
				machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
				delete buf;
				IncreasePC();
				return;
			}
			// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				// So byte thuc su = NewPos - OldPos
				NewPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
				System2User(virtAddr, NewPos - OldPos, buf); 
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				// Truong hop con lai la doc file co noi dung la NULL tra ve -2
				//printf("\nDoc file rong.");
				machine->WriteRegister(2, -2);
			}
			delete buf;
			IncreasePC();
			return;
		}

		case SC_Write:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte write thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Ghi file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the write vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the write vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Xet truong hop ghi file only read (type quy uoc la 1) hoac file stdin (type quy uoc la 2) thi tra ve -1
			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
			{
				printf("\nKhong the write file stdin hoac file only read.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
				{
					// So byte thuc su = NewPos - OldPos
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					IncreasePC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
				{
					gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole 
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
				machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
				delete buf;
				IncreasePC();
				return;
			}
		}

		case SC_Seek:
		{
			// Input: Vi tri(int), id cua file(OpenFileID)
			// Output: -1: Loi, Vi tri thuc su: Thanh cong
			// Cong dung: Di chuyen con tro den vi tri thich hop trong file voi tham so la vi tri can chuyen va id cua file
			int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
			int id = machine->ReadRegister(5); // Lay id cua file
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 14)
			{
				printf("\nKhong the seek vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the seek vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Kiem tra co goi Seek tren console khong
			if (id == 0 || id == 1)
			{
				printf("\nKhong the seek tren file console.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
			pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
			if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
			{
				printf("\nKhong the seek file den vi tri nay.");
				machine->WriteRegister(2, -1);
			}
			else
			{
				// Neu hop le thi tra ve vi tri di chuyen thuc su trong file
				fileSystem->openf[id]->Seek(pos);
				machine->WriteRegister(2, pos);
			}
			IncreasePC();
			return;
		}
		case SC_Exec:
		{
			// Input: vi tri int
			// Output: Fail return -1, Success: return id cua thread dang chay
			// SpaceId Exec(char *name);
			int virtAddr;
			virtAddr = machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
			char* name;
			name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
	
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			OpenFile *oFile = fileSystem->Open(name);
			if (oFile == NULL)
			{
				printf("\nExec:: Can't open this file.");
				machine->WriteRegister(2,-1);
				IncreasePC();
				return;
			}

			delete oFile;

			// Return child process id
			int id = pTab->ExecUpdate(name); 
			machine->WriteRegister(2,id);

			delete[] name;	
			IncreasePC();
			return;
		}
		case SC_Join:
		{       
			// int Join(SpaceId id)
			// Input: id dia chi cua thread
			// Output: 
			int id = machine->ReadRegister(4);
			
			int res = pTab->JoinUpdate(id);
			
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Exit:
		{
			//void Exit(int status);
			// Input: status code
			int exitStatus = machine->ReadRegister(4);

			if(exitStatus != 0)
			{
				IncreasePC();
				return;
				
			}			
			
			int res = pTab->ExitUpdate(exitStatus);
			//machine->WriteRegister(2, res);

			currentThread->FreeSpace();
			currentThread->Finish();
			IncreasePC();
			return; 
				
		}
		case SC_CreateSemaphore:
		{
			// int CreateSemaphore(char* name, int semval).
			int virtAddr = machine->ReadRegister(4);
			int semval = machine->ReadRegister(5);

			char *name = User2System(virtAddr, MaxFileLength + 1);
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}
			
			int res = semTab->Create(name, semval);

			if(res == -1)
			{
				DEBUG('a', "\n Khong the khoi tao semaphore");
				printf("\n Khong the khoi tao semaphore");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;				
			}
			
			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}

		case SC_Wait:			
		{
			// int Wait(char* name)
			int virtAddr = machine->ReadRegister(4);

			char *name = User2System(virtAddr, MaxFileLength + 1);
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}
			
			int res = semTab->Wait(name);

			if(res == -1)
			{
				DEBUG('a', "\n Khong ton tai ten semaphore nay!");
				printf("\n Khong ton tai ten semaphore nay!");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;				
			}
			
			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Signal:		
		{
			// int Signal(char* name)
			int virtAddr = machine->ReadRegister(4);

			char *name = User2System(virtAddr, MaxFileLength + 1);
			if(name == NULL)
			{
				DEBUG('a', "\n Not enough memory in System");
				printf("\n Not enough memory in System");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;
			}
			
			int res = semTab->Signal(name);

			if(res == -1)
			{
				DEBUG('a', "\n Khong ton tai ten semaphore nay!");
				printf("\n Khong ton tai ten semaphore nay!");
				machine->WriteRegister(2, -1);
				delete[] name;
				IncreasePC();
				return;				
			}
			
			delete[] name;
			machine->WriteRegister(2, res);
			IncreasePC();
			return;
		}
		case SC_Sum:
		{
			// int Sum(int a, int b)
			int a = machine->ReadRegister(4);
			int b = machine->ReadRegister(5);
			int sum = a + b;
			machine->WriteRegister(2, sum);
			IncreasePC();
			return;
		}
		default:
			break;
		}
		IncreasePC();
	}
}

