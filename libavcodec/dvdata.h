/*
 * Constants for DV codec
 * Copyright (c) 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * Constants for DV codec.
 */

#ifndef AVCODEC_DVDATA_H
#define AVCODEC_DVDATA_H

#include "libavutil/rational.h"
#include "avcodec.h"

typedef struct DVwork_chunk {
    uint16_t  buf_offset;
    uint16_t  mb_coordinates[5];
} DVwork_chunk;

/*
 * DVprofile is used to express the differences between various
 * DV flavors. For now it's primarily used for differentiating
 * 525/60 and 625/50, but the plans are to use it for various
 * DV specs as well (e.g. SMPTE314M vs. IEC 61834).
 */
typedef struct DVprofile {
    int              dsf;                   /* value of the dsf in the DV header */
    int              video_stype;           /* stype for VAUX source pack */
    int              frame_size;            /* total size of one frame in bytes */
    int              difseg_size;           /* number of DIF segments per DIF channel */
    int              n_difchan;             /* number of DIF channels per frame */
    AVRational       time_base;             /* 1/framerate */
    int              ltc_divisor;           /* FPS from the LTS standpoint */
    int              height;                /* picture height in pixels */
    int              width;                 /* picture width in pixels */
    AVRational       sar[2];                /* sample aspect ratios for 4:3 and 16:9 */
    DVwork_chunk    *work_chunks;           /* each thread gets its own chunk of frame to work on */
    uint32_t        *idct_factor;           /* set of iDCT factor tables */
    enum PixelFormat pix_fmt;               /* picture pixel format */
    int              bpm;                   /* blocks per macroblock */
    const uint8_t   *block_sizes;           /* AC block sizes, in bits */
    int              audio_stride;          /* size of audio_shuffle table */
    int              audio_min_samples[3];  /* min amount of audio samples */
                                            /* for 48kHz, 44.1kHz and 32kHz */
    int              audio_samples_dist[5]; /* how many samples are supposed to be */
                                            /* in each frame in a 5 frames window */
    const uint8_t  (*audio_shuffle)[9];     /* PCM shuffling table */
} DVprofile;

