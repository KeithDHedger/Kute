/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:32:48 GMT 2018 keithdhedger@gmail.com

 * This file (main.cpp) is part of Kute.

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <mp4v2/mp4v2.h>
#include <FLAC/metadata.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "globals.h"
#include "mp4tags.h"
#include "flactags.h"
#include "mp3tags.h"
#include "config.h"

struct option long_options[] =
{
	{"read-all",0,0,'r'},
	{"artist",1,0,'a'},
	{"album",1,0,'l'},
	{"title",1,0,'n'},
	{"track",1,0,'t'},
	{"total-tracks",1,0,'T'},
	{"cd",1,0,'C'},
	{"compilation",1,0,'i'},
	{"year",1,0,'y'},
	{"genre",1,0,'g'},
	{"composer",1,0,'m'},
	{"comment",1,0,'o'},
	{"copy",0,0,'Y'},
	{"force-flac",0,0,'f'},
	{"force-aac",0,0,'c'},
	{"force-mp3",0,0,'p'},
	{"use-quotes",0,0,'q'},
	{"remove-all",0,0,'R'},
	{"help",0,0,'?'},
	{0,0,0,0}
};

void printhelp(void)
{
	printf("kute,version %s\n",VERSION);
	printf("Usage: kute [OPTION] FILE [FILE]...\n");
	printf("OPTION			Usage\n\n");
	printf("r,--read-all		Print tags to stdout (default)\n");
	printf("a,--artist=ARG		Set tag ARTIST to ARG\n");
	printf("l,--album=ARG		Set tag ALBUM to ARG\n");
	printf("n,--title=ARG		Set tag TITLE to ARG\n");
	printf("t,--track=ARG		Set tag TRACK to ARG\n");
	printf("T,--total-tracks=ARG	Set tag TOTALTRACKS to ARG\n");
	printf("C,--cd=ARG		Set tag CD to ARG\n");
	printf("i,--compilation=ARG	Set tag COMPILATION to ARG\n");
	printf("			Note this is non standard and ARG should be 1 for part of a compilation or 0 for not\n");
	printf("y,--year=ARG		Set tag YEAR to ARG\n");
	printf("g,--genre=ARG		Set tag GENRE to ARG\n");
	printf("m,--composer=ARG	Set tag COMPOSER to ARG\n");
	printf("o,--comment=ARG		Set tag COMMENT to ARG\n");
	printf("Y,--copy		Copy all tags to [FILE] [FILE] ...\n");
	printf("f,--force-flac		Don't try to guess the file type treat it as a FLAC file \n");
	printf("c,--force-aac		Don't try to guess the file type treat it as an AAC file\n");
	printf("p,--force-mp3		Don't try to guess the file type treat it as an MP3 file\n");
	printf("			The force options may be needed if the file has NO tag data\n");
	printf("q,--use-quotes		Quote file/artist/album names etc\n");
	printf("R,--remove-all		Remove ALL Mp4 tags\n");
	printf("?,--help		Print this help\n");
}

void getFileType(void)
{
	MP4FileHandle	filehandle=NULL;
	int				mp3tmpfile;
	id3_file		*mp3file;
	id3_tag			*tag;
	char			*mp4info=NULL;

	if(strcasestr(filename,".mp3")!=NULL)
		{
			filetype=IS_MP3;
			return;
		}

	if(strcasestr(filename,".flac")!=NULL)
		{
			filetype=IS_FLAC;
			return;
		}

	if(strcasestr(filename,".m4a")!=NULL)
		{
			filetype=IS_MP4;
			return;
		}

	if((mp3tmpfile=open(filename,O_RDONLY)) > 0)
		{
			if((mp3file=id3_file_fdopen(mp3tmpfile,ID3_FILE_MODE_READONLY))!=NULL)
				{
					if(((tag=id3_file_tag(mp3file))!=NULL) && (tag->nframes!=0))
						{
							id3_file_close(mp3file);
							close(mp3tmpfile);
							filetype=IS_MP3;
							return;
						}
				}
		}

	FLAC__Metadata_Chain *chain;
	chain=FLAC__metadata_chain_new();
	if(chain!=NULL && FLAC__metadata_chain_read(chain,filename))
		{
			filetype=IS_FLAC;
			return;
		}

	mp4info=MP4FileInfo(filename,0);
	if(mp4info!=NULL)
		filetype=IS_MP4;
}

