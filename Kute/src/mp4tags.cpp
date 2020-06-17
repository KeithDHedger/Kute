/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:34:22 GMT 2018 keithdhedger@gmail.com

 * This file (mp4tags.cpp) is part of Kute.

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
#include "mp4tags.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

u_int16_t	track=0;
u_int16_t	totaltracks=0;
u_int16_t	cd=0;
u_int16_t	totalcds=0;
u_int8_t	compilation=0;
char		*tbuff=(char*)malloc(100);

void setMp4Tag(void)
{
	MP4FileHandle	mp4file;
	const MP4Tags	*tags;

	mp4file=MP4Modify(filename,0);
	if(mp4file==MP4_INVALID_FILE_HANDLE)
		{
			printf("Could not open %s for Writing!\n",filename);
			exit(2);
		}

	tags=MP4TagsAlloc();
	MP4TagsFetch(tags,mp4file);

	if(tagstoset[SETTITLE]==1)
		MP4TagsSetName(tags,title);

	if(tagstoset[SETALBUM]==1)
		MP4TagsSetAlbum(tags,album);

	if(tagstoset[SETARTIST]==1)
		MP4TagsSetArtist(tags,artist);

	if(tagstoset[SETTRACK]==1 || tagstoset[SETTOTALTRACKS]==1)
		{
			MP4TagTrack	trck= {0,0};
			if(tagstoset[SETTRACK]==1)
				trck.index=strtol(trackstring,NULL,10);
			if(tagstoset[SETTOTALTRACKS]==1)
				trck.total=strtol(totaltracksstring,NULL,10);
			MP4TagsSetTrack(tags,&trck);
		}

	if(tagstoset[SETCD]==1 || tagstoset[SETTOTALCDS]==1)
		{
			long t=0;
			MP4TagDisk	dsk= {0,0};
			if(tagstoset[SETCD]==1)
				dsk.index=atoi(cdstring);

			if(tagstoset[SETTOTALCDS]==1)
				{
					char* ptr=strrchr(cdstring,'/');
					if(ptr!=NULL)
						t=atoi(++ptr);
				}
			dsk.total=t;

			if((dsk.index==0) && (dsk.total==0))
				MP4TagsSetDisk(tags,NULL);
			else
				MP4TagsSetDisk(tags,&dsk);
		}

	if(tagstoset[SETCOMPILATION]==1)
		{
			uint8_t comp;
			comp=strtol(compilationstring,NULL,10);
			MP4TagsSetCompilation(tags,&comp);
		}

	if(tagstoset[SETYEAR]==1)
		MP4TagsSetReleaseDate(tags,year);

	if(tagstoset[SETGENRE]==1)
		MP4TagsSetGenre(tags,genre);

	if(tagstoset[SETCOMPOSER]==1)
		MP4TagsSetComposer(tags,composer);

	if(tagstoset[SETCOMMENT]==1)
		MP4TagsSetComments(tags,comment);

	MP4TagsStore(tags,mp4file);
	MP4TagsFree(tags);
	MP4Close(mp4file);
	MP4Optimize(filename,NULL);
}

bool doReadAllM4aTags(void)
{
	MP4FileHandle	mp4file=NULL;
	const MP4Tags	*tags;
	const char		*retstr=NULL;

	mp4file=MP4Read(filename);

	if(mp4file==MP4_INVALID_FILE_HANDLE)
		{
			printf("ERROR Could not open %s for reading as a mp4 file!\n",filename);
			return false;
		}

	tags=MP4TagsAlloc();
	MP4TagsFetch(tags,mp4file);

	retstr=tags->name;
	if(retstr==NULL)
		title=strdup("");
	else
		title=strdup(retstr);

	retstr=tags->artist;
	if(retstr==NULL)
		artist=strdup("");
	else
		artist=strdup(retstr);

	retstr=tags->album;
	if(retstr==NULL)
		album=strdup("");
	else
		album=strdup(retstr);

	if(tags->track)
		{
			asprintf(&trackstring,"%i",tags->track->index);
			asprintf(&totaltracksstring,"%i",tags->track->total);
		}
	else
		{
			trackstring=strdup("");
			totaltracksstring=strdup("");
		}

	cd=0;
	totalcds=0;
	if(tags->disk)
		{
			if(tags->disk->total>tags->disk->index)
				asprintf(&cdstring,"%i/%i",tags->disk->index,tags->disk->total);
			else
				asprintf(&cdstring,"%i",tags->disk->index);
			cd=tags->disk->index;
			totalcds=tags->disk->total;
		}
	else
		{
			cdstring=strdup("");
		}

	retstr=tags->genre;
	if(retstr==NULL)
		genre=strdup("");
	else
		genre=strdup(retstr);

	retstr=tags->releaseDate;
	if(retstr==NULL)
		year=strdup("");
	else
		year=strdup(retstr);

	retstr=tags->composer;
	if(retstr==NULL)
		composer=strdup("");
	else
		composer=strdup(retstr);

	retstr=tags->comments;
	if(retstr==NULL)
		comment=strdup("");
	else
		comment=strdup(retstr);

	if(tags->compilation)
		asprintf(&compilationstring,"%i",*tags->compilation);
	else
		compilationstring=strdup("");

	if(!tags)
		MP4TagsFree(tags);
	if(!mp4file)
		MP4Close(mp4file);

	return true;
}

