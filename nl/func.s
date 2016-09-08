	.file	"func.cpp"
	.weakref	_Z20__gthrw_pthread_oncePiPFvvE,pthread_once
	.weakref	_Z26__gthrw_pthread_key_createPjPFvPvE,pthread_key_create
	.weakref	_Z26__gthrw_pthread_key_deletej,pthread_key_delete
	.weakref	_Z27__gthrw_pthread_getspecificj,pthread_getspecific
	.weakref	_Z27__gthrw_pthread_setspecificjPKv,pthread_setspecific
	.weakref	_Z22__gthrw_pthread_createPmPK16__pthread_attr_sPFPvS3_ES3_,pthread_create
	.weakref	_Z26__gthrw_pthread_mutex_lockP15pthread_mutex_t,pthread_mutex_lock
	.weakref	_Z29__gthrw_pthread_mutex_trylockP15pthread_mutex_t,pthread_mutex_trylock
	.weakref	_Z28__gthrw_pthread_mutex_unlockP15pthread_mutex_t,pthread_mutex_unlock
	.weakref	_Z30__gthrw_pthread_mutexattr_initP19pthread_mutexattr_t,pthread_mutexattr_init
	.weakref	_Z33__gthrw_pthread_mutexattr_settypeP19pthread_mutexattr_ti,pthread_mutexattr_settype
	.weakref	_Z33__gthrw_pthread_mutexattr_destroyP19pthread_mutexattr_t,pthread_mutexattr_destroy
	.weakref	_Z26__gthrw_pthread_mutex_initP15pthread_mutex_tPK19pthread_mutexattr_t,pthread_mutex_init
	.text
	.align 2
	.type	_ZSt17__verify_groupingPKcmRKSs, @function
_ZSt17__verify_groupingPKcmRKSs:
.LFB1361:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	subq	$80, %rsp
.LCFI2:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rdi
	call	_ZNKSs4sizeEv
	decq	%rax
	movq	%rax, -32(%rbp)
	movq	-16(%rbp), %rax
	decq	%rax
	movq	%rax, -48(%rbp)
	leaq	-48(%rbp), %rsi
	leaq	-32(%rbp), %rdi
	call	_ZSt3minImERKT_S2_S2_
	movq	(%rax), %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -56(%rbp)
	movb	$1, -57(%rbp)
	movq	$0, -72(%rbp)
.L2:
	movq	-72(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jae	.L5
	cmpb	$0, -57(%rbp)
	je	.L5
	movq	-56(%rbp), %rsi
	movq	-24(%rbp), %rdi
	call	_ZNKSsixEm
	movsbl	(%rax),%edx
	movq	-72(%rbp), %rax
	addq	-8(%rbp), %rax
	movsbl	(%rax),%eax
	cmpl	%eax, %edx
	sete	%al
	movb	%al, -57(%rbp)
	leaq	-56(%rbp), %rax
	decq	(%rax)
	leaq	-72(%rbp), %rax
	incq	(%rax)
	jmp	.L2
.L5:
	cmpq	$0, -56(%rbp)
	je	.L6
	cmpb	$0, -57(%rbp)
	je	.L6
	movq	-56(%rbp), %rsi
	movq	-24(%rbp), %rdi
	call	_ZNKSsixEm
	movsbl	(%rax),%edx
	movq	-40(%rbp), %rax
	addq	-8(%rbp), %rax
	movsbl	(%rax),%eax
	cmpl	%eax, %edx
	sete	%al
	movb	%al, -57(%rbp)
	leaq	-56(%rbp), %rax
	decq	(%rax)
	jmp	.L5
.L6:
	movq	-24(%rbp), %rdi
	movl	$0, %esi
	call	_ZNKSsixEm
	movsbl	(%rax),%edx
	movq	-40(%rbp), %rax
	addq	-8(%rbp), %rax
	movsbl	(%rax),%eax
	cmpl	%eax, %edx
	jg	.L8
	movzbl	-57(%rbp), %eax
	andl	$1, %eax
	movb	%al, -73(%rbp)
	jmp	.L9
.L8:
	movb	$0, -73(%rbp)
.L9:
	movzbl	-73(%rbp), %eax
	movb	%al, -57(%rbp)
	movzbl	-57(%rbp), %eax
	leave
	ret
.LFE1361:
	.size	_ZSt17__verify_groupingPKcmRKSs, .-_ZSt17__verify_groupingPKcmRKSs
	.local	_ZSt8__ioinit
	.comm	_ZSt8__ioinit,1,1
.globl _ZN8Function12return_valueE
	.bss
	.align 8
	.type	_ZN8Function12return_valueE, @object
	.size	_ZN8Function12return_valueE, 8
_ZN8Function12return_valueE:
	.zero	8
	.text
	.align 2
.globl _ZN8Function3RunEP5Value
	.type	_ZN8Function3RunEP5Value, @function
_ZN8Function3RunEP5Value:
.LFB1925:
	pushq	%rbp
.LCFI3:
	movq	%rsp, %rbp
.LCFI4:
	subq	$48, %rsp
.LCFI5:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	addq	$8, %rdi
	call	_ZNSt6vectorI11InstructionSaIS0_EE5beginEv
	movq	%rax, -32(%rbp)
.L11:
	movq	-8(%rbp), %rdi
	addq	$8, %rdi
	call	_ZNSt6vectorI11InstructionSaIS0_EE3endEv
	movq	%rax, -40(%rbp)
	leaq	-40(%rbp), %rsi
	leaq	-32(%rbp), %rdi
	call	_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_
	testb	%al, %al
	je	.L10
	leaq	-32(%rbp), %rdi
	call	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv
	jmp	.L11
.L10:
	leave
	ret
.LFE1925:
	.size	_ZN8Function3RunEP5Value, .-_ZN8Function3RunEP5Value
	.section	.gnu.linkonce.t._ZSt3minImERKT_S2_S2_,"ax",@progbits
	.align 2
	.weak	_ZSt3minImERKT_S2_S2_
	.type	_ZSt3minImERKT_S2_S2_, @function
_ZSt3minImERKT_S2_S2_:
.LFB1926:
	pushq	%rbp
.LCFI6:
	movq	%rsp, %rbp
.LCFI7:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	(%rax), %rax
	cmpq	(%rdx), %rax
	jae	.L15
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
	jmp	.L14
.L15:
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)
.L14:
	movq	-24(%rbp), %rax
	leave
	ret
