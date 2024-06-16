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
main_temp1	DW
_go2:
	PUSH	WORD [main_b]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[main_b], AX
	itostr	main_b, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	PUSH	WORD [main_b]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go2
_go3:
	PUSH	WORD [main_d]
	PUSH	1
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go4
	JMP	_go5
_go4:
	PUSH	89
	POP	AX
	MOV	[main_x], AX
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
	JMP	_go6
_go5:
	PUSH	88
	POP	AX
	MOV	[main_x], AX
_go6:
	readstr	_buf
	strtoi	_buf, '$', main_a
	newline
_go7:
	PUSH	WORD [main_a]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go8
	JMP	_go9
_go8:
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
	PUSH	WORD [main_a]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[main_a], AX
	JMP	_go7
_go9:
	MOV	AX, 4C00H
	INT	21H
