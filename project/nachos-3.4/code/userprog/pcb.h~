// pcb.h 
//  Process Control Block
//	Save all information to control the process
// All rights reserved.
/////////////////////////////////////////////////
// 	DH KHTN - DHQG TPHCM			/
// 	1512034 Nguyen Dang Binh		/
// 	1512042 Nguyen Thanh Chung		/
// 	1512123 Hoang Ngoc Duc			/
/////////////////////////////////////////////////

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block
class PCB
{
private:
    Semaphore* joinsem;         // semaphore cho quá trình join
    Semaphore* exitsem;         // semaphore cho quá trình exit
    Semaphore* multex;          // semaphore cho quá trình truy xuất đọc quyền  

    int exitcode;		
    int numwait;                // số tiến trình đã join

    char FileName[32];          // Ten cua tien trinh

    Thread* thread;             // Tien trinh cua chuong trinh
public:
    int parentID;               // ID cua tien trinh cha
    
    char boolBG;                // Kiem tra neu la tien trinh nen
    
    PCB(int = 0);               // Contructor
    ~PCB();                     // Destructor

    int Exec(char*,int);        // Tao mot thread moi
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();           // Trả về số lượng tiến trình chờ


    void JoinWait();            // 1. Tiến trình cha đợi tiến trình con kết thúc
                        
    void ExitWait();             // 4. Tiến trình con kết thúc

    void JoinRelease();         // 2. Báo cho tiến trình cha thực thi tiếp
    void ExitRelease();         // 3. Cho phép tiến trình con kết thúc

    void IncNumWait();          // Tăng số tiến trình chờ
    void DecNumWait();          // Giảm số tiến trình chờ

    void SetExitCode(int);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char*);    // Set ten tien trinh
    char* GetFileName();        // Tra ve ten tien trinh

};

#endif // PCB_H
