/******************************************************************************
* lowlevel cpu arch functions of Cortex-M0
* Copyright (C) 2015-2016 jiangxiaogang <kerndev@foxmail.com>
*
* This file is part of klite.
* 
* klite is free software; you can redistribute it and/or modify it under the 
* terms of the GNU Lesser General Public License as published by the Free 
* Software Foundation; either version 2.1 of the License, or (at your option) 
* any later version.
*
* klite is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with klite; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
******************************************************************************/
#include "kernel.h"
#include "internal.h"
#include "cpu.h"

#define TCB_OFFSET_STATE		(0)
#define TCB_OFFSET_SP			(4)
#define NVIC_INT_CTRL   		(*((volatile uint32_t*)0xE000ED04))
#define PEND_INT_SET			(1<<28)

void cpu_tcb_init(struct tcb* tcb, uint32_t sp_min, uint32_t sp_max)
{
	tcb->sp = (uint32_t*)(sp_max & 0xFFFFFFF8);	//
	*(--tcb->sp) = 0x01000000;         			// xPSR
	*(--tcb->sp) = (uint32_t)tcb->main;			// PC
	*(--tcb->sp) = (uint32_t)kthread_exit; 		// LR
	*(--tcb->sp) = 0;         					// R12
	*(--tcb->sp) = 0;         					// R3
	*(--tcb->sp) = 0;         					// R2
	*(--tcb->sp) = 0;         					// R1
	*(--tcb->sp) = (uint32_t)tcb->arg;  		// R0
	*(--tcb->sp) = 0;         					// R7
	*(--tcb->sp) = 0;         					// R6
	*(--tcb->sp) = 0;         					// R5
	*(--tcb->sp) = 0;         					// R4
	*(--tcb->sp) = 0;         					// R11
	*(--tcb->sp) = 0;         					// R10
	*(--tcb->sp) = 0;         					// R9
	*(--tcb->sp) = 0;         					// R8
}

void cpu_tcb_switch(void)
{
	NVIC_INT_CTRL = PEND_INT_SET;
}

void SysTick_Handler(void)
{
	kernel_tick();
}
