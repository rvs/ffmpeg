#!/bin/sh
TEST_CASES="dv25ntsc.dv dv25pal.dv dv50ntsc.dv dv50pal.dv voxnews.dv \
            armando/1080i50.mov  armando/1080i60.mov  armando/1080p24.mov  armando/1080p30.mov \
            armando/720p24.mov  armando/720p25.mov  armando/720p30.mov  armando/720p50.mov  armando/720p60.mov"

for i in $TEST_CASES ; do
   echo =============== staring $i ===================== 
   ./ffmpeg -y -t 1 -i ~/DV/$i test-`basename $i`.dv
   ./ffplay test-`basename $i`.dv 
   echo --------------- finished $i --------------------
done
