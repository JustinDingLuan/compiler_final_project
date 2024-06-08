;************** hcfmain.asm ****************
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
hcfmain_msg1	DB	' keyin a number (a): ','$'
hcfmain_msg2	DB	' keyin a number (b): ','$'
hcfmain_msg4	DB	'L.C.M =','$'
hcfmain_a	DW	0
hcfmain_b	DW	0
hcfmain_c	DW	0
hcfmain_hcf	DW	0
hcfmain_lcm	DW	0
_start1:
	JMP	_go2
hcfproc:
	JMP	_start2
hcfproc_msg3	DB	'H.C.F =','$'
hcfproc_big	DW	0
hcfproc_small	DW	0
_start2:
	JMP	_go3
nextdivide:
	JMP	_start3
nextdivide_q	DW	0
nextdivide_r	DW	0
_start3:
	PUSH	WORD [hcfproc_big]
	PUSH	WORD [hcfproc_small]
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[nextdivide_q], AX
	PUSH	WORD [hcfproc_big]
	PUSH	WORD [hcfproc_small]
	PUSH	WORD [nextdivide_q]
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[nextdivide_r], AX
	PUSH	WORD [hcfproc_small]
	POP	AX
	MOV	[hcfproc_big], AX
	PUSH	WORD [nextdivide_r]
	POP	AX
	MOV	[hcfproc_small], AX
	RET
_go3:
	PUSH	WORD [hcfmain_a]
	POP	AX
	MOV	[hcfproc_big], AX
	PUSH	WORD [hcfmain_b]
	POP	AX
	MOV	[hcfproc_small], AX
_go4:
	PUSH	WORD [hcfproc_small]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go5
	JMP	_go6
_go5:
	CALL	nextdivide
	JMP	_go4
_go6:
	dispstr	hcfproc_msg3
	itostr	hcfproc_big, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	PUSH	WORD [hcfproc_big]
	POP	AX
	MOV	[hcfmain_hcf], AX
	RET
_go2:
	JMP	_go7
lcmproc:
	JMP	_start7
_start7:
	PUSH	WORD [hcfmain_a]
	PUSH	WORD [hcfmain_b]
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	PUSH	WORD [hcfmain_hcf]
	POP	BX
	MOV	DX, 0
	POP	AX
	DIV	BX
	PUSH	AX
	POP	AX
	MOV	[hcfmain_lcm], AX
	RET
_go7:
	dispstr	hcfmain_msg1
	readstr	_buf
	strtoi	_buf, '$', hcfmain_a
	newline
	dispstr	hcfmain_msg2
	readstr	_buf
	strtoi	_buf, '$', hcfmain_b
	newline
	PUSH	WORD [hcfmain_a]
	PUSH	WORD [hcfmain_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JL	_go8
	JMP	_go9
_go8:
	PUSH	WORD [hcfmain_a]
	POP	AX
	MOV	[hcfmain_c], AX
	PUSH	WORD [hcfmain_b]
	POP	AX
	MOV	[hcfmain_a], AX
	PUSH	WORD [hcfmain_c]
	POP	AX
	MOV	[hcfmain_b], AX
_go9:
	CALL	hcfproc
	CALL	lcmproc
	dispstr	hcfmain_msg4
	itostr	hcfmain_lcm, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
