//
//Keith Hedger
//Mon Jun 29 11:36:15 BST 2009
//
//mp4tags.cpp
//

#include <mp4.h>

#include "globals.h"
#include "mp4tags.h"

u_int16_t	track=0;
u_int16_t	totaltracks=0;
u_int16_t	cd=0;
u_int16_t	totalcds=0;
u_int8_t	compilation=0;
char		*tbuff=(char*)malloc(100);

void set_tag(void)
{
	MP4FileHandle	filehandle = NULL;
	filehandle = MP4Modify(filename,0,0);
	if (filehandle == MP4_INVALID_FILE_HANDLE)
	{
		printf("Could not open %s for Writing!\n",filename);
		exit(2);
	}

	if (tagstoset[SETTITLE]==1)
		MP4SetMetadataName(filehandle,title);
	if (tagstoset[SETALBUM]==1)
		MP4SetMetadataAlbum(filehandle,album);
	if (tagstoset[SETARTIST]==1)
		MP4SetMetadataArtist(filehandle,artist);
	if (tagstoset[SETTRACK]==1 || tagstoset[SETTOTALTRACKS]==1)
		{
		MP4GetMetadataTrack(filehandle,&track,&totaltracks);
		if (tagstoset[SETTRACK]==1)
			track=strtol(trackstring,NULL, 10);
		if (tagstoset[SETTOTALTRACKS]==1)
			totaltracks=strtol(totaltracksstring,NULL, 10);
		MP4SetMetadataTrack(filehandle,track,totaltracks);
		}
	if (tagstoset[SETCD]==1 || tagstoset[SETTOTALCDS]==1)
		MP4SetMetadataDisk(filehandle,cd,totalcds);
	if (tagstoset[SETCOMPILATION]==1)
		MP4SetMetadataCompilation(filehandle,compilation);
	if (tagstoset[SETYEAR]==1)
		MP4SetMetadataYear(filehandle,year);
	if (tagstoset[SETGENRE]==1)
		MP4SetMetadataGenre(filehandle,genre);
	if (tagstoset[SETCOMPOSER]==1)
		MP4SetMetadataWriter(filehandle,composer);
	if (tagstoset[SETCOMMENT]==1)
		MP4SetMetadataComment(filehandle,comment);

	MP4Close(filehandle);
}

bool do_read_all(void)
{
	MP4FileHandle	filehandle = NULL;

	filehandle = MP4Read(filename, 0);

	if (filehandle == MP4_INVALID_FILE_HANDLE)
	{
		printf("ERROR Could not open %s for reading as a mp4 file!\n",filename);
		return false;
	}

	MP4GetMetadataName(filehandle,&title);
	if (title==NULL)
		title=(char*)"";
	MP4GetMetadataArtist(filehandle,&artist);
	if (artist==NULL)
		artist=(char*)"";
	MP4GetMetadataAlbum(filehandle,&album);
	if (album==NULL)
		album=(char*)"";
	MP4GetMetadataTrack(filehandle,&track,&totaltracks);
	sprintf(tbuff,"%i",track);
	trackstring=(char*)malloc(strlen(tbuff)+1);
	sprintf(trackstring,"%i",track);
	sprintf(tbuff,"%i",totaltracks);
	totaltracksstring=(char*)malloc(strlen(tbuff)+1);
	sprintf(totaltracksstring,"%i",totaltracks);

	MP4GetMetadataDisk(filehandle,&cd,&totalcds);
	sprintf(tbuff,"%i",cd);
	cdstring=(char*)malloc(strlen(tbuff)+1);
	sprintf(cdstring,"%i",cd);

	MP4GetMetadataGenre(filehandle,&genre);
	if (genre==NULL)
		genre=(char*)"";
	MP4GetMetadataYear(filehandle,&year);
	if (year==NULL)
		year=(char*)"";
	MP4GetMetadataCompilation(filehandle,&compilation);
	sprintf(tbuff,"%i",compilation);
	compilationstring=(char*)malloc(strlen(tbuff)+1);
	sprintf(compilationstring,"%i",compilation);

	MP4GetMetadataWriter(filehandle,&composer);
	if (composer==NULL)
		composer=(char*)"";
	MP4GetMetadataComment(filehandle,&comment);
	if (comment==NULL)
		comment=(char*)"";
	
	if (!filehandle)
		MP4Close(filehandle);
	return true;
}

