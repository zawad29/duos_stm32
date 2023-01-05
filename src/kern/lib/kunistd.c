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
#include <kunistd.h>
 /* Add your functions here */


void write(uint8_t fd, uint8_t* data, uint16_t size) {
    asm("SVC %0" :: "I"(SYS_write));
}
void read(uint8_t fd, uint8_t* data, uint16_t size) {
    asm("SVC %0" :: "I"(SYS_read));
}

void __sys_write(unsigned int* args) {
    uint8_t fd = (uint8_t)args[0];
    uint8_t* data = (uint8_t*)args[1];
    uint16_t size = (uint16_t)args[2];
    if (fd == STDOUT_FILENO) {
        _USART_WRITE(USART2, data);
    }
}

void __sys_read(unsigned int* args) {
    uint8_t fd = (uint8_t)args[0];
    uint8_t* data = (uint8_t*)args[1];
    uint16_t size = (uint16_t)args[2];
    if (fd == STDIN_FILENO) {
        _USART_READ(USART2, data, size);
    }

}

void __sys_reboot() {
    SCB->AIRCR = (0x5FA << 16) | (1 << 2); //system reset test
    for (;;) {}
}

void __sys_setTaskStatus() {}

void __sys_getpid() {}

void __sys_getTime() {}