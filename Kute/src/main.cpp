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
	{"force-flac",0,0,'f'},
	{"force-aac",0,0,'c'},
	{"force-mp3",0,0,'p'},
	{"use-quotes",0,0,'q'},
	{"remove-all",0,0,'R'},
	{"list-genres",0,0,'G'},
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
	printf("o,--comment=ARG	Set tag COMMENT to ARG\n");
	printf("f,--force-flac		Don't try to guess the file type treat it as a FLAC file \n");
	printf("c,--force-aac		Don't try to guess the file type treat it as an AAC file\n");
	printf("p,--force-mp3		Don't try to guess the file type treat it as an MP3 file\n");
	printf("			The force options may be needed if the file has NO tag data\n");
	printf("q,--use-quotes		Quote file/artist/album names etc\n");
	printf("R,--remove-all		Remove ALL Mp4 tags\n");
	printf("G,--list-genres		List standard genres\n");
	printf("?,--help		Print this help\n");
}


void getFileType(void)
{
	MP4FileHandle	filehandle=NULL;
	int				mp3tmpfile;
	id3_file		*mp3file;
	id3_tag			*tag;
	char			*mp4info=NULL;

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

const char	*mp3Genres[]={
"Blues","Classic Rock","Country","Dance","Disco","Funk","Grunge","Hip-Hop","Jazz","Metal","New Age","Oldies","Other","Pop","R&B","Rap","Reggae","Rock","Techno","Industrial","Alternative","Ska","Death Metal","Pranks","Soundtrack","Euro-Techno","Ambient","Trip-Hop","Vocal","Jazz+Funk","Fusion","Trance","Classical","Instrumental","Acid","House","Game","Sound Clip","Gospel","Noise","AlternRock","Bass","Soul","Punk","Space","Meditative","Instrumental Pop","Instrumental Rock","Ethnic","Gothic","Darkwave","Techno-Industrial","Electronic","Pop-Folk","Eurodance","Dream","Southern Rock","Comedy","Cult","Gangsta Rap","Top 40","Christian Rap","Pop / Funk","Jungle","Native American","Cabaret","New Wave","Psychedelic","Rave","Showtunes","Trailer","Lo-Fi","Tribal","Acid Punk","Acid Jazz","Polka","Retro","Musical","Rock & Roll","Hard Rock","Folk","Folk-Rock","National Folk","Swing","Fast Fusion","Bebob","Latin","Revival","Celtic","Bluegrass","Avantgarde","Gothic Rock","Progressive Rock","Psychedelic Rock","Symphonic Rock","Slow Rock","Big Band","Chorus","Easy Listening","Acoustic","Humour","Speech","Chanson","Opera","Chamber Music","Sonata","Symphony","Booty Bass","Primus","Porn Groove","Satire","Slow Jam","Club","Tango","Samba","Folklore","Ballad","Power Ballad","Rhythmic Soul","Freestyle","Duet","Punk Rock","Drum Solo","A Cappella","Euro-House","Dance Hall","Goa","Drum & Bass","Club-House","Hardcore","Terror","Indie","BritPop","Negerpunk","Polsk Punk","Beat","Christian Gangsta Rap","Heavy Metal","Black Metal","Crossover","Contemporary Christian","Christian Rock","Merengue","Salsa","Thrash Metal","Anime","JPop","Synthpop",NULL};

const char	*extraGenres[]={
"Abstract","Art Rock","Baroque","Bhangra","Big Beat","Breakbeat","Chillout","Downtempo","Dub","EBM","Eclectic","Electro","Electroclash","Emo","Experimental","Garage","Global","IDM","Illbient","Industro-Goth","Jam Band","Krautrock","Leftfield","Lounge","Math Rock","New Romantic","Nu-Breakz","Post-Punk","Post-Rock","Psytrance","Shoegaze","Space Rock","Trop Rock","World Music","Neoclassical","Audiobook","Audio Theatre","Neue Deutsche Welle","Podcast","Indie Rock","G-Funk","Dubstep","Garage Rock","Psybient",NULL};

void listGenres(void)
{
	int	cnt=0;
	int xtracnt=0;
	printf("---------------------\nStandard MP3 genres\n---------------------\n");
	while(mp3Genres[cnt]!=NULL)
		{
			printf("%i: %s\n",cnt,mp3Genres[cnt]);
			cnt++;
		}

	printf("---------------------\nNon standard genres\n---------------------\n");
	while(extraGenres[xtracnt]!=NULL)
		{
			printf("%i: %s\n",cnt,extraGenres[xtracnt++]);
			cnt++;
		}
}

int main(int argc,char **argv)
{
	int c;
	bool setATag=false;
	readall=true;
	force=false;
	filetype=UNKNOWN_TYPE;
	bool tags_ok=false;

	while(1)
		{
			int option_index=0;
			c=getopt_long (argc,argv,":alntTCiygmo:rfcpqRG?h",long_options,&option_index);
			if(c==-1)
				break;

			switch (c)
				{
				case 'G':
					listGenres();
					exit(0);
				case 'r':
					readall=true;
					setATag=false;
					break;

				case 'a':
					setATag=true;
					artist=optarg;
					tagstoset[SETARTIST]=1;
					break;

				case 'l':
					setATag=true;
					album=optarg;
					tagstoset[SETALBUM]=1;
					break;

				case 'n':
					setATag=true;
					title=optarg;
					tagstoset[SETTITLE]=1;
					break;
				case 't':
					setATag=true;
					trackstring=optarg;
					if(trackstring[0]=='0' && strlen(trackstring)==1)
						trackstring=(char*)"";

					tagstoset[SETTRACK]=1;
					break;
				case 'T':
					setATag=true;
					totaltracksstring=optarg;
					if(totaltracksstring[0]=='0' && strlen(totaltracksstring)==1)
						totaltracksstring=(char*)"";

					tagstoset[SETTOTALTRACKS]=1;
					break;
				case 'C':
					setATag=true;
					cdstring=optarg;
					if(cdstring[0]=='0' && strlen(cdstring)==1)
						cdstring=(char*)"";

					tagstoset[SETCD]=1;
					break;

				case 'i':
					setATag=true;
					compilationstring=optarg;
					if(compilationstring[0]=='0' && strlen(compilationstring)==1)
						compilationstring=(char*)"";

					tagstoset[SETCOMPILATION]=1;
					break;
				case 'y':
					setATag=true;
					year=optarg;
					tagstoset[SETYEAR]=1;
					break;
				case 'g':
					setATag=true;
					genre=optarg;
					tagstoset[SETGENRE]=1;
					break;
				case 'm':
					composer=optarg;
					setATag=true;
					tagstoset[SETCOMPOSER]=1;
					break;
				case 'o':
					setATag=true;
					comment=optarg;
					tagstoset[SETCOMMENT]=1;
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
							tags_ok=doReadAllFlacTags();
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
					printTags();
					if(optind < argc)
						printf("\n");
				}
			optind++;
		}

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

	return 0;
}

