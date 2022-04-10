#pragma once

#include "map_types.h"
#include "spellcross.h"
#include "other.h"
#include "spell_units.h"

class SpellMapEventRec
{
private:

public:

    enum
    {
        EVT_VOID = 0,
        EVT_MISSION_START,
        EVT_SEE_PLACE
    };

    // event trigger parameters
    int evt_type;
    MapXY position;
    int probability;

    // event is below probability (will not be excuted)
    int hide;

    // event executed?
    int is_done;

    // next event at the same position (used for chaining in events map)
    SpellMapEventRec *next;

    // event new units list
    vector<MapUnit*> units;

    SpellMapEventRec();
    ~SpellMapEventRec();
    int isMissionStart();
    int isSeePlace();
};

typedef vector<SpellMapEventRec*> SpellMapEventsList;

class SpellMapEvents
{
private:
    
    // map size
    int x_size;
    int y_size;
    // unit index (only for loading)
    int next_index;
    // list of events
    vector<SpellMapEventRec*> events;
    // events map
    vector<SpellMapEventRec*> events_map;

    MapXY ConvXY(int mxy);
    int ConvXY(MapXY pos);
    int GetNextID();
    SpellMapEventRec *FindEvent(int type, int probab, MapXY pos={-1,-1});
    SpellMapEventRec **EventMap(MapXY pos);

public:

    SpellMapEvents(int x_size, int y_size);
    ~SpellMapEvents();
    int AddEvent(SpellData* data, SpellDEF* def, SpellDefCmd* cmd);
    void ResetEvents();
    SpellMapEventRec* GetEvent(MapXY pos);
    int CheckEvent(MapXY pos);
    int CheckEvent(int pos);
    SpellMapEventsList GetEvents(MapXY pos,bool clear=false);
    SpellMapEventsList GetEvents(int pos,bool clear=false);
    SpellMapEventsList GetMissionStartEvent(bool clear=false);

};
