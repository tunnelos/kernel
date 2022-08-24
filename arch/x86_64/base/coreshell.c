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

drawtask_t *__coreshell_drawtasks;
coreshell_hddsettings_t *__coreshell_settings;
ide_rw_t __coreshell_settings_write;
int __coreshell_currentKey[2];

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
void __gui_drawProgressBar(vector2d_t pos, vector2d_t maxSize, int percentage)
{
    float t = (float)percentage / (float)100;
    float x = t * (float)maxSize.x;
    __gui_drawRectangle(pos, (vector2d_t){(int)x, maxSize.y}, COLOR_GREEN);
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

void __coreshell_install_stage3() {
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    puts_gui("Tunnel OS", COLOR_BLACK, alignText("Tunnel OS").x, 1);
    puts_gui("System will reboot in 5 seconds...", COLOR_RED, 1, 3);
    int p = 0;
    __gui_drawRectangle((vector2d_t){2, 5}, (vector2d_t){76, 3}, COLOR_BLACK);
    __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, p);
    bool reboot = false;
    while(!reboot) {
        if(p == 100) {
            reboot = true;
        }
        wait(100);
        p += 20;
        __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, p);
    }
    __coreshell_settings->installstate = Ready; 
    __ide_get_access(__coreshell_settings_write);
    __tunnel_reboot(); 
}

void __coreshell_install_stage2() {
    __coreshell_settings_write.lba = 0;
    __coreshell_settings_write.sectors = 1;
    __coreshell_settings_write.rw = true;
    __coreshell_settings_write.selector = 0;
    __coreshell_settings_write.drive = __ide_devices[0].drive;
    __coreshell_settings_write.buffer = (uint32_t)__coreshell_settings;

    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    puts_gui("Tunnel OS: Configuration", COLOR_BLACK, alignText("Tunnel OS: Configuration").x, 1);
    __coreshell_settings->users[0].avaliable = false;
    puts_gui("Choose username for your user:", COLOR_DARK_GREEN, 1, 3);
    puts_gui("Press Enter to continue.", COLOR_BLUE, 1, 6);
    char *buffer = __coreshell_settings->users[0].name;
    int bufferI = 0;
    bool isEnter = false;
    __gui_drawInputBar((vector2d_t){1, 4}, buffer, 64);
    putc_gui(12, COLOR_DARK_GRAY, 1, 4);
    while(!isEnter) {
        wait(1);
        if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
            isEnter = true;
        } else if (__coreshell_currentKey[0] == 0x0E) {
            if(bufferI != 0) bufferI--;
            buffer[bufferI] = 0;
            __gui_drawInputBar((vector2d_t){1, 4}, buffer, 64);
            putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
        } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 64){
            buffer[bufferI] =__coreshell_currentKey[1];
            bufferI++;
            __gui_drawInputBar((vector2d_t){1, 4}, buffer, 64);
            putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
        }
    }
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    puts_gui("Tunnel OS: Configuration", COLOR_BLACK, alignText("Tunnel OS: Configuration").x, 1);
    __coreshell_settings->users[0].avaliable = false;
    puts_gui("Type password for your user:", COLOR_DARK_GREEN, 1, 3);
    puts_gui("Press Enter to continue.", COLOR_BLUE, 1, 6);
    buffer = __coreshell_settings->users[0].password;
    char buffer2[16];
    __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
    isEnter = false;
    bufferI = 0;
    while(!isEnter) {
        wait(1);
        if (__coreshell_currentKey[0] == 0x1C && bufferI != 0) {
            isEnter = true;
        } else if (__coreshell_currentKey[0] == 0x0E) {
            if(bufferI != 0) bufferI--;
            buffer[bufferI] = 0;
            buffer2[bufferI] = 0;
            __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
            putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
        } else if (__coreshell_currentKey[1] != '?' && __coreshell_currentKey[1] != 0 && bufferI < 16){
            buffer[bufferI] =__coreshell_currentKey[1];
            buffer2[bufferI] = '*';
            bufferI++;
            __gui_drawInputBar((vector2d_t){1, 4}, buffer2, 16);
            putc_gui(12, COLOR_DARK_GRAY, 1 + bufferI, 4);
        }
    }
    __coreshell_install_stage3();
}

