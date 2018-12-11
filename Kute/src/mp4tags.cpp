//
//Keith Hedger
//Mon Jun 29 11:36:15 BST 2009
//
//mp4tags.cpp
//

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

void set_mp4_tag(void)
{
	MP4FileHandle	mp4file;
	const MP4Tags	*tags;

	mp4file = MP4Modify(filename,0);
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

	if (tagstoset[SETTRACK]==1 || tagstoset[SETTOTALTRACKS]==1)
		{
			MP4TagTrack	trck={0,0};
			if(tagstoset[SETTRACK]==1)
				trck.index=strtol(trackstring,NULL,10);
			if (tagstoset[SETTOTALTRACKS]==1)
				trck.total=strtol(totaltracksstring,NULL, 10);
			MP4TagsSetTrack(tags,&trck);
		}

	if(tagstoset[SETCD]==1 || tagstoset[SETTOTALCDS]==1)
		{
			long t=0;
			MP4TagDisk	dsk={0,0};
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

bool do_read_all(void)
{
	MP4FileHandle	mp4file=NULL;
	const MP4Tags	*tags;

	mp4file=MP4Read(filename);

	if (mp4file==MP4_INVALID_FILE_HANDLE)
		{
			printf("ERROR Could not open %s for reading as a mp4 file!\n",filename);
			return false;
		}

	tags=MP4TagsAlloc();
	MP4TagsFetch(tags,mp4file);

	title=(char*)tags->name;
	if(title==NULL)
		title=(char*)"";

	artist=(char*)tags->artist;
	if(artist==NULL)
		artist=(char*)"";
	
	album=(char*)tags->album;
	if(album==NULL)
		album=(char*)"";

	if(tags->track)
		{
			asprintf(&trackstring,"%i",tags->track->index);
			asprintf(&totaltracksstring,"%i",tags->track->total);
		}
	else
		{
			trackstring=(char*)"";
			totaltracksstring=(char*)"";
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
			cdstring=(char*)"";
		}

	genre=(char*)tags->genre;
	if(genre==NULL)
		genre=(char*)"";

	year=(char*)tags->releaseDate;
	if(year==NULL)
		year=(char*)"";
	
	if(tags->compilation)
		asprintf(&compilationstring,"%i",*tags->compilation);
	else
		compilationstring=(char*)"";

	composer=(char*)tags->composer;
	if(composer==NULL)
		composer=(char*)"";

	comment=(char*)tags->comments;
	if(comment==NULL)
		comment=(char*)"";
	
	if(!tags)
		MP4TagsFree(tags);
	if (!mp4file)
		MP4Close(mp4file);

	return true;
}

