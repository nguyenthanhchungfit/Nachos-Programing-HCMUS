// ptable.h 
//      Control process running include PCB table with MAX (10) element.
//      Constructor of PTable class will create parent process on 0 position.
//      From parent process, we will create other process by calling Exec().
// All rights reserved.

/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM			/
// 	1512034 Nguyen Dang Binh		/
// 	1512042 Nguyen Thanh Chung		/
// 	1512123 Hoang Ngoc Duc			/
/////////////////////////////////////////////////

#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable
{
private:
	int psize;
	BitMap *bm;                 // đánh dấu các vị trí đã được sử dụng trong pcb
	PCB* pcb[MAX_PROCESS];

	Semaphore* bmsem;           // dùng để ngăn chặn trường hợp nạp 2 tiến trình cùng

public:
     PTable(int = 10);           // Khoi tao size doi tuong pcb
                                // de luu size process.
                                // Gan gia tri ban dau la null.
    ~PTable();                  // Huy cac doi tuong da tao
		
    int ExecUpdate(char*);      // Xử lý cho system call SC_Exit
    int ExitUpdate(int);        // Xử lý cho system call SC_Exit
    int JoinUpdate(int);        // Xử lý cho system call SC_Join

    int GetFreeSlot();          // tìm free slot để lưu thông tin cho tiến trình mới
    bool IsExist(int pid);      // kiểm tra tồn tại processID này không?
    
    void Remove(int pid);       // khi tiến trình kết thúc, delete processID ra khỏi mảng quản lý nó

    char* GetFileName(int id);  // Trả về tên của tiến trình
};
#endif // PTABLE_H

