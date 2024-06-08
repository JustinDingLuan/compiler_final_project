;************** degree.asm ****************
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
degree_msg	DB	' please keyin a degree (deg) : ','$'
degree_msgDeg	DB	' deg = ','$'
degree_msgC	DB	' C degree in PROGRAM degree = ','$'
degree_msgF	DB	' F degree in PROGRAM degree = ','$'
degree_msgF2	DB	' F degree in PROCEDURE cTOf = ','$'
degree_deg	DW	0
degree_f	DW	0
degree_c	DW	0
_start1:
	JMP	_go2
cTOf:
	JMP	_start2
cTOf_f	DW	0
_start2:
	PUSH	WORD [degree_deg]
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
	dispstr	degree_msgF2
	itostr	cTOf_f, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	RET
_go2:
	JMP	_go3
fTOc:
	JMP	_start3
fTOc_g	DW	0
_start3:
	PUSH	WORD [degree_deg]
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
	MOV	[degree_c], AX
	RET
_go3:
	dispstr	degree_msg
	readstr	_buf
	strtoi	_buf, '$', degree_deg
	newline
	CALL	cTOf
	CALL	fTOc
	dispstr	degree_msgF
	itostr	degree_f, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	dispstr	degree_msgC
	itostr	degree_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
