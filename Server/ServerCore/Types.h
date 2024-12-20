#pragma once
#include <atomic>
#include <mutex>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#define Size16(val) static_cast<int16>(sizeof(val))
#define Size32(val) static_cast<int32>(sizeof(val))
#define Len16(arr) static_cast<int16>(sizeof(arr) / sizeof(arr[0]))
#define Len32(arr) static_cast<int32>(sizeof(arr) / sizeof(arr[0]))


template<typename T>
using TAtomic = std::atomic<T>;
using FCriticalSection = std::mutex;
using FEvent = std::condition_variable;
using FScopeLock = std::lock_guard<std::mutex>;
using FUniqueLock = std::unique_lock<std::mutex>;

template<typename T>
using TUniquePtr = std::unique_ptr<T>;
template<typename T>
using TSharedPtr = std::shared_ptr<T>;
template<typename T>
using TWeakPtr = std::weak_ptr<T>;
template<typename T>
using TSharedFromThis = std::enable_shared_from_this<T>;