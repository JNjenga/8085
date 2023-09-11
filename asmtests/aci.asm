; Tests ACI instruction
; Conditions: Carry flag should be set before running aci instruction
; Expected Results: reg a = 57h(87)
MVI A, 14H
CMC ; Complementary carry
ACI 42h
