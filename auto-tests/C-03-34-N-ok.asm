segment	.data
align	4
global	x:object
x:
segment	.rodata
align	4
_L1:
	db	"ola", 0
segment	.data
align	4
	dd	_L1
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
	call	prints
	add	esp, 4
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
extern	prints
