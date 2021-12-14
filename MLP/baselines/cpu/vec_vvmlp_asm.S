.text
.align 2

.globl vec_vvmlp_asm
.type  vec_vvmlp_asm,@function

.globl vec_vvcopy_asm
.type  vec_vvcopy_asm,@function

# assumes calling convention:
# a0 has unsigned int n
# a1 has T * result
# a2 has T * A
# a3 has T * B

vec_vvmlp_asm:
    vsetcfg 3, 1
    vsetvl t0, a0
stripmine:
    vsetvl t1, a0
    vmca va0, a1
    vmca va1, a2
    vmca va2, a3
    la t5, vvmlp
    vf 0(t5)
    slli t2, t1, 2
    add a2, a2, t2
    add a3, a3, t2
    sub a0, a0, t1
    bnez a0, stripmine
    mv a0, t0
    fence
    ret

# assumes calling convention:
# a0 has unsigned int n
# a1 has T * B
# a2 has T * C

vec_vvcopy_asm:
    vsetcfg 2, 1
    vsetvl t0, a0
stripmine2:
    vsetvl t1, a0
    vmca va0, a1
    vmca va1, a2
    la t5, vvcopy
    vf 0(t5)
    slli t2, t1, 2
    add a1, a1, t2
    add a2, a2, t2
    sub a0, a0, t1
    bnez a0, stripmine2
    fence
    ret

# vector thread asm
.align 3
vvmlp :
    vpset vp0
    vlw vv0, va0
    vlw vv1, va1
    vlw vv2, va2
    vmul vv1, vv1, vv2
    vadd vv0, vv0, vv1
    vsw vv0, va0
    vstop

vvcopy:
    vpset vp0
    vlw vv1, va1
    vsw vv1, va0
    vstop
    