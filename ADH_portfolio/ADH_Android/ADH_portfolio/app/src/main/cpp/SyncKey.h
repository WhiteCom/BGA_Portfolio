#pragma once
#include "std/iType.h"
#include "std/iPoint.h"
//#include "../../../../../../../../../AppData/Local/Android/Sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/linux/time.h"


#define _qkNum 1000
struct SyncKey
{
    //Queue를 이용하여 쓰레드 처리
    iKeyStat stat;
    iPoint point;
    int qkNum, qkInsertNum, qkRemoveNum=0, qkMax = 0;
    SyncKey* qk, *qkInsert;
    struct timeval prevQueuekey {0,0};

    void loadKey();
    void freeKey();
    void insertKey(iKeyStat stat, iPoint point);
    void removeKey();
    void updateKey();
};