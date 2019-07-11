/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:33:50 GMT 2018 keithdhedger@gmail.com

 * This file (globals.h) is part of Kute.

 * Kute is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation,either version 3 of the License,or
 * at your option) any later version.

 * Kute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Kute.  If not,see <http://www.gnu.org/licenses/>.
*/

#include <mp4v2/mp4v2.h>
#include <id3v1tag.h>
#include <mpegfile.h>

#include <stdlib.h>

#ifndef _GLOBALS_

#define IS_FLAC 1
#define IS_MP4 2
#define IS_MP3 3
#define UNKNOWN_TYPE 4

enum {SETTITLE=0,SETALBUM,SETARTIST,SETTRACK,SETTOTALTRACKS,SETCD,SETCOMPILATION,SETYEAR,SETGENRE,SETCOMPOSER,SETCOMMENT,MAXTAG};

extern char	*filename;
extern int	filetype;
extern bool	readall;
extern bool	force;
extern bool	quotes;

extern bool	removealltags;
extern char	*title;
extern char	*album;
extern char	*artist;

extern char	*year;
extern char	*genre;
extern char	*composer;
extern char	*comment;

extern char	*cdstring;
extern char	*totaltracksstring;
extern char	*trackstring;
extern char	*compilationstring;
extern int	tagstoset[MAXTAG];

extern char	*tempbuffer;

void printTags(void);

#define _GLOBALS_

#endif