void __coreshell_install_stage1()
{
    __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
    puts_gui("Tunnel OS", COLOR_BLACK, alignText("Tunnel OS").x, 1);
    puts_gui("Do you want to format hard drive?", COLOR_BLACK, 1, 3);
    puts_gui(" - Yes: press Enter", COLOR_RED, 1, 4);
    puts_gui(" - No : press Right Shift", COLOR_GREEN, 1, 5);
    bool KeyYes = false;
    bool KeyNo = false;
    int i = 0;
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
        puts_gui("Tunnel OS: Drive formatting", COLOR_BLACK, alignText("Tunnel OS: Drive formatting").x, 1);
        puts_gui("Formatting hard drive...", COLOR_RED, 1, 3);
        int sectorsToFormat = (__ide_devices[0].size * 1024 / 512 / 2);
        i = 0;

        float p1 = 0;
        float p2 = 0;
        __gui_drawRectangle((vector2d_t){2, 5}, (vector2d_t){76, 3}, COLOR_BLACK);
        __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, p1);
        int m = sectorsToFormat;
        void *blankData = malloc(512);
        while (i < m)
        {
            p1 = ((float)i / (float)m) * (float)100;
            if (p2 != p1)
            {
                p2 = p1;
                __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, (int)p1);
            }
            __coreshell_settings_write.lba = i;
            __coreshell_settings_write.buffer = (uint32_t)blankData;
            __ide_get_access(__coreshell_settings_write);
            i++;
        }
        free(__coreshell_settings);
        __coreshell_settings = __coreshell_createSettings();
        __coreshell_settings->installstate = NotConfigured;
        __coreshell_settings_write.lba = 0;
        __coreshell_settings_write.buffer = (uint32_t)__coreshell_settings;
        __ide_get_access(__coreshell_settings_write);
        free(blankData);
        __gui_drawRectangle((vector2d_t){1, 3}, (vector2d_t){25, 1}, COLOR_WHITE);
        puts_gui("Format complete", COLOR_GREEN, 1, 3);
        __gui_drawProgressBar((vector2d_t){2, 5}, (vector2d_t){76, 3}, 100);
        puts_gui("Let's configure this installation ", COLOR_BLACK, 1, 10);
        puts_gui("Press Enter to continue", COLOR_DARK_GREEN, 1, 11);
        KeyYes = false;
        while (KeyYes != true)
        {
            wait(1);
            if (__coreshell_currentKey[0] == 0x1C)
                KeyYes = true;
        }
        __coreshell_install_stage2();
    }
    else
    {
        __gui_drawRectangle((vector2d_t){0, 0}, (vector2d_t){80, 30}, COLOR_WHITE);
        puts_gui("Tunnel OS: Drive formatting", COLOR_BLACK, alignText("Tunnel OS: Drive formatting").x, 1);
        puts_gui("Exiting...", COLOR_GREEN, 1, 3);
    }
}

void __coreshell_init_coreExecuter()
{
    __coreshell_drawtasks = (drawtask_t *)malloc(1024 * sizeof(drawtask_t));
    int i = 0;
    while (i < 1024)
    {
        __coreshell_drawtasks[i].state = Unused;
        i++;
    }
    i = 0;
    __coreshell_settings = __coreshell_createSettings();

    if (__ide_devices[0].connected)
    {
        // Check if settings are already written and are they corrupted or not

        __coreshell_settings_write.buffer = (uint32_t)((void *)__coreshell_settings);
        __coreshell_settings_write.lba = 0;
        __coreshell_settings_write.sectors = 1;
        __coreshell_settings_write.rw = false;
        __coreshell_settings_write.selector = 0;
        __coreshell_settings_write.drive = __ide_devices[0].drive;
        __ide_get_access(__coreshell_settings_write);

        bool toPrepare = false;

        if (strncmp(__coreshell_settings->signature, "TUNNEL CORESHELL", 16) == 1)
        {
            // They are corrupted
            __coreshell_settings_write.rw = true;
            toPrepare = true;
        }

        if (toPrepare)
        {
            __coreshell_install_stage1();
        }
        else
        {
            if(__coreshell_settings->installstate == NotConfigured) {
                __coreshell_install_stage2();
            } else if (__coreshell_settings->installstate == NotInstalled) {
                __coreshell_install_stage1();
            } else {
                printf(COLOR_GREEN, 1, 1, "It works!");
            }
        }
    }
    else
    {
        free(__coreshell_settings);
    }

    int secs = 0;
    while (1)
    {
        wait(1000 / 10);
        secs++;
    }
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