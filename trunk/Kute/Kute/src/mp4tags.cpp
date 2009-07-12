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
		{
		if (title==NULL)
			MP4DeleteMetadataName(filehandle);
		else
			MP4SetMetadataName(filehandle,title);
		}
	if (tagstoset[SETALBUM]==1)
		{
		if (album==NULL)
			MP4DeleteMetadataAlbum(filehandle);
		else
			MP4SetMetadataAlbum(filehandle,album);
		}
	if (tagstoset[SETARTIST]==1)
		{
		if (artist==NULL)
			MP4DeleteMetadataArtist(filehandle);
		else
			MP4SetMetadataArtist(filehandle,artist);
		}
	if (tagstoset[SETTRACK]==1 || tagstoset[SETTOTALTRACKS]==1)
		{
		if (trackstring==NULL)
			MP4DeleteMetadataTrack(filehandle);
		else
			{
			MP4GetMetadataTrack(filehandle,&track,&totaltracks);
			if (tagstoset[SETTRACK]==1)
				track=strtol(trackstring,NULL, 10);
			if (tagstoset[SETTOTALTRACKS]==1)
				totaltracks=strtol(totaltracksstring,NULL, 10);
			MP4SetMetadataTrack(filehandle,track,totaltracks);
			}
		}
	if (tagstoset[SETCD]==1 || tagstoset[SETTOTALCDS]==1)
		{
		if (cdstring==NULL||strlen(cdstring)==0)
			{
			MP4DeleteMetadataDisk(filehandle);
			}
		else
			{
			totalcds=cd=strtol(cdstring,NULL, 10);
			MP4SetMetadataDisk(filehandle,cd,totalcds);
			}
		}
	if (tagstoset[SETCOMPILATION]==1)
		{
		if (compilationstring==NULL)
			MP4DeleteMetadataCompilation(filehandle);
		else
			MP4SetMetadataCompilation(filehandle,compilation);
		}
	if (tagstoset[SETYEAR]==1)
		{
		if (year==NULL)
			MP4DeleteMetadataYear(filehandle);
		else
			MP4SetMetadataYear(filehandle,year);
		}
	if (tagstoset[SETGENRE]==1)
		{
		if (genre==NULL)
			MP4DeleteMetadataGenre(filehandle);
		else
			MP4SetMetadataGenre(filehandle,genre);
		}
	if (tagstoset[SETCOMPOSER]==1)
		{
		if (composer==NULL)
			MP4DeleteMetadataWriter(filehandle);
		else
			MP4SetMetadataWriter(filehandle,composer);
		}
	if (tagstoset[SETCOMMENT]==1)
		{
		if (comment==NULL)
			MP4DeleteMetadataComment(filehandle);
		else
			MP4SetMetadataComment(filehandle,comment);
		}

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
	if (track>0)
		{	
		sprintf(tbuff,"%i",track);
		trackstring=(char*)malloc(strlen(tbuff)+1);
		sprintf(trackstring,"%i",track);
	
		sprintf(tbuff,"%i",totaltracks);
		totaltracksstring=(char*)malloc(strlen(tbuff)+1);
		sprintf(totaltracksstring,"%i",totaltracks);
		}
	else
		{
		trackstring=(char*)calloc(1,1);
		totaltracksstring=(char*)calloc(1,1);
		}


	MP4GetMetadataDisk(filehandle,&cd,&totalcds);
		if (compilation>0)
			{	
			sprintf(tbuff,"%i",cd);
			cdstring=(char*)malloc(strlen(tbuff)+1);
			sprintf(cdstring,"%i",cd);
			}
		else
			cdstring=(char*)calloc(1,1);

	MP4GetMetadataGenre(filehandle,&genre);
	if (genre==NULL)
		genre=(char*)"";
	MP4GetMetadataYear(filehandle,&year);
	if (year==NULL)
		year=(char*)"";
	MP4GetMetadataCompilation(filehandle,&compilation);
		if (compilation>0)
			{	
			sprintf(tbuff,"%i",compilation);
			compilationstring=(char*)malloc(strlen(tbuff)+1);
			sprintf(compilationstring,"%i",compilation);
			}
		else
			compilationstring=(char*)calloc(1,1);

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

