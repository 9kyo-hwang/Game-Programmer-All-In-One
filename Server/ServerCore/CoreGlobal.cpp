#include "pch.h"
#include "CoreGlobal.h"
#include "FThreadManager.h"

TUniquePtr<FThreadManager> GThreadManager = make_unique<FThreadManager>();