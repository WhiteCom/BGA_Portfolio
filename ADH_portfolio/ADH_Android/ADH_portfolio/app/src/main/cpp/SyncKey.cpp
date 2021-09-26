#include "iNDK.h"
#include "SyncKey.h"
#include "Game.h"

void SyncKey::loadKey()
{
    qk = new SyncKey[_qkNum];
    qkNum = 0;

    qkInsert = new SyncKey[_qkNum];
    qkInsertNum = 0;
}

void SyncKey::freeKey()
{
    delete qk;
    delete qkInsert;
}

void SyncKey::insertKey(iKeyStat stat, iPoint point) // thread 000
{
    timeval curr;
    gettimeofday(&curr, NULL);
    float delta = (curr.tv_sec - prevQueuekey.tv_sec) +
                  (curr.tv_usec - prevQueuekey.tv_usec) / 1000000.f;

    if( stat==iKeyStatMoved && delta < 0.016f)
        return;

    prevQueuekey = curr;

    if(qkInsertNum == _qkNum)
        return;

    SyncKey* k = &qkInsert[qkInsertNum];
    qkInsertNum++;

    k->stat = stat;
    k->point = point;
}

void SyncKey::removeKey()
{
    int copyNum = qkInsertNum - qkRemoveNum;
    memcpy(qkInsert, &qkInsert[qkRemoveNum], sizeof(SyncKey) * copyNum);
    qkInsertNum -= qkRemoveNum;
    qkRemoveNum = 0;
}

void SyncKey::updateKey() // thread 111
{
    if( qkNum + qkInsertNum > _qkNum )
    {
        qkInsertNum = _qkNum - qkNum;
        // #issue! 프레임이 굉장히 안좋게 나오는 경우 간헐적으로 발생
        xprint("키값이 씹힘...");
    }

    memcpy(&qk[qkNum], qkInsert, sizeof(SyncKey)*qkInsertNum);
    qkNum += qkInsertNum;
    qkRemoveNum = qkInsertNum;

    for(int i=0; i<qkNum; i++)
        keyGame(qk[i].stat, qk[i].point);
    qkNum = 0;

    if(qkRemoveNum > 0)
        removeKey();
}