#pragma once
#include "./api_definitions.h"

void (* __api_init)();
const char *(*__arch_getarch)();
const char *(* __arch_getenvironment)();
void (* __avx_init)();
int (* digitcount)(unsigned int num);
int (* __cmos_getUpdate)();
unsigned char (* __cmos_getRTCreg)(unsigned char reg);
void (* __cmos_getRTC)();
void (* __cmos_read)(unsigned char *data_array);
void (* __coreshell_init_coreRenderer)();
void (* __coreshell_init_coreIOHandler)();
void (* __coreshell_init_coreExecuter)();
void (* __coreshell_install_stage1)();
void (* __coreshell_install_stage2)();
void (* __coreshell_install_stage3)();
coreshell_hddsettings_t *(* __coreshell_createSettings)();
vector2d_t (* alignText)(const char *text);
void (* __gui_drawRectangle)(vector2d_t pos, vector2d_t size, int color);
void (* __gui_drawInputBar)(vector2d_t pos, const char *buffer, int maxSymbols);
void (* __gui_drawProgressBar)(vector2d_t pos, vector2d_t maxSize, int percentage, int color);
void (* __gui_drawImage24)(BMPImage *image, vector2d_t pos);
void (* __gui_drawImage32)(BMPImage *image, vector2d_t pos);
void (* __coreshell_init_all)();
void (* __coreshell_onDesktop)(coreshell_user_t *user);
void (* __coreshell_loginscreen)();
void (* cpptest_test00)();
int (* __cpuid_check_sse)();
int (* __cpuid_check_avx)();
int (* __cpuid_check_avx2)();
int (* __cpuid_check_avx512)();
int (* strlen)(const char *str);
char *(* tolower)(char *data);
char (* tolowerchar)(char data);
int (* strncmp)(const char *s1, const char *s2, size_t n);
int (* strcmp)(const char *s1, const char *s2);
char *(* strcpy)(char *dest, const char *src);
char *(* strchr)(const char *s, char c);
char *(* strrchr)(const char *s, char c);
double (* strtod)(const char *nptr, char **endptr);
char *(* strcat)(char *dest, const char *src);
char *(* strncat)(char *dest, const char *src, size_t n);
void (* __desktop_add_task)(string task);
void (* __desktop_terminate_task)(string task);
void (* __desktop_render_tasks)();
int (* __desktop_get_current_tasks)();
void (* __desktop_init)();
void (* __encoder_fill_chartable)();
uint16_t *(* __encoder_ascii2ct)(const char *data);
bool (* __fpu_check)();
void *(* __fs_makeSectorAction)(int sID, int sSize, void *buffer, enum SectorAction action, uint8_t drive);
bool (* __fs_tunnelFindFS)(uint8_t drive);
tunnelfs_t (* __fs_tunnelCreateFS)(int *percentage, uint8_t drive);
int (* __fs_tunnelFindFileMeta)(char *name, char *extension, tunnelfs_t fsInstance);
uint8_t *(* __fs_tunnelReadDataFromMeta)(int id, tunnelfs_t fsInstance);
int (* __fs_tunnelAllocateFile)(char *name, char *extension, int sSize, tunnelfs_t fsInstance);
void (* __fs_tunnelFreeFile)(int fID, tunnelfs_t fsInstance);
void (* __fs_tunnelSaveFile)(int fID, uint8_t *buffer, tunnelfs_t fsInstance);
bool (* __fs_readFATCheck)(uint8_t drive);
enum FATType (* __fs_getFATType)(uint8_t drive);
halcategory_t *(* __hal_newcategory)(halmember_t *members, const char *name);
halmember_t   *(* __hal_newmember)  (unitype_t *functions, unitype_t *variables, const char *name);
void (* __ide_init)(uint32_t *bars);
uint8_t (* __ide_read)(uint8_t channel, uint8_t reg);
void (* __ide_write)(uint8_t channel, uint8_t reg, uint8_t data);
void (* __ide_read_buffer)(uint8_t channel, uint8_t reg, uint32_t *buffer, uint32_t quads);
uint8_t (* __ide_polling)(uint8_t channel, bool advanced);
void (* __ide_error)(uint32_t drive, uint8_t err);
void (* __ide_get_access)(ide_rw_t data);
void (* __idt_exception_handler)(int interrupt_id);
void (* __idt_interrupt_handler)(int interrupt_id);
void (* __idt_set_descriptor)(uint8_t vector, void *isr, uint8_t flags);
void (* __idt_init)();
void (* __cli)();
void (* __sti)();
bool (* __keyboard_ps2_init)();
void (* __keyboard_ps2_append_cin)(char c);
void (* _start)();
void (* __main_core0init)();
int (* abs)(int i);
double (* fabs)(double x);
float (* acos)(float x);
int (* pow)(int x, int y);
int (* floor)(float x);
float (* round)(float number);
float (* sqrt)(float number);
float (* modf)(float x, float *iptr);
float (* acos)(float x);
void *(* memcpy)(void *dest, const void *src, size_t n);
void (* memset)(void *dest, int c, size_t n);
void (* __mm_fillblocks)();
int (* __mm_findoffset)(int blocks);
void *(* malloc)(size_t size);
void *(* calloc)(size_t size);
void (* free)(void *address);
tunnel_memory_block_t (* __mm_get_blockinformation)(void *address);
void *(* realloc)(void *address, size_t size);
void (* __network_init)();
void (* __nmi_init)();
void (* __nmi_disable)();
const char *(* pcb)(bool j);
void (* crash)(const char *str, uint16_t id, bool interrupt);
void (* __pic_eoi)(uint8_t irq);
void (* __pic_remap)(int *offset);
extern void (* __pic_disable)();
void (* __pic_mask)(uint8_t irq);
void (* __pic_unmask)(uint8_t irq);
uint16_t (* __pic_get_irq_reg)(int ocw3);
uint16_t (* __pic_get_irr)();
uint16_t (* __pic_get_isr)();
void (* __pit_event_timer)();
uint16_t (* __pit_count)();
void (* __pit_set_count)(uint16_t count);
void (* __pit_init)();
void (* __pit_eoi)();
void (* __placeholder_init)();
unitype_t *(* __placeholder_unitype)(unitype_t *args);
void (* __rtc_init)();
bool (* __serial_init)();
int (* __serial_got_signal)();
char (* __serial_read)();
int (* __serial_transmit_check)();
void (* __serial_write_char)(char a);
void (* __serial_write_raw)(const char *data, uint64_t size);
void (* __serial_write_fmt)(const char *fmt, ...);
int *(* __sort_counting)(int *data, int size);
void (* __sse_init)();
void (* __sse_save)();
unsigned char * (* stb_csample_uint8)(unsigned char *src, int channels, int stride, int width, int height, int x, int y);
int * (* stb_csample_int)(int *src, int channels, int stride, int width, int height, int x, int y);
float * (* stb_csample_float)(float *src, int channels, int stride, int width, int height, int x, int y);
void (* stb_bilerp_uint8)(unsigned char *src, int stride, int width, int height, float x, float y);
void (* stb_bilerp_uint8_3)(unsigned char *src, int stride, int width, int height, float x, float y);
void (* stb_bilerp_uint8_4)(unsigned char *src, int stride, int width, int height, float x, float y);
void (* stb_bilerp_float)(float *src, int stride, int width, int height, float x, float y);
void (* stb_bilerp_float_3)(float *src, int stride, int width, int height, float x, float y);
void (* stb_bilerp_int)(int *src, int stride, int width, int height, float x, float y);
void (* puts)(const char *str, uint32_t color, int x, int y);
void (* putc)(const char c, uint32_t color, int x, int y);
int (* sprintf)(char *str, const char *fmt, ...);
void (* puts_gui)(const char *str, uint32_t color, int x, int y);
void (* putc_gui)(const char c, uint32_t color, int x, int y);
void (* printf)(uint32_t color, int x, int y, const char *fmt, ...);
char *(* itoa)(int num, char *buffer, int base, int x, int y, int color, bool use_additional);
char *(* itoalong)(uint64_t num, char *buffer, int base, int x, int y, int color, bool use_additional);
char (* itoh)(int num, bool upper, int x, int y, int color, bool use_additional);
char (* itoc)(int num);
char* (* strrev)(char* src);
void (* srand)(int seed);
int (* rand)();
int (* TUNNEL_RANDOM)();
void (* __systemconf_init)();
void (* __test_unitest)();
uint8_t (* inb)(uint16_t port);
uint16_t (* inw)(uint16_t port);
uint32_t (* inl)(uint16_t port);
void (* outb)(uint16_t port, uint8_t val);
void (* outw)(uint16_t port, uint16_t val);
void (* outl)(uint16_t port, uint32_t val);
void (* io_wait)(void);
void (* wait)(uint64_t ms);
void (* wait_ns)(uint128_t ns);
void (* insl)(uint16_t reg, uint32_t *buffer, int quads);
int (* __tools_get_cpu)();
void (* __tools_int_test)(uint8_t interrupt_number);
void (* __tunnel_shutdown)();
void (* __tunnel_reboot)();
void (* push)(int x);
int (* pop)();
unitype_t *(* __uni_create)(const char *variable_template);
void (* __uni_delete)(unitype_t *args);
unitype_t *(* __uni_find)(unitype_t *args, const char *name);

