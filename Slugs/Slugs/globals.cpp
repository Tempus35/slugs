#include "globals.h"

cGlobals::cGlobals() {
	GetCurrentDirectory(255, sApplicationPath);
}

cGlobals::~cGlobals() {
}

char *cGlobals::CombiStr(char *sPath) {
	sprintf_s(sBuffer, "%s\\%s", sApplicationPath, sPath);
	return sBuffer; 
}

/*
void cGlobals::Draw_Circle(HGE *hge, float cx, float cy, float Radius, int Segments, DWORD color)
{
  float EachAngle;
  float a;
  float x1;
  float x2;
  float y1;
  float y2;
 
  EachAngle = 2.0 * M_PI / (float)Segments;
 
  x2 = Radius;
  y2 = 0.0;
 
  for(a=0.0; a<= (2.0*M_PI + EachAngle); a+=EachAngle) {
    x1 = x2;
    y1 = y2;
    x2 = Radius * cos(a);
    y2 = Radius * sin(a);
    hge->Gfx_RenderLine(x1+cx, y1+cy, x2+cx, y2+cy, color);
  }
} 
*/