#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>

#include "bootloadex.h"
#include "pspbtcnf.h"


int file_exists(const char *path)
{
    int ret;

    if (ble_config->boot_storage == MS_BOOT)
        ret = ble_config->extra_io.psp_io.FatOpen(path);
    else
        ret = sceBootLfatOpen(path);

    if (ret >= 0) {
        if (ble_config->boot_storage == MS_BOOT)
            ble_config->extra_io.psp_io.FatClose(path);
        else
            sceBootLfatClose();
        return 1;
    }

    return 0;
}

int loadcoreModuleStartPSP(void * arg1, void * arg2, void * arg3, int (* start)(void *, void *, void *)){
    loadCoreModuleStartCommon((u32)start);

    flushCache();
    return start(arg1, arg2, arg3);
}

// patch boot on psp
void patchBootPSP(void* UnpackBootConfigPatchedPSP){

    _sw(0x27A40004, UnpackBootConfigArg); // addiu $a0, $sp, 4
    _sw(JAL(UnpackBootConfigPatchedPSP), UnpackBootConfigCall); // Hook UnpackBootConfig

    // make sure we read as little ram as possible
    int patches = (ble_config->boot_storage == MS_BOOT)? 6:5;
    
    for (u32 addr = REBOOT_TEXT; addr<reboot_end && patches; addr+=4){
        u32 data = _lw(addr);
        if (data == 0x02A0E821 || data == 0x0280E821){ // found loadcore jump on PSP
            _sw(0x3821 | ((_lw(addr-4) & 0x3E00000) >> 5), addr-4); // ADDU $a3 $zero <reg>
            _sw(JUMP(loadcoreModuleStartPSP), addr);
            _sw(data, addr + 4);
            patches--;
            addr += 4;
        }
        else if (data == 0x2C860040 || data == 0x2C850040){ // kdebug patch
            _sw(0x03E00008, addr-4); // make it return 1
            _sw(0x24020001, addr); // rebootexcheck1
            patches--;
        }
        else if (data == 0x24D90001 || data == 0x256A0001){  // rebootexcheck5
            u32 a = addr;
            u32 insMask;
            do {
                a-=4;
                insMask = _lw(a) & 0xFFFF0000;
            } while (insMask != 0x04400000 && insMask != 0x04420000);
            _sw(NOP, a); // Killing Branch Check bltz/bltzl ...
        }
        else if (data == 0x27BDFFE0 && _lw(addr+4) == 0x3C028861 && ble_config->boot_storage == MS_BOOT) { // nand enc
            MAKE_DUMMY_FUNCTION_RETURN_0(addr);
            patches--;
        }
        else {
            if (ble_config->boot_type == TYPE_REBOOTEX){
                if (data == 0x34650001){ // rebootexcheck2
                    _sw(NOP, addr-4); // Killing Branch Check bltz ...
                    patches--;
                }
                else if (data == 0x00903021 && _lw(addr+4) == 0x00D6282B){ // rebootexcheck3 and rebootexcheck4
                    u32 a = addr;
                    do {a-=4;} while (_lw(a) != NOP);
                    _sw(NOP, a-4); // Killing Branch Check beqz
                    _sw(NOP, addr+8); // Killing Branch Check bltz ...
                    patches--;
                }
            }
            else if (ble_config->boot_type == TYPE_PAYLOADEX){
                if (data == 0x25AC003F){ // payloadexcheck2
                    _sw(NOP, addr-44); // Killing Branch Check bltz ...
                    patches--;
                }
                else if (data == 0x01F7702B){ // rebootexcheck3 and rebootexcheck4
                    _sw(NOP, addr-12); // Killing Branch Check bltz
                    _sw(NOP, addr+4); // Killing Branch Check beqz ...
                    patches--;
                }
            }
        }
    }

    patchRebootIoPSP();

    // Flush Cache
    flushCache();
}


int is_fatms371(void)
{
    return file_exists(PATH_FATMS_HELPER + sizeof("flash0:") - 1) && file_exists(PATH_FATMS_371 + sizeof("flash0:") - 1);
}

