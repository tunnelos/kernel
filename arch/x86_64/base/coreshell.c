#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

#include "../include/coreshell.h"
#include "../include/tools.h"
#include "../include/tunnel.h"
#include "../include/assert.h"
#include "../include/mm.h"
#include "../include/ide.h"
#include "../include/stdlib.h"
#include "../include/stdint.h"
#include "../include/keyboard_ps2.h"
#include "../include/tunnel.h"
#include "../include/bmp.h"
#include "../include/fs.h"

drawtask_t *__coreshell_drawtasks;
coreshell_hddsettings_t *__coreshell_settings;
ide_rw_t __coreshell_settings_write;
int __coreshell_currentKey[2];
int __coreshell_percentage = 0;
bool __coreshell_inGUI = false;
bool __coreshell_requestFormat = false;

tunnelfs_t fsInstance;

coreshell_hddsettings_t *__coreshell_createSettings()
{
    coreshell_hddsettings_t *ret = (coreshell_hddsettings_t *)calloc(sizeof(coreshell_hddsettings_t));
    memcpy(ret->signature, "TUNNEL CORESHELL", 16);
    int i = 0;
    while(i < 4) {
        ret->users[i].avaliable = true;
        ret->users[i].permissions.useAccount = true;
        i++;
    }
    ret->installstate = NotInstalled;
    ret->rev = 0x10;
    return ret;
}
vector2d_t alignText(const char *text)
{
    vector2d_t t;
    int l = strlen(text);
    int i = 0;
    int newlines = 0;
    t.x = (80 - l) / 2;
    while (i < l)
    {
        if (text[i] == '\n')
            newlines++;
        i++;
    }
    t.y = (30 - newlines) / 2;
    return t;
}

