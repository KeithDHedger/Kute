/*
--read-all '/media/LinuxData/Music/Anastacia/Anastacia/01 Seasons Change.m4a'
*/
/*
** (U)niversal (T)ag (C)ommand
** Copyright (C) Keith Hedger, 2008.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/
//Mon 31 Jul 2006 12:30:55 BST 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <mp4.h>
#include <FLAC/metadata.h>

#include "globals.h"
#include "mp4tags.h"
#include "flactags.h"
#include "mp3tags.h"

struct option long_options[] =
	{
		{"read-all",0,0,'r'},
		{"artist",1,0,'a'},
		{"album",1,0,'l'},
		{"title",1,0,'n'},
		{"track",1,0,'t'},
		{"total-tracks",1,0,'T'},
		{"cd",1,0,'C'},
		//{"total-cds",1,0,'C'},
		{"compilation",1,0,'i'},
		{"year",1,0,'y'},
		{"genre",1,0,'g'},
		{"composer",1,0,'m'},
		{"comment",1,0,'o'},
		{"force-flac",0,0,'f'},
		{"force-aac",0,0,'c'},
		{"force-mp3",0,0,'p'},
		{"help",0,0,'?'},
		{0, 0, 0, 0}
	};

void printhelp(void)
{
	printf("Usage: utc [OPTION] [FILE]\n");
}


void get_file_type(void)
{

	MP4FileHandle	filehandle=NULL;
	int mp3tmpfile;
	id3_file *mp3file;
	id3_tag *tag;
	char	*mp4info=NULL;

	if ( (mp3tmpfile=open(filename,O_RDONLY)) > 0 )
		{

		if ((mp3file = id3_file_fdopen(mp3tmpfile,ID3_FILE_MODE_READONLY)) != NULL)
			{

			if ( ((tag = id3_file_tag(mp3file)) != NULL) && (tag->nframes != 0))
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
	if (chain != NULL && FLAC__metadata_chain_read(chain,filename))
 		{
		filetype=IS_FLAC;
		return;
    		}

	mp4info=MP4FileInfo(filename,0);
	if (mp4info!=NULL)
		filetype=IS_AAC;
}

int main(int argc, char **argv)
{
	int c;
	bool set_a_tag=false;
	readall=true;
	force=false;
	filetype=UNKNOWN_TYPE;
	bool tags_ok=false;

	while (1)
		{
		int option_index = 0;
		c = getopt_long (argc, argv, ":alntTCiygmo:rfcp?h",long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
			{
			case 'r':
				readall=true;
				set_a_tag=false;
				break;
			
			case 'a':
				set_a_tag=true;
				artist=optarg;
				tagstoset[SETARTIST]=1;
				break;
			
			case 'l':
				set_a_tag=true;
				album=optarg;
				tagstoset[SETALBUM]=1;
				break;
			
			case 'n':
				set_a_tag=true;
				title=optarg;
				tagstoset[SETTITLE]=1;
				break;
			case 't':
				set_a_tag=true;
				trackstring=optarg;
				tagstoset[SETTRACK]=1;
				break;
			case 'T':
				set_a_tag=true;
				totaltracksstring=optarg;
				tagstoset[SETTOTALTRACKS]=1;
				break;
			case 'C':
				set_a_tag=true;
				cdstring=optarg;
				tagstoset[SETCD]=1;
				break;
//			case 'C':
//				set_a_tag=true;
//				tagstoset[SETTOTALCDS]=1;
//				break;
			case 'i':
				set_a_tag=true;
				compilationstring=optarg;
				tagstoset[SETCOMPILATION]=1;
				break;
			case 'y':
				set_a_tag=true;
				year=optarg;
				tagstoset[SETYEAR]=1;
				break;
			case 'g':
				set_a_tag=true;
				genre=optarg;
				tagstoset[SETGENRE]=1;
				break;
			case 'm':
				composer=optarg;
				set_a_tag=true;
				tagstoset[SETCOMPOSER]=1;
				break;
			case 'o':
				set_a_tag=true;
				comment=optarg;
				tagstoset[SETCOMMENT]=1;
				break;
			case 'f':
				force=true;
				filetype=IS_FLAC;
				break;
			case 'c':
				force=true;
				filetype=IS_AAC;
				break;
			case 'p':
				force=true;
				filetype=IS_MP3;
				break;
			case '?':
			case 'h':
				printhelp();
				return 0;
				break;

			default:
				printf ("?? Unknown argument ??\n");
				return 1;
			break;
			}
		}

	if (optind < argc)
		filename=argv[optind];

				//filename="/media/BackingStore/testriperrs/xxx (copy).mp3";
				//set_a_tag=true;
				//artist="an artist";;
				//tagstoset[SETARTIST]=1;


	if (force==false)
		get_file_type();

	if (set_a_tag==true)
		{
		readall=false;
		switch (filetype)
			{
			case IS_AAC:
				set_tag();
				break;
			case IS_FLAC:
				set_flac_tags();
				break;
			case IS_MP3:
				set_mp3_tags();
				break;
			default:
				printf("Unknown file type\n");
				exit(1);
				break;
			}
		}
	
	if (readall==true)
		{

		switch(filetype)
			{
			case IS_FLAC:
				tags_ok=do_read_all_flac();
				break;
			case IS_AAC:
				tags_ok=do_read_all();
				break;
			case IS_MP3:
				tags_ok=read_all_mp3();
				break;
			default:
				printf("No readable tags in file %s\n",filename);
				return 1;
				break;
			}

		}
		if (tags_ok==true)
			print_tags();
/*
	free(title);
	free(artist);
	free(album);
	free(cdstring);
	free(year);
	free(totaltracksstring);
	free(trackstring);
	free(genre);
	free(comment);
	free(composer);
*/

	return 0;
}

