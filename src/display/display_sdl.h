/* display_sdl.h    -- SDL display
 * $Id: display_sdl.h,v 1.3 2005/06/29 03:20:34 kvance Exp $
 * Copyright (C) 2002 Kev Vance <kvance@kvance.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place Suite 330; Boston, MA 02111-1307, USA.
 */

#include "SDL.h"

#include "textblock.h"

#include <stdio.h>
#include <stdbool.h>

#ifndef DISPLAY_SDL
#define DISPLAY_SDL 1

/* These are in bytes. */
#define TEXT_MODE_VRAM (80 * 25 * 2)

/* This structure has pointers to all important video information */
typedef struct
{
	SDL_Window *window;
	SDL_Renderer *renderer;
        SDL_Texture *texture;          /* GPU texture to be updated           */
        Uint32 *pixels;                /* Screen pixels, each element an ARGB */
	textBlock *buffer;             /* Emualted VRAM                       */
	Uint8 *char_set;               /* Pointer to character set            */
	Uint32 *palette;               /* Pointer to palette                  */
	Uint32 write_x;                /* Current x write position in VRAM    */
	Uint32 write_y;                /* Current y write position in VRAM    */

	/* Info for reinitializing the screen  */
	Uint32 width, height, depth, vflags;

        bool is_fullscreen;
        bool is_dirty;
} video_info;

/* Prototypes */
void display_load_charset(Uint8 *dest, char *name);
void display_load_palette(Uint32 *dest, char *name);
void display_init(video_info *vdest, Uint32 width, Uint32 height,
		Uint32 depth);
void display_end(video_info *vdest);
void display_putch(video_info *vdest, Uint32 x, Uint32 y, Uint8 ch, Uint8 co);
void display_gotoxy(video_info *vdest, Uint32 x, Uint32 y);
void display_redraw(video_info *vdest);
void display_update(video_info *vdest, int x, int y, int width, int height);
void display_update_and_present(video_info *vdest, int x, int y, int width,
		int height);

/* For RegisterDisplays() */
extern displaymethod display_sdl;

#endif