int patch_bootconf_fatms371(char *buffer, int length)
{
    int newsize;

    newsize = AddPRX(buffer, "/kd/fatms.prx", PATH_FATMS_HELPER+sizeof(PATH_FLASH0)-2, 0xEF & ~VSH_RUNLEVEL);
    RemovePrx(buffer, "/kd/fatms.prx", 0xEF & ~VSH_RUNLEVEL);
    newsize = AddPRX(buffer, "/kd/wlan.prx", PATH_FATMS_371+sizeof(PATH_FLASH0)-2, 0xEF & ~VSH_RUNLEVEL);

    return newsize;
}

// IO Patches
char path[128];

int _sceBootLfatMount()
{
    return ble_config->extra_io.psp_io.FatMount();
}

int _sceBootLfatRead(char * buffer, int length)
{
    //load on reboot module
    if(rebootmodule_open && p_rmod != NULL && size_rmod > 0)
    {
        int min;

        //copy load on reboot module
        min = size_rmod < length ? size_rmod : length;
        if (min > 0){
            memcpy(buffer, (void*)p_rmod, min);
            p_rmod += min;
            size_rmod -= min;
        }

        //set filesize
        return min;
    }

    if (ble_config->boot_storage == MS_BOOT)
        return ble_config->extra_io.psp_io.FatRead(buffer, length);
    
    //forward to original function
    return sceBootLfatRead(buffer, length);
}

int _sceBootLfatOpen(char * filename)
{
    //load on reboot module open
    if(strcmp(filename, REBOOT_MODULE) == 0)
    {
        //mark for read
        rebootmodule_open = 1;
        p_rmod = reboot_conf->rtm_mod.buffer;
        size_rmod = reboot_conf->rtm_mod.size;

        //return success
        return 0;
    }

    if (ble_config->boot_storage == MS_BOOT){
        strcpy(path, "/TM/DCARK");
        strcat(path, filename);

        if (ble_config->boot_type == TYPE_PAYLOADEX){
            if (memcmp(filename+4, "pspbtcnf", 8) == 0)
                memcpy(&path[strlen(path) - 4], "_dc.bin", 8);
        }

        return ble_config->extra_io.psp_io.FatOpen(path);
    }
    else {
        // patch to allow custom boot
        if (strncmp(filename+4, "pspbtcnf", 8) == 0){
            int res = -1;
            // check for custom btcnf
            filename[6] = 't'; // pstbtcnf.bin
            res = sceBootLfatOpen(filename);
            if (res >= 0) return res;
            filename[6] = 'p'; // fallback
        }

        //forward to original function
        return sceBootLfatOpen(filename);
    }
}

int _sceBootLfatClose(void)
{
    //reboot module close
    if(rebootmodule_open && p_rmod != NULL && size_rmod == 0)
    {
        //mark as closed
        rebootmodule_open = 0;
        p_rmod = NULL;
        size_rmod = 0;
        reboot_conf->rtm_mod.buffer = NULL;
        reboot_conf->rtm_mod.size = 0;

        //return success
        return 0;
    }
    
    if (ble_config->boot_storage == MS_BOOT)
        return ble_config->extra_io.psp_io.FatClose();
    
    //forward to original function
    return sceBootLfatClose();
}

void patchRebootIoPSP(){
    int patches = 3;
    for (u32 addr = REBOOT_TEXT; addr<reboot_end && patches; addr+=4){
        u32 data = _lw(addr);
        if (data == 0x8E840000 || data == 0x8EA40000){
            if (ble_config->boot_storage == MS_BOOT){
                int found = 0;
                for (int i=8; !found; i+=4) {
                    if (IS_JAL(_lw(addr-i))) {
                        _sw(JAL(_sceBootLfatMount), addr-i);
                        found = 1;
                    }
                }
            }
            sceBootLfatOpen = (void*)K_EXTRACT_CALL(addr-4);
            _sw(JAL(_sceBootLfatOpen), addr-4);
            patches--;
        }
        else if (data == 0xAE840004 || data == 0xAEA30004){
            addr += 4;
            while (!IS_JAL(_lw(addr))) { addr += 4; }
            sceBootLfatRead = (void*)K_EXTRACT_CALL(addr);
            _sw(JAL(_sceBootLfatRead), addr);
            patches--;
        }
        else if (data == 0xAE930008 || data == 0xAEB40008){
            sceBootLfatClose = (void*)K_EXTRACT_CALL(addr-4);
            _sw(JAL(_sceBootLfatClose), addr-4);
            patches--;
        }
    }
    // Flush Cache
    flushCache();
}