void __gui_drawRectangle(vector2d_t pos, vector2d_t size, int color)
{
    int x = pos.x;
    int y = pos.y;
    int xm = x + size.x;
    int ym = y + size.y;
    while (x < xm)
    {
        while (y < ym)
        {
            puts("\x08", color, x, y);
            y++;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage, int col)
{
    float t = (float)percentage / (float)100;
    float x = t * (float)maxSize.x;
    __gui_drawRectangle(pos, (vector2d_t){(int)x, maxSize.y}, col);
}
void __gui_drawInputBar(vector2d_t pos, const char *buffer, int maxSymbols) {
    __gui_drawRectangle(pos, (vector2d_t){maxSymbols, 1}, COLOR_LIGHT_GRAY);
    int i = 0;
    char b[2] = {' ', 0};
    while(i < maxSymbols) {
        if(buffer[i] != 0) {
            b[0] = buffer[i];
            puts_gui(b, COLOR_BLACK, pos.x, pos.y);
            pos.x++;
        }
        i++;
    }
}
void __gui_drawImage24(BMPImage *image, vector2d_t pos) {
    int x = pos.x;
    int y = pos.y;
    int xm = image->header.width_px + pos.x;
    int ym = image->header.height_px + pos.y; 
    int p = 0;
    char col[4] = { //ARGB
        0x00, image->data[p + 0], image->data[p + 1], image->data[p + 2]
    };
    while(x < xm) {
        while(y < ym) {
            col[1] = image->data[p + 0];
            col[2] = image->data[p + 1];
            col[3] = image->data[p + 2];
            *((uint32_t*)((uint64_t)&fb + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = *(uint32_t *)&col;
            y++;
            p += 3;
        }
        x++;
        y = pos.y;
    }
}
void __gui_drawImage32(BMPImage *image, vector2d_t pos) {
    int x = pos.x;
    int y = pos.y;
    int xm = image->header.width_px + pos.x;
    int ym = image->header.height_px + pos.y; 
    int p = 0;
    int *imd = (int *)&image->data;
    while(x < xm) {
        while(y < ym) {
            *((uint32_t*)((uint64_t)&fb + (y * tunnelos_sysinfo.bootboot.fb_scanline) + (x * 4))) = imd[p];
            y++;
            p++;
        }
        x++;
        y = pos.y;
    }
}

void __coreshell_install_stage3() {
    // __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    // puts_gui("Tunnel OS", COLOR_BLACK, alignText("Tunnel OS").x, 1);
    // puts_gui("System will reboot in 5 seconds...", COLOR_RED, 1, 3);
    // int p = 0;
    // __gui_drawRectangle((vector2d_t){2, 5}, (vector2d_t){76, 3}, COLOR_BLACK);
    // __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, p);
    // bool reboot = false;
    // while(!reboot) {
    //     if(p == 100) {
    //         reboot = true;
    //     }
    //     wait(100);
    //     p += 20;
    //     __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, p);
    // }
    // __coreshell_settings->installstate = Ready; 
    // __ide_get_access(__coreshell_settings_write);
    // __tunnel_reboot(); 
}

void __coreshell_install_stage2() {
    // __coreshell_settings_write.lba = 0;
    // __coreshell_settings_write.sectors = 1;
    // __coreshell_settings_write.rw = true;
    // __coreshell_settings_write.selector = 0;
    // __coreshell_settings_write.drive = __ide_devices[0].drive;
    // __coreshell_settings_write.buffer = (uint32_t)__coreshell_settings;

    // __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    // puts_gui("Tunnel OS: Configuration", COLOR_BLACK, alignText("Tunnel OS: Configuration").x, 1);
    // __coreshell_settings->users[0].avaliable = false;
    // puts_gui("Choose username for your user:", COLOR_DARK_GREEN, 1, 3);
    // puts_gui("Press Enter to continue.", COLOR_BLUE, 1, 6);
    // char *buffer = __coreshell_settings->users[0].name;
    // int bufferI = 0;
    // bool isEnter = false;
    // __gui_drawInputBar((vector2d_t){1, 4}, buffer, 16);
    // putc_gui(12, COLOR_DARK_GRAY, 1, 4);
    // while(!isEnter) {
    //     wait(1);
    //     if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
    //         isEnter = true;
    //     } else if (__coreshell_currentKey[0] == 0x0E) {
    //         if(bufferI != 0) bufferI--;
    //         buffer[bufferI] = 0;
    //         __gui_drawInputBar((vector2d_t){1, 4}, buffer, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 16){
    //         buffer[bufferI] =__coreshell_currentKey[1];
    //         bufferI++;
    //         __gui_drawInputBar((vector2d_t){1, 4}, buffer, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     }
    // }
    // __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    // puts_gui("Tunnel OS: Configuration", COLOR_BLACK, alignText("Tunnel OS: Configuration").x, 1);
    // __coreshell_settings->users[0].avaliable = false;
    // puts_gui("Type password for your user:", COLOR_DARK_GREEN, 1, 3);
    // puts_gui("Press Enter to continue.", COLOR_BLUE, 1, 6);
    // buffer = __coreshell_settings->users[0].password;
    // char buffer2[16];
    // __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
    // isEnter = false;
    // bufferI = 0;
    // while(!isEnter) {
    //     wait(1);
    //     if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
    //         isEnter = true;
    //     } else if (__coreshell_currentKey[0] == 0x0E) {
    //         if(bufferI != 0) bufferI--;
    //         buffer[bufferI] = 0;
    //         buffer2[bufferI] = 0;
    //         __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 16){
    //         buffer[bufferI] =__coreshell_currentKey[1];
    //         buffer2[bufferI] = '*';
    //         bufferI++;
    //         __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     }
    // }
    // __coreshell_install_stage3();
}

void __coreshell_install_stage1()
{
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    puts_gui("Tunnel OS", COLOR_BLACK, alignText("Tunnel OS").x, 1);
    puts_gui("Do you want to create TunnelFS?", COLOR_BLACK, 1, 3);
    puts_gui(" - Yes: press Enter", COLOR_RED, 1, 4);
    puts_gui(" - No : press Right Shift", COLOR_GREEN, 1, 5);
    bool KeyYes = false;
    bool KeyNo = false;
    while ((KeyYes != true) && (KeyNo != true))
    {
        wait(1);    
        if (__coreshell_currentKey[0] == 0x1C)
            KeyYes = true;
        if (__coreshell_currentKey[0] == 0x36)
            KeyNo = true;
    }
    if (KeyYes)
    {
        __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
        puts_gui("Tunnel OS: Formatting", COLOR_BLACK, alignText("Tunnel OS: Drive formatting").x, 1);
        puts_gui("Creating TunnelFS...", COLOR_DARK_GREEN, 1, 3);

        __gui_drawRectangle((vector2d_t){2, 5}, (vector2d_t){76, 3}, COLOR_BLACK);
        __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, __coreshell_percentage, COLOR_GREEN);
        __coreshell_requestFormat = true;
        while(__coreshell_percentage != 100) {
            wait(5);
            __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, __coreshell_percentage, COLOR_GREEN);
        }
        if(fsInstance.bootsector == NULL) {
            puts_gui("Error occured while TFS installtion.", COLOR_RED, 1, 3);
            __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, 100, COLOR_RED);
        } else {
            int test = __fs_tunnelAllocateFile("test            ", "txt ", 1, fsInstance);
            uint8_t *b = __fs_tunnelReadDataFromMeta(test, fsInstance);
            b[0] = 0xAA;
            __fs_tunnelSaveFile(test, b, fsInstance);
            
            __gui_drawRectangle((vector2d_t){1, 3}, (vector2d_t){25, 1}, COLOR_WHITE);
            puts_gui("TunnelFS installation complete", COLOR_DARK_GREEN, 1, 3);
            __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, 100, COLOR_GREEN);
        }
        // puts_gui("Let's configure this installation ", COLOR_BLACK, 1, 10);
        // puts_gui("Press Enter to continue", COLOR_DARK_GREEN, 1, 11);
        // KeyYes = false;
        // while (KeyYes != true)
        // {
        //     wait(1);
        //     if (__coreshell_currentKey[0] == 0x1C)
        //         KeyYes = true;
        // }
        // __coreshell_install_stage2();
        while(1);
    }
    else
    {
        __tunnel_shutdown();
    }
}

