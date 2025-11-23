#pragma once

#include "map_types.h"
#include "spellcross.h"
#include "other.h"
#include "spell_units.h"

// forward refs
class SpellMap;

class SpellMapEventUnitRec
{
public:
    MapUnit *unit;
    int is_placed;
    bool was_listed;
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

    // back ref to parent map
    SpellMap* map;

public:

    enum EvtTypes
    {
        EVT_VOID = 0,
        EVT_MISSION_START,
        EVT_SEE_PLACE,
        EVT_SEE_UNIT,
        EVT_DESTROY_UNIT,
        EVT_DESTROY_OBJ,
        EVT_DESTROY_ALL,
        EVT_TRANSPORT_UNIT,
        EVT_SAVE_UNIT
    };
    string EvtNames[9] = 
    {
        "Void",
        "MissionStart",
        "SeePlace",
        "SeeUnit",
        "DestroyUnit",
        "DestroyObject",
        "DestroyAllUnits",
        "TransportUnit",
        "SaveUnit"
    };       

    // event is being placed in edit mode?
    bool in_placement;    
    // temp flag used for event listing eg. during rendering in edit mode
    bool was_listed;

    // event trigger parameters
    int evt_type;
    MapXY position;
    int probability;
    int trig_unit_id;
    MapUnit *trig_unit;
    string type_name;
    int is_objective;
    wstring label;

    // event is below probability (will not be excuted)
    int hide;
    // event executed?
    int is_done;

    // next event at the same position (used for chaining in events map)
    SpellMapEventRec *next;

    // event spawn units list
    vector<SpellMapEventUnitRec> units;

    // event invoked text message(s) (there should be just one, but you never know...)
    vector<SpellMapEventMessageRec> texts;

    // video resource name to play
    std::string video;
    
    int SetType(int type_id);
    int AddUnit(MapUnit *unit);
    MapUnit *ExtractUnit(MapUnit* unit);
    void ClearUnits();
    void ClearTexts();
    SpellMapEventRec(SpellMap *parent_map);
    SpellMapEventRec(SpellMapEventRec* rec);
    ~SpellMapEventRec();
    std::tuple<std::string,std::string> FormatDEFrecord(int *initial_id);
    MapXY GetPosition();

    int CheckUnitInPos(bool clear=false);
        
    int isMissionStart();
    int isSeePlace();
    int isSeeUnit();
    int isDone();
    int isDestroyObject();
    int isTransportSave();
    int hasTargetUnit();
    int hasPosition();
    int isObjectiveType();
    int isEventType();


    vector<string> GetEventTypes();
};

typedef vector<SpellMapEventRec*> SpellMapEventsList;

class SpellMapEvents
{
private:
    
    std::string last_error;

    // back ref to parent map
    SpellMap *map;
    
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
    SpellMapEventRec *FindEvent(int type,int probab,int trig_unit);
    SpellMapEventRec **EventMap(MapXY pos);

public:

    SpellMapEvents(SpellMap *parent);
    ~SpellMapEvents();
    std::string GetLastError();
    int AddSpecialEvent(SpellData* data, SpellDEF* def, SpellDefCmd* cmd);
    int AddMissionObjective(SpellData* data,SpellDEF* def,SpellDefCmd* cmd);
    SpellMapEventRec* AddEvent(SpellMapEventRec *event);
    SpellMapEventRec* ExtractEvent(SpellMapEventRec* event);
    int EraseEvent(SpellMapEventRec* event);
    void ClearEvents();
    int RelinkUnits(vector<MapUnit*>* map_units=NULL);
    void ResetEvents();
    SpellMapEventRec* GetEvent(int index);
    SpellMapEventRec* GetEvent(MapXY pos); 
    SpellMapEventRec* GetAnotherEvent(SpellMapEventRec* evt);
    int GetEventsCount(MapXY pos);
    int CheckEvent(MapXY pos);
    int CheckEvent(int pos);
    SpellMapEventsList GetEvents(MapXY pos,bool clear=false);
    SpellMapEventsList GetEvents(int pos,bool clear=false);
    SpellMapEventsList GetDestroyObjectEvents(MapXY pos,bool clear=false);
    vector<SpellMapEventRec*> &GetEvents();
    SpellMapEventsList GetMissionStartEvent(bool clear=false);
    int AddMissionStartUnit(MapUnit *unit, int probab=100);
    SpellMapEventRec* AddSeeUnitEvent(MapUnit* unit,int probab=100);
    int ObjectivesDone();
    int GetEventID(SpellMapEventRec * target);

    void ListerClear();
    SpellMapEventsList ListerGet();
    std::vector<SpellMapEventUnitRec*> ListerGetMissionStart(MapXY pos={-1,-1});
};
