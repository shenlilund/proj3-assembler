LDA R3 OPDV
ADI R3 #4
LDA R4 END
ADI R4 #12
ADD R4 R3		;R4 contains instruction size
LDR R1 SB
SUB R1 R4		; SL (number) will be in R1
LDA R2 SL		; address of SL in R2
STR R1 R2		; store the stack length into SL variable
SL .INT 0
SP .INT 2999
FP .INT 2999
SB .INT 2999
SIZE .INT 7
CNT .INT 0
TENTH .INT 0
DATA .INT 0
FLAG .INT 0
C .BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
OPDV .INT 0
END TRP 0