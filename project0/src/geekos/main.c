/*
 * GeekOS C code entry point
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2004, Iulian Neamtiu <neamtiu@cs.umd.edu>
 * $Revision: 1.51 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/bootinfo.h>
#include <geekos/string.h>
#include <geekos/screen.h>
#include <geekos/mem.h>
#include <geekos/crc32.h>
#include <geekos/tss.h>
#include <geekos/int.h>
#include <geekos/kthread.h>
#include <geekos/trap.h>
#include <geekos/timer.h>
#include <geekos/keyboard.h>



static void PrintKey(ulong_t arg)
{
	Keycode key;
	int row, col, count = 0;

	while(1) {
		key = Wait_For_Key();

		/* filter release keycode */
		if (KB_KEY_RELEASE & (key>>8))
			continue;

		if (key == 0x0d)
			Print("\n");
		else
			Print("%c", key);

		/* Print count at corner of screen */
		Get_Cursor(&row, &col);
		Put_Cursor(0, 76);
		Print("    ");
		Put_Cursor(0, 76);
		Print("%d", count++);
		Put_Cursor(row, col);
	}
}

/*
 * Kernel C code entry point.
 * Initializes kernel subsystems, mounts filesystems,
 * and spawns init process.
 */
void Main(struct Boot_Info* bootInfo)
{
    Init_BSS();
    Init_Screen();
    Init_Mem(bootInfo);
    Init_CRC32();
    Init_TSS();
    Init_Interrupts();
    Init_Scheduler();
    Init_Traps();
    Init_Timer();
    Init_Keyboard();


    Set_Current_Attr(ATTRIB(BLACK, GREEN|BRIGHT));
    Print("Welcome to GeekOS!\n");
    Set_Current_Attr(ATTRIB(BLACK, GRAY));


    //TODO("Start a kernel thread to echo pressed keys and print counts");

    Print("starting echo thread\n");
    Start_Kernel_Thread(PrintKey, 0, PRIORITY_NORMAL, true);


    /* Now this thread is done. */
    Exit(0);
}









