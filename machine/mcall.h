// See LICENSE for license details.

#ifndef _RISCV_SBI_H
#define _RISCV_SBI_H

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8
/* ??D this slot is marked reserved in the legacy SBI extension
 * (see https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc)
 * If we were using a more recent SBI implementation, we could define a
 * vendor specific extension. Since we are using BBL, we simply take
 * one of the reserved extension ids in the legacy SBI extension. */
#define SBI_YIELD 9

#endif
