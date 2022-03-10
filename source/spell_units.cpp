//=============================================================================
// Loader of Spellcross units definition file JEDNOTKY.DEF.
// Loads CZ or EN version from binary data input.
// Decoders binary to list of unit records.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_units.h"
#include "spellcross.h"
#include "map.h"
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;

// default resource content
SpellUnitRec::SpellUnitRec()
{	
	gr_base = NULL;
	gr_aux = NULL;
	projectile = NULL;
	pnm_light_hit = NULL;
	pnm_armored_hit = NULL;
	pnm_air_hit = NULL;
	pnm_light_shot = NULL;
	pnm_armored_shot = NULL;
	pnm_air_shot = NULL;

	sound_move = NULL;
	sound_report = NULL;
	sound_contact = NULL;
	sound_hit = NULL;
	sound_die = NULL;
	sound_attack_light = NULL;
	sound_attack_armor = NULL;
	sound_attack_air = NULL;
}

SpellUnitRec::~SpellUnitRec()
{
	if(sound_move)
		delete sound_move;
	if(sound_report)
		delete sound_report;
	if(sound_contact)
		delete sound_contact;
	if(sound_hit)
		delete sound_hit;
	if(sound_die)
		delete sound_die;
	if(sound_attack_light)
		delete sound_attack_light;
	if(sound_attack_armor)
		delete sound_attack_armor;
	if(sound_attack_air)
		delete sound_attack_air;
}

