//=============================================================================
// Spellcross MAP related data types.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <tuple>

class MapXY
{
public:
	int x;
	int y;
	int IsSelected() {return(x >= 0 && y >= 0);};
	int IsEqual(int x, int y) {return(this->x == x && this->y == y);};
	MapXY() {x=-1;y=-1;};
	MapXY(int xpos, int ypos) { x=xpos;y=ypos; };
	// geometric distance of tile
	double Distance(MapXY mxy) { return(Distance(mxy.x,mxy.y)); };
	double Distance(int xx, int yy)
	{		
		auto [dx,dy] = this->RelativePos(xx,yy);
		return(sqrt(dx*dx + dy*dy));
	};
	// realtive position of tile <dx,dy>
	std::tuple<double,double> RelativePos(MapXY mxy) {return RelativePos(mxy.x,mxy.y);};
	std::tuple<double,double> RelativePos(int xx,int yy)
	{
		int xx1 = xx * 2 + (((yy & 1) != 0) ? 0 : 1);
		int yy1 = yy * 1;
		int xx2 = x * 2 + (((y & 1) != 0) ? 0 : 1);
		int yy2 = y * 1;
		return tuple(0.707106*(double)(xx1-xx2),0.707106*(double)(yy1-yy2));
	};
	
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
	int operator == (MapXY par)
	{		
		return(this->x == par.x && this->y == par.y);
	}
	int operator != (MapXY par)
	{
		return(this->x != par.x || this->y != par.y);
	}
};



