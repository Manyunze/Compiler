#include<stdio.h>
#include<stdlib.h>
#include"util.h"
#include"symbol.h"
#include"types.h"
#include "absyn.h"
#include"temp.h"
#include"tree.h"
#include"frame.h"
#include"translate.h"
#include"env.h"


E_enventry E_VarEntry(Tr_access access, Ty_ty ty) {
	E_enventry entry = checked_malloc(sizeof(*entry));
	entry->kind = E_varEntry;
	entry->u.var.ty = ty;
	entry->u.var.access = access;
	return entry;
}

E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals, Ty_ty result) {
	E_enventry entry = checked_malloc(sizeof(*entry));
	entry->kind = E_funEntry;
	entry->u.fun.formals = formals;
	entry->u.fun.result = result;
	entry->u.fun.level = level;
	entry->u.fun.label = label;
	return entry;
}

/* Ty_ty environment */

S_table E_base_tenv(void) {
	S_table table = S_empty();
	S_enter(table, S_Symbol("int"), Ty_Int());
	S_enter(table, S_Symbol("string"), Ty_String());
	return table;
}

/* E_enventry environment */

S_table E_base_venv(void) {
	S_table table = S_empty();
	Tr_level level;

	//Built-in functions are put in the same level where the main sits
	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("print"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("print"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Void()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("flush"), NULL);
	S_enter(table, S_Symbol("flush"), E_FunEntry(level, Temp_newlabel(), NULL, Ty_Void()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("getchar"), NULL);
	S_enter(table, S_Symbol("getchar"), E_FunEntry(level, Temp_newlabel(), NULL, Ty_String()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("ord"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("ord"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Int()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("chr"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("chr"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_String()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("size"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("size"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Int()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("substring"), U_BoolList(TRUE, U_BoolList(TRUE, U_BoolList(TRUE, NULL))));
	S_enter(table, S_Symbol("substring"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_String(), Ty_TyList(Ty_Int(), Ty_TyList(Ty_Int(), NULL))), Ty_String()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("concat"), U_BoolList(TRUE, U_BoolList(TRUE, NULL)));
	S_enter(table, S_Symbol("concat"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_String(), Ty_TyList(Ty_String(), NULL)), Ty_String()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("not"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("not"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_Int()));

	level = Tr_newLevel(Tr_outermost(), Temp_namedlabel("exit"), U_BoolList(TRUE, NULL));
	S_enter(table, S_Symbol("exit"), E_FunEntry(level, Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_Void()));

	return table;
}
