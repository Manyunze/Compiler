#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "translate.h"

struct Cx {patchList trues; patchList falses; T_stm stm;};

struct Tr_exp_ {enum {Tr_ex, Tr_nx, Tr_cx} kind;
                union{T_exp ex; T_stm nx; struct Cx cx;} u;
                };

static Tr_exp Tr_Ex(T_exp ex)
{
    Tr_exp p = (Tr_exp) checked_malloc(sizeof *p)
    p->kind = Tr_ex;
    p->u.ex = ex;
    return p;
}

static Tr_exp Tr_Nx(T_stm nx)
{
    Tr_exp p = (Tr_exp) checked_malloc(sizeof *p)
    p->kind = Tr_nx;
    p->u.nx = nx;
    return p;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm)
{
    Tr_exp p = (Tr_exp) checked_malloc(sizeof *p)
    p->kind = Tr_cx;
    p->u.cx.stm = stm;
    p->u.cx.trues = trues;
    p->u.cx.falses = falses;
    return p;
}

static T_exp unEx(Tr_exp e)
{
    switch (e->kind) {
        case Tr_ex:
            return e->u.ex;
        case Tr_cx: {
            Temp_temp r = Temp_newtemp();
            Temp_label t = Temp_newlabel(), f = Temp_newlabel();
            doPatch(e->u.cx.trues, t);
            doPatch(e->u.cx.falses, f);
            return T_Eseq(T_Move(T_Temp(r), T_Const(1)), 
                    T_Eseq(e->u.cx.stm,
                    T_Eseq(T_Label(f),
                    T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                    T_Eseq(T_Label(t),
                            T_Temp(r))))));
        }
        case Tr_nx: {
            return T_Eseq(e->u.nx, T_Const(0));
        }
        assert(0);
    }
}

static T_stm unNx(Tr_exp e)
{
    switch (e->kind) {
        case Tr_nx:
            return e->u.nx;
        case Tr_cx: {
            Temp_temp r = Temp_newtemp();
            Temp_label t = Temp_newlabel(), f = Temp_newlabel();
            doPatch(e->u.cx.trues, t);
            doPatch(e->u.cx.falses, f);
            return T_Exp(T_Eseq(T_Move(T_Temp(r), T_Const(1)), 
                    T_Eseq(e->u.cx.stm,
                    T_Eseq(T_Label(f),
                    T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                    T_Eseq(T_Label(t),
                            T_Temp(r)))))));
        }
        case Tr_ex: {
            return T_Exp(e->u.ex);
        }
        assert(0);
    }
}

static struct Cx unCx(Tr_exp e)
{
    switch (e->kind) {
        case Tr_cx:
            return e->u.cx;
        case Tr_ex: {
            struct Cx condCx;
            condCx.trues = NULL;
            condCx.falses = NULL;
            if (e->u.ex->u.kind == T_CONST) {
                condCx.stm = T_Cjump(T_gt, e->u.ex, T_Const(0), NULL, NULL);
                condCx.trues = PatchList(&(condCx.stm)->u.CJUMP.true, NULL);
                condCx.falses = PatchList(&(condCx.stm)->u.CJUMP.false, NULL);
            }
            else {
                condCx.stm = T_Exp(e->u.ex);
            }
            return condCx;
        }
        case Tr_nx:
            assert(0);
    }
    assert(0);
}

void doPatch(patchList tList, Temp_label label)
{
    for(; tList; tList=tList->tail)
    {
        *(tList->head) = label;
    }
}

patchList joinPatch(patchList first, patchList second)
{
    if(!first) return second;
    for(; first->tail; first=first->tail);
    first->tail = second;
    return first;
}
