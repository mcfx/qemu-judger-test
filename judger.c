#include <assert.h>
#include <glib.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <qemu-plugin.h>

QEMU_PLUGIN_EXPORT int qemu_plugin_version = QEMU_PLUGIN_VERSION;

static uint64_t insn_count_div;
static uint64_t insn_count_fp;
static uint64_t insn_count;

static uint64_t mem_count_cached;
static uint64_t mem_count;

#define CACHE_SIZE 1024
#define CACHE_WAYS 4
#define CACHE_LOWER_BITS 12

struct cache_entry {
  uint64_t addr;
  uint64_t last_access;
} cache[CACHE_SIZE];

static void vcpu_mem(unsigned int cpu_index, qemu_plugin_meminfo_t meminfo,
                     uint64_t vaddr, void *udata) {
  // printf("mem access: %016lx\n", vaddr);
  mem_count++;
  uint64_t vaddr_hi = vaddr >> CACHE_LOWER_BITS;
  uint64_t cpos = vaddr_hi % (CACHE_SIZE / CACHE_WAYS) * CACHE_WAYS;
  uint64_t least_last_access = -1ul, replace_pos = 0;
  for (int i = 0; i < CACHE_WAYS; i++) {
    if (cache[cpos + i].addr == vaddr_hi) {
      cache[cpos + i].last_access = insn_count;
      mem_count_cached++;
      return;
    }
    if (cache[cpos + i].last_access < least_last_access) {
      least_last_access = cache[cpos + i].last_access;
      replace_pos = cpos + i;
    }
  }
  cache[replace_pos].addr = vaddr_hi;
  cache[replace_pos].last_access = insn_count;
}

static void vcpu_tb_trans(qemu_plugin_id_t id, struct qemu_plugin_tb *tb) {
  size_t n = qemu_plugin_tb_n_insns(tb);
  size_t i;

  for (i = 0; i < n; i++) {
    struct qemu_plugin_insn *insn = qemu_plugin_tb_get_insn(tb, i);
    const unsigned char *data = qemu_plugin_insn_data(insn);
    int c_op = data[0] & 3;
    if (c_op == 3) {
      int opcode = data[0] >> 2 & 0x1f;

      switch (opcode) {
      case 0b01100: // rv32m muldiv
      case 0b01110: // rv64m muldiv
        if ((data[1] & 0x40) && (data[3] & 0xfe) == 0b00000010) {
          qemu_plugin_register_vcpu_insn_exec_inline(
              insn, QEMU_PLUGIN_INLINE_ADD_U64, &insn_count_div, 1);
          // printf("%lu %s\n", insn_count_div, qemu_plugin_insn_disas(insn));
        }
        break;
      case 0b10000: // fmadd
      case 0b10001: // fmsub
      case 0b10010: // fnmsub
      case 0b10011: // fnmadd
      case 0b10100: // op-fp
        qemu_plugin_register_vcpu_insn_exec_inline(
            insn, QEMU_PLUGIN_INLINE_ADD_U64, &insn_count_fp, 1);
        break;
      case 0b00000: // load
      case 0b00001: // load-fp
      case 0b01000: // store
      case 0b01001: // store-fp
        qemu_plugin_register_vcpu_mem_cb(insn, vcpu_mem, QEMU_PLUGIN_CB_NO_REGS,
                                         QEMU_PLUGIN_MEM_RW, NULL);
        break;
      }
    } else {
      if (c_op == 0 || (c_op == 2 && (data[1] >> 5 & 3) != 0)) {
        qemu_plugin_register_vcpu_mem_cb(insn, vcpu_mem, QEMU_PLUGIN_CB_NO_REGS,
                                         QEMU_PLUGIN_MEM_RW, NULL);
        break;
      }
    }

    qemu_plugin_register_vcpu_insn_exec_inline(insn, QEMU_PLUGIN_INLINE_ADD_U64,
                                               &insn_count, 1);
  }
}

static void plugin_exit(qemu_plugin_id_t id, void *p) {
  g_autofree gchar *out = g_strdup_printf(
      "insn: %lu, div: %lu, fp: %lu, mem: %lu, cached: %lu\n", insn_count,
      insn_count_div, insn_count_fp, mem_count, mem_count_cached);
  qemu_plugin_outs(out);
}

QEMU_PLUGIN_EXPORT int qemu_plugin_install(qemu_plugin_id_t id,
                                           const qemu_info_t *info, int argc,
                                           char **argv) {
  qemu_plugin_register_vcpu_tb_trans_cb(id, vcpu_tb_trans);
  qemu_plugin_register_atexit_cb(id, plugin_exit, NULL);
  return 0;
}
