#pragma once

// https://www.srividyaengg.ac.in/coursematerial/EEE/105522.pdf
// https://electricalvoice.com/opcodes-8085-microprocessor/
namespace lib8085
{
	enum InstructionSet
	{
		ACI, 
		ADC_A,
		ADC_B,
		ADC_C,
		ADC_D,
		ADC_E,
		ADC_H,
		ADC_L,
		ADC_M,
		ADD_A,
		ADD_B,
		ADD_C,
		ADD_D,
		ADD_E,
		ADD_H,
		ADD_L,
		ADD_M,
		ADI,
		ANA_A,
		ANA_B,
		ANA_C,
		ANA_D,
		ANA_E,
		ANA_H,
		ANA_L,
		ANA_M,
		ANI,
		CALL,
		CC,
		CM,
		CMA,
		CMC,
		CMP_A,
		CMP_B,
		CMP_C,
		CMP_D,
		CMP_E,
		CMP_H,
		CMP_L,
		CMP_M,
		CNC,
		CNZ,
		CP,
		CPE,
		CPI,
		CPO,
		CZ,
		DAA,
		DAD_B,
		DAD_D,
		DAD_H,
		DAD_SP,
		DCR_A,
		DCR_B,
		DCR_C,
		DCR_D,
		DCR_E,
		DCR_H,
		DCR_L,
		DCR_M,
		DCX_B,
		DCX_D,
		DCX_H,
		DCX_SP,
		DI,
		EI,
		HLT,
		IN,
		INR_A,
		INR_B,
		INR_C,
		INR_D,
		INR_E,
		INR_H,
		INR_L,
		INR_M,
		INX_B,
		INX_D,
		INX_H,
		INX_SP,
		JC,
		JM,
		JMP,
		JNC,
		JNZ,
		JP,
		JPE,
		JPO,
		JZ,
		LDA,
		LDAX_B,
		LDAX_D,
		LHLD,
		LXI_B,
		LXI_D,
		LXI_H,
		LXI_SP,
		MOV_A_A,
		MOV_A_B,
		MOV_A_C,
		MOV_A_D,
		MOV_A_E,
		MOV_A_H,
		MOV_A_L,
		MOV_A_M,
		MOV_B_A,
		MOV_B_B,
		MOV_B_C,
		MOV_B_D,
		MOV_B_E,
		MOV_B_H,
		MOV_B_L,
		MOV_B_M,
		MOV_C_A,
		MOV_C_B,
		MOV_C_C,
		MOV_C_D,
		MOV_C_E,
		MOV_C_H,
		MOV_C_L,
		MOV_C_M,
		MOV_D_A,
		MOV_D_B,
		MOV_D_C,
		MOV_D_D,
		MOV_D_E,
		MOV_D_H,
		MOV_D_L,
		MOV_D_M,
		MOV_E_A,
		MOV_E_B,
		MOV_E_C,
		MOV_E_D,
		MOV_E_E,
		MOV_E_H,
		MOV_E_L,
		MOV_E_M,
		MOV_H_A,
		MOV_H_B,
		MOV_H_C,
		MOV_H_D,
		MOV_H_E,
		MOV_H_H,
		MOV_H_L,
		MOV_H_M,
		MOV_L_A,
		MOV_L_B,
		MOV_L_C,
		MOV_L_D,
		MOV_L_E,
		MOV_L_H,
		MOV_L_L,
		MOV_L_M,
		MOV_M_A,
		MOV_M_B,
		MOV_M_C,
		MOV_M_D,
		MOV_M_E,
		MOV_M_H,
		MOV_M_L,
		MVI_A,
		MVI_B,
		MVI_C,
		MVI_D,
		MVI_E,
		MVI_H,
		MVI_L,
		MVI_M,
		NOP,
		ORA_A,
		ORA_B,
		ORA_C,
		ORA_D,
		ORA_E,
		ORA_H,
		ORA_L,
		ORA_M,
		ORI,
		OUT,
		PCHL,
		POP_B,
		POP_D,
		POP_H,
		POP_PSW,
		PUSH_B,
		PUSH_D,
		PUSH_H,
		PUSH_PSW,
		RAL,
		RAR,
		RC,
		RET,
		RIM,
		RLC,
		RM,
		RNC,
		RNZ,
		RP,
		RPE,
		RPO,
		RRC,
		RST_0,
		RST_1,
		RST_2,
		RST_3,
		RST_4,
		RST_5,
		RST_6,
		RST_7,
		RZ,
		SBB_A,
		SBB_B,
		SBB_C,
		SBB_D,
		SBB_E,
		SBB_H,
		SBB_L,
		SBB_M,
		SBI,
		SHLD,
		SIM,
		SPHL,
		STA,
		STAX_B,
		STAX_D,
		STAX_H,
		STC,
		SUB_A,
		SUB_B,
		SUB_C,
		SUB_D,
		SUB_E,
		SUB_H,
		SUB_L,
		SUB_M,
		SUI,
		XCHG,
		XRA_A,
		XRA_B,
		XRA_C,
		XRA_D,
		XRA_E,
		XRA_H,
		XRA_L,
		XRA_M,
		XRI,
		XTHL
    };
}
