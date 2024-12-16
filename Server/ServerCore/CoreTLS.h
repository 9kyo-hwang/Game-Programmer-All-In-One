#pragma once

// 쓰레드 별 전역 공간을 TLS(Thread Local Storage) 영역이라 부름
extern thread_local uint32 TLS_ThreadID;