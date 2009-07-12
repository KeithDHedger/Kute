//
//Keith Hedger
//Mon Jun 29 11:38:54 BST 2009
//
//globals.h
//

#include <mp4.h>

#ifndef _GLOBALS_
#define IS_FLAC 1
#define IS_AAC 2
#define IS_MP3 3
#define UNKNOWN_TYPE 4

#define SETTITLE 0
#define SETALBUM 1
#define SETARTIST 2
#define SETTRACK 3
#define SETTOTALTRACKS 4
#define SETCD 5
#define SETTOTALCDS 6
#define SETCOMPILATION 7
#define SETYEAR 8
#define SETGENRE 9
#define SETCOMPOSER 10
#define SETCOMMENT 11
#define	MAXTAG SETCOMMENT

extern char*		filename;
extern int		filetype;
extern bool		readall;
extern bool		force;
extern bool		quotes;
extern MP4FileHandle	filehandle;

extern char*		title;
extern char*		album;
extern char*		artist;

extern char*		year;
extern char*		genre;
extern char*		composer;
extern char*		comment;

extern char*		cdstring;
extern char*		totaltracksstring;
extern char*		trackstring;
extern char*		compilationstring;
extern int		tagstoset[12];

//extern char		bufferdata[2048];
//extern char*		flacbuffer;
//extern char*		gbuff;
extern char*		tempbuffer;
void print_tags(void);

#define _GLOBALS_

#endif
