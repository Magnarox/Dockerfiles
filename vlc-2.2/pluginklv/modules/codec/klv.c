/**
 * @file klv.c
 * @brief Codec which push KLV data on audio
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>

/* VLC core API headers */
#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_codec.h>
#include <vlc_aout.h>
#include <vlc_block.h>
#include <vlc_filter.h>
#include <vlc_block_helper.h>

#include "../packetizer/packetizer_helper.h"

#define VLC_CODEC_KLV VLC_FOURCC('k','l','v',' ')

/* Forward declarations */
static int  OpenDecoder   ( vlc_object_t * );
static void CloseDecoder(vlc_object_t * );
static block_t *DecodeBlock  ( decoder_t *, block_t ** );

static uint8_t *GetOutBuffer ( decoder_t *, block_t *, block_t ** );
static block_t *GetAoutBuffer( decoder_t *, block_t * );

static int  OpenFilter(vlc_object_t *);
static void CloseFilter(vlc_object_t *);
static block_t *Convert(filter_t *, block_t *);

/* Module descriptor */
vlc_module_begin()
    set_shortname("klv")
    set_category(CAT_INPUT)
    set_subcategory(SUBCAT_INPUT_ACODEC)
    set_description("KLV decoder")
    set_capability("decoder", 100)
    set_callbacks( OpenDecoder, CloseDecoder )

	//add_submodule()
	//	set_category(CAT_INPUT)
	//	set_subcategory(SUBCAT_INPUT_ACODEC)
	//	set_description("KLV converter")
	//	set_capability("audio converter", 100)
	//	set_callbacks(OpenFilter, CloseFilter)
vlc_module_end ()

struct decoder_sys_t
{
	/*
	* Input properties
	*/
	int i_state;

	block_bytestream_t bytestream;

	/*
	* Common properties
	*/
	date_t  end_date;

	mtime_t i_pts;
};

/*****************************************************************************
 * Open: probe the decoder and return score
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
	msg_Dbg(p_this, "MAGNAROX : open KLV decoder");
    decoder_t *p_dec = (decoder_t*)p_this;
	decoder_sys_t *p_sys;
    
    if( p_dec->fmt_in.i_codec != VLC_CODEC_KLV )
    {
        return VLC_EGENERIC;
    }
	msg_Dbg(p_this, "MAGNAROX : KLV decoder is choose");


	/* Allocate the memory needed to store the decoder's structure */
	if ((p_dec->p_sys = p_sys =
		(decoder_sys_t *)malloc(sizeof(decoder_sys_t))) == NULL)
		return VLC_ENOMEM;

	p_sys->i_state = STATE_NOSYNC;
	date_Set(&p_sys->end_date, 0);
	p_sys->i_pts = VLC_TS_INVALID;
	block_BytestreamInit(&p_sys->bytestream);

    /* Set output properties */
	es_format_Copy(&p_dec->fmt_out, &p_dec->fmt_in);
    p_dec->fmt_out.i_cat = AUDIO_ES;
    p_dec->fmt_out.i_codec = VLC_CODEC_KLV;
    p_dec->fmt_out.audio.i_rate = 0; /* So end_date gets initialized */
	p_dec->fmt_out.audio.i_bytes_per_frame = 0;

    /* Set callback */
    p_dec->pf_decode_audio = DecodeBlock;

    return VLC_SUCCESS;
}


static int OpenDecoder( vlc_object_t *p_this )
{   
    return Open( p_this );
}

static void CloseDecoder(vlc_object_t *p_this)
{
	decoder_t *p_dec = (decoder_t*)p_this;
	decoder_sys_t *p_sys = p_dec->p_sys;

	block_BytestreamRelease(&p_sys->bytestream);

	free(p_sys);
}

/****************************************************************************
 * DecodeBlock: the whole thing
 ****************************************************************************
 * This function is called just after the thread is launched.
 ****************************************************************************/
