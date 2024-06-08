;************** whiletest.asm ****************
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
whiletest_msg1	DB	' keyin a number (a): ','$'
whiletest_msg2	DB	' keyin a number (b): ','$'
whiletest_msg3	DB	'H.C.F =','$'
whiletest_a	DW	0
whiletest_b	DW	0
whiletest_c	DW	0
whiletest_q	DW	0
whiletest_r	DW	0
whiletest_hcf	DW	0
_start1:
	dispstr	whiletest_msg1
	readstr	_buf
	strtoi	_buf, '$', whiletest_a
	newline
	dispstr	whiletest_msg2
	readstr	_buf
	strtoi	_buf, '$', whiletest_b
	newline
	PUSH	WORD [whiletest_a]
	PUSH	WORD [whiletest_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JL	_go2
	JMP	_go3
_go2:
	PUSH	WORD [whiletest_a]
	POP	AX
	MOV	[whiletest_c], AX
	PUSH	WORD [whiletest_b]
	POP	AX
	MOV	[whiletest_a], AX
	PUSH	WORD [whiletest_c]
	POP	AX
	MOV	[whiletest_b], AX
_go3:
_go4:
	PUSH	WORD [whiletest_b]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go5
	JMP	_go6
_go5:
	PUSH	WORD [whiletest_a]
	PUSH	WORD [whiletest_b]
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[whiletest_q], AX
	PUSH	WORD [whiletest_a]
	PUSH	WORD [whiletest_b]
	PUSH	WORD [whiletest_q]
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[whiletest_r], AX
	PUSH	WORD [whiletest_b]
	POP	AX
	MOV	[whiletest_a], AX
	PUSH	WORD [whiletest_r]
	POP	AX
	MOV	[whiletest_b], AX
	JMP	_go4
_go6:
	PUSH	WORD [whiletest_a]
	POP	AX
	MOV	[whiletest_hcf], AX
	dispstr	whiletest_msg3
	itostr	whiletest_hcf, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
