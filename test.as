.entry ABA
inc W
mov #4, r4
dec r3
stop
LIST: .data 6, 15,2
lea LIST, r2
inc LIST
ABA: .string "abcdefg"
ABAR: .data -1, 5, -222 ,-3 ,4,	-10324
add W ,r2
.extern W