// decode units def file
SpellUnits::SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu, SpellGraphics *graphics,SpellSounds *sounds)
{
	int count;
	if (dlen % 206 == 0 && dlen % 207 != 0)
	{
		// CZE version
		is_eng = 0;
		count = dlen / 206;
	}
	else if(dlen % 206 != 0 && dlen % 207 == 0)
	{
		// ENG version
		is_eng = 1;
		count = dlen / 207;
	}
	else
	{
		// unknown
		throw runtime_error("Cannot identify game version of JEDNOTKY.DEF file with units definitions!");
	}
	
	// --- for each unit:
	for (int k = 0; k < count; k++)
	{
		uint8_t *rec = data;
		if (is_eng)
			data += 207;
		else
			data += 206;
		
		// new unit record
		SpellUnitRec *unit = new SpellUnitRec();

#define rdu8(rdu8_r) ((int)((unsigned int)*(rdu8_r)))
#define rdu16(rdu16_r) ((int)((unsigned int)*((unsigned short*)(rdu16_r))))
#define rdu32(rdu32_r) ((int)*((unsigned int*)(rdu32_r)))
#define rdsc(rdsc_d,rdsc_r,rdsc_n) memcpy((void*)(rdsc_d),(void*)(rdsc_r),rdsc_n)

		// unit name
		rdsc(unit->name, rec + 0x00, 28);

		// unit info resource
		rdsc(unit->info, rec + 0x1C, 9);

		// unit graphics resource
		rdsc(unit->gra, rec + 0x25, 6);
		for (int k = 5; k; k--)
			if (unit->gra[k] == '_')
				unit->gra[k] = '\0';

		// unit aditional graphics resource (tank turrets or so)
		rdsc(unit->grb, rec + 0x2B, 6);
		for (int k = 5; k; k--)
			if (unit->grb[k] == '_')
				unit->grb[k] = '\0';

		// unit icon
		rdsc(unit->icon, rec + 0x31, 9);

		// unit class flags
		// 0x01 - has turret
		// 0x02 - walk movement
		// 0x04 - flight movement
		// 0x08 - hover movement (can move on water)
		// 0x30 - {0-air, 1-light, 2-armored unit}
		// 0x40 - flesh and bones
		// 0x80 - only Demon
		unit->utype = rdu8(rec + 0x3A);

		// ap count
		unit->ap = rdu8(rec + 0x3B);

		// ap per move in forrest
		unit->apfw = rdu8(rec + 0x3C);

		// ap per move normal
		unit->apw = rdu8(rec + 0x3D);

		// man count (or tanks count)
		unit->cnt = rdu8(rec + 0x3E);

		// attack to light units
		unit->alig = rdu8(rec + 0x3F);

		// attack to armored units
		unit->aarm = rdu8(rec + 0x40);

		// attack to air units
		unit->aair = rdu8(rec + 0x41);

		// attack to objects
		unit->aobj = rdu8(rec + 0x42);

		// unit defence
		unit->def = rdu8(rec + 0x43);

		// some probability???
		unit->res2 = rdu8(rec + 0x44);

		// ???
		unit->res3 = rdu8(rec + 0x45);

		// max fire range
		unit->fire_range = rdu8(rec + 0x46);

		// special shot flags
		// 0x01 - high turret origin (probably???)
		// 0x02 - indirect fire (artilery)
		// 0x04 - steals action points
		// 0x08 - inefficient to armored targets (probably???)
		// 0x10 - can't fire from slopes
		// 0x20 - indirect missile (MLRS only???)
		// 0x40 - fire projectile
		// 0x80 - fire sensitivity (increased fire sensitivity???)
		unit->fire_flags = rdu8(rec + 0x47);		
		// aditional ENG version item
		// 0x100 - unit is healed by fire
		if (is_eng)
		{
			unit->fire_flags |= rdu8(rec + 0x48)<<8;
			rec++;
		}

		// ap per shot
		unit->aps = rdu8(rec + 0x48);


		// light attack hit animation (*.pnm animation)
		rdsc(unit->pnm_light_hit_name, rec + 0x49, 9);

		// light attack unit animation resource (*.fsu resource)
		rdsc(unit->anim_atack_light_name, rec + 0x52, 6);
		// used frames count
		unit->anim_atack_light_frames = rdu8(rec + 0x61);

		// light attack shot animation (*.pnm resource)
		rdsc(unit->pnm_light_shot_name, rec + 0x58, 9);

		// armored attack hit animation (*.pnm animation)
		rdsc(unit->pnm_armored_hit_name, rec + 0x62, 9);

		// armored attack unit animation resource (*.fsu resource)
		rdsc(unit->anim_atack_armor_name, rec + 0x6B, 6);
		// used frames count
		unit->anim_atack_armor_frames = rdu8(rec + 0x7A);

		// armored attack shot animation (*.pnm resource)
		rdsc(unit->pnm_armored_shot_name, rec + 0x71, 9);

		// air attack hit animation (*.pnm animation)
		rdsc(unit->pnm_air_hit_name, rec + 0x7B, 9);

		// air attack unit animation resource (*.fsu content)
		rdsc(unit->anim_atack_air_name, rec + 0x84, 6);
		// used frames count
		unit->anim_atack_air_frames = rdu8(rec + 0x93);

		// air attack shot animation (*.pnm resource)
		rdsc(unit->pnm_air_shot_name, rec + 0x8A, 9);

		// projectile visibility flags (0x01-light, 0x02-armored, 0x04-air attacks)
		unit->prjf = rdu8(rec + 0x94);

		// projectile resource (*.grf files)
		rdsc(unit->projetile_name, rec + 0x95, 13);

		// special action id
		// 1  - enable/disable radar (par3-radar indirect sight range)
		// 2  - show tank turret (UDES) (par3-unit to transform to)
		// 3  - hide tank turret (UDES) (par3-unit to transform to)
		// 4  - fire teleport movement (hell cavalery/demon)
		// 5  - create unit (par3-unit to create)
		// 6  - lower enemy morale (undead) (par1-range, par2-level, par3-range)
		// 7  - aircraft up (par3-unit to transform to)
		// 8  - aircraft land (par3-unit to transform to)
		// 9  - paralyze enemy (harpya) (par1-range, par2-???, par3-???)
		// 11 - freeze enemy units (par1-range, par2-???, par3-???)
		// 12 - dragons fear (par1-range, par2-???, par3-???)
		// 13 - autodestruction (par1-range, par2-intensity, par3-???)
		// 14 - breorns scream (par1-range, par2-???, par3-???)
		// 15 - kamize attack
		// 16 - transform to fortres (par3-unit to transform to)
		// 17 - transform from fortres (par3-unit to transfrom to)
		unit->said = rdu8(rec + 0xA2);

		// special action animation resource (*.fsu content)
		rdsc(unit->saan, rec + 0xA4, 6);
		// frames count
		unit->saanf = rdu8(rec + 0xAA);

		// ap per special action
		unit->saap = rdu8(rec + 0xAB);

		// special action parameter 1
		unit->sap1 = rdu8(rec + 0xAC);

		// special action parameter 2
		unit->sap2 = rdu8(rec + 0xAD);

		// special action parameter 3
		unit->sap3 = rdu8(rec + 0xAE);


		// die action
		unit->daid = rdu8(rec + 0xBB);

		// die action parameter 1
		unit->dap1 = rdu8(rec + 0xB9);

		// die action parameter 2
		unit->dap2 = rdu8(rec + 0xBA);

		// die action parameter 1
		unit->dap3 = rdu8(rec + 0xBD);

		// die action animation resource (*.fsu content)
		rdsc(unit->daan, rec + 0xB2, 6);
		// frames count
		unit->daanf = rdu8(rec + 0xB8);


		// direct sight
		unit->sdir = rdu8(rec + 0xAF);

		// visible men/vehicles count
		unit->vis = rdu8(rec + 0xB0);

		// rounds per dig level
		unit->dig = rdu8(rec + 0xB1);

		// unit movement sound class
		unit->smov = rdu8(rec + 0xBF);

		// unit light attack class
		unit->slig = rdu8(rec + 0xC0);

		// unit armored attack class
		unit->sarm = rdu8(rec + 0xC1);

		// unit air attack class
		unit->sair = rdu8(rec + 0xC2);

		// unit hit sound class
		unit->shit = rdu8(rec + 0xC3);

		// unit special sound class
		unit->sspc = rdu8(rec + 0xC4);

		// unit selection/report sound class
		unit->ssel = rdu8(rec + 0xC5);

		// unit min rank points
		unit->ramin = rdu32(rec + 0xC6);

		// unit max rank points
		unit->ramax = rdu32(rec + 0xCA);

		// --- assign FSU resources:
		if(fsu)
		{
			unit->gr_base = fsu->GetResource(unit->gra);
			unit->gr_aux = fsu->GetResource(unit->grb);
			unit->gr_attack_light = fsu->GetResource(unit->anim_atack_light_name);
			unit->gr_attack_armor = fsu->GetResource(unit->anim_atack_armor_name);
			unit->gr_attack_air = fsu->GetResource(unit->anim_atack_air_name);
		}

		// --- assign aux graphics
		if(graphics)
		{
			// find unit icon
			unit->icon_glyph = graphics->GetResource(unit->icon);

			// find unit projectile
			unit->projectile = graphics->GetProjectile(unit->projetile_name);

			// find shot/hit PNMs
			unit->pnm_air_hit = graphics->GetPNM(unit->pnm_air_hit_name);
			unit->pnm_air_shot = graphics->GetPNM(unit->pnm_air_shot_name);
			unit->pnm_light_hit = graphics->GetPNM(unit->pnm_light_hit_name);
			unit->pnm_light_shot = graphics->GetPNM(unit->pnm_light_shot_name);
			unit->pnm_armored_hit = graphics->GetPNM(unit->pnm_armored_hit_name);
			unit->pnm_armored_shot = graphics->GetPNM(unit->pnm_armored_shot_name);			
		}

		// --- assign sounds
		if(sounds)
		{
			unit->sound_move = sounds->GetMoveClass(unit->smov);
			unit->sound_report = sounds->GetReportClass(unit->ssel);
			unit->sound_contact = sounds->GetContactClass(unit->ssel);
			unit->sound_hit = sounds->GetHitClass(unit->shit);
			unit->sound_die = sounds->GetDieClass(unit->shit);
			unit->sound_attack_light = sounds->GetAttackClass(unit->slig);
			unit->sound_attack_armor = sounds->GetAttackClass(unit->sarm);
			unit->sound_attack_air = sounds->GetAttackClass(unit->sair);
		}

		// store unit to list
		units.push_back(unit);
	}

}

