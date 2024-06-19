;************** Error1.asm ****************
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
Error1_GlbVar1	DW	0
	PUSH	20
	POP	AX
	MOV	[Error1_GlbVar1], AX
Error1_GlbVar2	DW	'ntpucsie','$'
_start1:
	JMP	_go2
cTOf:
cTOf_c	DW	0
	POP	AX
	MOV	[cTOf_c], AX
cTOf_f	DW	0
	POP	AX
	MOV	[cTOf_f], AX
	PUSH	WORD [cTOf_c]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go3
	JMP	_go4
_go3:
	PUSH	WORD [cTOf_c]
	PUSH	9
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	PUSH	5
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	PUSH	32
	POP	BX
	POP	AX
	ADD	AX, BX
	PUSH	AX
	POP	AX
	MOV	[cTOf_f], AX
	JMP	_go5
_go4:
	itostr	cTOf_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
_go5:
	itostr	cTOf_f, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	RET
_go2:
	JMP	_go6
fTOc:
fTOc_f	DW	0
	PUSH	212
	POP	AX
	MOV	[fTOc_f], AX
fTOc_c	DW	0
	PUSH	0
	POP	AX
	MOV	[fTOc_c], AX
	PUSH	WORD [fTOc_f]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go7
	JMP	_go8
_go7:
	PUSH	WORD [fTOc_f]
	PUSH	32
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	PUSH	5
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	PUSH	9
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[fTOc_c], AX
	JMP	_go9
_go8:
	itostr	fTOc_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
_go9:
	RET
_go6:
	JMP	_go10
looptest:
looptest_a	DW	0
	PUSH	5
	POP	AX
	MOV	[looptest_a], AX
_go11:
	PUSH	WORD [looptest_a]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go12
	JMP	_go13
_go12:
	PUSH	WORD [looptest_a]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[looptest_a], AX
	itostr	looptest_a, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	JMP	_go11
_go13:
	RET
_go10:
	JMP	_main
_main:
main_a	DW	0
main_b	DW	0
	PUSH	3
	POP	AX
	MOV	[main_b], AX
main_c	DW	0
main_d	DW	0
	PUSH	5
	POP	AX
	MOV	[main_d], AX
main_x	DW	0
	PUSH	0
	POP	AX
	MOV	[main_x], AX
main_w	DW	0
	PUSH	WORD [main_x]
	POP	AX
	MOV	[main_w], AX
	dispstr	Error1_GlbVar2
_go14:
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
	JG	_go14
_go15:
	readstr	_buf
	strtoi	_buf, '$', main_c
	newline
	PUSH	WORD [main_c]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go16
	JMP	_go17
_go16:
	PUSH	100
	POP	AX
	MOV	[main_x], AX
	JMP	_go18
_go17:
	PUSH	100
	POP	AX
	MOV	[main_x], AX
_go18:
_go19:
	PUSH	WORD [main_d]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JNE	_go20
	JMP	_go21
_go20:
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
	PUSH	WORD [main_d]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[main_d], AX
	JMP	_go19
_go21:
	CALL	cTOf
	CALL	looptest
	MOV	AX, 4C00H
	INT	21H