void __coreshell_onDesktop(coreshell_user_t *user) {

}

void __coreshell_loginscreen() {
    // char *ubuffer = (char *)calloc(16);
    // char *pbuffer = (char *)calloc(16);
    // uloop:
    // __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    // puts_gui("Login", COLOR_BLACK, alignText("Login").x, 1);
    // puts_gui("Please, enter username", COLOR_DARK_GREEN, 1, 3);
    // puts_gui("Created users:", COLOR_DARK_GREEN, 2, 6);
    // int i = 0;
    // int ix = 18;
    // while(i < 4) {
    //     if(!__coreshell_settings->users[i].avaliable) {
    //         __gui_drawInputBar((vector2d_t){ix, 6}, __coreshell_settings->users[i].name, 14);
    //         ix += 15;
    //     }
    //     i++;
    // }
    // int bufferI = 0;
    // bool isEnter = false;
    // __gui_drawInputBar((vector2d_t){1, 4}, ubuffer, 16);
    // putc_gui(12, COLOR_DARK_GRAY, 1, 4);
    // while(!isEnter) {
    //     wait(1);
    //     if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
    //         isEnter = true;
    //     } else if (__coreshell_currentKey[0] == 0x0E) {
    //         if(bufferI != 0) bufferI--;
    //         ubuffer[bufferI] = 0;
    //         __gui_drawInputBar((vector2d_t){1, 4}, ubuffer, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 16){
    //         ubuffer[bufferI] =__coreshell_currentKey[1];
    //         bufferI++;
    //         __gui_drawInputBar((vector2d_t){1, 4}, ubuffer, 16);
    //         putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //     }
    // }
    // i = 0;
    // coreshell_user_t *user = NULL;
    // while(i < 4) {
    //     if(strncmp(ubuffer, __coreshell_settings->users[i].name, 16) == 0) {
    //         user = &__coreshell_settings->users[i];
    //     }
    //     i++;
    // }
    // if(user == NULL) {
    //     memset(ubuffer, 0, 16);
    //     jmp uloop;
    // }
    // bool isIncorrect = false;
    // ploop:
    // isEnter = false;
    // bufferI = 0;
    // if(user->permissions.loginWithoutPassword) {
    //     free(pbuffer);
    //     free(ubuffer);
    //     __coreshell_onDesktop(user);
    // } else {
    //     __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    //     puts_gui("Login", COLOR_BLACK, alignText("Login").x, 1);
    //     puts_gui("Please, enter password", COLOR_DARK_GREEN, 1, 3);
    //     if(isIncorrect) puts_gui("Password is incorrect!", COLOR_RED, 1, 6);
    //     __gui_drawInputBar((vector2d_t){1, 4}, pbuffer, 16);
    //     putc_gui(12, COLOR_DARK_GRAY, 1, 4);
    //     while(!isEnter) {
    //         wait(1);
    //         if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
    //             isEnter = true;
    //         } else if (__coreshell_currentKey[0] == 0x0E) {
    //             if(bufferI != 0) bufferI--;
    //             pbuffer[bufferI] = 0;
    //             __gui_drawInputBar((vector2d_t){1, 4}, pbuffer, 16);
    //             putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //         } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 16){
    //             pbuffer[bufferI] =__coreshell_currentKey[1];
    //             bufferI++;
    //             __gui_drawInputBar((vector2d_t){1, 4}, pbuffer, 16);
    //             putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
    //         }
    //     }
    //     if(strncmp(pbuffer, user->password, 16) == 0) {
    //         free(ubuffer);
    //         free(pbuffer);
    //         __coreshell_onDesktop(user);
    //     } else {
    //         memset(pbuffer, 0, 16);
    //         isIncorrect = true;
    //         jmp ploop;
    //     }
    // }
}

