//
//Keith Hedger
//Wed Jul 1 18:41:42 BST 2009
//
//mp3tags.cpp
//

#include <id3tag.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif

# ifdef HAVE_ASSERT_H
#  include <assert.h>
# endif

#include "globals.h"
#include "mp3tags.h"

int mp3tmpfile;
id3_file *mp3file;
id3_tag *tag;
id3_frame *frame;
id3_field *field;
char *latinstr=(char*)malloc(10000);
int  strcnt;
unsigned field_type;

void Id3tag_Genre_To_String (void)
{
	int genre_code=atoi(genre);

	if (genre_code>=ID3_INVALID_GENRE)
		{
		genre=(char*)realloc(genre,1);
		genre[0]=0;
		}
	else if (genre_code>GENRE_MAX)
		{
		genre=(char*)realloc(genre,8);
		strcpy(genre,"Unknown");
		}
	else
		{
		genre=(char*)realloc(genre,strlen(id3_genres[genre_code]+1));
		strcpy(genre,id3_genres[genre_code]);
		}
}

void get_mp3_tag(const char *tagname,char **tagdata)
{
	if ( (frame = id3_tag_findframe(tag,tagname, 0)) )
		{
		for (int i = 0; (field = id3_frame_field(frame, i)); i++)
			{
			field_type = id3_field_type(field);
			if (field_type==ID3_FIELD_TYPE_STRINGLIST)
				{
				strcnt = id3_field_getnstrings(field);
				for (int j = 0; j < strcnt; j++)
					{
					latinstr=(char*)id3_ucs4_utf8duplicate(id3_field_getstrings(field,j));
					(*tagdata)=(char*)malloc(strlen(latinstr)+1);
					strcpy((*tagdata),latinstr);
					}
 
				}
				if (field_type==ID3_FIELD_TYPE_STRINGFULL)
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

bool read_all_mp3(void)
{

	if ( (mp3tmpfile=open(filename,O_RDONLY)) < 0 )
		{
		printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
		return FALSE;
		}

	if ((mp3file = id3_file_fdopen(mp3tmpfile,ID3_FILE_MODE_READONLY)) == NULL)
		{
		printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
		close(mp3tmpfile);
		return FALSE;
		}
	
	if ( ((tag = id3_file_tag(mp3file)) == NULL) || (tag->nframes == 0))
		{
		printf("filetype=%i\n",filetype);
		printf("ERROR Could not open %s for reading as a MP3 file!\n",filename);
		id3_file_close(mp3file);
		return FALSE;
		}

	char *token;
	get_mp3_tag(ID3_FRAME_TITLE,&title);
	get_mp3_tag(ID3_FRAME_ALBUM,&album);
	get_mp3_tag(ID3_FRAME_ARTIST,&artist);
	get_mp3_tag(ID3_FRAME_TRACK,&trackstring);
	printf("t=%s tt=%s\n",trackstring,totaltracksstring);
	if(trackstring==NULL || strlen(trackstring)==0)
		{
		trackstring=(char*)"";
		totaltracksstring=(char*)"";
		}
	else
		{
		if(strchr(trackstring,'/'))
			{
			token=strtok(trackstring, "/");
			token=strtok(NULL, "/");
			totaltracksstring=(char*)malloc(strlen(token)+1);
			strcpy(totaltracksstring,token);
			}
		else
			{
			totaltracksstring=(char*)"";
			}
		}
	get_mp3_tag("TPOS",&cdstring);
	if(strchr(cdstring,'/'))
		{
		token=strtok(cdstring, "/");
		token=strtok(NULL, "/");
		cdstring=(char*)malloc(strlen(token)+1);
		strcpy(cdstring,token);
		}

	get_mp3_tag("TCMP",&compilationstring);
	get_mp3_tag(ID3_FRAME_YEAR,&year);
	get_mp3_tag(ID3_FRAME_GENRE,&genre);
	Id3tag_Genre_To_String();
	get_mp3_tag("TCOM",&composer);
	get_mp3_tag(ID3_FRAME_COMMENT,&comment);

	id3_file_close(mp3file);
	
	return true;

}

void set_mp3_tag(const char *tagname,enum id3_field_type type,char *tagdata)
{
	int nstrings;

	if (tagdata==NULL || strlen(tagdata)==0)
		{
		if ((frame = id3_tag_findframe(tag,tagname, 0)) != NULL)
			{
		//printf("XXXXXXXXXX c=%s len=%i\n",tagdata,strlen(tagdata));
			id3_tag_detachframe(tag,frame);
			id3_file_update(mp3file);
			return;
			}
		else
			return;
		}

	if ((frame = id3_tag_findframe(tag,tagname, 0)) == NULL)
		{
		if ((frame = id3_frame_new(tagname)) == NULL)
			{
			printf("cant make new frame\n");
			return;
			}
		}

	id3_tag_attachframe(tag,frame);


	for (int i = 0; (field = id3_frame_field(frame, i)); i++)
		{
		id3_ucs4_t    *ucs4;

        	switch (type)
			{
			case ID3_FIELD_TYPE_STRINGLIST:
				ucs4 = id3_utf8_ucs4duplicate((const id3_utf8_t*)tagdata);
				id3_field_setstrings (field, 1, &ucs4);
				free (ucs4);
				break;
				
			case ID3_FIELD_TYPE_STRINGFULL:
				ucs4 = id3_utf8_ucs4duplicate((const id3_utf8_t*)tagdata);
				id3_field_setfullstring(field,ucs4);
				free (ucs4);
				break;
			}
		}
}

#if 0
typedef struct id3_file {
  FILE *iofile;
  enum id3_file_mode mode;
  char *path;

  int flags;
fpos_t id3v2End;        // MDM

  struct id3_tag *primary;
   unsigned int ntags;
  struct filetag *tags;
}fileptr;
#endif

/*
00000000  49 44 33 04 00 00 00 00  07 76 54 45 4e 43 00 00  |ID3......vTENC..|
00000010  00 05 20 00 00 6b 75 74  65 00 00 00 00 00 00 00  |.. ..kute.......|
00000020  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|

*/
static unsigned char kute_data[] = {
0x49,0x44,0x33,0x04,0x00,0x00,0x00,0x00,0x07,0x76,0x54,0x45,0x4e,0x43,0x00,0x00,
0x00,0x05,0x20,0x00,0x00,0x6b,0x75,0x74,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0
};

void set_mp3_tags(void)
{

	if ((mp3file = id3_file_open(filename, ID3_FILE_MODE_READWRITE)) == NULL)
		{
		printf("eror here\n");
		return;
		}

	if ((tag = id3_file_tag(mp3file)) == NULL)
		{
		id3_file_close(mp3file);
		printf("eror there\n");
		return;
		}

	if ( tag->nframes == 0 || tag->paddedsize<48)
		{
		id3_file_close(mp3file);
		int fp;
		FILE *df;
		int c;
		
		fp = open(filename,O_RDWR);
		lseek(fp,-48,SEEK_SET);
		write(fp,&kute_data[0],48);
		close(fp);

		if ((mp3file = id3_file_open(filename, ID3_FILE_MODE_READWRITE)) == NULL)
			{
			printf("eror here\n");
			return;
			}

		if ((tag = id3_file_tag(mp3file)) == NULL)
			{
			id3_file_close(mp3file);
			printf("eror there\n");
			return;
			}
		}

	if (tagstoset[SETTITLE]==1)
		set_mp3_tag(ID3_FRAME_TITLE,ID3_FIELD_TYPE_STRINGLIST,title);
	if (tagstoset[SETARTIST]==1)
		set_mp3_tag(ID3_FRAME_ARTIST,ID3_FIELD_TYPE_STRINGLIST,artist);
	if (tagstoset[SETALBUM]==1)
		set_mp3_tag(ID3_FRAME_ALBUM,ID3_FIELD_TYPE_STRINGLIST,album);
	if (tagstoset[SETYEAR]==1)
		set_mp3_tag(ID3_FRAME_YEAR,ID3_FIELD_TYPE_STRINGLIST,year);
	
	if (tagstoset[SETGENRE]==1)
		{
		int num=id3_genre_number(id3_utf8_ucs4duplicate((const id3_utf8_t*)genre));
		sprintf(gbuff,"%i",num);
		set_mp3_tag(ID3_FRAME_GENRE,ID3_FIELD_TYPE_STRINGLIST,gbuff);
		}
	if (tagstoset[SETCOMPOSER]==1)
		set_mp3_tag("TCOM",ID3_FIELD_TYPE_STRINGLIST,composer);
	if (tagstoset[SETCOMMENT]==1)
		set_mp3_tag(ID3_FRAME_COMMENT,ID3_FIELD_TYPE_STRINGFULL,comment);
	if (tagstoset[SETCD]==1 || tagstoset[SETTOTALCDS]==1)
		set_mp3_tag("TPOS",ID3_FIELD_TYPE_STRINGLIST,cdstring);

	if (tagstoset[SETTRACK]==1 || tagstoset[SETTOTALTRACKS]==1)
		{
		//char * tbuff=(char*)calloc(100,1);
		for (int j=0;j<2048;j++)
			tempbuffer[j]=0;
		get_mp3_tag(ID3_FRAME_TRACK,&tempbuffer);
		if (tagstoset[SETTRACK]==1)
			sprintf(tempbuffer,"%s",trackstring);
		if (tagstoset[SETTOTALTRACKS]==1 && (totaltracksstring!=NULL && strlen(totaltracksstring)>0))
			{
			strcat(tempbuffer,"/");
			strcat(tempbuffer,totaltracksstring);
			}
		
		if (tagstoset[SETTRACK]==1 && (trackstring==NULL || strlen(trackstring)==0))
			{
			tempbuffer[0]=0;
			}

		printf("t=%s tt=%s tbuff=%s\n",trackstring,totaltracksstring,tempbuffer);
		set_mp3_tag(ID3_FRAME_TRACK,ID3_FIELD_TYPE_STRINGLIST,tempbuffer);
		}

	if (tagstoset[SETCOMPILATION]==1)
		{
		printf("XXXXXXXXXX c=%s len=%i\n",compilationstring,strlen(compilationstring));
		set_mp3_tag("TCMP",ID3_FIELD_TYPE_STRINGLIST,compilationstring);
		}


    int ret = id3_file_update(mp3file);

    id3_file_close(mp3file);

    if (ret)
        printf("Could not write id3 tag to track.\n");
}




