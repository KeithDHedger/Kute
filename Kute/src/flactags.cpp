/*
 *
 * ©K. D. Hedger. Tue 11 Dec 16:33:58 GMT 2018 keithdhedger@gmail.com

 * This file (flactags.cpp) is part of Kute.

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

#include <FLAC/metadata.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "flactags.h"

/*
 * Write Flac tag,using the level 2 flac interface
 */
void setFlacTag(FLAC__StreamMetadata *block,FLAC__StreamMetadata_VorbisComment_Entry entry,const char* tagtype,char* tagdata)
{
	if(tagdata!=NULL && strlen(tagdata)>0)
		{
			sprintf(tempbuffer,"%s=%s",tagtype,tagdata);
			entry.entry=(FLAC__byte *)tempbuffer;
			entry.length=strlen((const char *)entry.entry);
			FLAC__metadata_object_vorbiscomment_append_comment(block,entry,true);
		}
	else
		{
			FLAC__metadata_object_vorbiscomment_remove_entries_matching(block,tagtype);
		}
}

void setFlacTags(void)
{
	FLAC__Metadata_Chain	*chain;
	FLAC__Metadata_Iterator	*iter;

	// Create a new chain instance to get all blocks in one time
	chain=FLAC__metadata_chain_new();
	if(chain==NULL || !FLAC__metadata_chain_read(chain,filename))
		{
			if(chain==NULL)
				printf("Error with \"FLAC__metadata_chain_new\"\n");
			else
				printf("Error with \"FLAC__metadata_chain_read\"\n");

			printf("ERROR while opening file: '%s' as FLAC \n",filename);
			return;
		}

	iter=FLAC__metadata_iterator_new();
	if(iter==NULL)
		{
			printf("ERROR while opening file: '%s' as FLAC\n.",filename);
			return;
		}

	// Initialize the iterator to point to the first metadata block in the given chain.
	FLAC__metadata_iterator_init(iter,chain);

	while(FLAC__metadata_iterator_next(iter))
		{
			// Action to do according the type
			switch (FLAC__metadata_iterator_get_block_type(iter))
				{
				case FLAC__METADATA_TYPE_VORBIS_COMMENT:
				{
					// Get block data
					FLAC__StreamMetadata *block=FLAC__metadata_iterator_get_block(iter);
					FLAC__StreamMetadata_VorbisComment *vc=&block->data.vorbis_comment;

					FLAC__StreamMetadata_VorbisComment_Entry entry;

					if(tagstoset[SETTITLE]==1)
						setFlacTag(block,entry,"TITLE",title);

					if(tagstoset[SETARTIST]==1)
						setFlacTag(block,entry,"ARTIST",artist);

					if(tagstoset[SETALBUM]==1)
						setFlacTag(block,entry,"ALBUM",album);

					if(tagstoset[SETTRACK]==1)
						setFlacTag(block,entry,"TRACKNUMBER",trackstring);

					if(tagstoset[SETTOTALTRACKS]==1)
						setFlacTag(block,entry,"TRACKTOTAL",totaltracksstring);

					if(tagstoset[SETCD]==1)
						setFlacTag(block,entry,"DISCNUMBER",cdstring);

					if(tagstoset[SETCOMPILATION]==1)
						setFlacTag(block,entry,"COMPILATION",compilationstring);

					if(tagstoset[SETYEAR]==1)
						setFlacTag(block,entry,"DATE",year);

					if(tagstoset[SETGENRE]==1)
						setFlacTag(block,entry,"GENRE",genre);

					if(tagstoset[SETCOMPOSER]==1)
						setFlacTag(block,entry,"COMPOSER",composer);

					if(tagstoset[SETCOMMENT]==1)
						{
							setFlacTag(block,entry,"COMMENT",comment);
							setFlacTag(block,entry,"DESCRIPTION",comment);
						}

					if(tagstoset[SETDISCID]==1)
						setFlacTag(block,entry,"DISCID",discidstr);

					FLAC__metadata_chain_sort_padding(chain);
					FLAC__metadata_chain_write(chain,true,true);
					FLAC__metadata_iterator_delete(iter);
					FLAC__metadata_chain_delete(chain);
					return;
					break;
				}

				default:
					break;
				}
		}
}

void getFlacTagData(const char* name,char** tagname,FLAC__StreamMetadata *block)
{
	int											field_num=0;
	int											field_len;
	char										*field_value;
	FLAC__StreamMetadata_VorbisComment			*vc;
	FLAC__StreamMetadata_VorbisComment_Entry	*field;
	bool										tagchanged=false;

	vc=&block->data.vorbis_comment;

	while((field_num=FLAC__metadata_object_vorbiscomment_find_entry_from(block,field_num,name)) >= 0)
		{
			/* Extract field value */
			field=&vc->comments[field_num++];
			field_value=(char*)memchr(field->entry,'=',field->length);

			if(field_value)
				{
					field_value++;
					if(field_value && strlen(field_value) > 0)
						{
							field_len=field->length-(field_value-(char*)field->entry);
							*tagname=(char *) malloc(strlen(field_value)+1);
							strcpy(*tagname,field_value);
							tagchanged=true;
						}
				}
		}

	if(tagchanged==false)
		{
			*tagname=(char *)malloc(1);
			*tagname[0]=0;
		}
}

/*
 * Read tag data from a FLAC file using the level 1 flac interface,
 * Note:
 *  - if field is found but contains no info (strlen(str)==0),we don't read it
 */
bool FlacTagReadFileTag(char *filename)
{
	FLAC__Metadata_SimpleIterator *iter;
	char *string=NULL;
	int i;

	iter=FLAC__metadata_simple_iterator_new();
	if(iter==NULL || !FLAC__metadata_simple_iterator_init(iter,filename,true,false))
		{
			if(iter==NULL)
				printf("ERROR Could not open %s for reading as a FLAC file!\n",filename);
			else
				{
					FLAC__Metadata_SimpleIteratorStatus status=FLAC__metadata_simple_iterator_status(iter);
					FLAC__metadata_simple_iterator_delete(iter);
					printf("ERROR Could not open %s for reading as a FLAC file!\n",filename);
				}
			return false;
		}

	while(FLAC__metadata_simple_iterator_next(iter))
		{
			FLAC__StreamMetadata *block=FLAC__metadata_simple_iterator_get_block(iter);

			switch (FLAC__metadata_simple_iterator_get_block_type(iter))
				{
				case FLAC__METADATA_TYPE_VORBIS_COMMENT:
				{
					getFlacTagData("TITLE",&title,block);
					getFlacTagData("ARTIST",&artist,block);
					getFlacTagData("ALBUM",&album,block);
					getFlacTagData("DISCNUMBER",&cdstring,block);
					getFlacTagData("DATE",&year,block);
					getFlacTagData("TRACKTOTAL",&totaltracksstring,block);
					getFlacTagData("TRACKNUMBER",&trackstring,block);
					getFlacTagData("GENRE",&genre,block);
					getFlacTagData("COMPOSER",&composer,block);
					getFlacTagData("COMPILATION",&compilationstring,block);
					getFlacTagData("COMMENT",&comment,block);
					getFlacTagData("DISCID",&discidstr,block);

					if(comment!=NULL && strlen(comment) <1)
						getFlacTagData("DESCRIPTION",&comment,block);
				}
				}
		}
	return true;
}

bool doReadAllFlac(void)
{
	return FlacTagReadFileTag(filename);
}

