#ifndef GUARD_SOUNDWRAPPER_H
#define GUARD_SOUNDWRAPPER_H

// #include "m4a.h"?

// ??? Sound_GetCurrentSong(???);
// ??? sub_8002264(???);
// ??? Sound_SetVolume8002274(???);
void Sound_SetVolume80022EC(int volume);
void Sound_FadeOut800231C(int speed);
// ??? Sound_FadeOut800237C(???);
// ??? SoundStuff_80023E0(???);
// ??? Sound_PlaySong8002448(???);
// ??? PlaySong8002478(???);
// ??? Sound_PlaySong80024D4(???);
void Sound_PlaySong80024E4(int songId, int speed, struct MusicPlayerInfo *player);
// ??? sub_80024F0(???);
void Sound_PlaySong8002574(int songId, int b, struct MusicPlayerInfo *player);
void sub_8002620(int songId);
// ??? sub_8002670(???);
void sub_80026BC(u16 speed);
// ??? sub_800270C(???);
void ISuspectThisToBeMusicRelated_8002730(int volume, int b, int c, struct Proc *parent);
// ??? sub_8002788(???);
// ??? Some6CMusicRelatedWaitCallback(???);
void StartSongDelayed();
void PlaySong();
void Sound_SetDefaultMaxNumChannels();
void Sound_SetMaxNumChannels();
void sub_80028FC(int songId);
// ??? IsMusicProc2Running(???);
// ??? sub_800296C(???);
// ??? sub_80029BC(???);
// ??? sub_80029E8(???);
// ??? sub_8002A6C(???);
// ??? sub_8002A88(???);
void DeleteAll6CWaitMusicRelated();
// ??? sub_8002AC8(???);

#define PlaySoundEffect(id) \
    if (!gUnknown_0202BCF0.unk41_2) \
        m4aSongNumStart((id))

#endif  // GUARD_SOUNDWRAPPER_H
