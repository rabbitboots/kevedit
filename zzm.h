/* zzm.h  -- zzm file routines
 * $Id: zzm.h,v 1.4 2001/12/15 00:54:53 bitman Exp $
 * Copyright (C) 2000 Ryan Phillips <bitman@scn.org>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __ZZM_H
#define __ZZM_H

#include "svector.h"
#include "display.h"

typedef struct zzmplaystate {
	int octave;     /* Current octave */
	int duration;   /* Current note duration */
	int pos;        /* Position on zzm line */
	int slur;       /* Are we slurring presently? */
} zzmplaystate;

/* ZZM note types */
#define ZZM_ERROR 0   /* Usually end-of-string */
#define ZZM_NOTE  1   /* Just a note */
#define ZZM_REST  2   /* Rest for a moment */
#define ZZM_DRUM  3   /* Tick-tock */

typedef struct zzmnote {
	int type;       /* Type of note */
	int duration;   /* Millisecond duration */
	int index;      /* Frequency index or drum index */
	int octave;     /* Octave of note below MAXOCTAVE */
	int slur;       /* TRUE if note is to slur with the next */
} zzmnote;

/* zzmpullsong() - pulls song #songnum out of zzmv and returns it */
stringvector zzmpullsong(stringvector * zzmv, int songnum);

/* zzmpicksong() - presents a dialog to choose a song based on title */
int zzmpicksong(stringvector * zzmv, displaymethod * d);

/* resetzzmplaystate() - clears state to default settings */
void resetzzmplaystate(zzmplaystate * s);

/* zzmplaynote() - plays a single note from a tune */
zzmnote zzmgetnote(char * tune, zzmplaystate * s);

/* Play music to PC speaker */
void zzmPCspeakerPlaynote(zzmnote note);
void zzmPCspeakerFinish(void);

#endif
