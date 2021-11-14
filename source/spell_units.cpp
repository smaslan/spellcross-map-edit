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
#include "fsu_archive.h"
#include "sprites.h"
#include <vector>
#include <stdexcept>

using namespace std;

// default resource content
SpellUnitRec::SpellUnitRec()
{	
	gr_base = NULL;
	gr_aux = NULL;
}

// decode units def file
SpellUnits::SpellUnits(uint8_t* data, int dlen)
{
	SpellUnits(data, dlen, NULL);
}
SpellUnits::SpellUnits(uint8_t* data, int dlen, FSUarchive *fsu)
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

		// max range
		unit->rmax = rdu8(rec + 0x46);

		// special shot flags
		// 0x01 - high turret origin (probably???)
		// 0x02 - indirect fire (artilery)
		// 0x04 - steals action points
		// 0x08 - inefficient to armored targets (probably???)
		// 0x10 - can't fire from slopes
		// 0x20 - indirect missile (MLRS only???)
		// 0x40 - fire projectile
		// 0x80 - fire sensitivity (increased fire sensitivity???)
		unit->rsfl = rdu8(rec + 0x47);

		// aditional ENG version item
		// 0x01 - unit is healed by fire
		if (is_eng)
		{
			// read it
			unit->res1 = rdu8(rec + 0x48);

			// move record pointer after that
			rec++;
		}

		// ap per shot
		unit->aps = rdu8(rec + 0x48);


		// light attack hit animation (*.pnm animation)
		rdsc(unit->anhlig, rec + 0x49, 9);

		// light attack unit animation resource (*.fsu resource)
		rdsc(unit->anlig, rec + 0x52, 6);
		// used frames count
		unit->anligf = rdu8(rec + 0x61);

		// light attack shot animation (*.pnm resource)
		rdsc(unit->anslig, rec + 0x58, 9);

		// armored attack hit animation (*.pnm animation)
		rdsc(unit->anharm, rec + 0x62, 9);

		// armored attack unit animation resource (*.fsu resource)
		rdsc(unit->anarm, rec + 0x6B, 6);
		// used frames count
		unit->anarmf = rdu8(rec + 0x7A);

		// armored attack shot animation (*.pnm resource)
		rdsc(unit->ansarm, rec + 0x71, 9);

		// air attack hit animation (*.pnm animation)
		rdsc(unit->anhair, rec + 0x7B, 9);

		// air attack unit animation resource (*.fsu content)
		rdsc(unit->anair, rec + 0x84, 6);
		// used frames count
		unit->anairf = rdu8(rec + 0x93);

		// air attack shot animation (*.pnm resource)
		rdsc(unit->ansair, rec + 0x8A, 9);

		// projectile visibility flags (0x01-light, 0x02-armored, 0x04-air attacks)
		unit->prjf = rdu8(rec + 0x94);

		// projectile resource (*.grf files)
		rdsc(unit->prj, rec + 0x95, 13);

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
		if (fsu)
		{
			unit->gr_base = fsu->GetResource(unit->gra);
			unit->gr_aux = fsu->GetResource(unit->grb);
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



// render unit (complete, i.e. group of man or tank with turret)
void SpellUnitRec::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,
	uint8_t* shadow_filter, ::Sprite *sprt, int azim, int frame)
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

	/*if (man == 4)
		man = 40;*/

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
			if (part == 0)
				res = gr_base;
			else
				res = gr_aux;
			if (!res)
				break;
			
			int y_org = 0;
			if (res->stat.slopes == 1)
				slope = 'A';
												
			// get sprite record
			if (frame >= 0 && res->anim.frames)
			{
				// animate
				spr = res->anim.lists[azim][frame];
			}
			else
				spr = res->stat.lists[slope-'A'][azim];

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

			// render man of unit			
			spr->Render(buffer, buf_end, x_pos, y_pos, buf_x_size, shadow_filter);
			//buffer[x_pos + (spr->x_max + spr->x_min)/2 + (y_pos + spr->y_ofs + spr->y_size-128) * buf_x_size] = 252;
		}
	}

	// --- redner "stick" to ground:
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


}