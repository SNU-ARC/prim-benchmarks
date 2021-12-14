.text
.align 2

.globl vec_vvspmv_asm
.type  vec_vvspmv_asm,@function

# assumes calling convention:
# a0 has unsigned n
# a1 has float * result
# a2 has float * inVector
# a3 has float * col
# a4 has float * value

vec_vvspmv_asm:
    vsetcfg 4, 1
    vsetvl t0, a0
    li t4, 4
    li t6, 8
stripmine:
    vsetvl t1, a0
    vmca va0, a1
    vmcs vs1, a2
    vmca va1, a3
    vmca va2, a4
    vmcs vs2, t4
    vmca va3, t6
    la t5, vvspmv
    vf 0(t5)
    slli t2, t1, 2
    slli t3, t1, 3
    add a2, a2, t2
    add a3, a3, t3
    add a4, a4, t3
    sub a0, a0, t1
    bnez a0, stripmine
    mv a0, t0
    fence
    ret

# vector thread asm
.align 3
vvspmv :
    vpset vp0
    vlw vv0, va0
    vlstw vv1, va1, va3
    vlstw vv2, va2, va3
    vmul vv1, vv1, vs2
    vlxw vv3, vs1, vv1
    vfmul.s vv3, vv2, vv3
    vfadd.s vv0, vv0, vv3
    vsw vv0, va0
    vstop
