LOOP:   LDA COUNT
SUB MAX
JGE END
LDA SUM
ADD COUNT
STO SUM
LDA COUNT
ADD ONE
STO COUNT
JMP LOOP
END:    STP
MAX:    100
COUNT:  1
SUM:    0
ONE:    1