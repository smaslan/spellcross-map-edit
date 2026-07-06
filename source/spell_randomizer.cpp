//=============================================================================
// Spellcross units randomizer stuff
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_randomizer.h"
#include "other.h"
#include "spell_def.h"
#include "spell_units.h"


// init auto randomizer
UnitRandomizer::UnitRandomizer()
{
	last_error = "";
	rules.clear();
}

void UnitRandomizer::Clear()
{
	last_error = "";
	rules.clear();
}

// build DEF section with randomizer rules
std::string UnitRandomizer::MakeRulesDEF(std::string indent)
{
	std::string def;
	for(auto &rule: rules)
	{
		if(rule.rand_units.empty())
			continue;		
		def += indent + "AutoRandomizeRule(";
		def += string_format("%d,",rule.ref_unit);
		for(int k = 0; k < rule.rand_units.size(); k++)
		{
			def += string_format("%d",rule.rand_units[k]);
			if(k + 1 < rule.rand_units.size())
				def += ",";
		}
		def += ")\n";
	}
	return(def);
}

// try parse auto randomizer definition command
int UnitRandomizer::AddRule(SpellDefCmd *cmd, SpellUnits *units)
{
	last_error = "";
	if(!units)
		return(1);
	
	if(cmd->parameters.size() < 2)
	{
		last_error = string_format("Not enough parameters for auto randomzier command '%s'!",cmd->full_command.c_str());
		return(1);
	}

	// parse rule
	UnitRandomizerRule rule;
	for(int k = 0; k < cmd->parameters.size(); k++)
	{
		auto& unit_id_str = cmd->parameters[k];
		char* send;
		auto unit_type_id = std::strtol(unit_id_str.c_str(),&send,10);
		auto rand_unit = units->GetUnit(unit_type_id);
		if(!rand_unit)
		{
			last_error = string_format("Unit type '%s' not recognized for auto randomzier command '%s'!",unit_id_str.c_str(),cmd->full_command.c_str());
			return(1);
		}
		if(k == 0)
			rule.ref_unit = unit_type_id;
		else
		{
			if(std::find(rule.rand_units.begin(),rule.rand_units.end(),unit_type_id) != rule.rand_units.end())
			{
				last_error = string_format("Unit type '%d' already in auto randomzier random units list! Command '%s'!",unit_type_id,cmd->full_command.c_str());
				return(1);
			}
			rule.rand_units.push_back(unit_type_id);
		}
	}

	// check duplicate rules
	for(auto &rr: rules)
		if(rule.ref_unit == rr.ref_unit)
		{
			last_error = string_format("Reference unit type '%d' already in auto randomzier rules list! Command '%s'!",rule.ref_unit,cmd->full_command.c_str());
			return(1);
		}

	rules.push_back(rule);
	SortRules();
	return(0);
}

// try parse auto randomizer definition command
int UnitRandomizer::AddRule(SpellUnits* units,int& new_pos)
{
	last_error = "";
	new_pos = -1;
	if(!units)
		return(1);	
	
	// select first unused unit type as reference
	std::vector<int> list;
	for(auto &unit: units->GetUnits())
		list.push_back(unit->type_id);
	int new_ref_id = -1;
	for(auto &id: list)
	{
		bool dup = false;
		for(auto &rule: rules)
			if(rule.ref_unit == id)
				dup = true;
		if(!dup)
		{
			new_ref_id = id;
			break;
		}
	}
	if(new_ref_id < 0)
	{
		last_error = "Cannot add new rule! All unit types are already listed.";
		return(1);
	}

	UnitRandomizerRule rule;
	rule.ref_unit = new_ref_id;
	rules.push_back(rule);
	SortRules();
	for(auto &rule: rules)
		if(rule.ref_unit == new_ref_id)
			new_pos = &rule - rules.data();
	return(0);
}

// check rules for validity (duplicates and stuff)
int UnitRandomizer::CheckRules()
{
	last_error = "";
	for(auto &rule: rules)
	{
		bool dup = false;
		for(auto& rule2: rules)
		{
			if(&rule2 == &rule)
				continue;
			if(rule.ref_unit == rule2.ref_unit)
				dup = true;
		}
		if(dup)
		{
			last_error = string_format("Reference unit type '%d' already in auto randomzier rules list!",rule.ref_unit);
			return(1);
		}

		//std::sort(rule.rand_units.begin(),rule.rand_units.end());
		for(auto &id: rule.rand_units)
		{
			bool dup = false;
			for(auto &id2: rule.rand_units)
			{
				if(&id2 == &id)
					continue;
				if(id2 == id)
					dup = true;
			}
			if(dup)
			{
				last_error = string_format("Unit type '%d' is duplicate in auto randomzier random units list!",id);
				return(1);
			}
		}
	}
	return(0);
}

// remove auto randomizer rule
int UnitRandomizer::RemoveRule(int pos)
{
	last_error = "";
	if(pos < 0 || pos >= rules.size())
	{
		last_error = string_format("Auto randomizer rule '%d' not in the rule list!",pos);
		return(1);
	}
	rules.erase(rules.begin() + pos);
	return(0);
}

// sort rules by ref unit type
int comp_rules(UnitRandomizerRule &a,UnitRandomizerRule& b)
{
	return(b.ref_unit >= a.ref_unit);
}
int UnitRandomizer::SortRules()
{
	std::sort(rules.begin(), rules.end(),comp_rules);
	return(0);
}