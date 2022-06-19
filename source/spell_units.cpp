//=============================================================================
// Loader of Spellcross units definition file JEDNOTKY.DEF.
// Loads CZ or EN version from binary data input.
// Decoders binary to list of unit records.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "spell_units.h"
#include "spellcross.h"
#include "map.h"
#include <sstream>
#include <vector>
#include <stdexcept>
#include <random>

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
	action_button_glyph = NULL;

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
	if(sound_action)
		delete sound_action;
}

// copy resource name string ignoring trailing '_'
int GetNameStr(char *name, uint8_t* data, int count)
{
	int trail = false;
	for(int k = 0; k < count; k++)
	{
		if(data[k] == '_')
			trail = true;
		if(!trail)
			*name++ = data[k];
		else
			*name++ = '\0';
	}
	return(0);
}

// decode units def file
SpellUnits::SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu, SpellGraphics *graphics,SpellSounds *sounds,UnitBonuses* bonuses)
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
		GetNameStr(unit->gra, rec + 0x25,6);

		// unit aditional graphics resource (tank turrets or so)
		GetNameStr(unit->grb,rec + 0x2B, 6);

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
		unit->attack_light = rdu8(rec + 0x3F);

		// attack to armored units
		unit->attack_armored = rdu8(rec + 0x40);

		// attack to air units
		unit->attack_air = rdu8(rec + 0x41);

		// attack to objects
		unit->attack_objects = rdu8(rec + 0x42);

		// unit defence
		unit->defence = rdu8(rec + 0x43);

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
		GetNameStr(unit->anim_atack_light_name,rec + 0x52,6);
		
		// used frames count
		unit->anim_atack_light_frames = rdu8(rec + 0x61);

		// light attack shot animation (*.pnm resource)
		rdsc(unit->pnm_light_shot_name, rec + 0x58, 9);

		// armored attack hit animation (*.pnm animation)
		rdsc(unit->pnm_armored_hit_name, rec + 0x62, 9);

		// armored attack unit animation resource (*.fsu resource)
		GetNameStr(unit->anim_atack_armor_name,rec + 0x6B,6);
		// used frames count
		unit->anim_atack_armor_frames = rdu8(rec + 0x7A);

		// armored attack shot animation (*.pnm resource)
		rdsc(unit->pnm_armored_shot_name, rec + 0x71, 9);

		// air attack hit animation (*.pnm animation)
		rdsc(unit->pnm_air_hit_name, rec + 0x7B, 9);

		// air attack unit animation resource (*.fsu content)
		GetNameStr(unit->anim_atack_air_name,rec + 0x84,6);
		// used frames count
		unit->anim_atack_air_frames = rdu8(rec + 0x93);

		// air attack shot animation (*.pnm resource)
		rdsc(unit->pnm_air_shot_name, rec + 0x8A, 9);

		// projectile visibility flags (0x01-light, 0x02-armored, 0x04-air attacks)
		unit->projectile_visible = rdu8(rec + 0x94);

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
		unit->action_id = rdu8(rec + 0xA2);

		// special action animation resource (*.fsu content)
		GetNameStr(unit->action_fsu_name,rec + 0xA4,6);
		// frames count
		unit->action_fsu_frames = rdu8(rec + 0xAA);

		// ap per special action
		unit->action_ap = rdu8(rec + 0xAB);

		// special action parameters
		unit->action_params[0] = rdu8(rec + 0xAC);
		unit->action_params[1] = rdu8(rec + 0xAD);
		unit->action_params[2] = rdu8(rec + 0xAE);

		// die action
		unit->die_action_id = rdu8(rec + 0xBB);

		// die action parameters
		unit->die_action_params[0] = rdu8(rec + 0xB9);
		unit->die_action_params[1] = rdu8(rec + 0xBA);
		unit->die_action_params[2] = rdu8(rec + 0xBD);

		// die action animation resource (*.fsu content)
		GetNameStr(unit->die_anim_name,rec + 0xB2,6);
		// frames count
		unit->die_anim_frames = rdu8(rec + 0xB8);


		// direct sight
		unit->sdir = rdu8(rec + 0xAF);

		// visible men/vehicles count
		unit->vis = rdu8(rec + 0xB0);

		// rounds per dig level
		unit->dig_turns = rdu8(rec + 0xB1);

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
		unit->snd_action_id = rdu8(rec + 0xC4);

		// unit selection/report sound class
		unit->ssel = rdu8(rec + 0xC5);

		// unit min rank points
		unit->exp_min = rdu32(rec + 0xC6);
		// unit max rank points x200
		unit->exp_max = rdu32(rec + 0xCA);
		// precalculate experience points limits
		for(int e = 0; e < 12; e++)
			unit->exp_limits[e] = unit->CalcExperiencePts(e);

		// --- assign FSU resources:
		if(fsu)
		{
			unit->gr_base = fsu->GetResource(unit->gra);
			unit->gr_aux = fsu->GetResource(unit->grb);
			unit->gr_attack_light = fsu->GetResource(unit->anim_atack_light_name);
			unit->gr_attack_armor = fsu->GetResource(unit->anim_atack_armor_name);
			unit->gr_attack_air = fsu->GetResource(unit->anim_atack_air_name);
			unit->gr_action = fsu->GetResource(unit->action_fsu_name);
			unit->gr_die = fsu->GetResource(unit->die_anim_name);
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

			// assign special action button glyph
			if(unit->isActionTurretUp())
				unit->action_button_glyph = graphics->wm_glyph_up;
			else if(unit->isActionTurretDown())
				unit->action_button_glyph = graphics->wm_glyph_down;
			else if(unit->isActionToggleRadar())
			{
				unit->action_button_glyph = graphics->wm_glyph_radar_up;
				unit->action_button_glyph_b = graphics->wm_glyph_radar_down;
			}
			else if(unit->isActionTakeOff())
				unit->action_button_glyph = graphics->wm_glyph_up;
			else if(unit->isActionLand())
				unit->action_button_glyph = graphics->wm_glyph_down;
			else if(unit->isActionToFortres())
				unit->action_button_glyph = graphics->wm_glyph_down;
			else if(unit->isActionFromFortres())
				unit->action_button_glyph = graphics->wm_glyph_up;
			else if(unit->isActionCreateUnit())
				unit->action_button_glyph = graphics->wm_glyph_place_unit;


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
			unit->sound_action = sounds->GetSpecialClass(unit->snd_action_id);
		}

		// store BONUSES.DEF link
		unit->bonuses = bonuses;

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
int SpellUnitRec::isArmored()
{
	return((utype & UTYPE_TYPE_MASK) == UTYPE_TYPE_ARMORED);
}
int SpellUnitRec::isLand()
{
	return(isLight() || isArmored());
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
int SpellUnitRec::isMLRS()
{
	return(!!(fire_flags & FIRE_MLRS));
}
int SpellUnitRec::isFlashAndBones()
{
	return(!!(utype & UTYPE_FLAHS));
}
int SpellUnitRec::isMetal()
{
	// ###todo: maybe not correct?
	return(!isFlashAndBones());
}
int SpellUnitRec::isInefficientToArmor()
{
	return(!!(fire_flags & FIRE_INEFFICIENT_TO_ARMORRED));
}
int SpellUnitRec::isFireSensitive()
{
	return(!!(fire_flags & FIRE_FIRE_SENSITIVE));
}
int SpellUnitRec::isFireHealed()
{
	return(!!(fire_flags & FIRE_FIRE_HEALED));
}
int SpellUnitRec::hasFireAttack()
{
	return(!!(fire_flags & FIRE_FIRE_PROJECTILE));
}
int SpellUnitRec::isSingleMan()
{
	return(cnt == 1);
}
// calculate base experience points for given exp. level 1-12 (use for precalculation only)
int SpellUnitRec::CalcExperiencePts(int level)
{
	// very crude approximation of strange Spellcross experience boundaries
	// note: it's not accurate, but decently close...
	double a = (double)exp_min;
	double b = log(200.0*exp_max/exp_min)/log(12);
	level = min(max(level,0),11);
	int points = (int)(a*pow((double)level,b));
	return(points);
}
// get base experience points for given exp. level 1-12
int SpellUnitRec::GetExperiencePts(int level)
{
	return(exp_limits[min(max(level-1,0),11)]);
}
// get base experience points for next of given exp. level 1-12
int SpellUnitRec::GetNextExperiencePts(int level)
{
	return(exp_limits[min(max(level,0),11)]);
}

// uses projectile when shooting to target unit (or NULL to object)?
int SpellUnitRec::hasProjectile(SpellUnitRec* target)
{
	int has = false;
	if(target)
	{
		has = target->isLight() && (projectile_visible & PROJECTILE_LIGHT) || 
		target->isArmored() && (projectile_visible & PROJECTILE_ARMOR) ||
		target->isAir() && (projectile_visible & PROJECTILE_AIR);
	}
	else
	{
		has = projectile_visible & (PROJECTILE_ARMOR | PROJECTILE_LIGHT | PROJECTILE_AIR);
	}
	return(has);
}
// uses teleport move (demon, hell cavallery)?
int SpellUnitRec::usingTeleportMove()
{
	return(action_id == SPEC_ACT_FIRE_TELEPORT);
}
// has unit special action? (excluding teleport move), returns non-zero action ID if exist
int SpellUnitRec::hasSpecAction()
{
	if(action_id == SPEC_ACT_FIRE_TELEPORT)
		return(0);
	return(action_id);
}
int SpellUnitRec::isActionTurretUp()
{
	return(action_id == SPEC_ACT_SHOW_TURRET);
}
int SpellUnitRec::isActionTurretDown()
{
	return(action_id == SPEC_ACT_HIDE_TURRET);
}
int SpellUnitRec::isActionToggleRadar()
{
	return(action_id == SPEC_ACT_TOGGLE_RADAR);
}
int SpellUnitRec::isActionLand()
{
	return(action_id == SPEC_ACT_AIRCRAFT_DOWN);
}
int SpellUnitRec::isActionTakeOff()
{
	return(action_id == SPEC_ACT_AIRCRAFT_UP);
}
int SpellUnitRec::isActionToFortres()
{
	return(action_id == SPEC_ACT_TRANSFORM_TO_FORTRES);
}
int SpellUnitRec::isActionFromFortres()
{
	return(action_id == SPEC_ACT_TRANSFORM_FROM_FORTRES);
}
int SpellUnitRec::isActionCreateUnit()
{
	return(action_id == SPEC_ACT_CREATE_UNIT);
}
int SpellUnitRec::isActionKamikaze()
{
	return(action_id == SPEC_ACT_KAMIZAZE);
}

SpellGraphicItem *SpellUnitRec::GetActionBtnGlyph(int alt)
{
	if(alt)
		return(action_button_glyph_b);
	return(action_button_glyph);
}

// can unit attack target?
int SpellUnitRec::canAttack(SpellUnitRec* target)
{
	if(target->isLight() && !attack_light)
		return(false);
	if(target->isArmored() && !attack_armored)
		return(false);
	if(target->isAir() && !attack_air)
		return(false);
	return(true);
}
int SpellUnitRec::canAttackObject()
{	
	return(attack_objects > 0);
}
// get maximum dig level
int SpellUnitRec::GetMaxDig()
{
	if(isAir())
		return(0);
	if(!dig_turns)
		return(0);
	if(isLight())
		return(6);
	else
		return(2);
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


// render unit (complete, i.e. group of man or tank with turret) and stick for air units:
//   frame - animation frame if applicable
//   *fsu_anim - animation override
//   flight_alt - air unit flight altitute in percent
tuple<int,int> SpellUnitRec::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,
	uint8_t* filter, uint8_t* shadow_filter, ::Sprite *sprt, int man, int azim, int azim_turret, int frame, FSU_resource *fsu_anim, int flight_alt)
{
	// tile slope
	char slope = sprt->name[2];
	
	// visible man count in unit
	//int man = vis;
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

	// flight altitute (in pixels)
	flight_alt = (flight_alt>=0)?(AIR_UNIT_FLY_HEIGHT*flight_alt/100):AIR_UNIT_FLY_HEIGHT;

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
		
		int was_anim = false;
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
				if(fsu_anim->anim.azimuths)
					spr = fsu_anim->anim.lists[res_azim][frame];
				else
					spr = fsu_anim->anim.lists[slope - 'A'][frame];
				was_anim = true;
			}
			else if (frame >= 0 && res->anim.frames)
			{
				// animate
				spr = res->anim.lists[res_azim][frame];
				was_anim = true;
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
				y_pos -= flight_alt;
			}

			// store unit highest pixel offset
			if(uid == 0)
				x_status_bar = 40;
			y_status_bar = min(y_pos + (spr->y_ofs - 128) - buf_y_pos,y_status_bar);

			// render man of unit			
			spr->Render(buffer, buf_end, x_pos, y_pos, buf_x_size, shadow_filter, filter);

			// this should prevent rendering turret when animating
			if(was_anim)
				break;
		}
	}

	// --- render "stick" to ground:
	if (spr && isAir() && man == 1 && !gr_aux)
	{
		// only for air, single man unit, not tank

		// top of the stick
		x_pos = buf_x_pos + 40;
		y_pos = buf_y_pos - flight_alt + spr->y_ofs - 128 + spr->y_size + sprt->y_ofs;
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





//=============================================================================
// Unit bonuses stuff based on BONUSES.DEF file of COMMON.FS
//=============================================================================
UnitBonus::UnitBonus()
{
	level = 0;
	defence = 0;
	attack = 0;
	attack_count = 0;
	move = 0;
}


// load bonus data from BONUSES.DEF
UnitBonuses::UnitBonuses(string bonuses_def)
{
	// try load DEF data
	SpellDEF bonuses(bonuses_def);

	// for each bonus:
	list.resize(13);
	for(int k = 1; k <= 12; k++)
	{
		auto bonus = bonuses.GetSection(string_format("UnitLevel(%d)",k));
		if(!bonus)
			continue;

		for(auto& par : bonus->GetData())
		{
			if(par->parameters->empty())
				continue;
			if(!par->name.compare("Attack"))
				list[k].attack = std::stoi(par->parameters->at(0));
			else if(!par->name.compare("AttackPT"))
				list[k].attack_count = std::stoi(par->parameters->at(0));
			else if(!par->name.compare("Move"))
				list[k].move = std::stoi(par->parameters->at(0));
			else if(!par->name.compare("Defence"))
				list[k].defence = std::stoi(par->parameters->at(0));
		}

		delete bonus;
	}
}
// get bonus for given experience level
UnitBonus* UnitBonuses::GetBonus(int level)
{
	if(level > list.size())
		return(NULL);
	return(&list[level]);
}




//=============================================================================
// Map Unit object stuff
//=============================================================================
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
	experience_level = 1;
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
	dig_turns = 0;
	// turns since last activity
	idle_turns = 0;
	// action points
	action_points = 1;
	// unit active (set except insertion time)
	is_active = 0;
	// unit visible?
	hide = false;
	// enemy?
	is_enemy = 0;
	// unit being placed?
	in_placement = false;
	// moved
	was_moved = true;
	// created by event?
	is_event = false;
	// was unit already seen?
	was_seen = false;
	// is unit visible (0 - nope, 1 - yes (in last check), 2 - yes now)
	is_visible = 0;

	// next unit (for sorted rendering)
	next = NULL;
	// creator unit (if created ingame)
	parent = NULL;
	// child unit (if created ingame)
	child = NULL;

	// link to event that creates the unit (if exists)
	map_event = NULL;
	// link to event to be triggered (SeeUnit)
	trig_event = NULL;

	// sound refs
	sound_move = NULL;

	radar_up = false;
	altitude = 100;

	// FSU sprite index
	in_animation = NULL;
	azimuth = 0;
	azimuth_turret = 0;
	frame = 0;

	move_state = MapUnit::MOVE_STATE::IDLE;
	attack_state = MapUnit::ATTACK_STATE::IDLE;
	action_state = MapUnit::ACTION_STATE::IDLE;
}
// clear sound refs
int MapUnit::ClearSounds()
{
	if(sound_move)
		delete sound_move;
	sound_move = NULL;
	return(0);
}
MapUnit::~MapUnit()
{
	ClearSounds();

	if(child)
	{
		// unlink from child unit
		child->parent = NULL;
		child = NULL;
	}
	if(parent)
	{
		// unlink from parent unit
		parent->child = NULL;
		parent = NULL;
	}
	if(trig_event)
	{
		// unlink SeeUnit() event link
		trig_event->trig_unit = NULL;
		trig_event = NULL;
	}
}
// copy without sound refs because delete would loose the sounds for source object!
MapUnit::MapUnit(MapUnit &obj, bool relink_event_trigger)
{
	*this = obj;
	sound_move = NULL;

	parent = NULL;
	child = NULL;	
	map_event = NULL;

	if(relink_event_trigger)
	{
		// disconnect event trigger from source unit, move it to new unit
		obj.trig_event = NULL;
		if(trig_event)
			trig_event->trig_unit = this;
	}
	else
		trig_event = NULL;
	
	action_state = ACTION_STATE::IDLE;
	move_state = MOVE_STATE::IDLE;
	attack_state = ATTACK_STATE::IDLE;
}
// morph unit type to target (used e.g. for land/take off action)
int MapUnit::MorphUnit(SpellUnitRec* target, int health)
{
	// adapth health
	int org_max_count = unit->cnt;
	if(unit->cnt == 1)
		org_max_count = 100;
	int new_max_count = target->cnt;
	if(target->cnt == 1)
		new_max_count = 100;
	if(health)
	{
		man = unit->GetHP()*health/100;
		wounded = 0;
	}
	else
	{		
		man = man*new_max_count/org_max_count;
		wounded = wounded*new_max_count/org_max_count;
	}

	// change unit type
	unit = target;

	radar_up = false;
	if(unit->isAir())
	{
		dig_level = 0;
	}

	// reset sound refs so engine loads new sounds for new unit type
	if(AreSoundsDone())
		ClearSounds();

	return(0);
}

