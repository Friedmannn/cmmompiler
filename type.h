#ifndef GUARD_TYPE
#define GUARD_TYPE

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_{
	enum {basic, array, structrue} kind;
	union{
		//Basic type
		int basic;//1 is int and 2 is float;
		//Array type
		struct{
			Type elem;
			int size;
		}array;
		//Structure 
		FieldList structure;
	}u;
};

struct FieldList_{
	char* name;
	Type type;
	FieldList tail;  //Next Field;
};

extern int type_equal(Type t1, Type t2);
#endif
