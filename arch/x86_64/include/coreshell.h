#pragma once

#include "flags.h"
#include "stdint.h"
#include "bmp.h"

#pragma pack(push, 1)
typedef struct {
    int x;
    int y;
} vector2d_t;

enum DrawTask {
    Rectangle = 0,
    Text, Pixel
};
enum DrawState {
    Idle = 0,
    Unused, Active,
};
typedef struct {
    vector2d_t pos;
    vector2d_t size;
    
    char *text;
    int color;

    enum DrawTask task;
    enum DrawState state;
} drawtask_t;

enum CoreshellInstallationState {
    NotInstalled = 0,
    NotConfigured, Ready
};
typedef struct {
    bool loginWithoutPassword;
    bool useAccount;
} coreshell_uperms_t;
typedef struct {
    char name[16];
    char password[16];
    int uid;
    coreshell_uperms_t permissions;
    bool avaliable;
    char reserved[47];
} coreshell_user_t;
typedef struct {
    char signature[16];
    enum CoreshellInstallationState installstate;
    coreshell_user_t users[4];
    uint8_t rev;
    uint16_t turnedOnTimes;
    bool recoverFromError;
    char freeSpace[144];
} coreshell_hddsettings_t;

#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

extern drawtask_t *__coreshell_drawtasks;

#if INTERNAL_FUNCS_IN_HEADERS == 1  
// Core 2
void __coreshell_init_coreRenderer();
// Core 1
void __coreshell_init_coreIOHandler();
// Core 0
void __coreshell_init_coreExecuter();

// Installation stages
void __coreshell_install_stage1();
void __coreshell_install_stage2();
void __coreshell_install_stage3();

#endif

coreshell_hddsettings_t *__coreshell_createSettings();

vector2d_t alignText(const char *text);
void __gui_drawRectangle(vector2d_t pos, vector2d_t size, int color);
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols);
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, int color);
void __gui_drawImage24(BMPImage *image, vector2d_t pos);
void __gui_drawImage32(BMPImage *image, vector2d_t pos);

void __coreshell_init_all();

void __coreshell_onDesktop(coreshell_user_t *user);
void __coreshell_loginscreen();

#ifdef __cplusplus
}
#endif