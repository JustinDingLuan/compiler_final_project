;************** calltest2.asm ****************
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
calltest2_msg1	DB	' keyin a number (a): ','$'
calltest2_msg2	DB	' keyin a number (b): ','$'
calltest2_msg3	DB	'H.C.F =','$'
calltest2_msg4	DB	'L.C.M =','$'
calltest2_a	DW	0
calltest2_b	DW	0
calltest2_c	DW	0
calltest2_hcf	DW	0
calltest2_lcm	DW	0
_start1:
	JMP	_go2
hcfproc:
	JMP	_start2
hcfproc_big	DW	0
hcfproc_small	DW	0
hcfproc_q	DW	0
hcfproc_r	DW	0
_start2:
	PUSH	WORD [calltest2_a]
	POP	AX
	MOV	[hcfproc_big], AX
	PUSH	WORD [calltest2_b]
	POP	AX
	MOV	[hcfproc_small], AX
_go3:
	PUSH	WORD [hcfproc_small]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go4
	JMP	_go5
_go4:
	PUSH	WORD [hcfproc_big]
	PUSH	WORD [hcfproc_small]
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[hcfproc_q], AX
	PUSH	WORD [hcfproc_big]
	PUSH	WORD [hcfproc_small]
	PUSH	WORD [hcfproc_q]
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[hcfproc_r], AX
	PUSH	WORD [hcfproc_small]
	POP	AX
	MOV	[hcfproc_big], AX
	PUSH	WORD [hcfproc_r]
	POP	AX
	MOV	[hcfproc_small], AX
	JMP	_go3
_go5:
	PUSH	WORD [hcfproc_big]
	POP	AX
	MOV	[calltest2_hcf], AX
	RET
_go2:
	JMP	_go6
lcmproc:
	JMP	_start6
_start6:
	PUSH	WORD [calltest2_a]
	PUSH	WORD [calltest2_b]
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	PUSH	WORD [calltest2_hcf]
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[calltest2_lcm], AX
	RET
_go6:
	dispstr	calltest2_msg1
	readstr	_buf
	strtoi	_buf, '$', calltest2_a
	newline
	dispstr	calltest2_msg2
	readstr	_buf
	strtoi	_buf, '$', calltest2_b
	newline
	PUSH	WORD [calltest2_a]
	PUSH	WORD [calltest2_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JL	_go7
	JMP	_go8
_go7:
	PUSH	WORD [calltest2_a]
	POP	AX
	MOV	[calltest2_c], AX
	PUSH	WORD [calltest2_b]
	POP	AX
	MOV	[calltest2_a], AX
	PUSH	WORD [calltest2_c]
	POP	AX
	MOV	[calltest2_b], AX
_go8:
	CALL	hcfproc
	dispstr	calltest2_msg3
	itostr	calltest2_hcf, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	CALL	lcmproc
	dispstr	calltest2_msg4
	itostr	calltest2_lcm, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