// try update dig level if possible (call before end of turn)
int MapUnit::UpdateDigLevel()
{
	if(dig_level >= unit->GetMaxDig())
		return(0);
	if(dig_turns < unit->dig_turns)
		return(0);
	dig_level++;
	dig_turns = 0;
	return(1);
}
// clear dig level
void MapUnit::ClearDigLevel()
{
	dig_level = 0;
	dig_turns = 0;
}
// clears unit idle and dig counters
void MapUnit::ResetTurnsCounter()
{
	dig_turns = 0;
	idle_turns = 0;
}
// increment turns counetrs
void MapUnit::IncrementTurnsCounter()
{
	dig_turns++;
	idle_turns++;
}
// activate unit (after runtime creation)
void MapUnit::ActivateUnit()
{
	is_active = true;
}
// is unit active (after runtime creation)
int MapUnit::isActive()
{
	return(!!is_active);
}


// set full health
int MapUnit::ResetHealth()
{
	wounded = 0;
	man = unit->cnt;
	return(0);
}

// action points for this level of experience
int MapUnit::GetMaxAP()
{
	// basic AP
	int ap = unit->ap;

	// experience bonuses
	auto bonus = unit->bonuses->GetBonus(experience_level);
	ap += bonus->move*unit->apw;

	return(ap);
}

