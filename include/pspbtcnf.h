#ifndef PSPBTCNF_H
#define PSPBTCNF_H

#define PATH_FLASH0 "flash0:/"


typedef struct _btcnf_header
{
    unsigned int signature; // 0
    unsigned int devkit; // 4
    unsigned int unknown[2]; // 8
    unsigned int modestart; // 0x10
    int nmodes; // 0x14
    unsigned int unknown2[2]; // 0x18
    unsigned int modulestart; // 0x20
    int nmodules; // 0x24
    unsigned int unknown3[2]; // 0x28
    unsigned int modnamestart; // 0x30
    unsigned int modnameend; // 0x34
    unsigned int unknown4[2]; // 0x38
} _btcnf_header;

typedef struct _btcnf_module
{
    unsigned int module_path; // 0x00
    unsigned int unk_04; //0x04
    unsigned int flags; //0x08
    unsigned int unk_C; //0x0C
    unsigned char hash[0x10]; //0x10
} _btcnf_module;

enum {
    VSH_RUNLEVEL     =     0x01,
    GAME_RUNLEVEL    =     0x02,
    UPDATER_RUNLEVEL =     0x04,
    POPS_RUNLEVEL    =     0x08,
    APP_RUNLEVEL     =     0x20,
    UMDEMU_RUNLEVEL  =     0x40,
    MLNAPP_RUNLEVEL  =     0x80,
};

int SearchPrx(char *buffer, const char *modname);
int AddPRXNoCopyName(char * buffer, const char * insertbefore, int prxname_offset, u32 flags);
int AddPRX(char * buffer, const char * insertbefore, const char * prxname, u32 flags);
int RemovePrx(char *buffer, const char *prxname, u32 flags);
int MovePrx(char * buffer, const char * insertbefore, const char * prxname, u32 flags);
int ModifyPrxFlag(char *buffer, const char* modname, u32 flags);
int GetPrxFlag(char *buffer, const char* modname, u32 *flags);

#endif
