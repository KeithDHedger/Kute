/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:34:08 GMT 2018 keithdhedger@gmail.com

 * This file (mp3tags.cpp) is part of Kute.

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

#include <id3tag.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#include "globals.h"
#include "mp3tags.h"
using namespace TagLib;

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
					if(field_type==ID3_FIELD_TYPE_STRINGFULL)
						{
							latinstr=(char*)id3_ucs4_utf8duplicate(id3_field_getfullstring(field));
							(*tagdata)=(char*)malloc(strlen(latinstr)+1);
							strcpy((*tagdata),latinstr);
						}

				}
		}
	else
		{
			(*tagdata)=(char*)calloc(1,1);
		}
}

bool readAllMp3(bool nochange)
{
	int					genre_code;
	const id3_ucs4_t	*tnam;
	char				*token;

	if((mp3tmpfile=open(filename,O_RDONLY)) < 0)
		{
			printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
			return FALSE;
		}

	if((mp3file=id3_file_fdopen(mp3tmpfile,ID3_FILE_MODE_READONLY))==NULL)
		{
			printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
			close(mp3tmpfile);
			return FALSE;
		}

	if(((tag=id3_file_tag(mp3file))==NULL) || (tag->nframes==0))
		{
			printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
			id3_file_close(mp3file);
			return FALSE;
		}

	getMp3Tag(ID3_FRAME_TITLE,&title);
	getMp3Tag(ID3_FRAME_ARTIST,&artist);
	getMp3Tag(ID3_FRAME_ALBUM,&album);
	getMp3Tag(ID3_FRAME_TRACK,&trackstring);

	if(trackstring==NULL || strlen(trackstring)==0)
		{
			trackstring=(char*)"";
			totaltracksstring=(char*)"";
		}
	else
		{
			if(strchr(trackstring,'/'))
				{
					token=strtok(trackstring,"/");
					token=strtok(NULL,"/");
					totaltracksstring=(char*)malloc(strlen(token)+1);
					strcpy(totaltracksstring,token);
				}
			else
				totaltracksstring=(char*)"";
		}

	getMp3Tag("TPOS",&cdstring);
	getMp3Tag("TCMP",&compilationstring);
	getMp3Tag(ID3_FRAME_YEAR,&year);
	getMp3Tag(ID3_FRAME_GENRE,&genre);

	genre_code=atoi(genre);
	if(genre_code>-1)
		{
			tnam=id3_genre_index(genre_code);
			id3_latin1_t * data=id3_ucs4_latin1duplicate(tnam);
			genre=(char*)realloc(genre,strlen((const char*)data));
			strcpy(genre,(char*)data);
		}
	else
		genre=(char*)"";

	getMp3Tag("TCOM",&composer);
	getMp3Tag(ID3_FRAME_COMMENT,&comment);

	id3_file_close(mp3file);
	tag=NULL;

	return true;
}

void setMp3Tag(const char *tagname,enum id3_field_type type,char *tagdata)
{
	int nstrings;

	frame=NULL;
	while((frame=id3_tag_findframe(tag,tagname,0))!=NULL)
		{
			id3_tag_detachframe(tag,frame);
			id3_file_update(mp3file);
		}

	if((frame=id3_tag_findframe(tag,tagname,0))==NULL)
		{
			if((frame=id3_frame_new(tagname))==NULL)
				{
					printf("cant make new frame\n");
					return;
				}
		}

	id3_tag_attachframe(tag,frame);

	for (int i=0; (field=id3_frame_field(frame,i)); i++)
		{
			id3_ucs4_t    *ucs4;

			switch (type)
				{
				case ID3_FIELD_TYPE_STRINGLIST:
					ucs4=id3_utf8_ucs4duplicate((const id3_utf8_t*)tagdata);
					id3_field_setstrings (field,1,&ucs4);
					free (ucs4);
					break;

				case ID3_FIELD_TYPE_STRINGFULL:
					ucs4=id3_utf8_ucs4duplicate((const id3_utf8_t*)tagdata);
					id3_field_setfullstring(field,ucs4);
					free (ucs4);
					break;
				}
		}
}

static unsigned char kuteData[]=
{
	0x49,0x44,0x33,0x04,0x00,0x00,0x00,0x00,0x07,0x76,0x54,0x45,0x4e,0x43,0x00,0x00,
	0x00,0x05,0x20,0x00,0x00,0x6b,0x75,0x74,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0
};

