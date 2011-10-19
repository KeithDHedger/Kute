//
//Keith Hedger
//Mon Jun 29 11:38:54 BST 2009
//
//globals.cpp
//

#include <mp4v2/mp4v2.h>

#include "globals.h"

char*		filename;
int		filetype=UNKNOWN_TYPE;
bool		readall=false;
bool		force=false;
bool		quotes=false;

bool		removealltags=false;
char*		title=NULL;
char*		album=NULL;
char*		artist=NULL;
char*		year=NULL;
char*		genre=NULL;
char*		composer=NULL;
char*		comment=NULL;

int		tagstoset[12];
char*		cdstring=NULL;
char*		totaltracksstring=NULL;
char*		compilationstring=NULL;

char*		trackstring=NULL;
//char		bufferdata[2048];
//char*		flacbuffer=&bufferdata[0];
//char*		gbuff=&bufferdata[0];
char*		tempbuffer=(char*)calloc(2048,1);
void print_tags(void)
{
	if (quotes==true)
		{
		printf("FILENAME='%s'\nFILETYPE=%i\n",filename,filetype);
		printf("TITLE='%s'\nARTIST='%s'\nALBUM='%s'\nTRACK=%s\nTOTALTRACKS=%s\n",title,artist,album,trackstring,totaltracksstring);
		printf("CD=%s\nGENRE='%s'\nYEAR='%s'\nCOMPILATION=%s\n",cdstring,genre,year,compilationstring);
		printf("COMPOSER='%s'\nCOMMENT='%s'\n",composer,comment);

		}
	else
		{
		printf("FILENAME=%s\nFILETYPE=%i\n",filename,filetype);
		printf("TITLE=%s\nARTIST=%s\nALBUM=%s\nTRACK=%s\nTOTALTRACKS=%s\n",title,artist,album,trackstring,totaltracksstring);
		printf("CD=%s\nGENRE=%s\nYEAR=%s\nCOMPILATION=%s\n",cdstring,genre,year,compilationstring);
		printf("COMPOSER=%s\nCOMMENT=%s\n",composer,comment);
		}
}


