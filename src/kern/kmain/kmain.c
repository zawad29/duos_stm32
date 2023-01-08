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
#include <timer.h>
#include <types.h>
#include <syscall.h>
#include <unistd.h>


 // uint32_t debug1; uint32_t debug2;


 /// @brief data structures and variables for testing context switching
uint32_t curr_task;
uint32_t next_task;
uint32_t main_stack[1024], task0_stack[1024], task1_stack[1024], task2_stack[1024], task3_stack[1024];
TCB_TypeDef TCB[MAX_TASKS];	// Ready Queue for multitasking
uint32_t* PSP_Array[MAX_TASKS];
static uint32_t count = 0;
#define MAX_COUNT 10000000
void task0(void);
void task1(void);
void task2(void);
void task3(void);
void thread_init(void);
void demo_svc(void);

// __attribute__((naked)) void switch_to_psp() {
// 	asm("mrs r0, control");
// 	asm("orrs r0, r0, #0x3");
// 	asm("msr control, r0");
// 	asm("ISB");
// 	asm("BX LR");
// }

void kmain(void)
{
	__sys_init();


	thread_init();		// context switching demo 

	// demo_svc();		// svc call demo, change PendSV_Handler to blank

	while (1);

}

void demo_svc(void) {
	__sysTick_enable();
	//************* switch to thread mode **************************
	uint32_t* PSP_main = &main_stack[1023] - 16;
	*(PSP_main + 15) = DUMMY_XPSR;
	__asm volatile("MSR PSP, %0" : : "r" (PSP_main) : );
	__set_control(3);
	/***************************************************/
	printf("hello ");
	printf("everyone\n");
	// for (int i = 0; i < 10; i++) {
	// 	printf("%d", i);
	// }
	int llsds;
	uint8_t strr[50];
	printf("input an integer: ");
	scanf("%d ", &llsds);
	printf("llsd = %d\n", llsds);
	printf("input a string: ");
	scanf("%s", strr);
	printf("%s\n", strr);
	printf("end\n");
	printf("rebooting...\n");
	for (int i = 0; i < 999999; i++);
	printf("Time Elapsed %d ms\n", getTime());
	reboot();
}

// void demo_svc() {
// 	printf("oke\n");
// }

void thread_init() {
	// initialize TCB
	for (int i = 0; i < MAX_TASKS; i++) {
		TCB[i].digital_sinature = 0x00000001;
		TCB[i].execution_time = 0;
		TCB[i].waiting_time = 0;
		TCB[i].magic_number = 0xFECABAA0;
		TCB[i].status = TASK_READY_STATE;
		TCB[i].task_id = 1000 + i;
	}
	/***************************************************/
	// PSP_Array[0] = &task0_stack[1023];
	// *(PSP_Array[0] - 1) = 0x01000000;
	// *(PSP_Array[0] - 2) = task0;
	// PSP_Array[0] = &task0_stack[1023] - 16;

	TCB[0].psp = &task0_stack[1023] - 16;
	*(TCB[0].psp + 14) = task0;
	*(TCB[0].psp + 15) = DUMMY_XPSR;

	/**************************************************/
	// PSP_Array[1] = &task1_stack[1023];
	// *(PSP_Array[1] - 1) = 0x01000000;
	// *(PSP_Array[1] - 2) = task1;
	// PSP_Array[1] = &task1_stack[1023] - 16;

	TCB[1].psp = &task1_stack[1023] - 16;
	*(TCB[1].psp + 14) = task1;
	*(TCB[1].psp + 15) = DUMMY_XPSR;

	/*************************************************/
	// PSP_Array[2] = &task2_stack[1023];
	// *(PSP_Array[2] - 1) = 0x01000000;
	// *(PSP_Array[2] - 2) = task2;
	// PSP_Array[2] = &task2_stack[1023] - 16;

	TCB[2].psp = &task2_stack[1023] - 16;
	*(TCB[2].psp + 14) = task2;
	*(TCB[2].psp + 15) = DUMMY_XPSR;

	/**************************************************/
	// PSP_Array[3] = &task3_stack[1023];
	// *(PSP_Array[3] - 1) = 0x01000000;
	// *(PSP_Array[3] - 2) = task3;
	// PSP_Array[3] = &task3_stack[1023] - 16;

	TCB[3].psp = &task3_stack[1023] - 16;
	*(TCB[3].psp + 14) = task3;
	*(TCB[3].psp + 15) = DUMMY_XPSR;

	/****************************************************/

		// initialize with task 0
	curr_task = 0;
	// __asm volatile("MSR PSP, %0" : : "r" (PSP_Array[curr_task]) : );
	//TODO: switch psp_array to ready_queue TCB
	__asm volatile("MSR PSP, %0" : : "r" (TCB[curr_task].psp) : );
	__sysTick_enable();
	__set_control(3);

	int llsds;
	uint8_t strr[50];
	printf("input an integer: ");
	scanf("%d ", &llsds);
	printf("llsd = %d\n", llsds);
	printf("input a string: ");
	scanf("%s", strr);
	printf("%s\n", strr);
	printf("end of svc demo...\n");
	printf("starting multitasking demo...\n");
	printf("Time Elapsed %d ms\n", getTime());
	task0();
	printf("end of tasks\n");
}

