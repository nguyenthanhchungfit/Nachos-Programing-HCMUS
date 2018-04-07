// CAE - MULTI - SYNC CONSOLE CLASS

#include "synchcons.h"
#include "synch.h"

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Semaphore *synchReadAvail;
static Semaphore *synchWriteAvail;
static Semaphore *RLineBlock;
static Semaphore *WLineBlock;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
// 	Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void SynchReadFunct(int arg) { synchReadAvail->V(); }
static void SynchWriteFunct(int arg) { synchWriteAvail->V(); }


//----------------------------------------------------------------------
//   SynchConsole::SynchConsole()
//	Creates a synchronous console device with standard input and output
//----------------------------------------------------------------------

SynchConsole::SynchConsole()
{
	cons = new Console(NULL,NULL,SynchReadFunct,SynchWriteFunct,0);
	synchReadAvail = new Semaphore("Synch Read Avail",0);
	synchWriteAvail = new Semaphore("Synch Write Avail",0);
	RLineBlock = new Semaphore("Read Synch Line Block",1);
	WLineBlock = new Semaphore("Write Synch Line Block",1);
}

//----------------------------------------------------------------------
//   SynchConsole::SynchConsole(char *in, char *out)
//	Creates a synchronous console device with filenames
//----------------------------------------------------------------------

SynchConsole::SynchConsole(char *in, char *out)
{
	cons = new Console(in,out,SynchReadFunct,SynchWriteFunct,0);
	synchReadAvail = new Semaphore("Synch Read Avail",0);
	synchWriteAvail = new Semaphore("Synch Write Avail",0);
	RLineBlock = new Semaphore("Read Synch Line Block",1);
	WLineBlock = new Semaphore("Write Synch Line Block",1);
}

//----------------------------------------------------------------------
//   SynchConsole::~SynchConsole()
//	Delete a console device
//----------------------------------------------------------------------
	
SynchConsole::~SynchConsole()
{
	delete cons;
	delete synchReadAvail;
	delete synchWriteAvail;
	delete RLineBlock;
	delete WLineBlock;
}

//----------------------------------------------------------------------
//   int SynchConsole::Write(char *into, int numBytes)
//	Writes numBytes of into buffer to I/O device 
//	Returns the number of bytes written
//----------------------------------------------------------------------

int SynchConsole::Write(char *from, int numBytes)
{
	int loop;			// General purpose counter

	WLineBlock->P();			// Block for the line

//	printf("[%s]:\n",currentThread->getName());	//DEBUG: Print thread

	for (loop = 0; loop < numBytes; loop++)
	{
		cons->PutChar(from[loop]);		// Write and wait
		synchWriteAvail->P();			// Block for a character
	}

	WLineBlock->V();				// Free Up
	return numBytes;				// Return the bytes out
}

//----------------------------------------------------------------------
//   int SynchConsole::Read(char *into, int numBytes)
//	Read numBytes of into buffer to I/O device 
//	Returns the number of bytes written  (may be less if CR or ^D)
//----------------------------------------------------------------------

int
SynchConsole::Read(char *into, int numBytes)
{
	int loop;
	int eolncond = FALSE;
	char ch;

	for (loop = 0; loop < numBytes; loop++)
		into[loop] = 0;

	loop = 0;

	RLineBlock->P();				// Block for a read line

//	printf("{%s}:\n",currentThread->getName());	// DEBUG print thread

	while ( (loop < numBytes) && (eolncond == FALSE) )
	{
		do
		{
			synchReadAvail->P();		// Block for single char
			ch = cons->GetChar();		// Get a char (could)
		} while ( ch == EOF);

		if ( (ch == '\012') || (ch == '\001') )
		{
			eolncond = TRUE;
		}
		else
		{
			into[loop] = ch;		// Put the char in buf
			loop++;				// Auto inc
		}
	}

	RLineBlock->V();				// UnBLock

	if (ch == '\001')				// CTRL-A Returns -1
		return -1;				// For end of stream
	else
		return loop;				// How many did we rd
}

// CAE - MULTI - END SECTION
