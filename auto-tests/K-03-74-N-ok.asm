segment	.text
align	4
f:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	lea	eax, [ebp+8]
	push	eax
	pop	eax
	push	dword [eax]
	lea	eax, [ebp+12]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	add	dword [esp], eax
	call	printi
	add	esp, 4
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
	push	dword 4
	push	dword 3
	call	f
	add	esp, 0
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
extern	printi
