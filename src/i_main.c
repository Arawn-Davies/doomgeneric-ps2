//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//

//#include "config.h"



//#include "doomtype.h"
//#include "i_system.h"
#include "m_argv.h"

#include <ps2_all_drivers.h>
#include <tamtypes.h>

#include <iopcontrol.h>
#include <sifrpc.h>
#include <sbv_patches.h>

#include <debug.h>
#include <stdio.h>
#include <stdbool.h>
#include <loadfile.h>
#include <string.h>
#include <malloc.h>
#include <kernel.h>
#include <d_main.h>


//
// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
//

//void D_DoomMain (void);

//void M_FindResponseFile(void);

//void dg_Create();

static void reset_IOP() {
	SifInitRpc(0);
	#if !defined(DEBUG) || defined(BUILD_FOR_PCSX2)
	/* Comment this line if you don't wanna debug the output */
	while(!SifIopReset(NULL, 0)){};
	#endif

	while(!SifIopSync()){};
	SifInitRpc(0);
	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();
}

static void init_drivers()
{
    init_fileXio_driver();
	init_memcard_driver(true);
	init_usb_driver(true);
	init_cdfs_driver();
	init_joystick_driver(true);
	init_audio_driver();
	init_poweroff_driver();
	init_hdd_driver(true, true);
}

int main(int argc, char **argv)
{
    printf("\r\nreboot IOP");
    reset_IOP();

    printf("\r\ninit drivers");
    init_drivers();

    printf("init_scr();");
    init_scr();
    scr_printf("init_scr(); done");
    
    // save arguments
    
    myargc = argc;
    myargv = argv;

    M_FindResponseFile();

    // start doom
    scr_printf("\r\nStarting D_DoomMain\r\n");
    
	dg_Create();

    while (true)
    {
        scr_printf("D_DoomMain(); runs here...");
        printf("D_DoomMain(); runs here...");
        D_DoomMain ();
    }
        

    
}

