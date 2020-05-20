#pragma once

class CGlobalVarsBase
{
public:
    float     realtime;
    int       framecount;
    float     absoluteframetime;
    float     absoluteframestarttimestddev;
    float     cur_time;
    float     frame_time;
    int       max_clients;
    int       tick_cound;
    float     interval_per_tick;
    float     interpolation_amount;
    int       simTicksThisFrame;
    int       networkProtocol;
    void*     pSaveData;
    bool      bClient;
    bool      bRemoteClient;

private:
    // 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
    int       nTimestampNetworkingBase;
    // 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
    //  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
    int       nTimestampRandomizeWindow;
};
extern CGlobalVarsBase* csgo_globals;