.LFE1926:
	.size	_ZSt3minImERKT_S2_S2_, .-_ZSt3minImERKT_S2_S2_
	.section	.gnu.linkonce.t._ZNSt6vectorI11InstructionSaIS0_EE5beginEv,"ax",@progbits
	.align 2
	.weak	_ZNSt6vectorI11InstructionSaIS0_EE5beginEv
	.type	_ZNSt6vectorI11InstructionSaIS0_EE5beginEv, @function
_ZNSt6vectorI11InstructionSaIS0_EE5beginEv:
.LFB1930:
	pushq	%rbp
.LCFI8:
	movq	%rsp, %rbp
.LCFI9:
	subq	$16, %rsp
.LCFI10:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rsi
	leaq	-16(%rbp), %rdi
	call	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_
	movq	-16(%rbp), %rax
	leave
	ret
.LFE1930:
	.size	_ZNSt6vectorI11InstructionSaIS0_EE5beginEv, .-_ZNSt6vectorI11InstructionSaIS0_EE5beginEv
	.section	.gnu.linkonce.t._ZNSt6vectorI11InstructionSaIS0_EE3endEv,"ax",@progbits
	.align 2
	.weak	_ZNSt6vectorI11InstructionSaIS0_EE3endEv
	.type	_ZNSt6vectorI11InstructionSaIS0_EE3endEv, @function
_ZNSt6vectorI11InstructionSaIS0_EE3endEv:
.LFB1931:
	pushq	%rbp
.LCFI11:
	movq	%rsp, %rbp
.LCFI12:
	subq	$16, %rsp
.LCFI13:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rsi
	addq	$8, %rsi
	leaq	-16(%rbp), %rdi
	call	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_
	movq	-16(%rbp), %rax
	leave
	ret
.LFE1931:
	.size	_ZNSt6vectorI11InstructionSaIS0_EE3endEv, .-_ZNSt6vectorI11InstructionSaIS0_EE3endEv
	.section	.gnu.linkonce.t._ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_,"ax",@progbits
	.align 2
	.weak	_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_
	.type	_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_, @function
_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_:
.LFB1932:
	pushq	%rbp
.LCFI14:
	movq	%rsp, %rbp
.LCFI15:
	pushq	%rbx
.LCFI16:
	subq	$24, %rsp
.LCFI17:
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	-16(%rbp), %rdi
	call	_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv
	movq	%rax, %rbx
	movq	-24(%rbp), %rdi
	call	_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv
	movq	%rax, %rdx
	movq	(%rbx), %rax
	cmpq	(%rdx), %rax
	setne	%al
	movzbl	%al, %eax
	addq	$24, %rsp
	popq	%rbx
	leave
	ret
