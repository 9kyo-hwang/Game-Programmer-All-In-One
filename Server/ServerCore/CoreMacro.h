#pragma once

// ∏≈≈©∑Œ
#define MUTEX_MULTI(Count) mutex MutexArray[Count];
#define MUTEX MUTEX_MULTI(1)
#define LOCK_INDEX(Index) FScopeLock Lock_##Index(MutexArray[Index]);
#define LOCK LOCK_INDEX(0)