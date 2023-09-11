; Test ADC Instruction
; Expected result: reg a = 80h(128)
MVI C, 3DH
MVI A, 42H
CMC ; Complementary carry
ADC C