// has unit full AP?
int MapUnit::HasMaxAP()
{
	return(action_points == GetMaxAP());
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
	if(unit->isActionKamikaze())
		return(action_points != 0);
	
	// actual fires count
	if(ext_ap >= 0)		
		return(floor(ext_ap/ap_per_fire));
	return(floor(action_points/ap_per_fire));
}

// get max fires count
int MapUnit::GetMaxFireCount()
{
	// get max fires count
	if(unit->isActionKamikaze())
		return(1);
	int ap = GetMaxAP();
	if(!unit->aps)
		return(0);
	int basic_fires = floor(unit->ap/unit->aps);
	
	// add bonus
	auto bonus = unit->bonuses->GetBonus(experience_level);
	basic_fires += bonus->attack_count;
	
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
	auto bonus = unit->bonuses->GetBonus(experience_level);
	move_range += bonus->move;	
	return(GetMaxAP()/(move_range-1));
}

// has unit wounded men?
int MapUnit::HasWounded()
{
	return(wounded);
}

// heal unit
int MapUnit::Heal()
{
	wounded = max(wounded - man,0);
	return(wounded);
}

// can do special action (if enough AP)
int MapUnit::CanSpecAction()
{
	if(!unit->hasSpecAction())
		return(false);
	return(action_points >= unit->action_ap);
}

