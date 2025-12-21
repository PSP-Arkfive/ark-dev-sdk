#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>

#include "bootloadex.h"
#include "pspbtcnf.h"

#define PATH_SYSTEMCTRL_PRO PATH_FLASH0 "kd/_systemctrl.prx"
#define PATH_VSHCTRL_PRO PATH_FLASH0 "kd/_vshctrl.prx"
#define PATH_RECOVERY_PRO PATH_FLASH0 "vsh/module/_recovery.prx"

int UnpackBootConfigPSP_PRO(char **p_buffer, int length)
{
    struct {
        u32 magic;
        u32 rebootex_size;
        u32 p2_size;
        u32 p9_size;
        char *insert_module_before;
        void *insert_module_binary;
        u32 insert_module_size;
        u32 insert_module_flags;
        u32 psp_fw_version;
        u8 psp_model;
        u8 iso_mode;
        u8 recovery_mode;
        u8 ofw_mode;
        u8 iso_disc_type;
    }* conf = (void*)(REBOOTEX_CONFIG);

    int result = length;
    int newsize;
    char *buffer;

    result = (*UnpackBootConfig)(*p_buffer, length);
    buffer = (void*)BOOTCONFIG_TEMP_BUFFER;
    memcpy(buffer, *p_buffer, length);
    *p_buffer = buffer;

    memset(conf, 0, 0x100);
    conf->magic = 0xC01DB15D;
    conf->psp_model = psp_model;
    conf->rebootex_size = 0;
    conf->psp_fw_version = FW_661;

    // Insert SystemControl
    newsize = AddPRX(buffer, "/kd/init.prx", PATH_SYSTEMCTRL_PRO+sizeof(PATH_FLASH0)-2, 0x000000EF);
    if (newsize > 0) result = newsize;

    // Insert VSHControl
    if (SearchPrx(buffer, "/vsh/module/vshmain.prx") >= 0) {
        newsize = AddPRX(buffer, "/kd/vshbridge.prx", PATH_VSHCTRL_PRO+sizeof(PATH_FLASH0)-2, VSH_RUNLEVEL );
        if (newsize > 0) result = newsize;
    }

    // insert recovery
    if (ark_config->recovery){
        RemovePrx(buffer, "/vsh/module/vshmain.prx", VSH_RUNLEVEL);
        newsize = AddPRX(buffer, "/vsh/module/vshmain.prx", PATH_RECOVERY_PRO+sizeof(PATH_FLASH0)-2, VSH_RUNLEVEL);
        if (newsize > 0) result = newsize;
    }

    return result;
}
