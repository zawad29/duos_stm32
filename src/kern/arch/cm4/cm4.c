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

#include <cm4.h>
#include <clock.h>
#include <syscall.h>
volatile static uint32_t __mscount;
/************************************************************************************
* __SysTick_init(uint32_t reload)
* Function initialize the SysTick clock. The function with a weak attribute enables
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) void __SysTick_init(uint32_t reload)
{
    SYSTICK->CTRL &= ~(1 << 0); //disable systick timer
    SYSTICK->VAL = 0; // initialize the counter
    __mscount = 0;
    SYSTICK->LOAD = PLL_N * reload - 1;
    SYSTICK->CTRL |= 1 << 1 | 1 << 2; //enable interrupt and internal clock source
    SYSTICK->CTRL |= 1 << 0; //enable systick counter
}

/************************************************************************************
* __sysTick_enable(void)
* The function enables the SysTick clock if already not enabled.
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/
__attribute__((weak)) void __sysTick_enable(void)
{
    if (SYSTICK->CTRL & ~(1 << 0)) SYSTICK->CTRL |= 1 << 0;
}
__attribute__((weak)) void __sysTick_disable(void)
{
    if (!(SYSTICK->CTRL & ~(1 << 0))) SYSTICK->CTRL &= ~(1 << 0);
}
__attribute__((weak)) uint32_t __getSysTickCount(void)
{
    return SYSTICK->VAL;
}
/************************************************************************************
* __updateSysTick(uint32_t count)
* Function reinitialize the SysTick clock. The function with a weak attribute enables
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) void __updateSysTick(uint32_t count)
{
    SYSTICK->CTRL &= ~(1 << 0); //disable systick timer
    SYSTICK->VAL = 0; // initialize the counter
    __mscount = 0;
    SYSTICK->CTRL |= 1 << 1 | 1 << 2; //enable interrupt and internal clock source
    SYSTICK->LOAD = PLL_N * count;
    SYSTICK->CTRL |= 1 << 0; //enable systick counter
}

/************************************************************************************
* __getTime(void)
* Function return the SysTick elapsed time from the begining or reinitialing. The function with a weak attribute enables
* redefining the function to change its characteristics whenever necessary.
**************************************************************************************/

__attribute__((weak)) uint32_t __getTime(void)
{
    return (__mscount + (SYSTICK->LOAD - SYSTICK->VAL) / (PLL_N * 1000));
}
__attribute__((weak)) void SysTick_Handler()
{
    __mscount += (SYSTICK->LOAD) / (PLL_N * 1000);
}

/**
 *  Enable FPU
 *
 */


void __enable_fpu()
{
    SCB->CPACR |= ((0xF << 20));
}



/*
 *
 *    *********************** NVIC Functions ********************************
 *
*/


uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{

    if ((int32_t)(IRQn) >= 0)
    {
        return (NVIC->IP[((uint32_t)IRQn)] >> 4U);
    }
    else {
        if (IRQn == SysTick_IRQn) {
            return (SCB->SHPR3 >> 4 >> 24) & 0xF;
        }
        else if (IRQn = PendSV_IRQn) {
            return (SCB->SHPR3 >> 4 >> 16) & 0xF;
        }
        else if (IRQn = SVCall_IRQn) {
            return (SCB->SHPR2 >> 4 >> 24) & 0xF;
        }
        else if (IRQn = MemoryManagement_IRQn) {
            return (SCB->SHPR1 >> 4 >> 0) & 0xF;
        }
        else if (IRQn = BusFault_IRQn) {
            return (SCB->SHPR1 >> 4 >> 8) & 0xF;
        }
        else if (IRQn = UsageFault_IRQn) {
            return (SCB->SHPR1 >> 4 >> 16) & 0xF;
        }
        else
            return 0;
    }
}