// initialize experience when unit is created
int MapUnit::InitExperience(int level)
{
	// set experience level
	experience_level = min(max(level,1),12);
	
	// generate random experience points based on level
	// note: crude approximation of Spellcross, it seems actual experience is always somewhere around 20% of current level
	int lev_a = unit->GetExperiencePts(level);
	int lev_b = unit->GetNextExperiencePts(level);
	experience = lev_a + (10 + rand()%11)*(lev_b - lev_a)/100;

	return(experience);
}


int MapUnit::Render(Terrain* data,uint8_t* buffer,uint8_t* buf_end,int buf_x_pos,int buf_y_pos,int buf_x_size,uint8_t *filter,uint8_t* hud_filter,Sprite* sprt,int show_hud)
{
	if(hide)
		return(0);	
	
	// filter for shadow rendering
	auto shadow_filter = data->filter.darker;
	
	int loc_frame = frame;
	if(!in_animation)
		loc_frame = -1; // static unit resource
	
	// visible man based on health
	int visible_man = (unit->vis - 1)*(wounded + man)/unit->GetHP() + 1;

	// render unit
	auto [x_status_bar,y_status_bar] = unit->Render(buffer, buf_end, buf_x_pos, buf_y_pos, buf_x_size,filter,shadow_filter, sprt, visible_man,azimuth, azimuth_turret, loc_frame, in_animation, altitude);
	
	if(!hud_filter)
		hud_filter = data->filter.nullpal;

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
			buf[x] = hud_filter[shadow_filter[buf[x]]];
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
				buf[x] = hud_filter[ap_color];
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
			buf[x] = hud_filter[hp_color];
	}

	// render special unit type mark
	int type_color = 0;
	if(type == MapUnitType::MissionUnit)
		type_color = 230;
	else if(type == MapUnitType::SpecUnit)
		type_color = 253;
	for(int y = 9; y < bar_h; y+=2)
	{
		uint8_t* buf = &psb[y*buf_x_size];
		for(int x = bar_w-3; x < bar_w-1; x++)
			buf[x] = hud_filter[type_color];
	}

	// render fire count
	if(!is_enemy)
	{
		int fires = GetFireCount();
		for(int k = 0; k < fires; k++)
		{		
			uint8_t* buf = &psb[9*buf_x_size + 1 + k*4];
			buf[0] = hud_filter[253]; buf[1] = hud_filter[253]; buf[2] = hud_filter[202];
			buf += buf_x_size;
			buf[0] = hud_filter[253]; buf[1] = hud_filter[202]; buf[2] = hud_filter[202];
		}
	}

	// render dig level
	for(int k = 0; k < dig_level; k++)
	{
		uint8_t* buf = &psb[12*buf_x_size + 1 + k*4];
		buf[0] = hud_filter[252]; buf[1] = hud_filter[252]; buf[2] = hud_filter[214];
		buf += buf_x_size;
		buf[0] = hud_filter[252]; buf[1] = hud_filter[214]; buf[2] = hud_filter[214];
	}

	if(commander_id && !is_enemy)
	{
		// render commander id
		data->font7->RenderSymbol(psb, psb_end, buf_x_size, 19, 1, '0'+commander_id,hud_filter[232]);

		// render commander mark
		if(is_commander)
			data->font7->RenderSymbol(psb,psb_end,buf_x_size,24,1,31,hud_filter[232]);

	}
	
	// return top pixel of unit
	return(y_status_bar);
}



