;************** main.asm ****************
;
	ORG	100H
	JMP	_start1
_intstr	DB	'     ','$'
_buf	TIMES 256 DB ' '
	DB 13,10,'$'
%include	"dispstr.mac"
%include	"itostr.mac"
%include	"readstr.mac"
%include	"strtoi.mac"
%include	"newline.mac"
_start1:
main_a	DW	0
main_b	DW	0
	PUSH	7
	POP	AX
	MOV	[main_b], AX
main_c	DW	0
main_d	DW	0
	PUSH	20
	POP	AX
	MOV	[main_d], AX
main_x	DW	0
	PUSH	87
	POP	AX
	MOV	[main_x], AX
main_w	DW	0
	PUSH	WORD [main_x]
	POP	AX
	MOV	[main_w], AX
main_t	DW	0
	PUSH	88
	POP	AX
	MOV	[main_t], AX
	PUSH	WORD [main_a]
	PUSH	1
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go2
	JMP	_go3
_go2:
	PUSH	89
	POP	AX
	MOV	[main_x], AX
_go3:
	PUSH	88
	POP	AX
	MOV	[main_x], AX
_go4:
	MOV	AX, 4C00H
	INT	21H
