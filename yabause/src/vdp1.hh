/*  Copyright 2003 Guillaume Duhamel

    This file is part of Yabause.

    Yabause is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Yabause is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yabause; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef VDP1_HH
#define VDP1_HH

#include "memory.hh"
#include "cpu.hh"
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef __WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

#include <vector>
#include "vdp2.hh"

#ifdef _arch_dreamcast
#define SAT2YAB1(alpha,temp)		((0x8000) | (temp & 0x1F) << 10 | (temp & 0x3E0) | (temp & 0x7C00) >> 10)
#elif defined WORDS_BIGENDIAN
#define SAT2YAB1(alpha,temp)		(alpha | (temp & 0x7C00) << 1 | (temp & 0x3E0) << 14 | (temp & 0x1F) << 27)
#else
#define SAT2YAB1(alpha,temp)		(alpha << 24 | (temp & 0x1F) << 3 | (temp & 0x3E0) << 6 | (temp & 0x7C00) << 9)
#endif

#ifdef _arch_dreamcast
#elif defined WORDS_BIGENDIAN
#define SAT2YAB2(alpha,dot1,dot2)       ((dot2 & 0xFF << 24) | ((dot2 & 0xFF00) << 8) | ((dot1 & 0xFF) << 8) | alpha)
#else
#define SAT2YAB2(alpha,dot1,dot2)       (alpha << 24 | ((dot1 & 0xFF) << 16) | (dot2 & 0xFF00) | (dot2 & 0xFF))
#endif

class Scu;
class Vdp2;
class Vdp2ColorRam;

typedef struct {
	unsigned long vdp1_loc;
	bool dirty;
	GLuint txr;
} vdp1Sprite;

class Vdp1VRAM : public Memory	{
	private:
		vector<vdp1Sprite> sprites;		
		
	public:
		Vdp1VRAM(unsigned long m, unsigned long size);
		~Vdp1VRAM();
		
		void setByte(unsigned long l, unsigned char d);
		void setWord(unsigned long l, unsigned short d);
		void setLong(unsigned long l, unsigned long d);
		vdp1Sprite getSprite(unsigned long l);
		void addSprite(vdp1Sprite &sp);
};

class Vdp1 : public Cpu, public Memory, public VdpScreen {
private:
  GLuint texture[1];
  SaturnMemory *satmem;
  Vdp1VRAM *vram;

  unsigned short localX;
  unsigned short localY;

  unsigned short returnAddr;
  	unsigned short	CMDCTRL;
	unsigned short	CMDLINK;
	unsigned short	CMDPMOD;
	unsigned short	CMDCOLR;
	unsigned short	CMDSRCA;
	unsigned short	CMDSIZE;
	short	CMDXA;
	short	CMDYA;
	short	CMDXB;
	short	CMDYB;
	short	CMDXC;
	short	CMDYC;
	short	CMDXD;
	short	CMDYD;
	unsigned short	CMDGRDA;
	unsigned short w;
	unsigned short h;
	unsigned short ww;
	unsigned short hh;

public:
  Vdp1(SaturnMemory *);
  void execute(unsigned long = 0);
  void stop(void);

  /*
  void setVdp2Ram(Vdp2 *, Vdp2ColorRam *);
  int getAlpha(void);
  int getColorOffset(void);
  */
  Memory *getVRam(void);

  void readCommand(unsigned long);
  void readTexture(vdp1Sprite *);
  
  void normalSpriteDraw(unsigned long);
  void scaledSpriteDraw(unsigned long);
  void distortedSpriteDraw(unsigned long);
  void polygonDraw(unsigned long);
  void polylineDraw(unsigned long);
  void lineDraw(unsigned long);
  void userClipping(unsigned long);
  void systemClipping(unsigned long);
  void localCoordinate(unsigned long);

  void draw(void);
  int getPriority(void);
  int getInnerPriority(void);
};

#endif
