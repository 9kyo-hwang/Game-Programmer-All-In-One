#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

TUniquePtr<FThreadManager> GThreadManager = make_unique<FThreadManager>();