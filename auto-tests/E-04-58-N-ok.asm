segment	.bss
align	4
x:
	resb	4
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	push	dword 1
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
_L2:
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword 5
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setle	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L3
	push	dword $x
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
_L1:
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword 2
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	push	dword $x
	pop	ecx
	pop	eax
	mov	[ecx], eax
	jmp	dword _L2
_L3:
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
extern	printi
