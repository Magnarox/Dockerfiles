/**
 * @file aoutklv.c
 * @brief Fake Audio Output for KLV data
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_aout.h>
#include <vlc_cpu.h>

static int Open(vlc_object_t * p_this);

vlc_module_begin()
	set_shortname("aklv")
	set_category(CAT_AUDIO)
	set_subcategory(SUBCAT_AUDIO_AOUT)

	set_description("KLV audio output")
	set_capability("audio output", 0)
	set_callbacks(Open, NULL)
	add_shortcut("aklv")
vlc_module_end()

#define A52_FRAME_NB 1536

static void Play(audio_output_t *aout, block_t *block)
{
	msg_Dbg(aout, "MAGNAROX : KLV audio output Play");

	block_Release(block);
	(void)aout;
}

static void Flush(audio_output_t *aout, bool wait)
{
	msg_Dbg(aout, "MAGNAROX : KLV audio output Flush");

	(void)aout; 
	(void)wait;
}

static int Start(audio_output_t *aout, audio_sample_format_t *fmt)
{
	msg_Dbg(aout, "MAGNAROX : KLV audio output Start");
	/*if (AOUT_FMT_SPDIF(fmt) && var_InheritBool(aout, "spdif"))
	{
		fmt->i_format = VLC_CODEC_SPDIFL;
		fmt->i_bytes_per_frame = AOUT_SPDIF_SIZE;
		fmt->i_frame_length = A52_FRAME_NB;
	}
	else
		fmt->i_format = HAVE_FPU ? VLC_CODEC_FL32 : VLC_CODEC_S16N;*/

	return VLC_SUCCESS;
}

static int Open(vlc_object_t *obj)
{
	audio_output_t *aout = (audio_output_t *)obj;

	aout->start = Start;
	aout->time_get = NULL;
	aout->play = Play;
	aout->pause = NULL;
	aout->flush = Flush;
	aout->stop = NULL;
	aout->volume_set = NULL;
	aout->mute_set = NULL;
	return VLC_SUCCESS;
}
