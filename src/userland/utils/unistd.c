/*
 * Copyright (c) 2022
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <unistd.h>
 /* Write your highlevel I/O details */
#include <stm32_peps.h>
#include <usart.h>
#include <kstring.h>
#include <stdint.h>
#include <stdarg.h>
#include <kunistd.h>
#include <syscall_def.h>
#include <cm4.h>

void reboot(void) {
    // asm("SVC %0" :: "I"(SYS_reboot));
    __svc(SYS_reboot);
}

// __svc(SYS___time) uint32_t getTime(void);
uint32_t getTime() {
    uint32_t t;
    uint32_t t_ptr = (uint32_t)&t;
    __asm volatile("MOV R0, %0" : : "r" (t_ptr) : );
    __svc(SYS___time);
    return t;
}

uint32_t getpid(void) {
    return 69;
}

// static char buffer[255];
// TODO: complete the printf func
void printf(char* format, ...)
{
    //write your code here
    char buffer[255]; // the print buffer
    uint16_t buff_pos = 0;
    char* tr;
    // char* buff = &buffer[0];
    uint32_t i;
    uint8_t* str;
    uint8_t* temp;
    va_list list;
    double dval;
    //uint32_t *intval;
    va_start(list, format);
    for (tr = format;*tr != '\0';tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            buffer[buff_pos++] = *tr;
            // *buff++ = *tr;
            tr++;
        }
        if (*tr == '\0') break;
        tr++;
        switch (*tr)
        {
        case 'c':
            i = va_arg(list, int);
            buffer[buff_pos++] = *tr;
            // *buff++ = *tr;
            break;
        case 'd':
            i = va_arg(list, int);
            if (i < 0)
            {
                buffer[buff_pos++] = '-';
                // *buff++ = '-';
                i *= -1;
            }
            // _USART_WRITE(USART2, (uint8_t*)convert(i, 10));
            temp = convert(i, 10);
            while (*temp != '\0') {
                buffer[buff_pos++] = *temp++;
                // *buff++ = *temp++;
            }
            break;
        case 'o':
            i = va_arg(list, int);
            if (i < 0)
            {
                buffer[buff_pos++] = '-';
                // *buff++ = '-';
                i = -i;
            }
            // _USART_WRITE(USART2, (uint8_t*)convert(i, 8));
            temp = convert(i, 8);
            while (*temp != '\0') {
                buffer[buff_pos++] = *temp++;
                // *buff++ = *temp++;
            }
            break;
        case 'x':
            i = va_arg(list, int);
            if (i < 0)
            {
                buffer[buff_pos++] = '-';
                // *buff++ = '-';
                i = -i;
            }
            // _USART_WRITE(USART2, (uint8_t*)convert(i, 16));
            temp = convert(i, 16);
            while (*temp != '\0') {
                buffer[buff_pos++] = *temp++;
                // *buff++ = *temp++;
            }
            break;
        case 's':
            str = va_arg(list, uint8_t*);
            // _USART_WRITE(USART2, str);
            while (*str != '\0') {
                buffer[buff_pos++] = *str++;
                // *buff++ = *str++;
            }
            break;
        case 'f':
            dval = va_arg(list, double);
            // _USART_WRITE(USART2, (uint8_t*)float2str(dval));
            temp = float2str(dval);
            while (*temp != '\0') {
                buffer[buff_pos++] = *temp++;
                // *buff++ = *temp++;
            }
            break;
        default:
            break;
        }
    }
    va_end(list);

    buffer[buff_pos++] = '\0';
    // *buff++ = '\0';

    //TODO: make system call
    write(STDOUT_FILENO, buffer, 255);
    // kprintf("%s", buffer);
}

//TODO: complete the scanf func
void scanf(char* format, ...)
{
    //write your code here
    va_list list;
    uint8_t* str;
    int* ii;
    double* ff;
    char* ptr;
    uint8_t buff[255];
    ptr = format;
    va_start(list, format);
    while (*ptr)
    {
        /* code */
        if (*ptr == '%') //looking for format of an input
        {
            ptr++;
            switch (*ptr)
            {
            case 'c': //charater
                /* code */
                // *(uint8_t*)va_arg(list, uint8_t*) = UART_GetChar(USART2);
                str = (uint8_t*)va_arg(list, uint8_t*);
                read(STDIN_FILENO, buff, 3);
                *str = buff[0];
                break;
            case 'd': //integer number 
                read(STDIN_FILENO, buff, 50);
                ii = va_arg(list, int*);
                *ii = __str_to_num(buff, 10);
                break;
            case 's': //need to update -- string
                str = (uint8_t*)va_arg(list, uint8_t*);
                read(STDIN_FILENO, buff, 255);
                // do
                // {
                //     *str++ = *buff++;
                // } while (*buff != '\0');
                for (uint16_t i = 0; buff[i] != '\0'; i++) {
                    *str++ = buff[i];
                }
                *str++ = '\0';
                break;
            case 'x': //hexadecimal number
                read(STDIN_FILENO, buff, 50);
                // *(uint32_t*)va_arg(list, uint32_t*) = __str_to_num(buff, 16);
                ii = va_arg(list, int*);
                *ii = __str_to_num(buff, 16);
                break;
            case 'o': //octal number
                read(STDIN_FILENO, buff, 50);
                // *(uint32_t*)va_arg(list, uint32_t*) = __str_to_num(buff, 8);
                ii = va_arg(list, int*);
                *ii = __str_to_num(buff, 8);
                break;
            case 'f': //floating point number
                read(STDIN_FILENO, buff, 50);
                // *(uint32_t*)va_arg(list, double*) = __str_to_num(buff, 10);
                ff = va_arg(list, double*);
                *ff = str2float(buff);
                break;
            default: //rest not recognized
                break;
            }
        }
        ptr++;
    }
    va_end(list);
}