// unit type
int SpellUnitRec::isAir()
{
	return((utype & UTYPE_TYPE_MASK) == UTYPE_TYPE_AIR);
}
int SpellUnitRec::isLight()
{
	return((utype & UTYPE_TYPE_MASK) == UTYPE_TYPE_LIGHT);
}
int SpellUnitRec::isHeavy()
{
	return((utype & UTYPE_TYPE_MASK) == UTYPE_TYPE_ARMORED);
}
int SpellUnitRec::hasTurret()
{
	return(!!(utype & UTYPE_TURRET));
}
int SpellUnitRec::isWalk()
{
	return(!!(utype & UTYPE_WALK));
}
int SpellUnitRec::isFly()
{
	return(!!(utype & UTYPE_FLY));
}
int SpellUnitRec::isHover()
{
	return(!!(utype & UTYPE_HOVER));
}
int SpellUnitRec::isTank()
{
	return(!isWalk() && !isFly());
}



// cleanup
SpellUnits::~SpellUnits()
{
	for (int k = 0; k < units.size(); k++)
		delete units[k];
	units.clear();
}

// get units count
int SpellUnits::Count()
{
	return(units.size());
}

// get unit record by order id
SpellUnitRec *SpellUnits::GetUnit(int uid)
{
	if (uid >= units.size())
		return(NULL);
	return(units[uid]);
}

