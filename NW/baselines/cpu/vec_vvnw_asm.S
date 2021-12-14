.text
.align 2

.globl vec_vvcopy_asm
.type  vec_vvcopy_asm,@function

# assumes calling convention:
# a0 has int n
# a1 has int * dest
# a2 has int * source

vec_vvcopy_asm:
    vsetcfg 2, 1
    vsetvl t0, a0
stripmine:
    vsetvl t1, a0
    vmca va0, a1
    vmca va1, a2
    la t5, vvcopy
    vf 0(t5)
    slli t2, t1, 2
    add a1, a1, t2
    add a2, a2, t2
    sub a0, a0, t1
    bnez a0, stripmine
    fence
    ret

# vector thread asm
.align 3
vvcopy:
    vpset vp0
@vp0    vlw vv0, va1
@vp0    vsw vv0, va0
    vstop