static block_t *DecodeBlock( decoder_t *p_dec, block_t **pp_block )
{
	msg_Dbg(p_dec, "MAGNAROX : KLV decoder DecodeBlock");

	decoder_sys_t *p_sys = p_dec->p_sys;
	block_t *p_block;
	uint8_t *p_buf;
	block_t *p_out_buffer;

	if (!pp_block || !*pp_block) return NULL;
	p_block = *pp_block;

	/* Verification d'usage pour ne pas prendre n'importe quoi*/
	if ((*pp_block)->i_flags&(BLOCK_FLAG_DISCONTINUITY | BLOCK_FLAG_CORRUPTED))
	{
		if ((*pp_block)->i_flags&BLOCK_FLAG_CORRUPTED)
		{
			p_sys->i_state = STATE_NOSYNC;
			block_BytestreamEmpty(&p_sys->bytestream);
		}
		date_Set(&p_sys->end_date, 0);
		block_Release(*pp_block);
		return NULL;
	}

	if (!date_Get(&p_sys->end_date) && (*pp_block)->i_pts <= VLC_TS_INVALID)
	{
		/* We've just started the stream, wait for the first PTS. */
		block_Release(*pp_block);
		return NULL;
	}

	/* Le vrai taf commence ici*/
	block_BytestreamPush(&p_sys->bytestream, *pp_block);

	if (p_block->i_buffer > 0
		&& !(p_block->i_flags & (BLOCK_FLAG_DISCONTINUITY | BLOCK_FLAG_CORRUPTED)))
	{
		msg_Dbg(p_dec, "MAGNAROX : p_block->i_pts = %d", p_block->i_pts);
		if (!(p_buf = GetOutBuffer(p_dec, p_block, &p_out_buffer)))
		{
			msg_Dbg(p_dec, "MAGNAROX : Audio Output Buffer is NULL!");
			return NULL;
		}

		block_GetBytes(&p_sys->bytestream,
			p_buf, p_out_buffer->i_buffer);

		/* Make sure we don't reuse the same pts twice */
		if (p_sys->i_pts == p_sys->bytestream.p_block->i_pts)
			p_sys->i_pts = p_sys->bytestream.p_block->i_pts = VLC_TS_INVALID;

		/* So p_block doesn't get re-added several times */
		*pp_block = block_BytestreamPop(&p_sys->bytestream);

		p_sys->i_state = STATE_NOSYNC;
	
		return p_out_buffer;
	}
	else
	{
		block_Release(p_block);
		return NULL;
	}
}

/*****************************************************************************
 * GetOutBuffer:
 *****************************************************************************/
static uint8_t *GetOutBuffer( decoder_t *p_dec,block_t *p_original_block, block_t **pp_out_buffer )
{
    uint8_t *p_buf;

	p_dec->fmt_out.audio.i_format = VLC_CODEC_KLV;
    p_dec->fmt_out.audio.i_rate     = 16000;
    p_dec->fmt_out.audio.i_channels = p_dec->fmt_in.audio.i_channels;
    p_dec->fmt_out.audio.i_frame_length = p_original_block->i_buffer;
    p_dec->fmt_out.audio.i_bytes_per_frame = p_original_block->i_buffer;

    p_dec->fmt_out.audio.i_original_channels = AOUT_CHAN_CENTER;
    p_dec->fmt_out.audio.i_physical_channels = AOUT_CHAN_CENTER & AOUT_CHAN_PHYSMASK;

    p_dec->fmt_out.i_bitrate = 16000;

    block_t *p_aout_buffer = GetAoutBuffer( p_dec, p_original_block);
    p_buf = p_aout_buffer ? p_aout_buffer->p_buffer : NULL;
    *pp_out_buffer = p_aout_buffer;

    return p_buf;
}

/*****************************************************************************
 * GetAoutBuffer:
 *****************************************************************************/
static block_t *GetAoutBuffer( decoder_t *p_dec , block_t *p_original_block)
{
    block_t *p_buf;

    p_buf = decoder_NewAudioBuffer( p_dec, p_original_block->i_buffer);
    if( !p_buf ) return NULL;

	msg_Dbg(p_dec, "MAGNAROX : Audio Buffer is ready");

	p_buf->i_pts = p_original_block->i_pts;
  
	//mtime_t i_pts = p_original_block->i_pts;
 //   p_buf->i_pts = date_Get( i_pts);
    //p_buf->i_length = date_Increment( i_pts, p_original_block->i_buffer)
    //                  - i_pts;

    return p_buf;
}


/*****************************************************************************
* Convert : convert data
*****************************************************************************/
static block_t *Convert(filter_t *p_filter, block_t *p_block)
{
	msg_Dbg(p_filter, "MAGNAROX : convert KLV");

	if (!p_block || !p_block->i_nb_samples)
	{
		if (p_block)
			block_Release(p_block);
		return NULL;
	}

	size_t i_out_size = 256 * 4;

	block_t *p_out = block_Alloc(i_out_size);
	if (unlikely(!p_out))
	{
		msg_Warn(p_filter, "can't get output buffer");
		block_Release(p_block);
		return NULL;
	}

	p_out->i_nb_samples = p_block->i_nb_samples;
	p_out->i_dts = p_block->i_dts;
	p_out->i_pts = p_block->i_pts;
	p_out->i_length = p_block->i_length;

	block_Release(p_block);

	return p_out;
}

/*****************************************************************************
* OpenFilter:
*****************************************************************************/
static int OpenFilter(vlc_object_t *p_this)
{
	msg_Dbg(p_this, "MAGNAROX : open KLV converter");
	filter_t *p_filter = (filter_t *)p_this;

	if (p_filter->fmt_in.audio.i_format != VLC_CODEC_KLV)
		return VLC_EGENERIC;

	if (p_filter->fmt_out.audio.i_format != VLC_CODEC_FL32)
		return VLC_EGENERIC;

	msg_Dbg(p_this, "MAGNAROX : %4.4s->%4.4s",
		(char *)&p_filter->fmt_in.audio.i_format,
		(char *)&p_filter->fmt_out.audio.i_format);

	p_filter->pf_audio_filter = Convert;

	return VLC_SUCCESS;
}

/*****************************************************************************
* CloseFilter : deallocate data structures
*****************************************************************************/
static void CloseFilter(vlc_object_t *p_this)
{

}
