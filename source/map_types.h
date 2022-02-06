//=============================================================================
// Spellcross MAP related data types.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

class MapXY
{
public:
	int x;
	int y;
	bool IsSelected() {return(x >= 0 && y >= 0);};
	MapXY() {x=0;y=0;};
	MapXY(int xpos, int ypos) { x=xpos;y=ypos; };
	MapXY operator + (MapXY par)
	{
		MapXY res(x,y);
		if(res.IsSelected() && par.IsSelected())
		{
			res.x += par.x;
			res.y += par.y;
		}
		return(res);
	}
	MapXY operator - (MapXY par)
	{
		MapXY res(x,y);
		if(res.IsSelected() && par.IsSelected())
		{
			res.x -= par.x;
			res.y -= par.y;
		}
		return(res);
	}
};