// get full units list
vector<SpellUnitRec*> &SpellUnits::GetUnits()
{
	return(units);
}


// render unit (complete, i.e. group of man or tank with turret) and stick for air units
tuple<int,int> SpellUnitRec::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,
	uint8_t* filter, uint8_t* shadow_filter, ::Sprite *sprt, int azim, int azim_turret, int frame, FSU_resource *fsu_anim)
{
	// tile slope
	char slope = sprt->name[2];
	
	// visible man count in unit
	int man = vis;
	if (man < 1)
		man = 1; // overrider for tanks
	if (man > 5)
		man = 5; // limit units to 4 (###todo: may be extended later)

	// buffer of man positions relatie to origin
	double uofs_x[5];
	double uofs_y[5];
	int uofs[5];

	// precalculate sin/cos to save time
	static int isa = 0;
	static double cosa[360];
	static double sina[360];
	for (isa = 0; isa < 360; isa++)
	{
		cosa[isa] = cos(isa / 180.0 * 3.1415);
		sina[isa] = sin(isa / 180.0 * 3.1415);
	}

	int man_id = 0;
	if (man == 1 || man == 5)
	{
		// one unit, place to center
		uofs_x[man_id] = 0;
		uofs_y[man_id] = 0;
		uofs[man_id] = 0;
		man_id++;
	}
	if (man >= 2 && man <= 5)
	{
		// 2-5 units - make ring around center
		int ang_step;
		int ang_ofs;		
		if (man == 2)
		{
			ang_ofs = 315;
			ang_step = 180;
		}
		else if (man == 3)
		{
			ang_ofs = 30;
			ang_step = 120;
		}
		else
		{
			ang_ofs = 1;
			ang_step = 90;
		}
		for (int k = 0; k < min(man,4); k++)
		{
			// calculate man placement
			uofs_x[man_id] = sina[ang_ofs];
			uofs_y[man_id] = cosa[ang_ofs];
			uofs[man_id] = 0;
			man_id++;
			// next angle
			ang_ofs += ang_step;
			if (ang_ofs >= 360)
				ang_ofs -= 360;
		}
	}
	/*else
	{
		int ang_step = 360/man;
		int ang_ofs = 0;
		for (int k = 0; k < man; k++)
		{
			// calculate man placement
			uofs_x[man_id] = sina[ang_ofs];
			uofs_y[man_id] = cosa[ang_ofs];
			uofs[man_id] = 0;
			man_id++;
			// next angle
			ang_ofs += ang_step;
			if (ang_ofs >= 360)
				ang_ofs -= 360;
		}
	}*/

	int x_pos;
	int y_pos;
	FSU_sprite* spr = NULL;

	int y_status_bar = 2^30;
	int x_status_bar = 0;

	// --- repeat for each man:
	for (int uid = 0; uid < man; uid++)
	{
		// search maximum y-offset because we render from back to front
		double min_y = 10000.0;
		for (int k = 0; k < man; k++)
		{
			if (!uofs[k] && uofs_y[k] < min_y)
			{
				min_y = uofs_y[k];
				man_id = k;
			}
		}
		uofs[man_id] = 1;
				
		for (int part = 0; part < 2; part++)
		{
			FSU_resource* res;
			int res_azim;
			if (part == 0)
			{
				res = gr_base;
				res_azim = azim;
			}
			else
			{
				res = gr_aux;
				res_azim = azim_turret;
			}
			if (!res)
				break;
			
			int y_org = 0;
			if (res->stat.slopes == 1)
				slope = 'A';
												
			// get sprite record
			if(fsu_anim && frame >= 0 && frame < fsu_anim->anim.frames)
			{
				// custom animation resource
				spr = fsu_anim->anim.lists[res_azim][frame];
			}
			else if (frame >= 0 && res->anim.frames)
			{
				// animate
				spr = res->anim.lists[res_azim][frame];
			}
			else
				spr = res->stat.lists[slope-'A'][res_azim];

			// unit position
			if (isWalk())
			{
				// for walking units we have to perform placement transform based on terrain for each man
				x_pos = buf_x_pos + (int)(uofs_x[man_id] * MAN_RING_DIAMETER);
				y_pos = buf_y_pos - (int)(sprt->GetTileProjY(uofs_x[man_id] * MAN_RING_DIAMETER, -uofs_y[man_id] * MAN_RING_DIAMETER));
			}
			else
			{
				// for all other units do nothing, place it as prescribed by unit sprite itself
				x_pos = buf_x_pos;
				y_pos = buf_y_pos + sprt->y_ofs;
			}
			
			if (isAir())
			{
				// air unit - shift unit up
				y_pos -= AIR_UNIT_FLY_HEIGHT;
			}

			// store unit highest pixel offset
			if(uid == 0)
				x_status_bar = 40;
			y_status_bar = min(y_pos + (spr->y_ofs - 128) - buf_y_pos,y_status_bar);

			// render man of unit			
			spr->Render(buffer, buf_end, x_pos, y_pos, buf_x_size, shadow_filter, filter);
		}
	}

	// --- render "stick" to ground:
	if (spr && isAir() && man == 1 && !gr_aux)
	{
		// only for air, single man unit, not tank

		// top of the stick
		x_pos = buf_x_pos + 40;
		y_pos = buf_y_pos - AIR_UNIT_FLY_HEIGHT + spr->y_ofs - 128 + spr->y_size + sprt->y_ofs;
		if (y_pos < 0)
			y_pos = 0;

		// bottom of the stick
		int y_down = buf_y_pos + sprt->y_size / 2 + sprt->y_ofs;
		if (y_down > (buf_end - buffer) / buf_x_size)
			y_down = (buf_end - buffer) / buf_x_size;

		// render stick
		for (int y = y_down; y >= y_pos; y-=3)
			buffer[x_pos + y * buf_x_size] = 250;

	}

	// return unit top-center (upper most pixel) offset from buffer origin
	return tuple(x_status_bar,y_status_bar);
}

