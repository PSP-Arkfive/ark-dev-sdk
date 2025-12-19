/*
 * This file is part of PRO CFW.

 * PRO CFW is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * PRO CFW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PRO CFW. If not, see <http://www.gnu.org/licenses/ .
 */

#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>

#include "rebootex.h"

#ifdef REBOOTEX
#define END_BUF_STR "ApplyPspRelSection"
#ifdef MS_IPL
#include "syscon.h"
#endif
#else
#define END_BUF_STR "StopBoot"
#define SYSCON_CTRL_RTRG 0x00000400
#define SYSCON_CTRL_HOME 0x00001000

ARKConfig _arkconf = {
    .magic = ARK_CONFIG_MAGIC,
#ifndef MS_IPL
    .arkpath = "ms0:/PSP/SAVEDATA/ARK_01234/", // default path for ARK files
    .exploit_id = CIPL_EXPLOIT_ID,
#else
    .arkpath = ARK_DC_PATH "/ARK_01234/", // default path for ARK files
    .exploit_id = DC_EXPLOIT_ID,
#endif
    .launcher = {0},
    .exec_mode = PSP_ORIG, // run ARK in PSP mode
    .recovery = 0,
};
#endif

#define REG32(addr)                 *((volatile uint32_t *)(addr))

// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    #ifdef DEBUG
    colorDebug(0xff00);
    #endif
    
    #if defined(REBOOTEX) && defined(MS_IPL)
    // GPIO enable
    REG32(0xbc10007c) |= 0xc8;
    __asm("sync"::);
    
    syscon_init();
    
    syscon_ctrl_ms_power(1);
    #endif

    #ifdef PAYLOADEX
    #ifndef VITA_PAYLOADEX
    u32 ctrl = _lw(BOOT_KEY_BUFFER);

    if ((ctrl & SYSCON_CTRL_HOME) == 0) {
        return sceReboot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    if ((ctrl & SYSCON_CTRL_RTRG) == 0) {
        _arkconf.recovery = 1;
    }
    #endif
    memcpy(ark_config, &_arkconf, sizeof(ARKConfig));
    #endif

    // check config
    checkRebootConfig();

    // scan for reboot functions
    findRebootFunctions();
    
    // patch reboot buffer
    patchRebootBuffer();
    
    // Forward Call
    return sceReboot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
