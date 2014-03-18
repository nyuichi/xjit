#ifndef XJIT_H__
#define XJIT_H__

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef __x86_64__
# error unsupported platform
#endif

#include <stdint.h>

typedef struct xJIT xJIT;
typedef struct xOperand xOperand;
typedef struct xReg xReg;

xJIT *xjit_new();
void xjit_ready(xJIT *);
const void *xjit_getcode(xJIT *);
void xjit_destroy(xJIT *);

#define xjit_ptr(...) xjit_addr_mode(__VA_ARGS__, xjit_bdxs, xjit_bdx, xjit_bd, xjit_b)(__VA_ARGS__)

#define xjit_rax (xjit_rax_())
#define xjit_rbx (xjit_rbx_())
#define xjit_rcx (xjit_rcx_())
#define xjit_rdx (xjit_rdx_())
#define xjit_rsp (xjit_rsp_())
#define xjit_rbp (xjit_rbp_())
#define xjit_rsi (xjit_rsi_())
#define xjit_rdi (xjit_rdi_())
#define xjit_r8 (xjit_r8_())
#define xjit_r9 (xjit_r9_())
#define xjit_r10 (xjit_r10_())
#define xjit_r11 (xjit_r11_())
#define xjit_r12 (xjit_r12_())
#define xjit_r13 (xjit_r13_())
#define xjit_r14 (xjit_r14_())
#define xjit_r15 (xjit_r15_())

enum {
  XJIT_CALL_OPERAND,
  XJIT_CALL_ABS,
  XJIT_CALL_LABEL
};

enum {
  XJIT_JMP_ABS,
  XJIT_JMP_LABEL
};

void xjit_push(xJIT *, const xOperand *);
void xjit_pop(xJIT *, const xOperand *);
void xjit_mov(xJIT *, const xOperand *, const xOperand *);
void xjit_call(xJIT *, const void *, uint8_t);
void xjit_ret(xJIT *);
void xjit_inc(xJIT *, const xOperand *);
void xjit_dec(xJIT *, const xOperand *);
void xjit_add(xJIT *, const xOperand *, const xOperand *);
void xjit_addi(xJIT *, const xOperand *, uint32_t);
void xjit_sub(xJIT *, const xOperand *, const xOperand *);
void xjit_subi(xJIT *, const xOperand *, uint32_t);
void xjit_test(xJIT *, const xOperand *, const xReg *);
void xjit_testi(xJIT *, const xOperand *, uint32_t);
void xjit_jmp(xJIT *, const void *, uint8_t);
void xjit_jz(xJIT *, const void *);

/* alias */

#if defined(XJIT_ALIAS)

# define ptr xjit_ptr

# define rax xjit_rax
# define rbx xjit_rbx
# define rcx xjit_rcx
# define rdx xjit_rdx
# define rsp xjit_rsp
# define rbp xjit_rbp
# define rsi xjit_rsi
# define rdi xjit_rdi

# define push xjit_push
# define pop xjit_pop
# define mov xjit_mov
# define call xjit_call
# define ret xjit_ret
# define r8 xjit_r8
# define r9 xjit_r9
# define r10 xjit_r10
# define r11 xjit_r11
# define r12 xjit_r12
# define r13 xjit_r13
# define r14 xjit_r14
# define r15 xjit_r15

# define inc xjit_inc
# define dec xjit_dec
# define add xjit_add
# define addi xjit_addi
# define sub xjit_sub
# define subi xjit_subi
# define test xjit_test
# define testi xjit_testi
# define jmp xjit_jmp
# define jz xjit_jz

#endif

/* private interfaces */

#define xjit_addr_mode(_1,_2,_3,_4,name,...) name
xOperand *xjit_b(const xOperand *base);
xOperand *xjit_bd(const xOperand *base, uint32_t disp);
xOperand *xjit_bdx(const xOperand *base, uint32_t disp, const xOperand *index);
xOperand *xjit_bdxs(const xOperand *base, uint32_t disp, const xOperand *index, uint8_t scale);

xOperand *xjit_rax_(void);
xOperand *xjit_rbx_(void);
xOperand *xjit_rcx_(void);
xOperand *xjit_rdx_(void);
xOperand *xjit_rsp_(void);
xOperand *xjit_rbp_(void);
xOperand *xjit_rsi_(void);
xOperand *xjit_rdi_(void);
xOperand *xjit_r8_(void);
xOperand *xjit_r9_(void);
xOperand *xjit_r10_(void);
xOperand *xjit_r11_(void);
xOperand *xjit_r12_(void);
xOperand *xjit_r13_(void);
xOperand *xjit_r14_(void);
xOperand *xjit_r15_(void);

#if defined(__cplusplus)
}
#endif

#endif