// get art images count
vector<string> SpellUnitRec::GetArtList(FSarchive* info_fs)
{
	vector<string> list;

	// put first item	
	list.push_back(string(this->info));

	// load list file
	string list_name = string(this->info) + ".LST";
	uint8_t *data;
	int size;
	int fail = info_fs->GetFile(list_name.c_str(), &data, &size);
	if(!fail)
	{			
		// parse next items
		list.clear();
		list.push_back(string(this->info));
		string str;
		str.resize(size);
		memcpy(str.data(),data,size);
		auto ss = std::stringstream{str};
		for(std::string line; std::getline(ss,line,'\n');)
		{
			if(line.back() == '\r')
				line = line.substr(0,line.size()-1);
			list.push_back(line);
		}
	}
		
	return(list);
}

// get count of available arts for unit
int SpellUnitRec::GetArtCount(FSarchive* info_fs)
{
	auto list = GetArtList(info_fs);
	return(list.size());
}




MapUnit::MapUnit()
{
	// unit idnetifier index within map
	id = 0;
	// unit type ID
	type_id = 0;
	unit =  NULL;
	// position
	coor = MapXY();
	// experience
	experience = 0;
	// man count
	man = 1;
	// unit type/behaviour
	type = MapUnitType::NormalUnit;
	// custom name
	name[0] = '\0';
	// commander id or zero	
	commander_id = 0;
	is_commander = 0;
	// dig in
	dig_level = 0;
	// action points
	action_points = 1;
	// unit active (set except insertion time)
	is_active = 0;
	// enemy?
	is_enemy = 0;
	// unit being placed?
	in_placement = false;
	// moved
	was_moved = true;

	// sound refs
	sound_report = NULL;
	sound_contact = NULL;
	sound_move = NULL;
	sound_hit = NULL;
	sound_die = NULL;
	sound_attack_light = NULL;
	sound_attack_armor = NULL;
	sound_attack_air = NULL;

	// FSU sprite index
	in_animation = NULL;
	azimuth = 0;
	azimuth_turret = 0;
	frame = 0;

	move_state = MapUnit::MOVE_STATE_IDLE;
	attack_state = MapUnit::ATTACK_STATE_IDLE;
}
MapUnit::~MapUnit()
{
	if(sound_report)
		delete sound_report;
	if(sound_contact)
		delete sound_contact;
	if(sound_move)
		delete sound_move;
	if(sound_hit)
		delete sound_hit;
	if(sound_die)
		delete sound_die;
	if(sound_attack_light)
		delete sound_attack_light;
	if(sound_attack_armor)
		delete sound_attack_armor;
	if(sound_attack_air)
		delete sound_attack_air;
}
MapUnit::MapUnit(MapUnit &obj)
{
	*this = obj;
	sound_report = NULL;
	sound_contact = NULL;
	sound_move = NULL;
	sound_hit = NULL;
	sound_die = NULL;
	sound_attack_light = NULL;
	sound_attack_armor = NULL;
	sound_attack_air = NULL;
}


