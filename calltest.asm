;************** calltest.asm ****************
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
calltest_msg1	DB	' please keyin a degree (deg) : ','$'
calltest_msg2	DB	' deg = ','$'
calltest_msg3	DB	' c = ','$'
calltest_msg4	DB	' f = ','$'
calltest_deg	DW	0
calltest_f	DW	0
calltest_c	DW	0
_start1:
	JMP	_go2
cTOf:
	JMP	_start2
_start2:
	PUSH	WORD [calltest_deg]
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
	MOV	[calltest_f], AX
	RET
_go2:
	JMP	_go3
fTOc:
	JMP	_start3
_start3:
	PUSH	WORD [calltest_deg]
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
	MOV	[calltest_c], AX
	RET
_go3:
	dispstr	calltest_msg1
	readstr	_buf
	strtoi	_buf, '$', calltest_deg
	newline
	CALL	cTOf
	dispstr	calltest_msg2
	itostr	calltest_deg, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	dispstr	calltest_msg4
	itostr	calltest_f, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	CALL	fTOc
	dispstr	calltest_msg2
	itostr	calltest_deg, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	dispstr	calltest_msg3
	itostr	calltest_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
