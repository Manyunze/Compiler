/*
 * temp.h 
 * ------
 * functions to create and manipulate temporary variables which are
 * used in the IR tree representation before it has been determined
 * which variables are to go into registers.
 *
 */
#ifndef _TEMP_H_
#define _TEMP_H_

/*
 * Temp_label --> Abstract name for static memory addresses
 * Temp_temp  --> Abstract name for local vars
 * Temp_map   --> For convenience, bind the temp register to a string representation
 *                (e.g. register t138 to "T_138"). Can be interpreted as a local register pool
 */
typedef struct Temp_temp_ *Temp_temp;
Temp_temp Temp_newtemp(void);

typedef struct Temp_tempList_ *Temp_tempList;
struct Temp_tempList_ { Temp_temp head; Temp_tempList tail;};
Temp_tempList Temp_TempList(Temp_temp h, Temp_tempList t);

typedef S_symbol Temp_label;
Temp_label Temp_newlabel(void);
Temp_label Temp_namedlabel(string name);
string Temp_labelstring(Temp_label s);

typedef struct Temp_labelList_ *Temp_labelList;
struct Temp_labelList_ { Temp_label head; Temp_labelList tail;};
Temp_labelList Temp_LabelList(Temp_label h, Temp_labelList t);

// Map is a linked list of table
typedef struct Temp_map_ *Temp_map;
Temp_map Temp_empty(void);
Temp_map Temp_layerMap(Temp_map over, Temp_map under);
void Temp_enter(Temp_map m, Temp_temp t, string s);
string Temp_look(Temp_map m, Temp_temp t);
void Temp_dumpMap(FILE *out, Temp_map m);

Temp_map Temp_name(void);

#endif
