;************** vartest2.asm ****************
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
vartest2_msg1	DB	' local variable (b) from PROCEDURE is ','$'
vartest2_msg2	DB	' local variable (b) from PROGRAM   is ','$'
vartest2_a	DW	0
_start1:
	JMP	_go2
proc:
	JMP	_start2
proc_b	DW	0
_start2:
	PUSH	123
	POP	AX
	MOV	[proc_b], AX
	dispstr	vartest2_msg1
	itostr	proc_b, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	RET
_go2:
	CALL	proc
	dispstr	vartest2_msg2
	MOV	AX, 4C00H
	INT	21H