// action points for this level of experience
int MapUnit::GetMaxAP()
{
	// basic AP
	int ap = unit->ap;

	// experience bonuses
	if(experience >= 6)
		ap += unit->apw;
	if(experience >= 9)
		ap += unit->apw;

	return(ap);
}

// reset unit action points
int MapUnit::ResetAP()
{
	action_points = GetMaxAP();
	return(action_points);
}

// get AP per single shot
int MapUnit::GetAPperFire()
{
	// get max fires count
	int ap = GetMaxAP();
	int basic_fires = GetMaxFireCount();
	if(!basic_fires)
		return(0);

	// get ap per fire for this level of experience
	return(floor(ap/basic_fires));
}

// get fires count
int MapUnit::GetFireCount(int ext_ap)
{
	int ap_per_fire = GetAPperFire();
	if(!ap_per_fire)
		return(0);
	
	// actual fires count
	if(ext_ap >= 0)		
		return(floor(ext_ap/ap_per_fire));
	return(floor(action_points/ap_per_fire));
}

// get max fires count
int MapUnit::GetMaxFireCount()
{
	// get max fires count
	int ap = GetMaxAP();
	if(!unit->aps)
		return(0);
	int basic_fires = floor(unit->ap/unit->aps);
	if(experience >= 5)
		basic_fires++;
	if(experience >= 10)
		basic_fires++;
	return(basic_fires);
}

