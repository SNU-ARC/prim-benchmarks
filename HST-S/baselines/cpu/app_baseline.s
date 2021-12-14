	.file	"app_baseline.c"
	.text
	.p2align 4,,15
	.type	histogram_host._omp_fn.0, @function
histogram_host._omp_fn.0:
.LFB71:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	24(%rdi), %ebx
	testl	%ebx, %ebx
	je	.L1
	movq	%rdi, %rbp
	call	omp_get_num_threads@PLT
	movl	%eax, %r12d
	call	omp_get_thread_num@PLT
	xorl	%edx, %edx
	movl	%eax, %ecx
	movl	%ebx, %eax
	divl	%r12d
	cmpl	%edx, %ecx
	jnb	.L8
	addl	$1, %eax
	xorl	%edx, %edx
.L8:
	imull	%eax, %ecx
	addl	%ecx, %edx
	addl	%edx, %eax
	cmpl	%eax, %edx
	jnb	.L1
	movl	20(%rbp), %ebx
	movl	16(%rbp), %r10d
	movq	8(%rbp), %r11
	movq	0(%rbp), %r8
	movl	%r10d, %r9d
	leal	-1(%rbx), %ecx
	imull	%edx, %r9d
	leaq	4(%r11,%rcx,4), %rdi
	.p2align 4,,10
	.p2align 3
.L6:
	testl	%ebx, %ebx
	movq	%r11, %rcx
	je	.L7
	.p2align 4,,10
	.p2align 3
.L5:
	movl	(%rcx), %esi
	addq	$4, %rcx
	imull	%r10d, %esi
	shrl	$12, %esi
	addl	%r9d, %esi
	addl	$1, (%r8,%rsi,4)
	cmpq	%rdi, %rcx
	jne	.L5
.L7:
	addl	$1, %edx
	addl	%r10d, %r9d
	cmpl	%edx, %eax
	jne	.L6
.L1:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE71:
	.size	histogram_host._omp_fn.0, .-histogram_host._omp_fn.0
	.p2align 4,,15
	.type	histogram_host._omp_fn.1, @function
histogram_host._omp_fn.1:
.LFB72:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movl	20(%rdi), %ebx
	testl	%ebx, %ebx
	je	.L15
	movq	%rdi, %rbp
	call	omp_get_num_threads@PLT
	movl	%eax, %r12d
	call	omp_get_thread_num@PLT
	xorl	%edx, %edx
	movl	%eax, %ecx
	movl	%ebx, %eax
	divl	%r12d
	cmpl	%edx, %ecx
	jnb	.L20
	addl	$1, %eax
	xorl	%edx, %edx
.L20:
	imull	%eax, %ecx
	addl	%ecx, %edx
	leal	(%rax,%rdx), %ecx
	cmpl	%ecx, %edx
	jnb	.L15
	movq	8(%rbp), %r8
	subl	$1, %eax
	movl	16(%rbp), %edi
	movq	0(%rbp), %rsi
	leaq	(%r8,%rdx,4), %rcx
	addq	%rax, %rdx
	leaq	4(%r8,%rdx,4), %rdx
	.p2align 4,,10
	.p2align 3
.L19:
	movl	(%rcx), %eax
	imull	%edi, %eax
	shrl	$12, %eax
	lock addl	$1, (%rsi,%rax,4)
	addq	$4, %rcx
	cmpq	%rcx, %rdx
	jne	.L19
.L15:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE72:
	.size	histogram_host._omp_fn.1, .-histogram_host._omp_fn.1
	.p2align 4,,15
	.type	histogram_host, @function
histogram_host:
.LFB67:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movq	%rdi, %r14
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	movl	%r8d, %r12d
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movl	%ecx, %ebp
	movl	%edx, %ebx
	movl	%r9d, %r13d
	subq	$64, %rsp
	.cfi_def_cfa_offset 112
	movl	112(%rsp), %edi
	movq	%rsi, (%rsp)
	movq	%fs:40, %rax
	movq	%rax, 56(%rsp)
	xorl	%eax, %eax
	call	omp_set_num_threads@PLT
	testl	%r12d, %r12d
	je	.L28
	movq	%r14, 8(%rsp)
	leaq	16(%rsp), %rsi
	leaq	histogram_host._omp_fn.1(%rip), %rdi
	movq	8(%rsp), %xmm0
	xorl	%ecx, %ecx
	movhps	(%rsp), %xmm0
	xorl	%edx, %edx
	movl	%ebp, 36(%rsp)
	movl	%ebx, 32(%rsp)
	movaps	%xmm0, 16(%rsp)
	call	GOMP_parallel@PLT