.LFE1932:
	.size	_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_, .-_ZN9__gnu_cxxneIP11InstructionSt6vectorIS1_SaIS1_EEEEbRKNS_17__normal_iteratorIT_T0_EESB_
	.section	.gnu.linkonce.t._ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv,"ax",@progbits
	.align 2
	.weak	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv
	.type	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv, @function
_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv:
.LFB1933:
	pushq	%rbp
.LCFI18:
	movq	%rsp, %rbp
.LCFI19:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	incq	(%rax)
	movq	-8(%rbp), %rax
	leave
	ret
.LFE1933:
	.size	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv, .-_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEppEv
	.section	.gnu.linkonce.t._ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_,"ax",@progbits
	.align 2
	.weak	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_
	.type	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_, @function
_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_:
.LFB1938:
	pushq	%rbp
.LCFI20:
	movq	%rsp, %rbp
.LCFI21:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	(%rdx), %rdx
	movq	%rdx, (%rax)
	leave
	ret
.LFE1938:
	.size	_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_, .-_ZN9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEEC1ERKS2_
	.section	.gnu.linkonce.t._ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv,"ax",@progbits
	.align 2
	.weak	_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv
	.type	_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv, @function
_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv:
.LFB1939:
	pushq	%rbp
.LCFI22:
	movq	%rsp, %rbp
.LCFI23:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	leave
	ret
.LFE1939:
	.size	_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv, .-_ZNK9__gnu_cxx17__normal_iteratorIP11InstructionSt6vectorIS1_SaIS1_EEE4baseEv
	.text
	.align 2
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1940:
	pushq	%rbp
.LCFI24:
	movq	%rsp, %rbp
.LCFI25:
	subq	$16, %rsp
.LCFI26:
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$65535, -8(%rbp)
	jne	.L22
	cmpl	$1, -4(%rbp)
	jne	.L22
	movl	$_ZSt8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$0, %esi
	movl	$__tcf_0, %edi
	call	__cxa_atexit
.L22:
	leave
	ret
.LFE1940:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.align 2
	.type	__tcf_0, @function
__tcf_0:
.LFB1941:
	pushq	%rbp
.LCFI27:
	movq	%rsp, %rbp
.LCFI28:
	subq	$16, %rsp
.LCFI29:
	movq	%rdi, -8(%rbp)
	movl	$_ZSt8__ioinit, %edi
	call	_ZNSt8ios_base4InitD1Ev
	leave
	ret
.LFE1941:
	.size	__tcf_0, .-__tcf_0
	.align 2
	.type	_GLOBAL__I__ZN8Function12return_valueE, @function
_GLOBAL__I__ZN8Function12return_valueE:
.LFB1942:
	pushq	%rbp
.LCFI30:
	movq	%rsp, %rbp
.LCFI31:
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	leave
	ret
.LFE1942:
	.size	_GLOBAL__I__ZN8Function12return_valueE, .-_GLOBAL__I__ZN8Function12return_valueE
	.section	.ctors,"aw",@progbits
	.align 8
	.quad	_GLOBAL__I__ZN8Function12return_valueE
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zP"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x9
	.byte	0x0
	.quad	__gxx_personality_v0
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.quad	.LFB1361
	.quad	.LFE1361-.LFB1361
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB1361
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.quad	.LFB1925
	.quad	.LFE1925-.LFB1925
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB1925
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.quad	.LFB1926
	.quad	.LFE1926-.LFB1926
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB1926
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.quad	.LFB1930
	.quad	.LFE1930-.LFB1930
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB1930
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.quad	.LFB1931
	.quad	.LFE1931-.LFB1931
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB1931
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.quad	.LFB1932
	.quad	.LFE1932-.LFB1932
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI14-.LFB1932
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI17-.LCFI15
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.quad	.LFB1933
	.quad	.LFE1933-.LFB1933
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI18-.LFB1933
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.quad	.LFB1938
	.quad	.LFE1938-.LFB1938
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI20-.LFB1938
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI21-.LCFI20
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.quad	.LFB1939
	.quad	.LFE1939-.LFB1939
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI22-.LFB1939
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.quad	.LFB1940
	.quad	.LFE1940-.LFB1940
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI24-.LFB1940
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI25-.LCFI24
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.quad	.LFB1941
	.quad	.LFE1941-.LFB1941
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI27-.LFB1941
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.quad	.LFB1942
	.quad	.LFE1942-.LFB1942
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI30-.LFB1942
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE23:
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.5 20051201 (Red Hat 3.4.5-2)"
