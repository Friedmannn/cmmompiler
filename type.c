
#include "type.h"

int type_equal(Type t1, Type t2){
	if(t1 == 0 || t2 == 0)
		return 0;
	if(t1->kind != t2->kind)
		return 0;
	if(t1->kind == basic)
		return (t1->u.basic == t2->u.basic);
	else if(t1->kind == array){
		return (type_equal(t1->u.array.elem, t2->u.array.elem));
	}
	return 0;
}