int main(int argc,char **argv)
{
	int			c;
	bool		setATag=false;
	bool		tags_ok=false;
	bool		copytofile=false;

	readall=true;
	force=false;
	filetype=UNKNOWN_TYPE;

	while(1)
		{
			int option_index=0;
			c=getopt_long (argc,argv,"a:l:n:t:T:C:i:y:g:m:o:YrfcpqR?h",long_options,&option_index);
			if(c==-1)
				break;

			switch (c)
				{
				case 'r':
					readall=true;
					setATag=false;
					break;

				case 'a':
					setATag=true;
					tagstoset[SETARTIST]=1;
					if(strlen(optarg)>0)
						artist=convertString(optarg);
					else
						{
							tagstoset[SETARTIST]=0;
							artist=strdup("");
						}
					break;

				case 'l':
					setATag=true;
					tagstoset[SETALBUM]=1;
					if(strlen(optarg)>0)
						album=convertString(optarg);
					else
						{
							tagstoset[SETALBUM]=0;
							album=strdup("");
						}
					break;

				case 'n':
					setATag=true;
					tagstoset[SETTITLE]=1;
					if(strlen(optarg)>0)
						title=convertString(optarg);
					else
						{
							tagstoset[SETTITLE]=0;
							title=strdup("");
						}
					break;
				case 't':
					setATag=true;
					tagstoset[SETTRACK]=1;
					if(strlen(optarg)>0)
						trackstring=convertString(optarg);
					else
						{
							tagstoset[SETTRACK]=0;
							trackstring=strdup("");
						}
					break;
				case 'T':
					setATag=true;
					tagstoset[SETTOTALTRACKS]=1;
					if(strlen(optarg)>0)
						totaltracksstring=convertString(optarg);
					else
						{
							tagstoset[SETTOTALTRACKS]=0;
							totaltracksstring=strdup("");
						}
					break;
				case 'C':
					setATag=true;
					tagstoset[SETCD]=1;
					if(strlen(optarg)>0)
						cdstring=convertString(optarg);
					else
						{
							tagstoset[SETCD]=0;
							cdstring=strdup("");
						}
					break;
				case 'i':
					setATag=true;
					tagstoset[SETCOMPILATION]=1;
					if(strlen(optarg)>0)
						compilationstring=convertString(optarg);
					else
						{
							tagstoset[SETCOMPILATION]=0;
							compilationstring=strdup("");
						}
					break;
				case 'y':
					setATag=true;
					tagstoset[SETYEAR]=1;
					if(strlen(optarg)>0)
						year=convertString(optarg);
					else
						{
							tagstoset[SETYEAR]=0;
							year=strdup("");
						}
					break;
				case 'g':
					setATag=true;
					tagstoset[SETGENRE]=1;
					if(strlen(optarg)>0)
						genre=convertString(optarg);
					else
						{
							tagstoset[SETGENRE]=0;
							genre=strdup("");
						}
					break;
				case 'm':
					tagstoset[SETCOMPOSER]=1;
					if(strlen(optarg)>0)
						composer=convertString(optarg);
					else
						{
							tagstoset[SETCOMPOSER]=0;
							composer=strdup("");
						}
					break;
				case 'o':
					tagstoset[SETCOMMENT]=1;
					if(strlen(optarg)>0)
						comment=convertString(optarg);
					else
						{
							tagstoset[SETCOMMENT]=0;
							comment=strdup("");
						}
					break;
				case 'f':
					force=true;
					filetype=IS_FLAC;
					break;
				case 'c':
					force=true;
					filetype=IS_MP4;
					break;
				case 'p':
					force=true;
					filetype=IS_MP3;
					break;
				case 'q':
					quotes=true;
					break;
				case 'R':
					removealltags=true;
					setATag=true;
					break;
				case '?':
				case 'h':
					printhelp();
					return 0;
					break;

				case 'Y':
					copytofile=true;
					readall=true;
					setATag=false;
					break;
				default:
					printf ("?? Unknown argument ??\n");
					return 1;
					break;
				}
		}

	while(optind < argc)
		{
			filename=argv[optind];

			if(force==false)
				getFileType();

			if(setATag==true)
				{
					readall=false;
					switch (filetype)
						{
						case IS_MP4:
							setMp4Tag();
							break;
						case IS_FLAC:
							setFlacTags();
							break;
						case IS_MP3:
							setMp3Tags();
							break;
						default:
							printf("Unknown file type\n");
							exit(1);
							break;
						}
				}

			if(readall==true)
				{
					switch(filetype)
						{
						case IS_FLAC:
							tags_ok=doReadAllFlac();
							break;
						case IS_MP4:
							tags_ok=doReadAllM4aTags();
							break;
						case IS_MP3:
							tags_ok=readAllMp3();
							break;
						default:
							printf("No readable tags in file %s\n",filename);
							return 1;
							break;
						}
				}

			if(tags_ok==true)
				{
					if(copytofile==false)
						printTags();
					else
						{
							setATag=true;
							for(int j=SETTITLE;j<MAXTAG;j++)
								tagstoset[j]=1;
						}
					if(optind < argc)
						printf("\n");
				}
			optind++;
		}

	freeAndNull(&artist);
	freeAndNull(&title);
	freeAndNull(&album);
	freeAndNull(&cdstring);
	freeAndNull(&year);
	freeAndNull(&totaltracksstring);
	freeAndNull(&trackstring);
	freeAndNull(&genre);
	freeAndNull(&comment);
	freeAndNull(&composer);
	freeAndNull(&compilationstring);

	return 0;
}

