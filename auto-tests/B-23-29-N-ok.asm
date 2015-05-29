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
	push	dword 2
	pop	eax
	cmp	eax, byte 0
	je	near _L1
segment	.rodata
align	4
_L2:
	db	"KO", 0
segment	.text
align	4
	push	dword $_L2
	call	prints
	add	esp, 4
	jmp	dword _L3
_L1:
segment	.rodata
align	4
_L4:
	db	"OK", 0
segment	.text
align	4
	push	dword $_L4
	call	prints
	add	esp, 4
_L3:
	lea	eax, [ebp+-4]
	push	eax
	pop	eax
	push	dword [eax]
	pop	eax
	leave
	ret
extern	prints
