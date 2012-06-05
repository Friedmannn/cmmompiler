#include "happy_tree_friends.h"

HappyTreeFriends* happy_insert(HappyTreeFriends* root, HappyTreeFriends* t){
	t->next = root;
	return t;
}