// reduce AP by single fire
int MapUnit::UpdateFireAP()
{
	int ap_per_fire = GetAPperFire();
	if(!ap_per_fire)
		return(1);
	if(action_points >= ap_per_fire)
		action_points -= ap_per_fire;	
	return(0);
}

// get AP per walk
int MapUnit::GetWalkAP()
{
	int move_range = unit->apw;
	if(experience >= 6)
		move_range++;
	if(experience >= 10)
		move_range++;
	return(GetMaxAP()/(move_range-1));
}

int MapUnit::Render(Terrain* data,uint8_t* buffer,uint8_t* buf_end,int buf_x_pos,int buf_y_pos,int buf_x_size,uint8_t *filter,Sprite* sprt,int show_hud)
{
	// filter for shadow rendering
	auto shadow_filter = data->filter.darker;
	
	int loc_frame = frame;
	if(!in_animation)
		loc_frame = -1; // static unit resource

	// render unit
	auto [x_status_bar,y_status_bar] = unit->Render(buffer, buf_end, buf_x_pos, buf_y_pos, buf_x_size,filter,shadow_filter, sprt, azimuth, azimuth_turret, loc_frame, in_animation);
	
	if(!filter)
		filter = data->filter.nullpal;

	// -- make status bar
	if(!show_hud)
		return(y_status_bar);
	const int bar_w = 28;
	const int bar_h = 15;
	// check valid rendering range
	auto *psb     = &buffer[(buf_x_pos + x_status_bar - bar_w/2) + (buf_y_pos + y_status_bar - bar_h/2)*buf_x_size];
	auto *psb_end = &buffer[(buf_x_pos + x_status_bar - bar_w/2 + bar_w-1) + (buf_y_pos + y_status_bar - bar_h/2 + bar_h-1)*buf_x_size];
	if(psb < buffer || psb_end >= buf_end)
		return(0);

	// render shadow background
	for(int y = 0; y < bar_h; y++)
	{
		uint8_t* buf = &psb[y*buf_x_size];
		for(int x = 0; x < bar_w; x++)
			buf[x] = filter[shadow_filter[buf[x]]];
	}

	// hitpoints bar size
	int hp_w;
	if(is_enemy)
		hp_w = 26;
	else
		hp_w = (commander_id)?17:26;
	int hp_h = 3;
	
	// hit points (in pixels)
	int hp = min((hp_w*man)/unit->cnt,hp_w);

	// action points (in pixels)
	int ap = min((hp_w*action_points)/GetMaxAP(),hp_w);
	
	// render action point (aliance only)
	if(!is_enemy)
	{
		int ap_color = (is_active)?230:228;
		for(int y = 1; y < 4; y++)
		{
			uint8_t* buf = &psb[y*buf_x_size];
			for(int x = 1; x <= ap; x++)
				buf[x] = filter[ap_color];
		}
	}
	
	// render hitpoints
	int hp_color = (is_active)?234:233;
	if(is_enemy)
		hp_color = 253;
	for(int y = 5; y < 8; y++)
	{
		uint8_t* buf = &psb[y*buf_x_size];
		for(int x = 1; x <= hp; x++)
			buf[x] = filter[hp_color];
	}

	// render special unit type mark
	int type_color = 0;
	if(type == MapUnitType::MissionUnit)
		type_color = 230;
	else if(type == MapUnitType::SpecUnit1)
		type_color = 253;
	for(int y = 9; y < bar_h; y+=2)
	{
		uint8_t* buf = &psb[y*buf_x_size];
		for(int x = bar_w-3; x < bar_w-1; x++)
			buf[x] = filter[type_color];
	}

	// render fire count
	if(!is_enemy)
	{
		int fires = GetFireCount();
		for(int k = 0; k < fires; k++)
		{		
			uint8_t* buf = &psb[9*buf_x_size + 1 + k*4];
			buf[0] = filter[253]; buf[1] = filter[253]; buf[2] = filter[202];
			buf += buf_x_size;
			buf[0] = filter[253]; buf[1] = filter[202]; buf[2] = filter[202];
		}
	}

	// render dig level
	for(int k = 0; k < dig_level; k++)
	{
		uint8_t* buf = &psb[12*buf_x_size + 1 + k*4];
		buf[0] = filter[252]; buf[1] = filter[252]; buf[2] = filter[214];
		buf += buf_x_size;
		buf[0] = filter[252]; buf[1] = filter[214]; buf[2] = filter[214];
	}

	if(commander_id && !is_enemy)
	{
		// render commander id
		data->font7->RenderSymbol(psb, psb_end, buf_x_size, 19, 1, '0'+commander_id,filter[232]);

		// render commander mark
		if(is_commander)
			data->font7->RenderSymbol(psb,psb_end,buf_x_size,24,1,31,filter[232]);

	}
	
	// return top pixel of unit
	return(y_status_bar);
}


