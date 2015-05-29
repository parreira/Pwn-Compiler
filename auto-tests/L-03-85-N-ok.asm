segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 9
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L1
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	jmp	dword _L2
_L1:
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	sub	dword [esp], eax
	call	g
	add	esp, 4
	push	eax
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
_L2:
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
g:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	push	dword 9
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 0
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
	pop	eax
	cmp	eax, byte 0
	je	near _L3
	push	dword 0
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
	jmp	dword _L4
_L3:
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	push	dword 1
	pop	eax
	sub	dword [esp], eax
	call	f
	add	esp, 4
	push	eax
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	add	dword [esp], eax
	push	dword [esp]
	lea	eax, [ebp+-4]
	push	eax
	pop	ecx
	pop	eax
	mov	[ecx], eax
	add	esp, 4
_L4:
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
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
	push	dword 3
	call	f
	add	esp, 4
	push	eax
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
