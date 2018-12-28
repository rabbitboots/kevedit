/* sdl_synth.c	-- SDL music synthesizer
 * $Id: sdl_synth.c,v 1.3 2005/06/29 03:20:34 kvance Exp $
 * Copyright (C) 2001 Kev Vance <kvance@kvance.com>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SDL.h"

#include "notes.h"
#include "sdl_synth.h"

Uint8 *masterplaybuffer = NULL;
static size_t playbuffersize = 0, playbufferloc = 0, playbuffermax = 0;

static int synthVolume = SYNTH_VOLUME_MAX;

int OpenSynth(SDL_AudioSpec * spec)
{
	SDL_AudioSpec desired, obtained;

	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		/* If the audio subsystem isn't ready, initialize it */
		if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
			fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
			return 1;
		}
	}

	/* Set desired sound opts */
	desired.freq = 44100;
	desired.format = AUDIO_U16SYS;
	desired.channels = 1;
	desired.samples = 4096;
	desired.callback = AudioCallback;
	desired.userdata = spec;

	/* Open audio device */
	if(SDL_OpenAudio(&desired, &obtained) < 0) {
		fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_PauseAudio(0);

	(*spec) = obtained;

	return 0;
}

void CloseSynth(void)
{
	/* Silence, close the audio, and clean up the memory we used. */
	SDL_PauseAudio(1);
	SDL_CloseAudio();

	AudioCleanUp();
}

int IsSynthBufferEmpty()
{
	return playbufferloc == playbuffermax;
}

void SynthPlayNote(SDL_AudioSpec audiospec, musicalNote note, musicSettings settings)
{
	/* Find the frequency and duration (wait) in seconds */
	float frequency = noteFrequency(note, settings);
	float wait    = noteDuration(note, settings) / 1000;
	float spacing = noteSpacing(note, settings) / 1000;

	if (note.type == NOTETYPE_NOTE) {
		/* Add the sound to the buffer */
		AddToBuffer(audiospec, frequency, wait);
	}

	/* Rests are simple */
	if (note.type == NOTETYPE_REST) {
		AddToBuffer(audiospec, 0, wait);
	}

	/* Drums */
	if (note.type == NOTETYPE_DRUM) {
		/* Okay, these drums sound terrible, but they're better than nothing. */
		int i;
		float breaktime = wait - ((float)DRUMBREAK) * DRUMCYCLES / 1000;

		/* Loop through each drum cycle */
		for (i = 0; i < DRUMCYCLES; i++) {
			AddToBuffer(audiospec, drums[note.index][i], ((float)DRUMBREAK) / 1000);
		}

		/* Add a break based on the current duration */
		if( breaktime > 0 ) {
			AddToBuffer(audiospec, 0, breaktime);
		}
	}

	if (spacing != 0.0)
		AddToBuffer(audiospec, 0, spacing);
}

int synthSetVolume( int newVolume ) {
	if( newVolume < 0 ) {
		synthVolume = 0;
	} else if( newVolume > SYNTH_VOLUME_MAX ) {
		synthVolume = SYNTH_VOLUME_MAX;
	} else {
		synthVolume = newVolume;
	}

	return synthVolume;
}
int synthGetVolume(void) {
	return synthVolume;
}
int synthGetVolumeMax(void) {
	return SYNTH_VOLUME_MAX;
}

int synthAdjustSampleVolume(int sample, int waveformBottom, int waveformTop, int newVolume, int volumeMax) {
	int middle = waveformBottom + abs(waveformTop - waveformBottom) / 2;
	int travel = abs(sample - middle);

	/* Reject invalid volume parameters */
	if( volumeMax == 0 || newVolume < 0 || newVolume > volumeMax ) {
		return middle;
	}
	float fraction = (float)newVolume / (float)volumeMax;

	int retval;

	if( sample > middle ) {
		retval = middle + travel*fraction;
	}
	else {
		retval = middle - travel*fraction;
	}

	return retval;
}

