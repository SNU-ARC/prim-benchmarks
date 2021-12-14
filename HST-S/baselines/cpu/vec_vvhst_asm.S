.text
.align 2

.globl vec_vvhst_asm
.type  vec_vvhst_asm,@function

# assumes calling convention:
# a0 has unsigned int nr_elements
# a1 has T * A
# a2 has unsigned int * histo
# a3 has unsigned int bins

vec_vvhst_asm:
    vsetcfg 2, 1
    li t3, 12
    li t4, 1
    li t6, 4
    vmcs vs1, a2
    vmcs vs2, a3 # bins
    vmcs vs3, t3 # 12
    vmcs vs4, t4 # 1
    vmcs vs5, t6 # 4
stripmine:
    vsetvl t1, a0
    vmca va0, a1
    la t5, vvhst
    vf 0(t5)
    slli t2, t1, 2
    add a1, a1, t2
    sub a0, a0, t1
    bnez a0, stripmine
    ret

# vector thread asm
.align 3
vvhst :
    vpset vp0
    vlw vv0, va0
    vmul vv0, vv0, vs2
    vsra vv0, vv0, vs3
    vmul vv0, vv0, vs5
    vadd vv0, vv0, vs1
    vamoadd.w vv1, 0(vv0), vs4
    vstop
