#pragma once

#define SAFE_DELETE(Ptr)	\
	if(Ptr)					\
	{						\
		delete Ptr;			\
		Ptr = nullptr;		\
	}