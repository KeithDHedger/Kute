/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:33:42 GMT 2018 keithdhedger@gmail.com

 * This file (globals.cpp) is part of Kute.

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

#include "globals.h"

char	*filename;
int		filetype=UNKNOWN_TYPE;
bool	readall=false;
bool	force=false;
bool	quotes=false;

bool	removealltags=false;
char	*title=NULL;
char	*album=NULL;
char	*artist=NULL;
char	*year=NULL;
char	*genre=NULL;
char	*composer=NULL;
char	*comment=NULL;

int		tagstoset[MAXTAG];
char	*cdstring=NULL;
char	*totaltracksstring=NULL;
char	*compilationstring=NULL;

char	*trackstring=NULL;
char	*tempbuffer=(char*)calloc(2048,1);

void printTags(void)
{
	if(quotes==true)
		{
			printf("FILENAME='%s'\nFILETYPE=%i\n",filename,filetype);
			printf("TITLE='%s'\nARTIST='%s'\nALBUM='%s'\nTRACK=%s\nTOTALTRACKS=%s\n",title,artist,album,trackstring,totaltracksstring);
			printf("CD=%s\nGENRE='%s'\nYEAR='%s'\nCOMPILATION=%s\n",cdstring,genre,year,compilationstring);
			printf("COMPOSER='%s'\nCOMMENT='%s'\n",composer,comment);

		}
	else
		{
			printf("FILENAME=%s\nFILETYPE=%i\n",filename,filetype);
			printf("TITLE=%s\nARTIST=%s\nALBUM=%s\nTRACK=%s\nTOTALTRACKS=%s\n",title,artist,album,trackstring,totaltracksstring);
			printf("CD=%s\nGENRE=%s\nYEAR=%s\nCOMPILATION=%s\n",cdstring,genre,year,compilationstring);
			printf("COMPOSER=%s\nCOMMENT=%s\n",composer,comment);
		}
}

void freeAndNull(char **data)
{
	if((data!=NULL) && (*data!=NULL))
		{
			free(*data);
			*data=NULL;
		}
}

