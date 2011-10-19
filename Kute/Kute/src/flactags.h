//
//Keith Hedger
//Mon Jun 29 13:57:57 BST 2009
//
//flactags.h
//

#include <FLAC/metadata.h>
#include "globals.h"
#include "mp4tags.h"

#ifndef _FLACTAGS_
#define _FLACTAGS_

void set_flac_tags(void);
bool Flac_Tag_Read_File_Tag (char *filename);
bool do_read_all_flac(void);
#endif