typedef void (*voidf)();

inline void __api_setValues() {
    *(voidf*)&__api_init = (voidf)0xffffffffffe04dd6;
    *(voidf*)&__arch_getarch = (voidf)0;
    *(voidf*)&__arch_getenvironment = (voidf)0;
    *(voidf*)&__avx_init = (voidf)0xffffffffffe09170;
    *(voidf*)&digitcount = (voidf)0xffffffffffe040c8;
    *(voidf*)&__cmos_getUpdate = (voidf)0xffffffffffe09198;
    *(voidf*)&__cmos_getRTCreg = (voidf)0xffffffffffe091c3;
    *(voidf*)&__cmos_getRTC = (voidf)0xffffffffffe091f0;
    *(voidf*)&__cmos_read = (voidf)0xffffffffffe0944f;
    *(voidf*)&__coreshell_init_coreRenderer = (voidf)0xffffffffffe08e21;
    *(voidf*)&__coreshell_init_coreIOHandler = (voidf)0xffffffffffe08d7a;
    *(voidf*)&__coreshell_init_coreExecuter = (voidf)0xffffffffffe08cca;
    *(voidf*)&__coreshell_install_stage1 = (voidf)0xffffffffffe0878d;
    *(voidf*)&__coreshell_install_stage2 = (voidf)0xffffffffffe08782;
    *(voidf*)&__coreshell_install_stage3 = (voidf)0xffffffffffe08777;
    *(voidf*)&__coreshell_createSettings = (voidf)0xffffffffffe082c5;
    *(voidf*)&alignText = (voidf)0xffffffffffe08354;
    *(voidf*)&__gui_drawRectangle = (voidf)0xffffffffffe083d5;
    *(voidf*)&__gui_drawInputBar = (voidf)0xffffffffffe084cc;
    *(voidf*)&__gui_drawProgressBar = (voidf)0xffffffffffe08449;
    *(voidf*)&__gui_drawImage24 = (voidf)0xffffffffffe0857f;
    *(voidf*)&__gui_drawImage32 = (voidf)0xffffffffffe086bf;
    *(voidf*)&__coreshell_init_all = (voidf)0xffffffffffe08e99;
    *(voidf*)&__coreshell_onDesktop = (voidf)0xffffffffffe08cac;
    *(voidf*)&__coreshell_loginscreen = (voidf)0xffffffffffe08cbf;
    *(voidf*)&cpptest_test00 = (voidf)0xffffffffffe08f9b;
    *(voidf*)&__cpuid_check_sse = (voidf)0xffffffffffe090b5;
    *(voidf*)&__cpuid_check_avx = (voidf)0xffffffffffe090db;
    *(voidf*)&__cpuid_check_avx2 = (voidf)0xffffffffffe09101;
    *(voidf*)&__cpuid_check_avx512 = (voidf)0xffffffffffe09127;
    *(voidf*)&strlen = (voidf)0xffffffffffe03ab9;
    *(voidf*)&tolower = (voidf)0xffffffffffe03b38;
    *(voidf*)&tolowerchar = (voidf)0xffffffffffe03b1a;
    *(voidf*)&strncmp = (voidf)0xffffffffffe03e6d;
    *(voidf*)&strcmp = (voidf)0xffffffffffe03fc9;
    *(voidf*)&strcpy = (voidf)0xffffffffffe03ecc;
    *(voidf*)&strchr = (voidf)0xffffffffffe03f04;
    *(voidf*)&strrchr = (voidf)0xffffffffffe03f62;
    *(voidf*)&strtod = (voidf)0xffffffffffe03bba;
    *(voidf*)&strcat = (voidf)0xffffffffffe04090;
    *(voidf*)&strncat = (voidf)0xffffffffffe04001;
    *(voidf*)&__desktop_add_task = (voidf)0xffffffffffe09a3c;
    *(voidf*)&__desktop_terminate_task = (voidf)0xffffffffffe09a8b;
    *(voidf*)&__desktop_render_tasks = (voidf)0xffffffffffe09ab3;
    *(voidf*)&__desktop_get_current_tasks = (voidf)0;
    *(voidf*)&__desktop_init = (voidf)0xffffffffffe098e8;
    *(voidf*)&__encoder_fill_chartable = (voidf)0xffffffffffe0a04c;
    *(voidf*)&__encoder_ascii2ct = (voidf)0xffffffffffe09fbd;
    *(voidf*)&__fpu_check = (voidf)0xffffffffffe082a4;
    *(voidf*)&__fs_makeSectorAction = (voidf)0xffffffffffe04fe4;
    *(voidf*)&__fs_tunnelFindFS = (voidf)0xffffffffffe0506f;
    *(voidf*)&__fs_tunnelCreateFS = (voidf)0xffffffffffe05123;
    *(voidf*)&__fs_tunnelFindFileMeta = (voidf)0xffffffffffe0535d;
    *(voidf*)&__fs_tunnelReadDataFromMeta = (voidf)0xffffffffffe05493;
    *(voidf*)&__fs_tunnelAllocateFile = (voidf)0xffffffffffe0558b;
    *(voidf*)&__fs_tunnelFreeFile = (voidf)0xffffffffffe0580e;
    *(voidf*)&__fs_tunnelSaveFile = (voidf)0xffffffffffe0597a;
    *(voidf*)&__fs_readFATCheck = (voidf)0xffffffffffe04de1;
    *(voidf*)&__fs_getFATType = (voidf)0xffffffffffe04e76;
    *(voidf*)&__hal_newcategory = (voidf)0;
    *(voidf*)&__hal_newmember = (voidf)0;
    *(voidf*)&__ide_init = (voidf)0xffffffffffe06eaf;
    *(voidf*)&__ide_read = (voidf)0xffffffffffe076d1;
    *(voidf*)&__ide_write = (voidf)0xffffffffffe077e2;
    *(voidf*)&__ide_read_buffer = (voidf)0xffffffffffe078f0;
    *(voidf*)&__ide_polling = (voidf)0xffffffffffe07a0f;
    *(voidf*)&__ide_error = (voidf)0xffffffffffe07ab6;
    *(voidf*)&__ide_get_access = (voidf)0xffffffffffe07cc2;
    *(voidf*)&__idt_exception_handler = (voidf)0xffffffffffe06584;
    *(voidf*)&__idt_interrupt_handler = (voidf)0xffffffffffe0666b;
    *(voidf*)&__idt_set_descriptor = (voidf)0xffffffffffe06786;
    *(voidf*)&__idt_init = (voidf)0xffffffffffe06818;
    *(voidf*)&__cli = (voidf)0xffffffffffe09c30;
    *(voidf*)&__sti = (voidf)0xffffffffffe09c32;
    *(voidf*)&__keyboard_ps2_init = (voidf)0xffffffffffe05b29;
    *(voidf*)&__keyboard_ps2_append_cin = (voidf)0xffffffffffe05b48;
    *(voidf*)&_start = (voidf)0xffffffffffe02000;
    *(voidf*)&__main_core0init = (voidf)0xffffffffffe023f9;
    *(voidf*)&abs = (voidf)0xffffffffffe0953d;
    *(voidf*)&fabs = (voidf)0xffffffffffe09559;
    *(voidf*)&acos = (voidf)0xffffffffffe09645;
    *(voidf*)&pow = (voidf)0xffffffffffe09773;
    *(voidf*)&floor = (voidf)0xffffffffffe097be;
    *(voidf*)&round = (voidf)0xffffffffffe095db;
    *(voidf*)&sqrt = (voidf)0xffffffffffe0957d;
    *(voidf*)&modf = (voidf)0xffffffffffe097da;
    *(voidf*)&acos = (voidf)0xffffffffffe09645;
    *(voidf*)&memcpy = (voidf)0xffffffffffe04542;
    *(voidf*)&memset = (voidf)0xffffffffffe045a4;
    *(voidf*)&__mm_fillblocks = (voidf)0xffffffffffe045fe;
    *(voidf*)&__mm_findoffset = (voidf)0xffffffffffe046ea;
    *(voidf*)&malloc = (voidf)0xffffffffffe047ee;
    *(voidf*)&calloc = (voidf)0xffffffffffe047b3;
    *(voidf*)&free = (voidf)0xffffffffffe04bba;
    *(voidf*)&__mm_get_blockinformation = (voidf)0xffffffffffe049a8;
    *(voidf*)&realloc = (voidf)0xffffffffffe04ad2;
    *(voidf*)&__network_init = (voidf)0xffffffffffe090a4;
    *(voidf*)&__nmi_init = (voidf)0xffffffffffe04d74;
    *(voidf*)&__nmi_disable = (voidf)0xffffffffffe04da5;
    *(voidf*)&pcb = (voidf)0xffffffffffe04107;
    *(voidf*)&crash = (voidf)0xffffffffffe04130;
    *(voidf*)&__pic_eoi = (voidf)0xffffffffffe09d40;
    *(voidf*)&__pic_remap = (voidf)0xffffffffffe09d78;
    *(voidf*)&__pic_disable = (voidf)0xffffffffffe09c50;
    *(voidf*)&__pic_mask = (voidf)0xffffffffffe09e75;
    *(voidf*)&__pic_unmask = (voidf)0xffffffffffe09ed3;
    *(voidf*)&__pic_get_irq_reg = (voidf)0xffffffffffe09f34;
    *(voidf*)&__pic_get_irr = (voidf)0xffffffffffe09f95;
    *(voidf*)&__pic_get_isr = (voidf)0xffffffffffe09fa9;
    *(voidf*)&__pit_event_timer = (voidf)0xffffffffffe06e81;
    *(voidf*)&__pit_count = (voidf)0xffffffffffe06de0;
    *(voidf*)&__pit_set_count = (voidf)0xffffffffffe06e38;
    *(voidf*)&__pit_init = (voidf)0xffffffffffe09b7f;
    *(voidf*)&__pit_eoi = (voidf)0xffffffffffe09c34;
    *(voidf*)&__placeholder_init = (voidf)0xffffffffffe11bb6;
    *(voidf*)&__placeholder_unitype = (voidf)0xffffffffffe11bc1;
    *(voidf*)&__rtc_init = (voidf)0xffffffffffe09c57;
    *(voidf*)&__serial_init = (voidf)0;
    *(voidf*)&__serial_got_signal = (voidf)0xffffffffffe05db5;
    *(voidf*)&__serial_read = (voidf)0xffffffffffe05dcf;
    *(voidf*)&__serial_transmit_check = (voidf)0xffffffffffe05df2;
    *(voidf*)&__serial_write_char = (voidf)0xffffffffffe05e0c;
    *(voidf*)&__serial_write_raw = (voidf)0xffffffffffe05e42;
    *(voidf*)&__serial_write_fmt = (voidf)0xffffffffffe05e8a;
    *(voidf*)&__sort_counting = (voidf)0xffffffffffe0a057;
    *(voidf*)&__sse_init = (voidf)0xffffffffffe09150;
    *(voidf*)&__sse_save = (voidf)0xffffffffffe09183;
    *(voidf*)&stb_csample_uint8 = (voidf)0xffffffffffe10dba;
    *(voidf*)&stb_csample_int = (voidf)0xffffffffffe10e59;
    *(voidf*)&stb_csample_float = (voidf)0xffffffffffe10f06;
    *(voidf*)&stb_bilerp_uint8 = (voidf)0xffffffffffe10fb3;
    *(voidf*)&stb_bilerp_uint8_3 = (voidf)0xffffffffffe1118e;
    *(voidf*)&stb_bilerp_uint8_4 = (voidf)0xffffffffffe113b6;
    *(voidf*)&stb_bilerp_float = (voidf)0xffffffffffe115d8;
    *(voidf*)&stb_bilerp_float_3 = (voidf)0xffffffffffe117b7;
    *(voidf*)&stb_bilerp_int = (voidf)0xffffffffffe119e5;
    *(voidf*)&puts = (voidf)0xffffffffffe02469;
    *(voidf*)&putc = (voidf)0xffffffffffe02680;
    *(voidf*)&sprintf = (voidf)0xffffffffffe02b31;
    *(voidf*)&puts_gui = (voidf)0xffffffffffe026bd;
    *(voidf*)&putc_gui = (voidf)0xffffffffffe028fd;
    *(voidf*)&printf = (voidf)0xffffffffffe031df;
    *(voidf*)&itoa = (voidf)0xffffffffffe0293a;
    *(voidf*)&itoalong = (voidf)0xffffffffffe02a22;
    *(voidf*)&itoh = (voidf)0xffffffffffe038f4;
    *(voidf*)&itoc = (voidf)0xffffffffffe03951;
    *(voidf*)&strrev = (voidf)0xffffffffffe03968;
    *(voidf*)&srand = (voidf)0xffffffffffe09cde;
    *(voidf*)&rand = (voidf)0xffffffffffe09cfc;
    *(voidf*)&TUNNEL_RANDOM = (voidf)0xffffffffffe10670;
    *(voidf*)&__systemconf_init = (voidf)0xffffffffffe10665;
    *(voidf*)&__test_unitest = (voidf)0xffffffffffe094ca;
    *(voidf*)&inb = (voidf)0xffffffffffe05b86;
    *(voidf*)&inw = (voidf)0xffffffffffe05ba8;
    *(voidf*)&inl = (voidf)0xffffffffffe05bcc;
    *(voidf*)&outb = (voidf)0xffffffffffe05bed;
    *(voidf*)&outw = (voidf)0xffffffffffe05c12;
    *(voidf*)&outl = (voidf)0xffffffffffe05c37;
    *(voidf*)&io_wait = (voidf)0xffffffffffe05c89;
    *(voidf*)&wait = (voidf)0xffffffffffe05ca3;
    *(voidf*)&wait_ns = (voidf)0xffffffffffe05d04;
    *(voidf*)&insl = (voidf)0xffffffffffe05d5a;
    *(voidf*)&__tools_get_cpu = (voidf)0xffffffffffe05c57;
    *(voidf*)&__tools_int_test = (voidf)0xffffffffffe09c39;
    *(voidf*)&__tunnel_shutdown = (voidf)0xffffffffffe03a5e;
    *(voidf*)&__tunnel_reboot = (voidf)0xffffffffffe03a07;
    *(voidf*)&push = (voidf)0xffffffffffe03a96;
    *(voidf*)&pop = (voidf)0xffffffffffe03aa8;
    *(voidf*)&__uni_create = (voidf)0xffffffffffe1071b;
    *(voidf*)&__uni_delete = (voidf)0xffffffffffe10c5a;
    *(voidf*)&__uni_find = (voidf)0xffffffffffe10d49;
}