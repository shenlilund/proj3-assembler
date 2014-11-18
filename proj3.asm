SUB R6 R6
ADI R6 #4
SUB R11 R6
MOV R0 R11
SUB R10 R6
MOV R11 R10
SUB R10 R6
STR R0 R10
SUB R10 R6
SUB R0 R0
ADI R0 #1
STR R0 R10		; Store w = 1 on the stack (passed parameter)
SUB R10 R6
SUB R0 R0 		; clear R0
STR R0 R10 		; Store x = 0 on the stack (passed parameter)
SUB R10 R6
STR R0 R10 		; store y = 0 on the stack (passed parameter)
SUB R10 R6
STR R0 R10 		; store z = 0 on the stack
SUB R10 R6 		; make space for local k variable
MOV R1 R8
ADI R1 #60
STR R1 R11 		; put the return address at the FP
JMP RESET
JMP END
RESET LDA R0 C 	; put start of C array in R0
MOV R2 R11	 	; put FP in R2
ADI R2 #-20
SUB R1 R1 		; R1 = 0
MOV R3 R11 		; duplicate FP into R3
SUB R4 R4
ADI R4 #24 		; move up 24 bytes from FP
SUB R3 R4 		; R3 now has where k is stored
STR R1 R3 		; Store k variable on stack
LDR R2 ZERO		; R2 has '0'
LOOP STB R2 R0 		; loop label, store '0' in c[k]
ADI R0 #1 
LDR R1 R3 		;grab k value and put in R1
ADI R1 #1
LDR R4 SIZE
SUB R4 R1 		; size - k (if greater than zero, keep looping)
STR R1 R3 		; store k variable on the stack
BGT R4 LOOP
MOV R3 R11		; duplicate FP into r3
SUB R4 R4		
ADI R4 #8    	
SUB R3 R4 		; move pointer up to correct spot
LDR R0 R3  		; load data from R3 address into R0
LDA R2 DATA
STR R0 R2 		; store data from R0 into R2 address
SUB R4 R4
ADI R4 #4    	
SUB R3 R4 		; move pointer up to correct spot
LDR R0 R3  		; load data from R3 address into R0
LDA R2 OPDV
STR R0 R2 		; store data from R0 into R2 address
SUB R3 R4 		; move pointer up to correct spot
LDR R0 R3  		; load data from R3 address into R0
LDA R2 CNT
STR R0 R2 		; store data from R0 into R2 address
SUB R3 R4 		; move pointer up to correct spot
LDR R0 R3  		; load data from R3 address into R0
LDA R2 FLAG
STR R0 R2 		; store data from R0 into R2 address
LDR R5 R11
MOV R10 R11 	; mov SP FP (SP = FP)
MOV R11 R3
SUB R3 R4
MOV R11 R0
LDR R11 R3 		; FP = PFP
JMR R0
SIZE .INT 7
CNT .INT 0
TENTH .INT 0
DATA .INT 0
FLAG .INT 0
OPDV .INT 0
ZERO .BYT '0'
C .BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
.BYT '0'
END TRP 0