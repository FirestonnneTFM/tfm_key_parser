#include "as3_op.hpp"

const char *AS3_OP[256];

// u-> a uint32_t value
// b-> a uint8_t value
// h-> a uint16_t value
// i-> an int32_t value, in s24 form

void init_as3_op() {
	for (int i = 0; i < 256; ++i) {
		AS3_OP[i] = nullptr;
	}
	AS3_OP[OP_getlocal] = "u";
	AS3_OP[OP_getlocal_0] = "";
	AS3_OP[OP_getlocal_1] = "";
	AS3_OP[OP_getlocal_2] = "";
	AS3_OP[OP_getlocal_3] = "";
	AS3_OP[OP_setlocal] = "u";
	AS3_OP[OP_setlocal_0] = "";
	AS3_OP[OP_setlocal_1] = "";
	AS3_OP[OP_setlocal_2] = "";
	AS3_OP[OP_setlocal_3] = "";
	AS3_OP[OP_pushscope] = "";
	AS3_OP[OP_callproperty] = "uu";
	AS3_OP[OP_getlex] = "u";
	AS3_OP[OP_initproperty] = "u";
	AS3_OP[OP_callproperty] = "uu";
	AS3_OP[OP_constructsuper] = "u";
	AS3_OP[OP_add] = "";
	AS3_OP[OP_findproperty] = "u";
	AS3_OP[OP_findpropstrict] = "u";
	AS3_OP[OP_construct] = "u";
	AS3_OP[OP_getproperty] = "u";
	AS3_OP[OP_dup] = "";
	AS3_OP[OP_call] = "u";
	AS3_OP[OP_pop] = "";
	AS3_OP[OP_kill] = "u";
	AS3_OP[OP_pushfalse] = "";
	AS3_OP[OP_pushtrue] = "";
	AS3_OP[OP_pushbyte] = "b";
	AS3_OP[OP_pushint] = "u";
	AS3_OP[OP_pushuint] = "u";
	AS3_OP[OP_pushshort] = "h";
	AS3_OP[OP_pushstring] = "u";
	AS3_OP[OP_returnvoid] = "";
	AS3_OP[OP_returnvalue] = "";
	AS3_OP[OP_not] = "";
	AS3_OP[OP_equals] = "";
	AS3_OP[OP_multiply] = "";
	AS3_OP[OP_divide] = "";
	AS3_OP[OP_setproperty] = "u";
	AS3_OP[OP_jump] = "i";
	AS3_OP[OP_convert_i] = "";
	AS3_OP[OP_convert_u] = "";
	AS3_OP[OP_convert_d] = "";
	AS3_OP[OP_convert_b] = "";
	AS3_OP[OP_convert_o] = "";
	AS3_OP[OP_bitxor] = "";
	AS3_OP[OP_lshift] = "";
	AS3_OP[OP_pushnull] = "";
	AS3_OP[OP_pushnan] = "";
	AS3_OP[OP_coerce_s] = "";
	AS3_OP[OP_coerce_o] = "";
	AS3_OP[OP_coerce] = "u";
	AS3_OP[OP_applytype] = "u";
	AS3_OP[OP_constructprop] = "uu";
	AS3_OP[OP_negate] = "";
	AS3_OP[OP_negate_i] = "";
	AS3_OP[OP_newactivation] = "";
	AS3_OP[OP_inclocal] = "u";
	AS3_OP[OP_inclocal_i] = "u";
	AS3_OP[OP_callpropvoid] = "uu";
	AS3_OP[OP_newcatch] = "u";
	AS3_OP[OP_swap] = "";
	AS3_OP[OP_setslot] = "u";
	AS3_OP[OP_popscope] = "";
	AS3_OP[OP_label] = "";
	AS3_OP[OP_ifeq] = "i";
	AS3_OP[OP_iffalse] = "i";
	AS3_OP[OP_ifge] = "i";
	AS3_OP[OP_ifgt] = "i";
	AS3_OP[OP_ifle] = "i";
	AS3_OP[OP_iflt] = "i";
	AS3_OP[OP_ifne] = "i";
	AS3_OP[OP_ifnge] = "i";
	AS3_OP[OP_ifngt] = "i";
	AS3_OP[OP_ifnle] = "i";
	AS3_OP[OP_ifnlt] = "i";
	AS3_OP[OP_ifstricteq] = "i";
	AS3_OP[OP_ifstrictneq] = "i";
	AS3_OP[OP_iftrue] = "i";
	AS3_OP[OP_getscopeobject] = "u";
}