void __coreshell_init_coreExecuter()
{
    //__coreshell_drawtasks = (drawtask_t *)calloc(1024 * sizeof(drawtask_t));
    int i = 0;
    while (i < 1024)
    {
        __coreshell_drawtasks[i].state = Unused;
        i++;
    }
    i = 0;
    //__coreshell_settings = __coreshell_createSettings();

    if (__ide_devices[0].connected)
    {
        // if(__fs_readFATCheck(0)) {
        //     printf(COLOR_DARK_GREEN, 1, 1, "Good. %d", __fs_getFATType(0));
        //     while(1);
        // } else {
        //     printf(COLOR_RED, 1, 1, "Bad.");
        //     while(1);
        // }
        if(__fs_tunnelFindFS(0)) {
            printf(COLOR_DARK_GREEN, 1, 1, "Good.");
            __coreshell_install_stage1();
            while(1);
        } else {
            __coreshell_install_stage1();
        }
    }
    __tunnel_shutdown();
}
void __coreshell_init_coreIOHandler()
{
    while (1)
    {
        wait(1);
        if (!(inb(KPS2_SR) & KPS2_OB))
        {
            __coreshell_currentKey[0] = 0;
            __coreshell_currentKey[1] = 0;
        }
        else
        {
            uint8_t scancode = inb(KPS2_DP);
            if (scancode == 0)
            {
                __coreshell_currentKey[0] = 0;
                __coreshell_currentKey[1] = 0;
            }
            else
            {
                __coreshell_currentKey[0] = scancode;
                __coreshell_currentKey[1] = __keyboard_ps2_lookuptable[scancode];
            }
        }
    }
}
void __coreshell_init_coreRenderer()
{
    while(!__coreshell_requestFormat);
    if(!__coreshell_inGUI) {
        fsInstance = __fs_tunnelCreateFS(&__coreshell_percentage, 0);
        __coreshell_requestFormat = false;
    }
    while (1)
    {
        wait(5);
    }
}

void __coreshell_init_all()
{
    assert(tunnelos_sysinfo.cores >= 3);
    int cid = __tools_get_cpu() - 1;
    switch (cid)
    {
    case 0:
    {
        __coreshell_init_coreExecuter();
        break;
    }
    case 1:
    {
        __coreshell_init_coreIOHandler();
        break;
    }
    case 2:
    {
        __coreshell_init_coreRenderer();
        break;
    }
    default:
    {
        while (1)
            ;
    }
    }
}
#pragma GCC diagnostic pop