.L23:
	movq	56(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L29
	addq	$64, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L28:
	.cfi_restore_state
	movq	%r14, 8(%rsp)
	leaq	16(%rsp), %rsi
	leaq	histogram_host._omp_fn.0(%rip), %rdi
	movq	8(%rsp), %xmm0
	xorl	%ecx, %ecx
	movhps	(%rsp), %xmm0
	xorl	%edx, %edx
	movl	%r13d, 40(%rsp)
	movl	%ebp, 36(%rsp)
	movl	%ebx, 32(%rsp)
	movaps	%xmm0, 16(%rsp)
	call	GOMP_parallel@PLT
	jmp	.L23
.L29:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE67:
	.size	histogram_host, .-histogram_host
	.p2align 4,,15
	.globl	start
	.type	start, @function
start:
.LFB63:
	.cfi_startproc
	testl	%edx, %edx
	movslq	%esi, %rsi
	jne	.L31
	movq	$0x000000000, 128(%rdi,%rsi,8)
.L31:
	salq	$4, %rsi
	addq	%rsi, %rdi
	xorl	%esi, %esi
	jmp	gettimeofday@PLT
	.cfi_endproc
.LFE63:
	.size	start, .-start
	.p2align 4,,15
	.globl	stop
	.type	stop, @function
stop:
.LFB64:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movslq	%esi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	leaq	4(%rbp), %rdi
	xorl	%esi, %esi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	salq	$4, %rdi
	addq	%rbx, %rdi
	call	gettimeofday@PLT
	leaq	(%rbx,%rbp,8), %rdx
	salq	$4, %rbp
	leaq	(%rbx,%rbp), %rdi
	pxor	%xmm0, %xmm0
	pxor	%xmm1, %xmm1
	movq	64(%rdi), %rax
	subq	(%rdi), %rax
	cvtsi2sdq	%rax, %xmm0
	movq	72(%rdi), %rax
	subq	8(%rdi), %rax
	cvtsi2sdq	%rax, %xmm1
	mulsd	.LC1(%rip), %xmm0
	addsd	%xmm1, %xmm0
	addsd	128(%rdx), %xmm0
	movsd	%xmm0, 128(%rdx)
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE64:
	.size	stop, .-stop
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"Time (ms): %f\t"
	.text
	.p2align 4,,15
	.globl	print
	.type	print, @function
print:
.LFB65:
	.cfi_startproc
	imull	$1000, %edx, %edx
	pxor	%xmm1, %xmm1
	movslq	%esi, %rsi
	movl	$1, %eax
	movsd	128(%rdi,%rsi,8), %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	cvtsi2sd	%edx, %xmm1
	divsd	%xmm1, %xmm0
	jmp	__printf_chk@PLT
	.cfi_endproc
.LFE65:
	.size	print, .-print
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.ascii	"\nUsage:  ./program [options]\n\nGeneral options:\n    -h   "
	.ascii	"     help\n    -w <W>    # of untimed warmup iterations (def"
	.ascii	"ault=1)\n    -e <E>    # of timed repetition iterations (def"
	.ascii	"ault=3)\n    -t <T>    # of threads (default=8)\n   "
	.string	" -x <X>    Weak (0) or strong (1) scaling (default=0)\n\nBenchmark-specific options:\n    -i <I>    input size (default=1536*1024 elements)\n    -b <B>    histogram size (default=256 bins)\n    -f <F>    input image file (default=../input/image_VanHateren.iml)\n"
	.text
	.p2align 4,,15
	.globl	usage
	.type	usage, @function
usage:
.LFB68:
	.cfi_startproc
	movq	stderr(%rip), %rcx
	leaq	.LC3(%rip), %rdi
	movl	$480, %edx
	movl	$1, %esi
	jmp	fwrite@PLT
	.cfi_endproc
.LFE68:
	.size	usage, .-usage
	.section	.rodata.str1.1
.LC4:
	.string	"./image_VanHateren.iml"
.LC5:
	.string	"\nUnrecognized option!\n"
.LC6:
	.string	"hi:b:w:e:f:x:t:"
.LC7:
	.string	"app_baseline.c"
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"p.n_threads > 0 && \"Invalid # of ranks!\""
	.text
	.p2align 4,,15
	.globl	input_params
	.type	input_params, @function
input_params:
.LFB69:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	leaq	.LC4(%rip), %rax
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdi, %r14
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	.L41(%rip), %rbx
	movl	%esi, %r12d
	movq	%rdx, %rbp
	movl	$1, %r13d
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	movl	$1572864, %r15d
	movl	$1, 28(%rsp)
	movq	%rax, 16(%rsp)
	movl	$3, 12(%rsp)
	movl	$1, 8(%rsp)
	movl	$256, 24(%rsp)
.L38:
	leaq	.LC6(%rip), %rdx
	movq	%rbp, %rsi
	movl	%r12d, %edi
	call	getopt@PLT
	testl	%eax, %eax
	js	.L54
	subl	$98, %eax
	cmpl	$22, %eax
	ja	.L39
	movslq	(%rbx,%rax,4), %rax
	addq	%rbx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L41:
	.long	.L40-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L42-.L41
	.long	.L43-.L41
	.long	.L39-.L41
	.long	.L53-.L41
	.long	.L45-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L46-.L41
	.long	.L39-.L41
	.long	.L39-.L41
	.long	.L47-.L41
	.long	.L48-.L41
	.text
	.p2align 4,,10
	.p2align 3
.L39:
	movq	stderr(%rip), %rcx
	leaq	.LC5(%rip), %rdi
	movl	$22, %edx
	movl	$1, %esi
	call	fwrite@PLT
.L53:
	movq	stderr(%rip), %rcx
	leaq	.LC3(%rip), %rdi
	movl	$480, %edx
	movl	$1, %esi
	call	fwrite@PLT
	xorl	%edi, %edi
	call	exit@PLT
	.p2align 4,,10
	.p2align 3
.L48:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, 28(%rsp)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L47:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, 8(%rsp)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L46:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, %r13d
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L45:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, %r15d
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L43:
	movq	optarg(%rip), %rax
	movq	%rax, 16(%rsp)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L42:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, 12(%rsp)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L40:
	movq	optarg(%rip), %rdi
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, 24(%rsp)
	jmp	.L38
	.p2align 4,,10
	.p2align 3
.L54:
	testl	%r13d, %r13d
	jle	.L55
	movl	24(%rsp), %eax
	movl	%r15d, (%r14)
	movl	%r13d, 28(%r14)
	movl	%eax, 4(%r14)
	movl	8(%rsp), %eax
	movl	%eax, 8(%r14)
	movl	12(%rsp), %eax
	movl	%eax, 12(%r14)
	movq	16(%rsp), %rax
	movq	%rax, 16(%r14)
	movl	28(%rsp), %eax
	movl	%eax, 24(%r14)
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r14, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L55:
	.cfi_restore_state
	leaq	__PRETTY_FUNCTION__.4612(%rip), %rcx
	leaq	.LC7(%rip), %rsi
	leaq	.LC8(%rip), %rdi
	movl	$143, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE69:
	.size	input_params, .-input_params
	.section	.rodata.str1.1
.LC9:
	.string	"mlockall failed:"
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"input_size % p.n_threads == 0 && \"Input size!\""
	.section	.rodata.str1.1
.LC11:
	.string	"rb"
.LC12:
	.string	"%s does not exist\n"
.LC13:
	.string	"Hwacha wrong %d, %d %d\n"
.LC14:
	.string	"Omp wrong %d, %d %d\n"
	.section	.rodata.str1.8
	.align 8
.LC15:
	.string	"******************************"
	.section	.rodata.str1.1
.LC16:
	.string	"Hwacha correct!"
.LC17:
	.string	"Hwacha wrong T_T"
.LC18:
	.string	"CPU "
.LC20:
	.string	"Hwacha "
.LC21:
	.string	"4 threads "
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB70:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	%edi, %ebx
	movl	$3, %edi
	movq	%rsi, %rbp
	subq	$344, %rsp
	.cfi_def_cfa_offset 400
	movq	%fs:40, %rax
	movq	%rax, 328(%rsp)
	xorl	%eax, %eax
	call	mlockall@PLT
	testl	%eax, %eax
	jne	.L85
	leaq	32(%rsp), %rdi
	movq	%rbp, %rdx
	movl	%ebx, %esi
	call	input_params
	movl	56(%rsp), %eax
	movl	32(%rsp), %ebx
	movl	$0, %edx
	movl	36(%rsp), %r12d
	movl	60(%rsp), %ecx
	testl	%eax, %eax
	movl	%eax, 12(%rsp)
	movl	%ebx, %eax
	je	.L86
	divl	%ecx
	testl	%edx, %edx
	jne	.L87
	movl	%ebx, %edi
	movl	%r12d, %ebp
	salq	$2, %rbp
	salq	$2, %rdi
	call	malloc@PLT
	movq	%rbp, %rdi
	movq	%rax, A(%rip)
	call	malloc@PLT
	movq	%rbp, %rdi
	movq	%rax, histo_host(%rip)
	call	malloc@PLT
	movq	%rbp, %rdi
	movq	%rax, histo_hwacha(%rip)
	call	malloc@PLT
	movq	%rax, histo_host_4(%rip)
.L74:
	leaq	224(%rsp), %r13
	movq	48(%rsp), %rcx
	movl	$100, %edx
	movl	$1, %esi
	xorl	%eax, %eax
	movq	A(%rip), %r14
	movq	%r13, %rdi
	call	__sprintf_chk@PLT
	leaq	.LC11(%rip), %rsi
	movq	%r13, %rdi
	call	fopen@PLT
	testq	%rax, %rax
	movq	%rax, %rbp
	je	.L62
	testl	%ebx, %ebx
	je	.L63
	leal	-1(%rbx), %eax
	leaq	30(%rsp), %r13
	movl	$4095, %r15d
	leaq	4(%r14,%rax,4), %rax
	movq	%rax, (%rsp)
	.p2align 4,,10
	.p2align 3
.L64:
	movq	%rbp, %rcx
	movl	$1, %edx
	movl	$2, %esi
	movq	%r13, %rdi
	call	fread@PLT
	movzwl	30(%rsp), %eax
	rolw	$8, %ax
	cmpw	$4095, %ax
	cmovnb	%r15d, %eax
	addq	$4, %r14
	movzwl	%ax, %eax
	movl	%eax, -4(%r14)
	cmpq	%r14, (%rsp)
	jne	.L64
.L63:
	movq	%rbp, %rdi
	call	fclose@PLT
	cmpl	$0, 12(%rsp)
	jne	.L88
	movq	histo_host(%rip), %rdi
	xorl	%edx, %edx
	xorl	%esi, %esi
	call	memset@PLT
.L67:
	leaq	64(%rsp), %rbp
	xorl	%esi, %esi
	movq	$0x000000000, 192(%rsp)
	xorl	%r13d, %r13d
	movq	%rbp, %rdi
	call	gettimeofday@PLT
	subq	$8, %rsp
	.cfi_def_cfa_offset 408
	movq	A(%rip), %rsi
	movq	histo_host(%rip), %rdi
	pushq	$1
	.cfi_def_cfa_offset 416
	movl	28(%rsp), %r15d
	movl	%r13d, %r9d
	movl	%ebx, %ecx
	movl	%r12d, %edx
	movl	%r15d, %r8d
	call	histogram_host
	xorl	%esi, %esi
	movq	%rbp, %rdi
	call	stop
	leaq	16(%rbp), %rdi
	xorl	%esi, %esi
	movq	$0x000000000, 216(%rsp)
	call	gettimeofday@PLT
	movq	histo_hwacha(%rip), %rdx
	movq	A(%rip), %rsi
	movl	%r12d, %ecx
	movl	%ebx, %edi
	xorl	%eax, %eax
	call	vec_vvhst_asm@PLT
	movl	$1, %esi
	movq	%rbp, %rdi
	call	stop
	leaq	32(%rbp), %rdi
	xorl	%esi, %esi
	movq	$0x000000000, 224(%rsp)
	call	gettimeofday@PLT
	movq	A(%rip), %rsi
	movq	histo_host_4(%rip), %rdi
	movl	%r12d, %edx
	movl	%r13d, %r9d
	movl	%r15d, %r8d
	movl	%ebx, %ecx
	movl	$4, (%rsp)
	call	histogram_host
	movl	$2, %esi
	movq	%rbp, %rdi
	call	stop
	testl	%r12d, %r12d
	popq	%rax
	.cfi_def_cfa_offset 408
	popq	%rdx
	.cfi_def_cfa_offset 400
	je	.L68
	leal	-1(%r12), %ebp
	leaq	.LC13(%rip), %r13
	xorl	%ebx, %ebx
	movl	$1, %r12d
	addq	$1, %rbp
	.p2align 4,,10
	.p2align 3
.L71:
	movq	histo_host(%rip), %rax
	movl	%ebx, %r15d
	leaq	0(,%rbx,4), %r14
	movl	(%rax,%rbx,4), %ecx
	movq	histo_hwacha(%rip), %rax
	movl	(%rax,%rbx,4), %eax
	cmpl	%eax, %ecx
	je	.L69
	movl	%eax, %r8d
	movl	%ebx, %edx
	movq	%r13, %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	xorl	%r12d, %r12d
	call	__printf_chk@PLT
	movq	histo_host(%rip), %rax
	movl	(%rax,%rbx,4), %eax
.L69:
	movq	histo_host_4(%rip), %rdx
	movl	(%rdx,%r14), %r8d
	cmpl	%eax, %r8d
	je	.L70
	leaq	.LC14(%rip), %rsi
	movl	%eax, %ecx
	movl	%r15d, %edx
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
.L70:
	addq	$1, %rbx
	cmpq	%rbp, %rbx
	jne	.L71
	leaq	.LC15(%rip), %rdi
	call	puts@PLT
	testb	%r12b, %r12b
	jne	.L75
	leaq	.LC17(%rip), %rdi
	call	puts@PLT
.L73:
	leaq	.LC15(%rip), %rdi
	call	puts@PLT
	leaq	.LC18(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movsd	192(%rsp), %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	divsd	.LC19(%rip), %xmm0
	call	__printf_chk@PLT
	movl	$10, %edi
	call	putchar@PLT
	leaq	.LC20(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movsd	200(%rsp), %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	divsd	.LC19(%rip), %xmm0
	call	__printf_chk@PLT
	movl	$10, %edi
	call	putchar@PLT
	leaq	.LC21(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movsd	208(%rsp), %xmm0
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	movl	$1, %eax
	divsd	.LC19(%rip), %xmm0
	call	__printf_chk@PLT
	movl	$10, %edi
	call	putchar@PLT
	call	munlockall@PLT
.L58:
	xorl	%eax, %eax
	movq	328(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L89
	addq	$344, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L86:
	.cfi_restore_state
	divl	%ecx
	testl	%edx, %edx
	jne	.L90
	movl	%ebx, %edi
	salq	$2, %rdi
	call	malloc@PLT
	xorl	%edi, %edi
	movq	%rax, A(%rip)
	call	malloc@PLT
	movq	%rax, histo_host(%rip)
	jmp	.L74
.L85:
	leaq	.LC9(%rip), %rdi
	call	perror@PLT
	jmp	.L58
.L88:
	movl	%r12d, %ebp
	movq	histo_host(%rip), %rdi
	xorl	%esi, %esi
	salq	$2, %rbp
	movq	%rbp, %rdx
	call	memset@PLT
	movq	histo_hwacha(%rip), %rdi
	movq	%rbp, %rdx
	xorl	%esi, %esi
	call	memset@PLT
	jmp	.L67
.L68:
	leaq	.LC15(%rip), %rdi
	call	puts@PLT
.L75:
	leaq	.LC16(%rip), %rdi
	call	puts@PLT
	jmp	.L73
.L62:
	leaq	.LC12(%rip), %rsi
	movl	$1, %edi
	movq	%r13, %rdx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
.L89:
	call	__stack_chk_fail@PLT
.L90:
	leaq	__PRETTY_FUNCTION__.4620(%rip), %rcx
	leaq	.LC7(%rip), %rsi
	leaq	.LC10(%rip), %rdi
	movl	$163, %edx
	call	__assert_fail@PLT
.L87:
	leaq	__PRETTY_FUNCTION__.4620(%rip), %rcx
	leaq	.LC7(%rip), %rsi
	leaq	.LC10(%rip), %rdi
	movl	$165, %edx
	call	__assert_fail@PLT
	.cfi_endproc
.LFE70:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.4620, @object
	.size	__PRETTY_FUNCTION__.4620, 5
__PRETTY_FUNCTION__.4620:
	.string	"main"
	.align 8
	.type	__PRETTY_FUNCTION__.4612, @object
	.size	__PRETTY_FUNCTION__.4612, 13
__PRETTY_FUNCTION__.4612:
	.string	"input_params"
	.local	histo_host_4
	.comm	histo_host_4,8,8
	.local	histo_hwacha
	.comm	histo_hwacha,8,8
	.local	histo_host
	.comm	histo_host,8,8
	.local	A
	.comm	A,8,8
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1093567616
	.align 8
.LC19:
	.long	0
	.long	1083129856
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