// return unit animation associated with attack to particular unit (or object)
FSU_resource *MapUnit::GetShotAnim(MapUnit *target, int *frame_stop)
{	
	FSU_resource *fsu_anim = NULL;
	int frames = 0;
	if(!target && unit->gr_attack_armor || target && target->unit->isArmored())
	{
		fsu_anim = unit->gr_attack_armor;
		frames = unit->anim_atack_armor_frames;
	}
	else if(!target && unit->gr_attack_light || target && target->unit->isLight())
	{
		fsu_anim = unit->gr_attack_light;
		frames = unit->anim_atack_light_frames;
	}
	else if(!target && unit->gr_attack_air || target && target->unit->isAir())
	{
		fsu_anim = unit->gr_attack_air;
		frames = unit->anim_atack_air_frames;
	}
	if(frame_stop)
		*frame_stop = frames;
	return(fsu_anim);
}

// return target hit animation if exist
AnimPNM *MapUnit::GetTargetHitPNM(MapUnit *target)
{
	if(!target && unit->pnm_armored_shot || target && target->unit->isArmored())
		return(unit->pnm_armored_shot);
	if(!target && unit->pnm_light_shot || target && target->unit->isLight())
		return(unit->pnm_light_shot);	
	if(!target && unit->pnm_air_shot || target && target->unit->isAir())
		return(unit->pnm_air_shot);
	return(NULL);
}

