/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:34:03 GMT 2018 keithdhedger@gmail.com

 * This file (flactags.h) is part of Kute.

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

#include <FLAC/metadata.h>
#include "globals.h"
#include "mp4tags.h"

#ifndef _FLACTAGS_
#define _FLACTAGS_

void setFlacTags(void);
bool FlacTagReadFileTag(char *filename);
bool doReadAllFlac(void);

#endif