void AddToBuffer(SDL_AudioSpec spec, float freq, float seconds)
{
	size_t notesize = seconds * spec.freq; /* Bytes of sound */
	size_t wordsize;
	size_t i, j;

	int osc = 1;

	/* Adjust amplitude */

	Uint8 u8on  = synthAdjustSampleVolume(U8_1, U8_0, U8_1, synthVolume, SYNTH_VOLUME_MAX);
	Uint8 u8off = synthAdjustSampleVolume(U8_0, U8_0, U8_1, synthVolume, SYNTH_VOLUME_MAX);
	Sint8 s8on  = synthAdjustSampleVolume(S8_1, S8_0, S8_1, synthVolume, SYNTH_VOLUME_MAX);
	Sint8 s8off = synthAdjustSampleVolume(S8_0, S8_0, S8_1, synthVolume, SYNTH_VOLUME_MAX);

	Uint16 uon  = synthAdjustSampleVolume(U16_1, U16_0, U16_1, synthVolume, SYNTH_VOLUME_MAX);
	Uint16 uoff = synthAdjustSampleVolume(U16_0, U16_0, U16_1, synthVolume, SYNTH_VOLUME_MAX);
	Sint16 son  = synthAdjustSampleVolume(S16_1, S16_0, S16_1, synthVolume, SYNTH_VOLUME_MAX);
	Sint16 soff = synthAdjustSampleVolume(S16_0, S16_0, S16_1, synthVolume, SYNTH_VOLUME_MAX);

	/* Don't let the callback function access the playbuffer while we're editing it! */
	SDL_LockAudio();

	if(spec.format == AUDIO_U8 || spec.format == AUDIO_S8)
		wordsize = 1;
	else
		wordsize = 2;

	if(playbuffersize != 0 && playbufferloc != 0) {
		/* Shift buffer back to zero */
		memmove(masterplaybuffer,
			&masterplaybuffer[playbufferloc],
			playbuffersize-playbufferloc);
		playbuffermax -= playbufferloc;
		playbufferloc = 0;
	}
	if(playbuffersize == 0) {
		/* Create buffer */
		masterplaybuffer = malloc(notesize*wordsize);
		playbuffersize   = notesize*wordsize;
	}
	if((notesize*wordsize) > (playbuffersize-playbuffermax)) {
		/* Make bigger buffer */
		masterplaybuffer = realloc(masterplaybuffer,
				playbuffersize+notesize*wordsize);

		playbuffersize += notesize*wordsize;
	}

	if(freq == 0) {
		/* Rest */
		memset(&masterplaybuffer[playbuffermax],
				spec.silence,
				notesize*wordsize);
		playbuffermax += notesize*wordsize;
	} else {
		/* Tone */
		float hfreq = (spec.freq/freq/2.0);
		for(i = 0, j = 0; i < notesize; i++, j++) {
			if(j >= hfreq) {
				osc ^= 1;
				j = 0;
			}
			if(spec.format == AUDIO_U8) {
				if(osc)
					masterplaybuffer[playbuffermax] = u8on;
				else
					masterplaybuffer[playbuffermax] = u8off;
			} else if(spec.format == AUDIO_S8) {
				if(osc)
					masterplaybuffer[playbuffermax] = s8on;
				else
					masterplaybuffer[playbuffermax] = s8off;
			} else if(spec.format == AUDIO_U16) {
				if(osc)
					memcpy(&masterplaybuffer[playbuffermax], &uon, 2);
				else
					memcpy(&masterplaybuffer[playbuffermax], &uoff, 2);
			} else if(spec.format == AUDIO_S16) {
				if(osc)
					memcpy(&masterplaybuffer[playbuffermax], &son, 2);
				else
					memcpy(&masterplaybuffer[playbuffermax], &soff, 2);
			}
			playbuffermax += wordsize;
		}
	}

	/* Now let AudioCallback do its work */
	SDL_UnlockAudio();
}

void AudioCallback(void *userdata, Uint8 *stream, int len)
{
	int i;
	for(i = 0; i < len && playbufferloc < playbuffermax; i++) {
		stream[i] = masterplaybuffer[playbufferloc];
		playbufferloc++;
	}
	for(; i < len; i++)
		stream[i] = ((SDL_AudioSpec *)userdata)->silence;
}

void AudioCleanUp()
{
	if(playbuffersize != 0) {
		free(masterplaybuffer);
		masterplaybuffer = NULL;
		playbuffersize = 0;
		playbufferloc = 0;
		playbuffermax = 0;
	}
}