void setMp3Tags(void)
{
	if((mp3file=id3_file_open(filename,ID3_FILE_MODE_READWRITE))==NULL)
		{
			printf("eror here\n");
			return;
		}

	if((tag=id3_file_tag(mp3file))==NULL)
		{
			id3_file_close(mp3file);
			printf("eror there\n");
			return;
		}

	if(tag->nframes==0 || tag->paddedsize<48)
		{
			id3_file_close(mp3file);
			int fp;
			FILE *df;
			int c;

			fp=open(filename,O_RDWR);
			lseek(fp,-48,SEEK_SET);
			write(fp,&kuteData[0],48);
			close(fp);

			if((mp3file=id3_file_open(filename,ID3_FILE_MODE_READWRITE))==NULL)
				{
					printf("eror here\n");
					return;
				}

			if((tag=id3_file_tag(mp3file))==NULL)
				{
					id3_file_close(mp3file);
					printf("eror there\n");
					return;
				}
		}

	if(tagstoset[SETTITLE]==1)
		setMp3Tag(ID3_FRAME_TITLE,ID3_FIELD_TYPE_STRINGLIST,title);
	if(tagstoset[SETARTIST]==1)
		setMp3Tag(ID3_FRAME_ARTIST,ID3_FIELD_TYPE_STRINGLIST,artist);
	if(tagstoset[SETALBUM]==1)
		setMp3Tag(ID3_FRAME_ALBUM,ID3_FIELD_TYPE_STRINGLIST,album);
	if(tagstoset[SETYEAR]==1)
		setMp3Tag(ID3_FRAME_YEAR,ID3_FIELD_TYPE_STRINGLIST,year);

	if(tagstoset[SETGENRE]==1)
		{
			int num=id3_genre_number(id3_utf8_ucs4duplicate((const id3_utf8_t*)genre));
			sprintf(tempbuffer,"%i",num);
			setMp3Tag(ID3_FRAME_GENRE,ID3_FIELD_TYPE_STRINGLIST,tempbuffer);
		}
	if(tagstoset[SETCOMPOSER]==1)
		setMp3Tag("TCOM",ID3_FIELD_TYPE_STRINGLIST,composer);
	if(tagstoset[SETCOMMENT]==1)
		setMp3Tag(ID3_FRAME_COMMENT,ID3_FIELD_TYPE_STRINGFULL,comment);
	if(tagstoset[SETCD]==1)
		setMp3Tag("TPOS",ID3_FIELD_TYPE_STRINGLIST,cdstring);

	getMp3Tag(ID3_FRAME_TRACK,&tempbuffer);

	char	*ttrack=(char*)calloc(100,1);
	char	*ttot=(char*)calloc(100,1);
	if(strchr(tempbuffer,'/'))
		{
			sprintf(ttrack,"%s",strtok(tempbuffer,"/"));
			sprintf(ttot,"/%s",strtok(NULL,"/"));
		}
	else
		{
			ttot[0]=0;
			strcpy(ttrack,tempbuffer);
		}
	if(tagstoset[SETTOTALTRACKS]==1)
		{
			if(totaltracksstring!=NULL && strlen(totaltracksstring)>0)
				sprintf(ttot,"/%s",totaltracksstring);
			else
				sprintf(ttot,"");
		}
	if(tagstoset[SETTRACK]==1)
		sprintf(ttrack,"%s",trackstring);

	if(tagstoset[SETTOTALTRACKS]==1 || tagstoset[SETTRACK]==1)
		{
			if(ttrack!=NULL && strlen(ttrack)>0)
				sprintf(tempbuffer,"%s%s",ttrack,ttot);
			else
				sprintf(tempbuffer,"");

			setMp3Tag(ID3_FRAME_TRACK,ID3_FIELD_TYPE_STRINGLIST,tempbuffer);
		}

	if(tagstoset[SETCOMPILATION]==1)
		setMp3Tag("TCMP",ID3_FIELD_TYPE_STRINGLIST,compilationstring);

	int ret=id3_file_update(mp3file);

	id3_file_close(mp3file);

//copy to v1 tags
//note not all v2 tags can be copied.
	MPEG::File f(filename);
	int flags=MPEG::File::AllTags;

	ID3v1::Tag *id3v1tag = f.ID3v1Tag(true);

	id3v1tag->setTitle(f.tag()->title());
	id3v1tag->setArtist(f.tag()->artist());
	id3v1tag->setAlbum(f.tag()->album());
	id3v1tag->setComment(f.tag()->comment());
	id3v1tag->setGenre(f.tag()->genre());
	id3v1tag->setYear(f.tag()->year());
	id3v1tag->setTrack(f.tag()->track());
	f.save();

	if(ret)
		printf("Could not write id3 tag to track.\n");
}




