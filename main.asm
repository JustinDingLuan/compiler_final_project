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
main_temp	DW
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
	PUSH	30
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
	PUSH	0
	POP	AX
	MOV	[main_w], AX
	PUSH	10
	POP	AX
	MOV	[main_a], AX
	PUSH	70
	POP	AX
	MOV	[main_c], AX
main_bbcc	DW	0
	PUSH	77
	POP	AX
	MOV	[main_bbcc], AX
	PUSH	WORD [main_x]
	PUSH	1
	POP	BX
	POP	AX
	ADD	AX, BX
	PUSH	AX
	POP	AX
	MOV	[main_bbcc], AX
	JMP	_go4
_go3:
	PUSH	88
	POP	AX
	MOV	[main_x], AX
	PUSH	87
	POP	AX
	MOV	[main_w], AX
	PUSH	9
	POP	AX
	MOV	[main_a], AX
	PUSH	69
	POP	AX
	MOV	[main_c], AX
_go4:
main_z	DW	0
	PUSH	0
	POP	AX
	MOV	[main_z], AX
_go5:
	PUSH	WORD [main_x]
	PUSH	80
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go6
	JMP	_go7
_go6:
	PUSH	WORD [main_x]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[main_x], AX
	itostr	main_x, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	itostr	main_w, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	itostr	main_z, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	JMP	_go5
_go7:
	MOV	AX, 4C00H
	INT	21H
