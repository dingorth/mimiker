/*	$NetBSD: mcontext.h,v 1.22 2018/02/15 15:53:56 kamil Exp $	*/

/*-
 * Copyright (c) 1999, 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein, and by Jason R. Thorpe of Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MIPS_MCONTEXT_H_
#define _MIPS_MCONTEXT_H_

/*
 * General register state
 */
#define _NGREG 37 /* R0-R31, MDLO, MDHI, CAUSE, PC, SR */

#define _REG_R0 0
#define _REG_AT 1
#define _REG_V0 2
#define _REG_V1 3
#define _REG_A0 4
#define _REG_A1 5
#define _REG_A2 6
#define _REG_A3 7
#define _REG_T0 8
#define _REG_T1 9
#define _REG_T2 10
#define _REG_T3 11
#define _REG_T4 12
#define _REG_T5 13
#define _REG_T6 14
#define _REG_T7 15
#define _REG_S0 16
#define _REG_S1 17
#define _REG_S2 18
#define _REG_S3 19
#define _REG_S4 20
#define _REG_S5 21
#define _REG_S6 22
#define _REG_S7 23
#define _REG_T8 24
#define _REG_T9 25
#define _REG_K0 26
#define _REG_K1 27
#define _REG_GP 28
#define _REG_SP 29
#define _REG_S8 30
#define _REG_RA 31

/* XXX: The following conflict with <mips/regnum.h> */
#define _REG_MDLO 32
#define _REG_MDHI 33
#define _REG_CAUSE 34
#define _REG_EPC 35
#define _REG_SR 36

#ifndef __ASSEMBLER__

/* Make sure this is signed; we need pointers to be sign-extended. */
typedef long __greg_t;

typedef __greg_t __gregset_t[_NGREG];

/*
 * For the O32 ABI, there are 16 doubles, one at each even FP reg
 * number.  The FP registers themselves are 32-bits.
 */
typedef __greg_t __freg_t;

/*
 * Floating point register state
 */
struct __fpregset {
  union {
    double __fp_dregs[16];
    float __fp_fregs[32];
    int32_t __fp_regs[32];
  } __fp_r;
  unsigned int __fp_csr;
  unsigned int __fp_pad;
};

typedef struct __fpregset __fpregset_t;

typedef struct {
  __gregset_t __gregs;
  __fpregset_t __fpregs;
  __greg_t _mc_tlsbase;
  __greg_t __mc_unused;
} mcontext_t;

#endif /* !__ASSEMBLER__ */

#define _UC_SETSTACK 0x00010000
#define _UC_CLRSTACK 0x00020000
#define _UC_TLSBASE 0x00040000

#define _UC_MACHINE_SP(uc) ((uc)->uc_mcontext.__gregs[_REG_SP])
#define _UC_MACHINE_FP(uc) ((uc)->uc_mcontext.__gregs[_REG_S8])
#define _UC_MACHINE_PC(uc) ((uc)->uc_mcontext.__gregs[_REG_EPC])
#define _UC_MACHINE_INTRV(uc) ((uc)->uc_mcontext.__gregs[_REG_V0])

#define _UC_MACHINE_SET_PC(uc, pc) _UC_MACHINE_PC(uc) = (pc)

#endif /* _MIPS_MCONTEXT_H_ */
