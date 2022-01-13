.text
.align 2

.globl vec_vvred_asm
.type  vec_vvred_asm,@function

# assumes calling convention:
# a0 has unsigned int n
# a1 has T * result
# a2 has T * A

vec_vvred_asm:
    vsetcfg 2, 1
    vsetvl t0, a0
    la t5, set_result
    vf 0(t5)
stripmine:
    vsetvl t1, a0
    vmca va1, a2
    la t5, vvred
    vf 0(t5)
    slli t2, t1, 2
    add a2, a2, t2
    sub a0, a0, t1
    bnez a0, stripmine
    vsetvl t1, t0
    vmca va0, a1
    la t5, save_result
    vf 0(t5)
    mv a0, t0
    fence
    ret

# vector thread asm
.align 3
vvred :
    vpset vp0
    vlw vv1, va1
    vadd vv0, vv0, vv1
    vstop

set_result :
    vpset vp0
    vxor vv0, vv0, vv0
    vstop

save_result :
    vpset vp0
    vsw vv0, va0
