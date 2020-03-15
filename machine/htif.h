// See LICENSE for license details.

#ifndef _RISCV_HTIF_H
#define _RISCV_HTIF_H

#include <stdint.h>

#define TOHOST_DEV(dev) ((uint64_t)(dev) << 56)
#define TOHOST_CMD(cmd) ((uint64_t)(cmd) << 56 >> 8)
#define TOHOST_DATA(cmd) ((uint64_t)(cmd) << 16 >> 16)
#define TOHOST_CMD_DATA(cmd_data) (cmd_data << 8 >> 8)
#define TOHOST_DEV_CMD_DATA(dev, cmd, data) \
  (TOHOST_DEV(dev) | TOHOST_CMD(cmd) | TOHOST_DATA(data))

#define FROMHOST_DEV(fromhost_value) ((uint64_t)(fromhost_value) >> 56)
#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

extern uintptr_t htif;
void query_htif(uintptr_t dtb);
void htif_console_putchar(uint8_t);
int htif_console_getchar(void);
void htif_poweroff(void) __attribute__((noreturn));
uint64_t htif_yield(uint64_t cmd_data);

#endif
