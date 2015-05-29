segment	.data
align	4
global	x:object
x:
	dd	10
global	y:object
y:
	dd	3
global	z:object
z:
	dd	30
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
	push	dword $x
	pop	eax
	push	dword [eax]
	push	dword $y
	pop	eax
	push	dword [eax]
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	setne	cl
	mov	[esp], ecx
	push	dword [esp]
	pop	eax
	cmp	eax, byte 0
	jne	near _L1
	push	dword 7
	push	dword [esp]
	push	dword $z
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	or	dword [esp], eax
_L1:
	add	esp, 4
	push	dword $z
	pop	eax
	push	dword [eax]
	call	printi
	add	esp, 4
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
extern	printi