void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority)
{
    priority = priority & 0xF;
    if ((int32_t)(IRQn) >= 0)
    {
        NVIC->IP[((uint32_t)IRQn)] = (uint8_t)(priority << 4U);
    }
    else
    {
        if (IRQn == SysTick_IRQn) {
            SCB->SHPR3 &= ~((priority << 4U) << 24);
            SCB->SHPR3 |= (priority << 4U) << 24;
        }
        else if (IRQn = PendSV_IRQn) {
            SCB->SHPR3 &= ~((priority << 4U) << 16);
            SCB->SHPR3 |= (priority << 4U) << 16;
        }
        else if (IRQn = SVCall_IRQn) {
            SCB->SHPR2 &= ~((priority << 4U) << 24);
            SCB->SHPR2 |= (priority << 4U) << 24;
        }
        else if (IRQn = MemoryManagement_IRQn) {
            SCB->SHPR1 &= ~((priority << 4U) << 0);
            SCB->SHPR1 |= (priority << 4U) << 0;
        }
        else if (IRQn = BusFault_IRQn) {
            SCB->SHPR1 &= ~((priority << 4U) << 8);
            SCB->SHPR1 |= (priority << 4U) << 8;
        }
        else if (IRQn = UsageFault_IRQn) {
            SCB->SHPR1 &= ~((priority << 4U) << 16);
            SCB->SHPR1 |= (priority << 4U) << 16;
        }
    }
}

void __NVIC_EnableIRQ(IRQn_TypeDef IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        NVIC->ISER[reg_pos] = (1UL << bit_pos);
    }
    else {
        if (IRQn == UsageFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr |= 1 << 18;
            SCB->SHCSR = shcsr;
        }
        else if (IRQn == BusFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr |= 1 << 17;
            SCB->SHCSR = shcsr;
        }
        else if (IRQn == MemoryManagement_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr |= 1 << 16;
            SCB->SHCSR = shcsr;
        }
    }
}

void __NVIC_DisableIRQ(IRQn_TypeDef IRQn)
{
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        NVIC->ICER[reg_pos] = (1UL << bit_pos);
    }
    else {
        if (IRQn == UsageFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr &= ~(1 << 18);
            SCB->SHCSR = shcsr;
        }
        else if (IRQn == BusFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr &= ~(1 << 17);
            SCB->SHCSR = shcsr;
        }
        else if (IRQn == MemoryManagement_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            shcsr &= ~(1 << 16);
            SCB->SHCSR = shcsr;
        }
    }

}

uint32_t __NVIC_getStatus(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        return ((NVIC->ISER[reg_pos] >> bit_pos) & 0x1);
    }
    else {
        if (IRQn == UsageFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            return (shcsr >> 18) & 0x1;
        }
        else if (IRQn == BusFault_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            return (shcsr >> 17) & 0x1;
        }
        else if (IRQn == MemoryManagement_IRQn) {
            uint32_t shcsr = SCB->SHCSR;
            return (shcsr >> 16) & 0x1;
        }
        else return 0;
    }
}

void __set_pending_IRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        NVIC->ISPR[reg_pos] |= (1UL << bit_pos);
    }
    else {
        if (IRQn == NonMaskableInt_IRQn) {
            SCB->ICSR |= 1 << 31;
        }
        else if (IRQn == PendSV_IRQn) {
            SCB->ICSR |= 1 << 28;
        }
        else if (IRQn == SysTick_IRQn) {
            SCB->ICSR |= 1 << 26;
        }
        else if (IRQn == SVCall_IRQn) {
            SCB->SHCSR |= 1 << 15;
        }
        else if (IRQn == UsageFault_IRQn) {
            SCB->SHCSR |= 1 << 12;
        }
        else if (IRQn == MemoryManagement_IRQn) {
            SCB->SHCSR |= 1 << 13;
        }
        else if (IRQn == BusFault_IRQn) {
            SCB->SHCSR |= 1 << 14;
        }
    }
}

