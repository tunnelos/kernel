#include "../include/serial.h"

bool __serial_lock  = false;
bool __serial_lock2 = false;

int __serial_got_signal() {
    return inb(PORT + 5) & 1;
}
char __serial_read() {
    while(__serial_got_signal() == 0);
    return inb(PORT);
}
int __serial_transmit_check(){
    return inb(PORT + 5) & 0x20;
}
void __serial_write_char(char a){
    while(__serial_transmit_check() == 0);
    outb(PORT, a);
}
void __serial_write_raw(const char *data, uint64_t size) {
    while(__serial_lock2);
    __serial_lock2 = true;
    for (uint64_t i = 0; i < size; i++) __serial_write_char(data[i]);
    __serial_lock2 = false;
    return;
}
void __serial_write_fmt(const char *fmt, ...) {
    if(__serial_lock) while(__serial_lock);
    __serial_lock = true;

    va_list ap;
    va_start(ap, fmt);
    int i = 0;
    int len = strlen(fmt);
    bool setlong = false;
    
    for(; i < len; i++) {
        switch(fmt[i]) {
            case '%': {
                switch(fmt[i + 1]) {
                    case 'c': {
                        char arg = va_arg(ap, int);
                        __serial_write_char(arg);
                        i += 2;
                        break;
                    }
                    case 's': {
                        char *arg = va_arg(ap, char *);
                        __serial_write_raw(arg, strlen(arg));
                        i += 2;
                        break;
                    }
                    case 'l': {
                        setlong = true;
                        i += 2;
                        break;
                    }
                    case 'i':
                    case 'd': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            if(!arg) __serial_write_char('0');
                            else {
                                char buffer[20];
                                __serial_write_raw(itoalong(arg, buffer, 10, 0, 0, 0, false), strlen(itoalong(arg, buffer, 10, 0, 0, 0, false)));
                            }
                            i += 2;
                        } else {
                            uint32_t arg = va_arg(ap, uint32_t);
                            if(!arg) __serial_write_char('0');
                            else {
                                char buffer[20];
                                __serial_write_raw(itoa(arg, buffer, 10, 0, 0, 0, false), strlen(itoa(arg, buffer, 10, 0, 0, 0, false)));
                            }
                            i += 2;
                        }
                        break;
                    }
                    case 'x':
                    case 'X': {
                        if(setlong) {
                            setlong = false;
                            uint64_t arg = va_arg(ap, uint64_t);
                            char buffer[20];
                            __serial_write_raw(itoalong(arg, buffer, 16, 0, 0, 0, false), strlen(itoalong(arg, buffer, 16, 0, 0, 0, false)));
                            i += 2;
                        } else {
                            uint32_t arg = va_arg(ap, uint32_t);
                            char buffer[20];
                            __serial_write_raw(itoa(arg, buffer, 16, 0, 0, 0, false), strlen(itoa(arg, buffer, 16, 0, 0, 0, false)));
                            i += 2;
                        }
                        break;
                    }
                    case 'o': {
                        uint32_t arg = va_arg(ap, uint32_t);
                        char buffer[20];
                        __serial_write_raw(itoa(arg, buffer, 8, 0, 0, 0, false), strlen(itoa(arg, buffer, 8, 0, 0, 0, false)));
                        break;
                    }
                    default: {
                        va_end(ap);
                        break;
                    }
                }
            }
            default: {
                __serial_write_char(fmt[i]);
                va_end(ap);
                break;
            }
        }
    }

    __serial_lock = false;

    return;
}