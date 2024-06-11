;************** readwrite2.asm ****************
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
readwrite2_msg1	DB	' please key in two numbers: ','$'
readwrite2_msg2	DB	' numbers keyed in are ','$'
readwrite2_msg3	DB	' sum is ','$'
readwrite2_m	DW	0
readwrite2_n	DW	0
readwrite2_sum	DW	0
_start1:
	dispstr	readwrite2_msg1
	readstr	_buf
	strtoi	_buf, '$', readwrite2_m
	newline
	readstr	_buf
	strtoi	_buf, '$', readwrite2_n
	newline
	dispstr	readwrite2_msg2
	itostr	readwrite2_m, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	itostr	readwrite2_n, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	PUSH	WORD [readwrite2_m]
	PUSH	WORD [readwrite2_n]
	POP	BX
	POP	AX
	ADD	AX, BX
	PUSH	AX
	POP	AX
	MOV	[readwrite2_sum], AX
	dispstr	readwrite2_msg3
	itostr	readwrite2_sum, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
