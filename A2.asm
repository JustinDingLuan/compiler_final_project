;************** A2.asm ****************
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
A2_m	DW	0
A2_n	DW	0
_start1:
	JMP	_go2
B:
	JMP	_start2
B_n	DW	0
B_k	DW	0
_start2:
	PUSH	2
	POP	AX
	MOV	[B_n], AX
	itostr	B_n, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	RET
_go2:
	PUSH	1
	POP	AX
	MOV	[A2_n], AX
	itostr	A2_n, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	CALL	B
	MOV	AX, 4C00H
	INT	21H
