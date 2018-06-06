/*
 * translate.h - translate abstract language to tree medium language
 *
 */
typedef struct Tr_exp_ *Tr_exp;
typedef struct patchList_ *patchList;
struct patchList_ {Temp_label *head; patchList tail;};
static patchList PatchList(Temp_label *head, patchList tail);

static Tr_exp Tr_Ex(T_exp ex);
static Tr_exp Tr_Nx(T_stm nx);
static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm);

static T_exp unEx(Tr_exp e);
static T_stm unNx(Tr_exp e);
static struct Cx unCx(Tr_exp e);

void doPatch(patchList tList, Temp_label label);
patchList joinPatch(patchList first, patchList second);

//void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals);
//F_fragList Tr_getResult(void);