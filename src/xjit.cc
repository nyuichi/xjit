#include "xjit.h"

#define XBYAK_NO_OP_NAMES
#include "xbyak/xbyak.h"

extern "C" {

struct xJIT {
  Xbyak::CodeGenerator *codegen;
};

struct xOperand {
  xOperand(const Xbyak::Operand &op) : op(op) {}
  Xbyak::Operand op;
};

struct xReg {
  Xbyak::Reg reg;
};

xJIT *
xjit_new()
{
  xJIT *xjit;

  xjit = new xJIT;
  xjit->codegen = new Xbyak::CodeGenerator(100);
  return xjit;
}

void
xjit_ready(xJIT *xjit)
{
  xjit->codegen->ready();
}

const void *
xjit_getcode(xJIT *xjit)
{
  return xjit->codegen->getCode();
}

void
xjit_destroy(xJIT *xjit)
{
  delete xjit->codegen;
}

xOperand *xjit_b(const xReg *base) { return new xOperand(Xbyak::util::ptr[base->reg]); }
xOperand *xjit_bd(const xReg *base, uint32_t disp) { return new xOperand(Xbyak::util::ptr[base->reg + disp]); }
xOperand *xjit_bdx(const xReg *base, uint32_t disp, const xReg *index) { return new xOperand(Xbyak::util::ptr[base->reg + index->reg + disp]); }
xOperand *xjit_bdxs(const xReg *base, uint32_t disp, const xReg *index, uint8_t scale) { return new xOperand(Xbyak::util::ptr[base->reg + index->reg * scale + disp]); }

xOperand *xjit_rax_(void) { return new xOperand(Xbyak::util::rax); }
xOperand *xjit_rbx_(void) { return new xOperand(Xbyak::util::rbx); }
xOperand *xjit_rcx_(void) { return new xOperand(Xbyak::util::rcx); }
xOperand *xjit_rdx_(void) { return new xOperand(Xbyak::util::rdx); }
xOperand *xjit_rsp_(void) { return new xOperand(Xbyak::util::rsp); }
xOperand *xjit_rbp_(void) { return new xOperand(Xbyak::util::rbp); }
xOperand *xjit_rsi_(void) { return new xOperand(Xbyak::util::rsi); }
xOperand *xjit_rdi_(void) { return new xOperand(Xbyak::util::rdi); }
xOperand *xjit_r8_(void) { return new xOperand(Xbyak::util::r8); }
xOperand *xjit_r9_(void) { return new xOperand(Xbyak::util::r9); }
xOperand *xjit_r10_(void) { return new xOperand(Xbyak::util::r10); }
xOperand *xjit_r11_(void) { return new xOperand(Xbyak::util::r11); }
xOperand *xjit_r12_(void) { return new xOperand(Xbyak::util::r12); }
xOperand *xjit_r13_(void) { return new xOperand(Xbyak::util::r13); }
xOperand *xjit_r14_(void) { return new xOperand(Xbyak::util::r14); }
xOperand *xjit_r15_(void) { return new xOperand(Xbyak::util::r15); }

void xjit_push(xJIT *xjit, const xOperand *op) { xjit->codegen->push(op->op); }
void xjit_pop(xJIT *xjit, const xOperand *op) { xjit->codegen->pop(op->op); }
void xjit_mov(xJIT *xjit, const xOperand *op1, const xOperand *op2) { xjit->codegen->mov(op1->op, op2->op); }
void xjit_call(xJIT *xjit, const void *op, uint8_t mode) {
  switch (mode) {
  case XJIT_CALL_LABEL:
    xjit->codegen->call(static_cast<const char *>(op));
    break;
  case XJIT_CALL_ABS:
    xjit->codegen->call(op);
    break;
  }
}
void xjit_ret(xJIT *xjit) { xjit->codegen->ret(); }
void xjit_inc(xJIT *xjit, const xOperand *op) { xjit->codegen->inc(op->op); }
void xjit_dec(xJIT *xjit, const xOperand *op) { xjit->codegen->dec(op->op); }
void xjit_add(xJIT *xjit, const xOperand *op1, const xOperand *op2) { xjit->codegen->add(op1->op, op2->op); }
void xjit_addi(xJIT *xjit, const xOperand *op, uint32_t imm) { xjit->codegen->add(op->op, imm); }
void xjit_sub(xJIT *xjit, const xOperand *op1, const xOperand *op2) { xjit->codegen->sub(op1->op, op2->op); }
void xjit_subi(xJIT *xjit, const xOperand *op, uint32_t imm) { xjit->codegen->sub(op->op, imm); }
void xjit_test(xJIT *xjit, const xOperand *op, const xReg *reg) { xjit->codegen->test(op->op, reg->reg); }
void xjit_testi(xJIT *xjit, const xOperand *op, uint32_t imm) { xjit->codegen->test(op->op, imm); }
void xjit_jmp(xJIT *xjit, const void *op, uint8_t mode) {
  switch (mode) {
  case XJIT_JMP_LABEL:
    xjit->codegen->jmp(static_cast<const char *>(op));
    break;
  case XJIT_JMP_ABS:
    xjit->codegen->jmp(op);
    break;
  }
}
void xjit_jz(xJIT *xjit, const void *op) { xjit->codegen->jz(static_cast<const char *>(op)); }

}

// Local Variables:
// flycheck-clang-include-path: ("../include" "../extlib/xbyak")
// End:
