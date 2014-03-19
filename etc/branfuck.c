#define XJIT_ALIAS
#include "xjit.h"
#include <stdio.h>
#include <stdlib.h>

char stack[1000];

int
countc(char c)
{
  int count = 1;
  char e;

  while (1) {
    if ((e = getchar()) == EOF)
      break;
    if (e != c)
      break;
    count++;
  }
  ungetc(e, stdin);
  return count;
}

char buf[1000];

const char *
numl(int i, char c)
{
  snprintf(buf, 1000, ".%08x%c", i, c);
  return buf;
}

void
codegen(xJIT *xjit)
{
  xOperand *pputc, *pgetc, *stack;
  char c;
  const void *code;
  int lstack[100], *lcur = lstack, lno = 0;

  pputc = rbx;
  pgetc = rbp;
  stack = r12;

  push(rbx);
  push(rbp);
  push(r12);
  mov(pputc, rdi);                /* putchar */
  mov(pgetc, rsi);                /* getchar */
  mov(stack, rdx);                /* stack */

  while (~(c = getchar())) {
    switch (c) {
    case '+':
    case '-':
      {
        int cnt;

        cnt = countc(c);
        if (cnt == 1) {
          c == '+'
            ? inc(dword(stack))
            : dec(dword(stack));
        }
        else {
          addi(dword(stack), c == '+' ? cnt : -cnt);
        }
      }
      break;
    case '>':
    case '<':
      {
        int cnt;

        cnt = countc(c);
        addi(stack, 4 * (c == '>' ? cnt : -cnt));
      }
      break;
    case '.':
      {
        mov(rdi, dword(stack));
        call(pputc,XJIT_CALL_OPERAND);
      }
      break;
    case ',':
      {
        call(pgetc,XJIT_CALL_OPERAND);
        mov(dword(stack), eax);
      }
      break;
    case '[':
      {
        L(numl(lno, 'B'));      /* backward label */
        mov(eax, dword(stack));
        test(eax, eax);
        jz(numl(lno, 'F'), XJIT_LABEL_NEAR);
        *lcur++ = lno++;
      }
      break;
    case ']':
      {
        int no;

        no = *--lcur;
        jmp(numl(no, 'B'), XJIT_JMP_LABEL);
        L(numl(no, 'F'));       /* forward label */
      }
      break;
    default:
      break;
    }
  }

  pop(r12);
  pop(rbp);
  pop(rbx);
  ret();
}

int main()
{
  xJIT *xjit;
  const void *code;

  xjit = xjit_new();
  codegen(xjit);

  xjit_ready(xjit);
  code = xjit_getcode(xjit);
  ((void (*)(void *, void *, void *))code)(putchar, getchar, stack);

  xjit_destroy(xjit);

  return 0;
}