// return shot animation if exist (this is used to animate e.g. cannon fireball)
AnimPNM* MapUnit::GetFirePNM(MapUnit* target)
{
	if(!target && unit->pnm_armored_hit || target && target->unit->isArmored())
		return(unit->pnm_armored_hit);
	if(!target && unit->pnm_light_hit || target && target->unit->isLight())
		return(unit->pnm_light_hit);
	if(!target && unit->pnm_air_hit || target && target->unit->isAir())
		return(unit->pnm_air_hit);
	return(NULL);
}


// return shot animation if exist (this is used to animate e.g. cannon fireball)
tuple<int,int> MapUnit::GetFirePNMorigin(MapUnit* target, double azimuth)
{
	AnimPNM* pnm = GetFirePNM(target);
	/*if(!pnm)
		return tuple(0,0);*/
	if(pnm && unit->gr_aux)
		azimuth = round(azimuth/(double)unit->gr_aux->stat.azimuths)*(double)unit->gr_aux->stat.azimuths;

	int x = (int)(+cos(azimuth/180.0*M_PI)*SpellUnitRec::FIRE_RING_DIAMETER);
	int y = (int)(-sin(azimuth/180.0*M_PI)*SpellUnitRec::FIRE_RING_DIAMETER*cos(Sprite::PROJECTION_ANGLE/180.0*M_PI));
	return tuple(x,y);
}


