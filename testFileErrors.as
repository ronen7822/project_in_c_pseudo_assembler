.entry		LIST
.ectern 	L3
 MAIN:  	add #r3, LIST
LOOP:	prn #4q8
	lea T4,r6
	inc r9
	mov r3, K,
	su1b  r1,  r4, r8
	bneq	 END
	cmp 	 K,  #-6
	bne  &END
	dec #W 
.entry  MAIN
 
	jmp &LOOP
	add L3, L3
END: stop A1
 STR: .string abcd"
LIST: .data 6,-9, -4 			-1r
.data 	 -100,, 4
K: .data 31,,3
.extern W
.data 40000000, -1e,
