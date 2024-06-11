;************** iftest.asm ****************
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
iftest_msg1	DB	' please key in a number (a): ','$'
iftest_msg2	DB	' please key in a number (b): ','$'
iftest_msg3	DB	' the bigger is ','$'
iftest_a	DW	0
iftest_b	DW	0
iftest_bigger	DW	0
_start1:
	dispstr	iftest_msg1
	readstr	_buf
	strtoi	_buf, '$', iftest_a
	newline
	dispstr	iftest_msg2
	readstr	_buf
	strtoi	_buf, '$', iftest_b
	newline
	PUSH	WORD [iftest_a]
	PUSH	WORD [iftest_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JGE	_go2
	JMP	_go3
_go2:
	PUSH	WORD [iftest_a]
	POP	AX
	MOV	[iftest_bigger], AX
_go3:
	PUSH	WORD [iftest_b]
	PUSH	WORD [iftest_a]
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go4
	JMP	_go5
_go4:
	PUSH	WORD [iftest_b]
	POP	AX
	MOV	[iftest_bigger], AX
_go5:
	dispstr	iftest_msg3
	itostr	iftest_bigger, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
