//
//Keith Hedger
//Mon Jun 29 13:57:57 BST 2009
//
//flactags.cpp
//

#include <FLAC/metadata.h>
#include <stdlib.h>

#include "globals.h"
#include "flactags.h"


/*
 * Write Flac tag, using the level 2 flac interface
 */

void set_tag(FLAC__StreamMetadata *block,FLAC__StreamMetadata_VorbisComment_Entry entry,const char* tagtype,char* tagdata)
{

 	if (tagdata!=NULL && strlen(tagdata)>0)
		{
		sprintf(tempbuffer,"%s=%s",tagtype,tagdata);
		entry.entry = (FLAC__byte *)tempbuffer;
		entry.length = strlen((const char *)entry.entry);
		FLAC__metadata_object_vorbiscomment_append_comment(block, entry,true);
		}
	else
		{
		FLAC__metadata_object_vorbiscomment_remove_entries_matching(block,tagtype);
		}
}

void set_flac_tags(void)
{

	FLAC__Metadata_Chain *chain;
	FLAC__Metadata_Iterator *iter;

    // Create a new chain instance to get all blocks in one time
	chain = FLAC__metadata_chain_new();
		if (chain == NULL || !FLAC__metadata_chain_read(chain,filename))
			{
			if (chain == NULL)
				printf("Error with \"FLAC__metadata_chain_new\"\n");
			else
				printf("Error with \"FLAC__metadata_chain_read\"\n");
        
			printf("ERROR while opening file: '%s' as FLAC \n",filename);
			return;
			}

	iter = FLAC__metadata_iterator_new();
	if (iter == NULL)
		{
		printf("ERROR while opening file: '%s' as FLAC\n.",filename);
		return;
		}
    
    // Initialize the iterator to point to the first metadata block in the given chain.
	FLAC__metadata_iterator_init(iter,chain);
    
	while (FLAC__metadata_iterator_next(iter))
		{
        // Action to do according the type
		switch ( FLAC__metadata_iterator_get_block_type(iter) )
			{
			case FLAC__METADATA_TYPE_VORBIS_COMMENT:
				{
                // Get block data
				FLAC__StreamMetadata *block = FLAC__metadata_iterator_get_block(iter);
				FLAC__StreamMetadata_VorbisComment *vc = &block->data.vorbis_comment;

				FLAC__StreamMetadata_VorbisComment_Entry entry;
	
				if (tagstoset[SETTITLE]==1)
					set_tag(block,entry,"TITLE",title);

				if (tagstoset[SETARTIST]==1)
					set_tag(block,entry,"ARTIST",artist);
	
				if (tagstoset[SETALBUM]==1)
					set_tag(block,entry,"ALBUM",album);

				if (tagstoset[SETTRACK]==1)
					set_tag(block,entry,"TRACKNUMBER",trackstring);

				if (tagstoset[SETTOTALTRACKS]==1)
					set_tag(block,entry,"TRACKTOTAL",totaltracksstring);

				if (tagstoset[SETCD]==1)
					set_tag(block,entry,"DISCNUMBER",cdstring);

				if (tagstoset[SETCOMPILATION]==1)
					set_tag(block,entry,"COMPILATION",compilationstring);

				if (tagstoset[SETYEAR]==1)
					set_tag(block,entry,"DATE",year);

				if (tagstoset[SETGENRE]==1)
					set_tag(block,entry,"GENRE",genre);

				if (tagstoset[SETCOMPOSER]==1)
					set_tag(block,entry,"COMPOSER",composer);

				if (tagstoset[SETCOMMENT]==1)
					{
					set_tag(block,entry,"COMMENT",comment);
					set_tag(block,entry,"DESCRIPTION",comment);
					}

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

void get_tag_data(const char* name,char** tagname,FLAC__StreamMetadata *block)
{
	int						field_num=0;
	int						field_len;
	char						*field_value;
	FLAC__StreamMetadata_VorbisComment		*vc;
	FLAC__StreamMetadata_VorbisComment_Entry	*field;
	bool						tagchanged=false;	
	vc = &block->data.vorbis_comment;

	while ( (field_num = FLAC__metadata_object_vorbiscomment_find_entry_from(block,field_num,name)) >= 0 )
		{
                    /* Extract field value */
		field = &vc->comments[field_num++];
		field_value = (char*)memchr(field->entry, '=', field->length);

		if (field_value)
			{
			field_value++;
			if ( field_value && strlen(field_value) > 0 )
				{
				field_len=field->length-(field_value-(char*)field->entry);
				*tagname=(char *) malloc(strlen(field_value)+1);
				strcpy(*tagname,field_value);
				tagchanged=true;
				}
			}
		}
	if (tagchanged==false)
		{
		*tagname=(char *) malloc(1);
		*tagname[0]=0;
		}
}

/*
 * Read tag data from a FLAC file using the level 1 flac interface,
 * Note:
 *  - if field is found but contains no info (strlen(str)==0), we don't read it
 */
bool Flac_Tag_Read_File_Tag (char *filename)
{
	FLAC__Metadata_SimpleIterator *iter;
	char *string = NULL;
	int i;

	iter = FLAC__metadata_simple_iterator_new();
	if ( iter == NULL || !FLAC__metadata_simple_iterator_init(iter, filename, true, false) )
		{
		if ( iter == NULL )
			printf("ERROR Could not open %s for reading as a FLAC file!\n",filename);
		else
			{
			FLAC__Metadata_SimpleIteratorStatus status = FLAC__metadata_simple_iterator_status(iter);
			FLAC__metadata_simple_iterator_delete(iter);
			printf("ERROR Could not open %s for reading as a FLAC file!\n",filename);
			}
		return false;
		}

	while (FLAC__metadata_simple_iterator_next(iter))
		{
		FLAC__StreamMetadata *block = FLAC__metadata_simple_iterator_get_block(iter);

		switch ( FLAC__metadata_simple_iterator_get_block_type(iter) )
			{
			case FLAC__METADATA_TYPE_VORBIS_COMMENT:
				{
				get_tag_data("TITLE",&title,block);
				get_tag_data("ARTIST",&artist,block);
				get_tag_data("ALBUM",&album,block);
				get_tag_data("DISCNUMBER",&cdstring,block);
				get_tag_data("DATE",&year,block);
				get_tag_data("TRACKTOTAL",&totaltracksstring,block);
				get_tag_data("TRACKNUMBER",&trackstring,block);
				get_tag_data("GENRE",&genre,block);
				get_tag_data("COMPOSER",&composer,block);
				get_tag_data("COMPILATION",&compilationstring,block);
				get_tag_data("COMMENT",&comment,block);
				
				if (comment!=NULL && strlen(comment) <1)
					get_tag_data("DESCRIPTION",&comment,block);
				}
			}
		}
	return true;
}

bool do_read_all_flac(void)
{
	return Flac_Tag_Read_File_Tag (filename);
}