// check if all unit sounds are stoped (e.g. test before sounds clear when morphing unit)
int MapUnit::AreSoundsDone()
{	
	if(sound_move)
		if(!sound_move->isDone())
			return(false);
	return(true);
}

// play report sound
int MapUnit::PlayReport()
{
	auto sound_report = new SpellSound(*unit->sound_report);
	sound_report->Play(true);
	return(0);
}

// play contact sound
int MapUnit::PlayContact()
{
	auto sound_contact = new SpellSound(*unit->sound_contact);
	sound_contact->Play(true);
	return(0);
}

// play being hit sound
int MapUnit::PlayBeingHit()
{
	auto sound_hit = new SpellSound(*unit->sound_hit);
	sound_hit->Play(true);
	return(0);
}

// play die sound
int MapUnit::PlayDie()
{
	auto sound_die = new SpellSound(*unit->sound_die);
	sound_die->Play(true);
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
int MapUnit::PlayFire(MapUnit* target)
{
	SpellSound *sound = NULL;
	if(!target && unit->sound_attack_armor || target && target->unit->isArmored())
		sound = new SpellSound(*unit->sound_attack_armor->shot);
	else if(!target && unit->sound_attack_light->shot || target && target->unit->isLight())
		sound = new SpellSound(*unit->sound_attack_light->shot);
	else if(!target && unit->sound_attack_air->shot || target && target->unit->isAir())
		sound = new SpellSound(*unit->sound_attack_air->shot);
	if(sound)
		sound->Play(true);
	return(0);
}


// play target hit sound to given unit type (or object if target=NULL)
int MapUnit::PlayHit(bool missed)
{
	return(PlayHit(NULL,missed));
}
int MapUnit::PlayHit(MapUnit* target, bool missed)
{	
	SpellAttackSound *attack = NULL;
	if(!target && unit->sound_attack_armor)
		attack = unit->sound_attack_armor;
	else if(!target && unit->sound_attack_light)
		attack = unit->sound_attack_light;
	else if(!target && unit->sound_attack_air)
		attack = unit->sound_attack_air;
	else if(target->unit->isLight())
		attack = unit->sound_attack_light;
	else if(target->unit->isArmored())
		attack = unit->sound_attack_armor;
	else if(target->unit->isAir())
		attack = unit->sound_attack_air;
	if(!attack)
		return(1);

	SpellSound* sound = NULL;
	if(missed)
		sound = attack->hit_miss;
	else if(!target)
		sound = attack->hit_armor;
	else if(target->unit->isFlashAndBones())
		sound = attack->hit_flash;
	else if(target->unit->isMetal())
		sound = attack->hit_armor;	
	if(!sound)
		return(1);

	sound = new SpellSound(*sound);
	sound->Play(true);
	return(0);
}

// play report sound
int MapUnit::PlayAction()
{
	auto sound_action = new SpellSound(*unit->sound_action);
	sound_action->Play(true);
	return(0);
}


// --- attack damage model stuff

// get attack strength to target unit (or NULL to object)
int MapUnit::GetAttack(MapUnit *target)
{
	TARGET_TYPE type = TARGET_TYPE::NONE;
	if(!target)
		type = TARGET_TYPE::OBJECT;
	else if(target->unit->isLight())
		type = TARGET_TYPE::LIGHT;
	else if(target->unit->isArmored())
		type = TARGET_TYPE::ARMOR;
	else if(target->unit->isAir())
		type = TARGET_TYPE::AIR;
	return(GetAttack(type));
}
int MapUnit::GetAttack(MapUnit::TARGET_TYPE target)
{
	// get basic attack
	int attack = 0;
	if(target == TARGET_TYPE::OBJECT)
		attack = unit->attack_objects;
	else if(target == TARGET_TYPE::LIGHT)
		attack = unit->attack_light;
	else if(target == TARGET_TYPE::ARMOR)
		attack = unit->attack_armored;
	else if(target == TARGET_TYPE::AIR)
		attack = unit->attack_air;
	if(!attack)
		return(attack);
	
	// add bonuses
	auto bonus = unit->bonuses->GetBonus(experience_level);
	attack += bonus->attack;
		
	return(attack);
}

// get unit defence
int MapUnit::GetDefence()
{
	// basic defence
	int defence = unit->defence;
	if(!defence)
		return(defence);
	
	// add bonuses
	defence += unit->bonuses->GetBonus(experience_level)->defence;
	return(defence);
}


// apply damage model for attack to target tile/object
MapUnit::AttackResult MapUnit::DamageTarget(MapSprite *target)
{
	if(!target)
		return(AttackResult::Missed);

	ResetTurnsCounter();

	// attack strength
	double attack = GetAttack();
	
	// object's defence
	auto tile = target->GetDestructible();
	double defence = tile->destructible->defence;

	// damage model
	int hit = (int)(randgman(3.0, 2.0, 5.0)*attack - defence);
	if(hit < 0)
		return(AttackResult::Missed);
	
	// reduce HP
	target->hp = max(target->hp - hit, 0);

	if(!target->hp)
		return(AttackResult::Kill);
	else
		return(AttackResult::Hit);
}

// apply damage model for attack to target unit
MapUnit::AttackResult MapUnit::DamageTarget(MapUnit* target)
{
	if(!target)
		return(AttackResult::Missed);

	ResetTurnsCounter();

	// attack strength
	double attack = GetAttack(target);
	
	// reduce attack by man state of unit
	if(target->unit->isSingleMan())
		attack *= (double)(unit->GetHP() - wounded)/unit->GetHP();
	else
		attack *= (double)man/unit->GetHP();

	// target defence
	double defence = target->GetDefence();

	// defence bonus by dig level
	defence *= (1.0 + (double)target->dig_level*0.5);
		
	// reduce dig level of target
	target->dig_level = max(target->dig_level - 1, 0);

	// conditional bonuses/penalties
	double bonus = 1.0;
		
	// penalize armored attacks?
	if(target->unit->isArmored() && unit->isInefficientToArmor())
		bonus *= 0.7;

	// bonus of fire sensitivity
	if(target->unit->isFireSensitive() && unit->hasFireAttack())
		bonus *= 1.5;

	double morale = max(1.0,0.1);
	
	// randomize attack
	double rng_attack = randgman(3.5,2.0,3.0/morale,0.7*morale)*attack*bonus;

	// damage model
	double wound = (int)(rng_attack - 0.7*defence);
	double kill = (int)(rng_attack - defence);
	if(wound < 0.0 && kill < 0.0)
		return(AttackResult::Missed);

	// reduce HP
	if(target->unit->isSingleMan())
	{
		// for single-man unit only wounding
		target->wounded = min(target->wounded + (int)kill, target->unit->GetHP());
		if(target->wounded >= target->unit->GetHP())
			target->man = 0;
	}
	else
	{
		// for multi-man units

		// killed some
		target->wounded = target->wounded - (int)(0.5*kill);
		if(target->wounded < 0)
		{
			kill += (double)(-target->wounded);
			target->wounded = 0;
		}
		else
			kill -= 0.5*kill;
		target->man = max(target->man - (int)kill, 0);
				
		// wound some
		target->wounded = target->wounded + (int)wound;
		target->man = max(target->man - (int)wound, 0);
	}

	if(!target->man)
		return(AttackResult::Kill);
	else
		return(AttackResult::Hit);
}

// check if unit is dead
int MapUnit::isDead()
{
	return(!man);
}

// kill unit (exec linked events, then delete the object, so no touchy afterwards and call Extract() from units list before calling this!)
SpellMapEventRec *MapUnit::Kill()
{
	SpellMapEventRec *evt = NULL;
	if(trig_event)
	{		
		// unlink from event
		evt = trig_event;
		evt->trig_unit = NULL;
		trig_event = NULL;

		// mark as done
		evt->is_done = true;		
	}

	// destroy this unit
	delete this;

	// eventually return triggered event
	return(evt);
}