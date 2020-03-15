// See LICENSE for license details.

#include "htif.h"
#include "atomic.h"
#include "mtrap.h"
#include "fdt.h"
#include "syscall.h"
#include <string.h>

volatile uint64_t *tohost;
volatile uint64_t *fromhost;
uintptr_t htif;
static spinlock_t htif_lock = SPINLOCK_INIT;

#define TOHOST_OFFSET		(0)
#define FROMHOST_OFFSET		(8)

#define TOHOST(base_int)	(uint64_t *)(base_int + TOHOST_OFFSET)
#define FROMHOST(base_int)	(uint64_t *)(base_int + FROMHOST_OFFSET)

int htif_console_getchar()
{
  spinlock_lock(&htif_lock);
  *fromhost = 0;
  *tohost = TOHOST_DEV_CMD_DATA(1, 0, 0);
  uint64_t ack = *fromhost;
  spinlock_unlock(&htif_lock);
  return (int)(FROMHOST_DATA(ack))-1;
}

void htif_console_putchar(uint8_t ch)
{
  spinlock_lock(&htif_lock);
  *fromhost = 0;
  *tohost = TOHOST_DEV_CMD_DATA(1, 1, ch);
  uint64_t ack = *fromhost;
  (void) ack; /* no need for info in ack */
  spinlock_unlock(&htif_lock);
}

void htif_poweroff(void)
{
  while (1) {
    spinlock_lock(&htif_lock);
    *fromhost = 0;
    *tohost = TOHOST_DEV_CMD_DATA(0, 0, 1); /* does not return */
    uint64_t ack = *fromhost;
    (void) ack;
    spinlock_unlock(&htif_lock);
  }
}

uint64_t htif_yield(uint64_t cmd_data)
{
  spinlock_lock(&htif_lock);
  *fromhost = 0;
  *tohost = TOHOST_DEV(2) | TOHOST_CMD_DATA(cmd_data);
  uint64_t ack = *fromhost;
  spinlock_unlock(&htif_lock);
  return ack;
}

struct htif_scan
{
  int compat;
  uint64_t reg;
};

static void htif_open(const struct fdt_scan_node *node, void *extra)
{
  struct htif_scan *scan = (struct htif_scan *)extra;
  memset(scan, 0, sizeof(*scan));
}

static void htif_prop(const struct fdt_scan_prop *prop, void *extra)
{
  struct htif_scan *scan = (struct htif_scan *)extra;
  if (!strcmp(prop->name, "compatible") && fdt_string_list_index(prop, "ucb,htif0") >= 0) {
    scan->compat = 1;
  } else if (!strcmp(prop->name, "reg")) {
    fdt_get_address(prop->node->parent, prop->value, &scan->reg);
  }
}

static void htif_done(const struct fdt_scan_node *node, void *extra)
{
  struct htif_scan *scan = (struct htif_scan *)extra;
  if (!scan->compat || !scan->reg) return;

  htif = 1;
  fromhost = FROMHOST((uintptr_t)scan->reg);
  tohost = TOHOST((uintptr_t)scan->reg);
}

void query_htif(uintptr_t fdt)
{
  struct fdt_cb cb;
  struct htif_scan scan;

  memset(&cb, 0, sizeof(cb));
  cb.open = htif_open;
  cb.prop = htif_prop;
  cb.done = htif_done;
  cb.extra = &scan;

  fdt_scan(fdt, &cb);
}