// play report sound
int MapUnit::PlayReport()
{
	if(!sound_report)
		sound_report = new SpellSound(*unit->sound_report);
	sound_report->Play();
	return(0);
}

// play contact sound
int MapUnit::PlayContact()
{
	if(!sound_contact)
		sound_contact = new SpellSound(*unit->sound_contact);
	sound_contact->Play();
	return(0);
}

// play being hit sound
int MapUnit::PlayBeingHit()
{
	if(!sound_hit)
		sound_hit = new SpellSound(*unit->sound_hit);
	sound_hit->Play();
	return(0);
}

// play die sound
int MapUnit::PlayDie()
{
	if(!sound_die)
		sound_die = new SpellSound(*unit->sound_die);
	sound_die->Play();
	return(0);
}

// play move sound
int MapUnit::PlayMove()
{
	if(!sound_move)
		sound_move = new SpellSound(*unit->sound_move);
	sound_move->Play();
	return(0);
}
// stop move sound
int MapUnit::PlayStop()
{
	sound_move->StopMove();
	return(0);
}

// play fire sound to given unit type
int MapUnit::PlayFire(SpellUnitRec* target)
{
	// make local copy of sound objects (because of polyphony for multiple units)
	if(!sound_attack_light)
		sound_attack_light = new SpellAttackSound(*unit->sound_attack_light);
	if(!sound_attack_armor)
		sound_attack_armor = new SpellAttackSound(*unit->sound_attack_armor);
	if(!sound_attack_air)
		sound_attack_air = new SpellAttackSound(*unit->sound_attack_air);

	// play shot sound
	if(target->isLight())
		sound_attack_light->shot->Play();
	else if(target->isHeavy())
		sound_attack_armor->shot->Play();
	else if(target->isAir())
		sound_attack_air->shot->Play();

	return(0);
}

// play target hit sound to given unit type
int MapUnit::PlayHit(SpellUnitRec* target)
{
	// make local copy of sound objects (because of polyphony for multiple units)
	if(!sound_attack_light)
		sound_attack_light = new SpellAttackSound(*unit->sound_attack_light);
	if(!sound_attack_armor)
		sound_attack_armor = new SpellAttackSound(*unit->sound_attack_armor);
	if(!sound_attack_air)
		sound_attack_air = new SpellAttackSound(*unit->sound_attack_air);

	// play shot sound
	if(target->isLight())
		sound_attack_light->hit_flash->Play();
	else if(target->isHeavy())
		sound_attack_armor->hit_flash->Play();
	else if(target->isAir())
		sound_attack_air->hit_flash->Play();

	return(0);
}