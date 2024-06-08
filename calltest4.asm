;************** calltest4.asm ****************
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
calltest4_msg1	DB	' please keyin a degree (deg) : ','$'
calltest4_msg2	DB	' deg = ','$'
calltest4_msg3	DB	' c = ','$'
calltest4_msg4	DB	' f = ','$'
calltest4_deg	DW	0
calltest4_f	DW	0
calltest4_c	DW	0
_start1:
	JMP	_go2
cTOf:
	JMP	_start2
cTOf_fdeg	DW	0
cTOf_deg	DW	0
_start2:
	PUSH	WORD [cTOf_deg]
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
	MOV	[cTOf_fdeg], AX
	PUSH	WORD [cTOf_fdeg]
	POP	AX
	MOV	[calltest4_f], AX
	PUSH	WORD [cTOf_fdeg]
	POP	AX
	MOV	[cTOf_deg], AX
	itostr	cTOf_deg, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	RET
_go2:
	JMP	_go3
fTOc:
	JMP	_start3
_start3:
	PUSH	WORD [cTOf_deg]
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
	MOV	[calltest4_c], AX
	RET
_go3:
	dispstr	calltest4_msg1
	readstr	_buf
	strtoi	_buf, '$', calltest4_deg
	newline
	CALL	cTOf
	dispstr	calltest4_msg2
	itostr	calltest4_deg, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	dispstr	calltest4_msg4
	itostr	calltest4_f, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	CALL	fTOc
	dispstr	calltest4_msg2
	itostr	calltest4_deg, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	dispstr	calltest4_msg3
	itostr	calltest4_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
