//
//Keith Hedger
//Wed Jul 1 18:41:42 BST 2009
//
//mp3tags.h
//

#ifndef _MP3TAGS_
#define _MP3TAGS_

//#include <id3.h>
#include <id3tag.h>
#include "globals.h"

#define GENRE_MAX ( sizeof(id3_genres)/sizeof(id3_genres[0]) - 1 )
#define ID3_INVALID_GENRE 255
static const char *id3_genres[] =
{
    "Blues",            /* 0 */
    "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",             /* 5 */
    "Grunge",
    "Hip-Hop", 
    "Jazz",
    "Metal",
    "New Age",          /* 10 */
    "Oldies",
    "Other", 
    "Pop",
    "R&B",
    "Rap",              /* 15 */
    "Reggae", 
    "Rock",
    "Techno",
    "Industrial",
    "Alternative",      /* 20 */
    "Ska",
    "Death Metal", 
    "Pranks",
    "Soundtrack",
    "Euro-Techno",      /* 25 */
    "Ambient",
    "Trip-Hop", 
    "Vocal",
    "Jazz+Funk", 
    "Fusion",           /* 30 */
    "Trance",
    "Classical",
    "Instrumental", 
    "Acid",
    "House",            /* 35 */
    "Game",
    "Sound Clip", 
    "Gospel",
    "Noise",
    "Altern Rock",      /* 40 */
    "Bass",
    "Soul",
    "Punk",
    "Space",
    "Meditative",       /* 45 */
    "Instrumental Pop",
    "Instrumental Rock", 
    "Ethnic",
    "Gothic",
    "Darkwave",         /* 50 */
    "Techno-Industrial", 
    "Electronic", 
    "Pop-Folk",
    "Eurodance", 
    "Dream",            /* 55 */
    "Southern Rock", 
    "Comedy", 
    "Cult",
    "Gangsta",
    "Top 40",           /* 60 */
    "Christian Rap", 
    "Pop/Funk", 
    "Jungle",
    "Native American", 
    "Cabaret",          /* 65 */
    "New Wave",
    "Psychadelic", 
    "Rave",
    "Showtunes", 
    "Trailer",          /* 70 */
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz", 
    "Polka",            /* 75 */
    "Retro",
    "Musical",
    "Rock & Roll", 
    "Hard Rock", 
    "Folk",             /* 80 */
    "Folk/Rock",
    "National Folk", 
    "Swing",
    "Fast Fusion",
    "Bebob",            /* 85 */
    "Latin",
    "Revival",
    "Celtic",
    "Bluegrass",
    "Avantgarde",       /* 90 */
    "Gothic Rock",
    "Progressive Rock",
    "Psychedelic Rock", 
    "Symphonic Rock", 
    "Slow Rock",        /* 95 */
    "Big Band", 
    "Chorus",
    "Easy Listening", 
    "Acoustic", 
    "Humour",           /* 100 */
    "Speech",
    "Chanson", 
    "Opera",
    "Chamber Music", 
    "Sonata",           /* 105 */
    "Symphony",
    "Booty Bass", 
    "Primus",
    "Porn Groove", 
    "Satire",           /* 110 */
    "Slow Jam", 
    "Club",
    "Tango",
    "Samba",
    "Folklore",         /* 115 */
    "Ballad",
    "Power Ballad",
    "Rhythmic Soul",
    "Freestyle",
    "Duet",             /* 120 */
    "Punk Rock",
    "Drum Solo",
    "A Capella",
    "Euro-House",
    "Dance Hall",       /* 125 */
    "Goa",
    "Drum & Bass",
    "Club-House",
    "Hardcore",
    "Terror",           /* 130 */
    "Indie",
    "BritPop",
    "Negerpunk",
    "Polsk Punk",
    "Beat",             /* 135 */
    "Christian Gangsta Rap",
    "Heavy Metal",
    "Black Metal",
    "Crossover",
    "Contemporary Christian",/* 140 */
    "Christian Rock",
    "Merengue",
    "Salsa",
    "Thrash Metal",
    "Anime",            /* 145 */
    "JPop",
    "Synthpop"
};

bool read_all_mp3(void);
void set_mp3_tags(void);

#endif