void __clear_pending_IRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        NVIC->ICPR[reg_pos] |= (1UL << bit_pos);
    }
    else {
        if (IRQn == PendSV_IRQn) {
            SCB->ICSR |= 1 << 27;
        }
        else if (IRQn == SysTick_IRQn) {
            SCB->ICSR |= 1 << 25;
        }
    }
}

uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        return ((NVIC->ISPR[reg_pos] >> bit_pos) & 0x1);
    }
    else {
        if (IRQn == NonMaskableInt_IRQn) {
            return (SCB->ICSR >> 31) & 0x1;
        }
        else if (IRQn == PendSV_IRQn) {
            return (SCB->ICSR >> 28) & 0x1;
        }
        else if (IRQn == SysTick_IRQn) {
            return (SCB->ICSR >> 26) & 0x1;
        }
        else if (IRQn == UsageFault_IRQn) {
            return (SCB->SHCSR >> 12) & 0x1;
        }
        else if (IRQn == MemoryManagement_IRQn) {
            return (SCB->SHCSR >> 13) & 0x1;
        }
        else if (IRQn == BusFault_IRQn) {
            return (SCB->SHCSR >> 14) & 0x1;
        }
        else if (IRQn == SVCall_IRQn) {
            return (SCB->SHCSR >> 15) & 0x1;
        }
        else {
            return 0;
        }
    }
}

uint32_t __get_active_IRQn(IRQn_TypeDef IRQn) {
    if ((int32_t)(IRQn) >= 0)
    {
        uint32_t reg_pos = (((uint32_t)IRQn) >> 5UL);
        uint32_t bit_pos = (((uint32_t)IRQn) & 0x1FUL);
        return ((NVIC->IABR[reg_pos] >> bit_pos) & 0x1);
    }
    else {
        if (IRQn == MemoryManagement_IRQn) {
            return (SCB->SHCSR >> 0) & 0x1;
        }
        else if (IRQn == BusFault_IRQn) {
            return (SCB->SHCSR >> 1) & 0x1;
        }
        else if (IRQn == UsageFault_IRQn) {
            return (SCB->SHCSR >> 3) & 0x1;
        }
        else if (IRQn == SVCall_IRQn) {
            return (SCB->SHCSR >> 7) & 0x1;
        }
        else if (IRQn == DebugMonitor_IRQn) {
            return (SCB->SHCSR >> 8) & 0x1;
        }
        else if (IRQn == PendSV_IRQn) {
            return (SCB->SHCSR >> 10) & 0x1;
        }
        else if (IRQn == SysTick_IRQn) {
            return (SCB->SHCSR >> 11) & 0x1;
        }
        else {
            return 0;
        }
    }
}




void __disable_irq(void) {
    asm volatile ("CPSID I");
}

void __enable_irq(void) {
    asm volatile ("CPSIE I");
}

void __set_PRIMASK(uint32_t value) {
    value = value & 0x1;
    asm volatile ("MSR primask, %0" : : "r" (value));
}

uint32_t __get_PRIMASK(void) {
    uint32_t value = 0;
    asm volatile ("MRS %0, primask" : "=r" (value) : );
    return value;
}

void __set_BASEPRI(uint32_t value) {
    value = value & 0xF;
    asm volatile ("MSR basepri, %0" : : "r" (value));
}

void __unset_BASEPRI(void) {
    uint32_t value = 0x0;
    asm volatile ("MSR basepri, %0" : : "r" (value));
}

uint32_t __get_BASEPRI(void) {
    uint32_t value = 99;
    asm volatile ("MRS %0, basepri" : "=r" (value));
    return value;
}

void __set_FAULTMASK(uint32_t value) {
    value = value & 0x1;
    asm volatile ("MSR faultmask, %0" : : "r" (value));
}

void __disable_fault_irq(void) {
    asm volatile ("CPSID F");
}

void __enable_fault_irq(void) {
    asm volatile ("CPSIE F");
}

