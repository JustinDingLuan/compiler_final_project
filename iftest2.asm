;************** iftest2.asm ****************
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
iftest2_msg1	DB	' please key in a number (a): ','$'
iftest2_msg2	DB	' please key in a number (b): ','$'
iftest2_msg3	DB	' the bigger is ','$'
iftest2_a	DW	0
iftest2_b	DW	0
iftest2_bigger	DW	0
_start1:
	dispstr	iftest2_msg1
	readstr	_buf
	strtoi	_buf, '$', iftest2_a
	newline
	dispstr	iftest2_msg2
	readstr	_buf
	strtoi	_buf, '$', iftest2_b
	newline
	PUSH	WORD [iftest2_a]
	PUSH	WORD [iftest2_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JGE	_go2
	JMP	_go3
_go2:
	PUSH	WORD [iftest2_a]
	POP	AX
	MOV	[iftest2_bigger], AX
	dispstr	iftest2_msg3
	itostr	iftest2_bigger, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
_go3:
	PUSH	WORD [iftest2_b]
	PUSH	WORD [iftest2_a]
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go4
	JMP	_go5
_go4:
	PUSH	WORD [iftest2_b]
	POP	AX
	MOV	[iftest2_bigger], AX
	dispstr	iftest2_msg3
	itostr	iftest2_bigger, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
_go5:
	MOV	AX, 4C00H
	INT	21H
