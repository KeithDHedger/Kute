/*
 *
 * ©K. D. Hedger. Tue 16 Jun 12:47:21 BST 2020 keithdhedger@gmail.com

 * This file (mp3title.cpp) is part of Kute.

 * Kute is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * Kute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Kute.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "globals.h"
#include "mp3tags.h"
#include "config.h"

using namespace TagLib;

char		*artist=NULL;
int			mp3tmpfile;
id3_file	*mp3file;
id3_tag		*tag;
id3_frame	*frame;
id3_field	*field;
char		*latinstr=(char*)malloc(10000);
int			strcnt;
unsigned	field_type;

void getMp3Tag(const char *tagname,char **tagdata)
{
	if((frame=id3_tag_findframe(tag,tagname,0)))
		{
			for (int i=0; (field=id3_frame_field(frame,i)); i++)
				{
					field_type=id3_field_type(field);
					if(field_type==ID3_FIELD_TYPE_STRINGLIST)
						{
							strcnt=id3_field_getnstrings(field);
							for (int j=0; j < strcnt; j++)
								{
									latinstr=(char*)id3_ucs4_utf8duplicate(id3_field_getstrings(field,j));
									(*tagdata)=(char*)malloc(strlen(latinstr)+1);
									strcpy((*tagdata),latinstr);
								}

						}
				}
		}
}

bool readMp3Title(const char *filename)
{
	if((mp3tmpfile=open(filename,O_RDONLY)) < 0)
		{
			fprintf(stderr,"ERROR Could not open %s for reading!\n",filename);
			return false;
		}

	if((mp3file=id3_file_fdopen(mp3tmpfile,ID3_FILE_MODE_READONLY))==NULL)
		{
			fprintf(stderr,"ERROR Could not open %s for reading as a MP3 file!\n",filename);
			close(mp3tmpfile);
			return false;
		}

	if(((tag=id3_file_tag(mp3file))==NULL) || (tag->nframes==0))
		{
			fprintf(stderr,"No readable tags in file %s\n",filename);
			id3_file_close(mp3file);
			return false;
		}

	getMp3Tag(ID3_FRAME_ARTIST,&artist);

	id3_file_close(mp3file);
	return true;
}

int main(int argc,char **argv)
{
	if(!readMp3Title(argv[1]))
		return(1);
	
	printf("%s",artist);
	free(artist);
	return(0);
}