/* Start.s 
 *	Assembly language assist for user programs running on top of Nachos.
 *
 *	Since we don't want to pull in the entire C library, we define
 *	what we need for a user program here, namely Start and the system
 *	calls.
 */

#define IN_ASM
#include "syscall.h"

        .text   
        .align  2

/* -------------------------------------------------------------
 * __start
 *	Initialize running a C program, by calling "main". 
 *
 * 	NOTE: This has to be first, so that it gets loaded at location 0.
 *	The Nachos kernel always starts a program by jumping to location 0.
 * -------------------------------------------------------------
 */

	.globl __start
	.ent	__start
__start:
	jal	main
	move	$4,$0		
	jal	Exit	 /* if we return from main, exit(0) */
	.end __start

/* -------------------------------------------------------------
 * System call stubs:
 *	Assembly language assist to make system calls to the Nachos kernel.
 *	There is one stub per system call, that places the code for the
 *	system call into register r2, and leaves the arguments to the
 *	system call alone (in other words, arg1 is in r4, arg2 is 
 *	in r5, arg3 is in r6, arg4 is in r7)
 *
 * 	The return value is in r2. This follows the standard C calling
 * 	convention on the MIPS.
 * -------------------------------------------------------------
 */

	.globl Halt
	.ent	Halt
Halt:
	addiu $2,$0,SC_Halt
	syscall
	j	$31
	.end Halt

	.globl Exit
	.ent	Exit
Exit:
	addiu $2,$0,SC_Exit
	syscall
	j	$31
	.end Exit

	.globl Exec
	.ent	Exec
Exec:
	addiu $2,$0,SC_Exec
	syscall
	j	$31
	.end Exec

	.globl Join
	.ent	Join
Join:
	addiu $2,$0,SC_Join
	syscall
	j	$31
	.end Join

	.globl CreateFile
	.ent	CreateFile
CreateFile:
	addiu $2,$0,SC_CreateFile
	syscall
	j	$31
	.end CreateFile

	.globl Open
	.ent	Open
Open:
	addiu $2,$0,SC_Open
	syscall
	j	$31
	.end Open

	.globl Read
	.ent	Read
Read:
	addiu $2,$0,SC_Read
	syscall
	j	$31
	.end Read

	.globl Write
	.ent	Write
Write:
	addiu $2,$0,SC_Write
	syscall
	j	$31
	.end Write

	.globl Close
	.ent	Close
Close:
	addiu $2,$0,SC_Close
	syscall
	j	$31
	.end Close

	.globl Fork
	.ent	Fork
Fork:
	addiu $2,$0,SC_Fork
	syscall
	j	$31
	.end Fork

	.globl Yield
	.ent	Yield
Yield:
	addiu $2,$0,SC_Yield
	syscall
	j	$31
	.end Yield

	.globl ReadInt
	.ent ReadInt
ReadInt:
	addiu $2, $0, SC_ReadInt
	syscall
	j	$31
	.end ReadInt 

	.globl PrintInt
	.ent PrintInt
PrintInt:
	addiu $2, $0, SC_PrintInt
	syscall
	j	$31
	.end PrintInt

	.globl ReadChar
	.ent ReadChar
ReadChar:
	addiu $2, $0, SC_ReadChar
	syscall
	j	$31
	.end ReadChar

	.globl PrintChar
	.ent PrintChar
PrintChar:
	addiu $2, $0, SC_PrintChar
	syscall
	j	$31
	.end PrintChar

	.globl ReadString
	.ent ReadString
ReadString:
	addiu $2, $0, SC_ReadString
	syscall
	j	$31
	.end ReadString

	.globl PrintString
	.ent PrintString
PrintString:
	addiu $2, $0, SC_PrintString
	syscall
	j	$31
	.end PrintString
	

	.globl Seek
	.ent	Seek
Seek :
	addiu $2, $0, SC_Seek
	syscall
	j	$31
	.end Seek

	.globl CreateSemaphore
	.ent	CreateSemaphore
CreateSemaphore :
	addiu $2, $0, SC_CreateSemaphore
	syscall
	j	$31
	.end CreateSemaphore

	.globl Wait
	.ent	Wait
Wait :
	addiu $2, $0, SC_Wait
	syscall
	j	$31
	.end Wait

	.globl Signal
	.ent	Signal
Signal :
	addiu $2, $0, SC_Signal
	syscall
	j	$31
	.end Signal
	

	.globl Sum
	.ent	Sum
Sum :
	addiu $2, $0, SC_Sum
	syscall
	j	$31
	.end Sum

/* dummy function to keep gcc happy */
        .globl  __main
        .ent    __main
__main:
        j       $31
        .end    __main

