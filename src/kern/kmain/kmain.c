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

#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
#include <syscall_def.h>
 //#include "../include/float.h"


void delay(uint32_t dly)
{
	uint32_t t_target = __getTime() + dly;
	while (t_target > __getTime())
		;
}

void kmain(void)
{
	__sys_init();

	uint32_t status;
	//uint32_t b=0;
	float x = 50.59;
	uint8_t y = 23, f = 56;
	// x++;
	kprintf("%d %d %f\n", y, f, x);
	kprintf("After Input\n");
	uint8_t p[8] = "1234.34\0";
	x = str2float(p);
	printf("After Input: x=%f\n", x);
	kprintf("Time Elapse %d ms\n", __getTime());

	// __disable_irq();
	// status = __get_PRIMASK();
	// kprintf("%d\n", status);
	// // __enable_irq();
	// __set_PRIMASK(0);
	// status = __get_PRIMASK();
	// kprintf("%d\n", status);
	// __set_BASEPRI(6);
	// status = __get_BASEPRI();
	// printf("%d\n", status);

	uint32_t ssssdas = 4545;

	// asm("SVC #50");
	printf("hello\n");

	printf("%s", "everyone\n");


	for (int i = 0; i < 10; i++) {
		kprintf("%d", i);
	}

	int llsds;
	scanf("%d", &llsds);
	printf("llsd = %d\n", llsds);

	// uint8_t* pqpqp;
	// kscanf("%s", pqpqp);
	// printf("%s\n", pqpqp);
	printf("end\n");

	asm("SVC %0" :: "I"(SYS_reboot));

	kprintf("system reset...");

	while (1) {
		//	kprintf((uint8_t*)"%d",(uint8_t*)a);
		//	kscanf((uint8_t*)"%d",(uint8_t*)b);
		//	kprintf((uint8_t*)"%d",(uint8_t*)b);
		//	a++;
		//	b++;
			//you can change the following line by replacing a delay function
			//for(uint32_t i=0;i<100000000;i++){kprintf("Time Elapse %d ms\n",__getTime());}	
	}
}



// void USART2_Handler(void) {
// 	// uint8_t* buff;
// 	// _USART_READ_STR(USART2, buff, 50);
// 	// kprintf("%s", buff);


// 	kprintf("Interrupt: ");
// 	UART_GetChar(USART2);

// 	kprintf("USART2\n");
// }