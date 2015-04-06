/*
 * File:    arm7i_msr_12.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   
 *
 * Copyright (c) 2009  Li XianJing
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-1-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <jmachine.h>
#include <jinstruction_arm7.h>

JRet arm7_12_execute(JInstruction* thiz, jinst_t instr)
{
	J_DECL_MACHINE(thiz);
	j_ret_val_if_fail(m != NULL, JRET_INVALID_PARAM);

	if(J_RDEST(instr) == J_RPC)
	{
		if(J_RLHS(instr) == J_RPC)
		{
			if(J_BIT(instr, 5))
			{
				m->r[J_RLR] = m->pc + I_SIZE;	
			}
			m->r[J_RPC] = m->r[J_RRHS(instr)];
		}
		else
		{
			jword_t cpsr  = J_DPRHS;
			arm7_fix_cpsr(m, instr, cpsr);
		}
	}

	return JRET_OK;
}

JRet arm7_12_das(JInstruction* thiz, jinst_t instr, char* t)
{
	j_ret_val_if_fail(t != NULL, JRET_INVALID_PARAM);
	
	J_DECL_MACHINE(thiz);
	if(J_RDEST(instr) == J_RPC)
	{
		if(J_RLHS(instr) == J_RPC)
		{
			m->r[J_RPC] = m->r[J_RLR];
			sprintf(t, "b%sx %s\n",
				J_BIT(instr, 5) ? "l":"",
				arm7_r_name(J_RRHS(instr)));
		}
		else
		{
			sprintf(t, "msr%s CPSR_%s%s%s%s, %s\n",
				arm7_cond_name(J_ARM_COND(instr)),
				J_BIT(instr, 19) ? "f":"",
				J_BIT(instr, 18) ? "s":"",
				J_BIT(instr, 17) ? "x":"",
				J_BIT(instr, 16) ? "c":"",
				arm7_r_name(J_RRHS(instr)));
		}
	}

	return JRET_OK;
}

JRet arm7_12_as(JInstruction* thiz, const char* t, jinst_t* i)
{
	j_ret_val_if_fail(thiz != NULL, JRET_INVALID_PARAM);

	return JRET_OK;
}

void arm7_12_test(void)
{
	char str[256] = {0};
	jword_t result = 0;
	jword_t text[12] = {0xe121f001, 0xe12fff33, 0xe12fff1e};

	JInstruction* thiz = ARM7_INSTR_CREATE(12);
	JMachine* m = jmachine_create(text, sizeof(text), 0);
	
	jinstruction_init(thiz, m);
	assert(jinstruction_execute(thiz, text[0]) == JRET_OK);
	assert(jinstruction_das(thiz, text[0], str) == JRET_OK);
	printf("%s", str);
	assert(jinstruction_execute(thiz, text[1]) == JRET_OK);
	assert(jinstruction_das(thiz, text[1], str) == JRET_OK);
	printf("%s", str);
	assert(jinstruction_execute(thiz, text[2]) == JRET_OK);
	assert(jinstruction_das(thiz, text[2], str) == JRET_OK);
	printf("%s", str);

	jinstruction_destroy(thiz);

	return ;
}
