#pragma once

#include "map_types.h"
#include "spellcross.h"
#include "other.h"
#include "spell_units.h"

class SpellMapEventUnitRec
{
public:
    MapUnit *unit;
    int is_placed;
    SpellMapEventUnitRec(MapUnit *unit) {this->unit = unit; is_placed = false;};
};
class SpellMapEventMessageRec
{
public:
    SpellTextRec* text;
    int is_done;
    SpellMapEventMessageRec(SpellTextRec *text) {this->text = text; is_done = false;};
};


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
    string type_name;

    // event is below probability (will not be excuted)
    int hide;
    // event executed?
    int is_done;

    // next event at the same position (used for chaining in events map)
    SpellMapEventRec *next;

    // event new units list
    vector<SpellMapEventUnitRec> units;

    // event invoked text message(s) (there should be just one, but you never know...)
    vector<SpellMapEventMessageRec> texts;

    SpellMapEventRec();
    ~SpellMapEventRec();
    int isMissionStart();
    int isSeePlace();
    int isDone();
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
    vector<SpellMapEventRec*> &GetEvents();
    SpellMapEventsList GetMissionStartEvent(bool clear=false);

};