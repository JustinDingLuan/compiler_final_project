;************** readwrite.asm ****************
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
readwrite_msg1	DB	' please key in a number: ','$'
readwrite_msg2	DB	' number keyed in is ','$'
readwrite_msg3	DB	' value doubled is ','$'
readwrite_n	DW	0
readwrite_d	DW	0
_start1:
	dispstr	readwrite_msg1
	readstr	_buf
	strtoi	_buf, '$', readwrite_n
	newline
	dispstr	readwrite_msg2
	itostr	readwrite_n, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	PUSH	WORD [readwrite_n]
	PUSH	2
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	AX
	MOV	[readwrite_d], AX
	dispstr	readwrite_msg3
	itostr	readwrite_d, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
