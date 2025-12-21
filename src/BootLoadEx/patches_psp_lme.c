#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>

#include "bootloadex.h"
#include "pspbtcnf.h"

#define PATH_RECOVERY_ME PATH_FLASH0 "vsh/module/recovery.prx"

void xor_cipher(u8* data, u32 size, u8* key, u32 key_size)
{
    u32 i;

    for (i = 0; i < size; i++)
    {
        data[i] ^= key[i % key_size];
    }
}

int MEPRXDecrypt(PSP_Header* prx, unsigned int size, unsigned int * newsize){
    xor_cipher((u8*)prx + 0x150, 0x10, prx->key_data1, 0x10);
    xor_cipher((u8*)prx + 0x150, prx->comp_size, &prx->scheck[0x38], 0x20);
    unPatchLoadCorePRXDecrypt();
    return 0;
}

int MECheckExec(unsigned char * addr, void * arg2){
    unPatchLoadCoreCheckExec();
    return 0;
}

int patch_bootconf_me_recovery(char *buffer, int length)
{
    int result = length;
    int newsize;

    newsize = AddPRX(buffer, "/kd/usersystemlib.prx", "/kd/usbstorms.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/kd/usersystemlib.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/kd/libatrac3plus.prx", "/kd/usbstorboot.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/kd/libatrac3plus.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/kd/mediasync.prx", "/kd/usbstor.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/kd/mediasync.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/kd/vshctrl_02g.prx", "/kd/usbstormgr.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/kd/vshctrl_02g.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/vsh/module/paf.prx", "/kd/usbdev.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/vsh/module/paf.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/vsh/module/common_gui.prx", "/kd/lflash_fatfmt.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/vsh/module/common_gui.prx", VSH_RUNLEVEL);

    newsize = AddPRX(buffer, "/vsh/module/common_util.prx", "/kd/usersystemlib.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/vsh/module/common_util.prx", VSH_RUNLEVEL);
    
    newsize = AddPRX(buffer, "/vsh/module/vshmain.prx", "/vsh/module/recovery.prx", VSH_RUNLEVEL);
    if (newsize > 0) result = newsize;
    RemovePrx(buffer, "/vsh/module/vshmain.prx", VSH_RUNLEVEL);
    
    if (psp_model == PSP_GO)
    {
        newsize = AddPRX(buffer, "/vsh/module/mcore.prx", "/kd/usbstoreflash.prx", VSH_RUNLEVEL);
        if (newsize > 0) result = newsize;
        RemovePrx(buffer, "/vsh/module/mcore.prx", VSH_RUNLEVEL);
    }
}

int UnpackBootConfigPSP_LME(char **p_buffer, int length){

    int result = length;
    int newsize;
    char *buffer;

    result = (*UnpackBootConfig)(*p_buffer, length);
    buffer = (void*)BOOTCONFIG_TEMP_BUFFER;
    memcpy(buffer, *p_buffer, length);
    *p_buffer = buffer;

    // clear config
    memset((void*)0x88FB0000, 0, 0x100);

    // recovery
    if (ark_config->recovery){
        newsize = patch_bootconf_me_recovery(buffer, result);
        if (newsize > 0) result = newsize;
    }

    return result;
}