/* unquant tables (not used directly) */
static const uint8_t dv_quant_shifts[22][4] = {
  { 3,3,4,4 },
  { 3,3,4,4 },
  { 2,3,3,4 },
  { 2,3,3,4 },
  { 2,2,3,3 },
  { 2,2,3,3 },
  { 1,2,2,3 },
  { 1,2,2,3 },
  { 1,1,2,2 },
  { 1,1,2,2 },
  { 0,1,1,2 },
  { 0,1,1,2 },
  { 0,0,1,1 },
  { 0,0,1,1 },
  { 0,0,0,1 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
};

static const uint8_t dv_quant_offset[4] = { 6,  3,  0,  1 };
static const uint8_t dv_quant_areas[4]  = { 6, 21, 43, 64 };

/* quantization quanta by QNO for DV100 */
static const uint8_t dv100_qstep[16] = {
    1, /* QNO = 0 and 1 both have no quantization */
    1,
    2, 3, 4, 5, 6, 7, 8, 16, 18, 20, 22, 24, 28, 52
};

/* DV25/50 DCT coefficient weights and inverse weights */
/* created by dvtables.py */
static const int dv_weight_bits = 18;
static const int dv_weight_88[64] = {
 131072, 257107, 257107, 242189, 252167, 242189, 235923, 237536,
 237536, 235923, 229376, 231390, 223754, 231390, 229376, 222935,
 224969, 217965, 217965, 224969, 222935, 200636, 218652, 211916,
 212325, 211916, 218652, 200636, 188995, 196781, 205965, 206433,
 206433, 205965, 196781, 188995, 185364, 185364, 200636, 200704,
 200636, 185364, 185364, 174609, 180568, 195068, 195068, 180568,
 174609, 170091, 175557, 189591, 175557, 170091, 165371, 170627,
 170627, 165371, 160727, 153560, 160727, 144651, 144651, 136258,
};
static const int dv_weight_248[64] = {
 131072, 242189, 257107, 237536, 229376, 200636, 242189, 223754,
 224969, 196781, 262144, 242189, 229376, 200636, 257107, 237536,
 211916, 185364, 235923, 217965, 229376, 211916, 206433, 180568,
 242189, 223754, 224969, 196781, 211916, 185364, 235923, 217965,
 200704, 175557, 222935, 205965, 200636, 185364, 195068, 170627,
 229376, 211916, 206433, 180568, 200704, 175557, 222935, 205965,
 175557, 153560, 188995, 174609, 165371, 144651, 200636, 185364,
 195068, 170627, 175557, 153560, 188995, 174609, 165371, 144651,
};
static const int dv_weight_1080_y[64] = {
     8192, 65536, 65536, 61681, 61681, 61681, 58254, 58254,
    58254, 58254, 58254, 58254, 55188, 58254, 58254, 55188,
    55188, 55188, 55188, 55188, 55188, 24966, 27594, 26214,
    26214, 26214, 27594, 24966, 23831, 24385, 25575, 25575,
    25575, 25575, 24385, 23831, 23302, 23302, 24966, 24966,
    24966, 23302, 23302, 21845, 22795, 24385, 24385, 22795,
    21845, 21400, 21845, 23831, 21845, 21400, 10382, 10700,
    10700, 10382, 10082,  9620, 10082,  9039,  9039,  8525,
};
static const int dv_weight_1080_c[64] = {
     8192, 65536, 65536, 61681, 61681, 61681, 41943, 41943,
    41943, 41943, 40330, 41943, 40330, 41943, 40330, 40330,
    40330, 38836, 38836, 40330, 40330, 24966, 27594, 26214,
    26214, 26214, 27594, 24966, 23831, 24385, 25575, 25575,
    25575, 25575, 24385, 23831, 11523, 11523, 12483, 12483,
    12483, 11523, 11523, 10923, 11275, 12193, 12193, 11275,
    10923,  5323,  5490,  5924,  5490,  5323,  5165,  5323,
     5323,  5165,  5017,  4788,  5017,  4520,  4520,  4263,
};
static const int dv_weight_720_y[64] = {
     8192, 65536, 65536, 61681, 61681, 61681, 58254, 58254,
    58254, 58254, 58254, 58254, 55188, 58254, 58254, 55188,
    55188, 55188, 55188, 55188, 55188, 24966, 27594, 26214,
    26214, 26214, 27594, 24966, 23831, 24385, 25575, 25575,
    25575, 25575, 24385, 23831, 15420, 15420, 16644, 16644,
    16644, 15420, 15420, 10923, 11398, 12193, 12193, 11398,
    10923, 10700, 10923, 11916, 10923, 10700,  5191,  5350,
     5350,  5191,  5041,  4810,  5041,  4520,  4520,  4263,
};
static const int dv_weight_720_c[64] = {
     8192, 43691, 43691, 40330, 40330, 40330, 29127, 29127,
    29127, 29127, 29127, 29127, 27594, 29127, 29127, 27594,
    27594, 27594, 27594, 27594, 27594, 12483, 13797, 13107,
    13107, 13107, 13797, 12483, 11916, 12193, 12788, 12788,
    12788, 12788, 12193, 11916,  5761,  5761,  6242,  6242,
     6242,  5761,  5761,  5461,  5638,  5461,  6096,  5638,
     5461,  2661,  2745,  2962,  2745,  2661,  2583,  2661,
     2661,  2583,  2509,  2394,  2509,  2260,  2260,  2131,
};
static const int dv_inverse_shift = 32;
static const int dv_iweight_bits = 14;
static const int dv100_inverse_shift = 20;

static const uint8_t dv_audio_shuffle525[10][9] = {
  {  0, 30, 60, 20, 50, 80, 10, 40, 70 }, /* 1st channel */
  {  6, 36, 66, 26, 56, 86, 16, 46, 76 },
  { 12, 42, 72,  2, 32, 62, 22, 52, 82 },
  { 18, 48, 78,  8, 38, 68, 28, 58, 88 },
  { 24, 54, 84, 14, 44, 74,  4, 34, 64 },

  {  1, 31, 61, 21, 51, 81, 11, 41, 71 }, /* 2nd channel */
  {  7, 37, 67, 27, 57, 87, 17, 47, 77 },
  { 13, 43, 73,  3, 33, 63, 23, 53, 83 },
  { 19, 49, 79,  9, 39, 69, 29, 59, 89 },
  { 25, 55, 85, 15, 45, 75,  5, 35, 65 },
};

static const uint8_t dv_audio_shuffle625[12][9] = {
  {   0,  36,  72,  26,  62,  98,  16,  52,  88}, /* 1st channel */
  {   6,  42,  78,  32,  68, 104,  22,  58,  94},
  {  12,  48,  84,   2,  38,  74,  28,  64, 100},
  {  18,  54,  90,   8,  44,  80,  34,  70, 106},
  {  24,  60,  96,  14,  50,  86,   4,  40,  76},
  {  30,  66, 102,  20,  56,  92,  10,  46,  82},

  {   1,  37,  73,  27,  63,  99,  17,  53,  89}, /* 2nd channel */
  {   7,  43,  79,  33,  69, 105,  23,  59,  95},
  {  13,  49,  85,   3,  39,  75,  29,  65, 101},
  {  19,  55,  91,   9,  45,  81,  35,  71, 107},
  {  25,  61,  97,  15,  51,  87,   5,  41,  77},
  {  31,  67, 103,  21,  57,  93,  11,  47,  83},
};

static const av_unused int dv_audio_frequency[3] = {
    48000, 44100, 32000,
};

/* macroblock bit budgets */
static const uint8_t block_sizes_dv2550[8] = {
    112, 112, 112, 112, 80, 80, 0, 0,
};

static const uint8_t block_sizes_dv100[8] = {
    80, 80, 80, 80, 80, 80, 64, 64,
};

enum dv_section_type {
     dv_sect_header  = 0x1f,
     dv_sect_subcode = 0x3f,
     dv_sect_vaux    = 0x56,
     dv_sect_audio   = 0x76,
     dv_sect_video   = 0x96,
};

enum dv_pack_type {
     dv_header525     = 0x3f, /* see dv_write_pack for important details on */
     dv_header625     = 0xbf, /* these two packs */
     dv_timecode      = 0x13,
     dv_audio_source  = 0x50,
     dv_audio_control = 0x51,
     dv_audio_recdate = 0x52,
     dv_audio_rectime = 0x53,
     dv_video_source  = 0x60,
     dv_video_control = 0x61,
     dv_video_recdate = 0x62,
     dv_video_rectime = 0x63,
     dv_unknown_pack  = 0xff,
};

#define DV_PROFILE_IS_HD(p) ((p)->video_stype & 0x10)
#define DV_PROFILE_IS_1080i50(p) (((p)->video_stype == 0x14) && ((p)->dsf == 1))
#define DV_PROFILE_IS_720p50(p)  (((p)->video_stype == 0x18) && ((p)->dsf == 1))

/* minimum number of bytes to read from a DV stream in order to
   determine the profile */
#define DV_PROFILE_BYTES (6*80) /* 6 DIF blocks */

/**
 * largest possible DV frame, in bytes (1080i50)
 */
#define DV_MAX_FRAME_SIZE 576000

/**
 * maximum number of blocks per macroblock in any DV format
 */
#define DV_MAX_BPM 8

const DVprofile* ff_dv_frame_profile(const DVprofile *sys,
                                  const uint8_t* frame, unsigned buf_size);
const DVprofile* ff_dv_codec_profile(AVCodecContext* codec);

static inline int dv_write_dif_id(enum dv_section_type t, uint8_t chan_num,
                                  uint8_t seq_num, uint8_t dif_num,
                                  uint8_t* buf)
{
    buf[0] = (uint8_t)t;       /* Section type */
    buf[1] = (seq_num  << 4) | /* DIF seq number 0-9 for 525/60; 0-11 for 625/50 */
             (chan_num << 3) | /* FSC: for 50Mb/s 0 - first channel; 1 - second */
             7;                /* reserved -- always 1 */
    buf[2] = dif_num;          /* DIF block number Video: 0-134, Audio: 0-8 */
    return 3;
}


static inline int dv_write_ssyb_id(uint8_t syb_num, uint8_t fr, uint8_t* buf)
{
    if (syb_num == 0 || syb_num == 6) {
        buf[0] = (fr << 7) | /* FR ID 1 - first half of each channel; 0 - second */
                 (0  << 4) | /* AP3 (Subcode application ID) */
                 0x0f;       /* reserved -- always 1 */
    }
    else if (syb_num == 11) {
        buf[0] = (fr << 7) | /* FR ID 1 - first half of each channel; 0 - second */
                 0x7f;       /* reserved -- always 1 */
    }
    else {
        buf[0] = (fr << 7) | /* FR ID 1 - first half of each channel; 0 - second */
                 (0  << 4) | /* APT (Track application ID) */
                 0x0f;       /* reserved -- always 1 */
    }
    buf[1] = 0xf0 |            /* reserved -- always 1 */
             (syb_num & 0x0f); /* SSYB number 0 - 11   */
    buf[2] = 0xff;             /* reserved -- always 1 */
    return 3;
}

#endif /* AVCODEC_DVDATA_H */