//TODO: change pendsv handler to blank for SVC demo.
__attribute__((naked)) void PendSV_Handler(void)
{
	// Save current context
	__asm volatile("MRS R0, PSP"); // Get current process stack pointer value
	__asm volatile("STMDB R0!, {R4-R11}"); // Save R4 to R11 in task stack (8 regs) Other registers auto pushed by uC
	__asm volatile("LDR R2, [%0]" : : "r" (&curr_task) : ); // Get current task ID
	// __asm volatile("STR R0, [%0]" : : "r" (&PSP_Array[curr_task]) : ); // Save PSP of curr_task into PSP_array
	__asm volatile("STR R0, [%0]" : : "r" (&TCB[curr_task].psp) : ); // Save PSP of curr_task into PSP_array
	// Load next context
	// next_task = (curr_task + 1) % MAX_TASKS;
	__asm volatile("LDR R4, [%0]" : : "r" (&next_task) : ); // Get next task ID
	__asm volatile("STR R4, [%0]" : : "r" (&curr_task) : ); // Set curr_task = next_task
	// __asm volatile("LDR R0, [%0]" : : "r" (&PSP_Array[next_task]) : ); // Load PSP value from PSP_array
	__asm volatile("LDR R0, [%0]" : : "r" (&TCB[next_task].psp) : ); // Load PSP value from PSP_array
	__asm volatile("LDMIA R0!, {R4-R11}"); // Load R4 to R11 from task stack (8 regs) Other registers auto poped by uC
	__asm volatile("MSR PSP, R0"); // Set PSP to next task
	__asm volatile("BX LR");
}
/**/
// void PendSV_Handler(void) {}











/*************************************/
void task0(void)
{
	uint32_t value = 0;
	uint32_t inc_count = 0;
	while (1) {
		value = count;
		value++;
		if (value != count + 1) { // we check is if some other task(s) increased the count
			printf("Error %d != %d\n", value, count + 1); /* It is an SVC call*/
			exit();
		}
		else {
			count = value;
			inc_count++;
		}
		if (count >= MAX_COUNT) {
			/* display how many increments it has successfully done!! */
			uint16_t task_id = getpid(); /* It is an SVC call*/
			// uint16_t task_id = 1000 + curr_task;
			printf("Total increment done by task %d is: %d\n", task_id, inc_count);
			printf("Task %d Time Elapsed %d ms\n", task_id, getTime());
			break;
		}
		// printf("Task 0: Time Elapsed %d ms\n", getTime());

	}
	exit();
	while (1);
}

void task1(void)
{
	uint32_t value = 0;
	uint32_t inc_count = 0;
	while (1) {
		value = count;
		value++;
		if (value != count + 1) { // we check is if some other task(s) increased the count
			printf("Error %d != %d\n", value, count + 1); /* It is an SVC call*/
			yield();
		}
		else {
			count = value;
			inc_count++;
		}
		if (count >= MAX_COUNT) {
			/* display how many increments it has successfully done!! */
			uint16_t task_id = getpid(); /* It is an SVC call*/
			// uint16_t task_id = 1000 + curr_task;
			printf("Total increment done by task %d is: %d\n", task_id, inc_count);
			printf("Task %d Time Elapsed %d ms\n", task_id, getTime());
			break;
		}
		// printf("Task 1: Time Elapsed %d ms\n", getTime());
	}exit();
	while (1);
}

void task2(void)
{
	uint32_t value = 0;
	uint32_t inc_count = 0;
	while (1) {
		value = count;
		value++;
		if (value != count + 1) { // we check is if some other task(s) increased the count
			printf("Error %d != %d\n", value, count + 1); /* It is an SVC call*/
			yield();
		}
		else {
			count = value;
			inc_count++;
		}
		if (count >= MAX_COUNT) {
			/* display how many increments it has successfully done!! */
			uint16_t task_id = getpid(); /* It is an SVC call*/
			// uint16_t task_id = 1000 + curr_task;
			printf("Total increment done by task %d is: %d\n", task_id, inc_count);
			printf("Task %d Time Elapsed %d ms\n", task_id, getTime());
			break;
		}
		// printf("Task 2: Time Elapsed %d ms\n", getTime());
	}exit();
	while (1);
}
void task3(void)
{
	uint32_t value = 0;
	uint32_t inc_count = 0;
	while (1) {
		value = count;
		value++;
		if (value != count + 1) { // we check is if some other task(s) increased the count
			printf("Error %d != %d\n", value, count + 1); /* It is an SVC call*/
			yield();
		}
		else {
			count = value;
			inc_count++;
		}
		if (count >= MAX_COUNT) {
			/* display how many increments it has successfully done!! */
			uint16_t task_id = getpid(); /* It is an SVC call*/
			// uint16_t task_id = 1000 + curr_task;
			printf("Total increment done by task %d is: %d\n", task_id, inc_count);
			printf("Task %d Time Elapsed %d ms\n", task_id, getTime());
			break;
		}
		// printf("Task 3: Time Elapsed %d ms\n", getTime());
	}exit();
	while (1);
}

