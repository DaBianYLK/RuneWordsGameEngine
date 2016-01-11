#define SafeDelete(ptr)\
	if (ptr)\
	{\
		delete ptr;\
		ptr = nullptr;\
	}