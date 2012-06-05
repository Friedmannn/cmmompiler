#ifndef GUARD_HAPPY_TREE_FRIENDS
#define GUARD_HAPPY_TREE_FRIENDS

typedef struct HappyTreeFriends_ HappyTreeFriends;
struct HappyTreeFriends_{
	int happy;
	HappyTreeFriends* next;
};

extern HappyTreeFriends* happy_insert(HappyTreeFriends* root, HappyTreeFriends* t);


#endif
