#include "audio.h"

// pretty much just a wraper for SDL2_mixer
// https://wiki.libsdl.org/SDL2_mixer/CategoryAPI


#ifndef _SOUNDS_MAX_AMOUNT_
#define _SOUNDS_MAX_AMOUNT_ (128)
#endif
uint64_t sounds_amount = 0;
sound_t* sounds_list[_SOUNDS_MAX_AMOUNT_];
const uint64_t SOUNDS_MAX_AMOUNT = _SOUNDS_MAX_AMOUNT_;

#ifndef _MUSICS_MAX_AMOUNT_
#define _MUSICS_MAX_AMOUNT_ (128)
#endif
uint64_t musics_amount = 0;
music_t* musics_list[_MUSICS_MAX_AMOUNT_];
const uint64_t MUSICS_MAX_AMOUNT = _MUSICS_MAX_AMOUNT_;


#ifndef _AUDIO_RATE_
#define _AUDIO_RATE_ (44100)
#endif
const int32_t AUDIO_RATE = _AUDIO_RATE_;
#ifndef _AUDIO_FORMAT_
#define _AUDIO_FORMAT_ (MIX_DEFAULT_FORMAT)
#endif
const uint16_t AUDIO_FORMAT = MIX_DEFAULT_FORMAT;
#ifndef _AUDIO_CHANNELS_AMOUNT_
#define _AUDIO_CHANNELS_AMOUNT_ (8)
#endif
const int32_t AUDIO_CHANNELS_AMOUNT = _AUDIO_CHANNELS_AMOUNT_;
#ifndef _AUDIO_CHUNKSIZE_
#define _AUDIO_CHUNKSIZE_ (2048)
#endif
const int32_t AUDIO_CHUNKSIZE = _AUDIO_CHUNKSIZE_;


void audio_init() {
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS | MIX_INIT_WAVPACK);

    if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS_AMOUNT, AUDIO_CHUNKSIZE)) {
        printf("SDL_Mixer error: OpenAudio %s\n", Mix_GetError());
    }
}

void audio_reserve_channels(int32_t num) {
    Mix_ReserveChannels(num);
}
void audio_set_channel_finished_callback(void(*callback)(int32_t channel)) {
    Mix_ChannelFinished(callback);
}

sound_t* audio_sound_load(const char* path) {
    if (sounds_amount >= SOUNDS_MAX_AMOUNT) return NULL;

    sound_t* sound = malloc(sizeof(sound_t));
    if (sound == NULL) return NULL;
    
    sound->sound = Mix_LoadWAV(path);
    if (sound->sound == NULL) {
        printf("Failed to load WAV \"%s\", with error: %s\n", path, Mix_GetError());
        free(sound);
        return NULL;
    }

    // append to sounds_list
    *((uint64_t*)&sound->sound_index) = sounds_amount;
    sounds_list[sounds_amount] = sound;
    sounds_amount += 1;
    
    return sound;
}
sound_t* audio_sound_load_from_file_buffer(uint8_t* memory) {
    if (sounds_amount >= SOUNDS_MAX_AMOUNT) return NULL;

    sound_t* sound = malloc(sizeof(sound_t));
    if (sound == NULL) return NULL;
    
    sound->sound = Mix_QuickLoad_WAV(memory);
    if (sound->sound == NULL) {
        printf("Failed to load WAV at %p, with error: %s\n", memory, Mix_GetError());
        free(sound);
        return NULL;
    }

    // append to sounds_list
    *((uint64_t*)&sound->sound_index) = sounds_amount;
    sounds_list[sounds_amount] = sound;
    sounds_amount += 1;
    
    return sound;
}
sound_t* audio_sound_load_from_buffer(uint8_t* memory, uint32_t length) {
    if (sounds_amount >= SOUNDS_MAX_AMOUNT) return NULL;

    sound_t* sound = malloc(sizeof(sound_t));
    if (sound == NULL) return NULL;
    
    sound->sound = Mix_QuickLoad_RAW(memory, length);
    if (sound->sound == NULL) {
        printf("Failed to load WAV at %p, with error: %s\n", memory, Mix_GetError());
        free(sound);
        return NULL;
    }

    // append to sounds_list
    *((uint64_t*)&sound->sound_index) = sounds_amount;
    sounds_list[sounds_amount] = sound;
    sounds_amount += 1;
    
    return sound;
}
music_t* audio_music_load(const char* path) {
    if (musics_amount >= MUSICS_MAX_AMOUNT) return NULL;

    music_t* music = malloc(sizeof(music_t));
    if (music == NULL) return NULL;

    music->music = Mix_LoadMUS(path);
    if (music->music == NULL) {
        printf("Failed to load music file: %s\n", Mix_GetError());
        free(music);
        return NULL;
    }

    // append to musics_list
    *((uint64_t*)&music->music_index) = musics_amount;
    musics_list[musics_amount] = music;
    musics_amount += 1;

    return music;
}

int32_t audio_sound_play(sound_t* sound) {
    return Mix_PlayChannel(-1, sound->sound, 0);
}
int32_t audio_sound_play_at_channel(int32_t channel, sound_t* sound) {
    return Mix_PlayChannel(channel, sound->sound, 0);
}
void audio_music_play(music_t* music) {
    Mix_PlayMusic(music->music, -1);
}
int32_t audio_sound_play_fade_in(sound_t* sound, int32_t ms) {
    return Mix_FadeInChannel(-1, sound->sound, 0, ms);
}
int32_t audio_sound_play_fade_in_at_channel(int32_t channel, sound_t* sound, int32_t ms) {
    return Mix_FadeInChannel(channel, sound->sound, 0, ms);
}
void audio_music_play_fade_in(music_t* music, int32_t ms) {
    Mix_FadeInMusic(music->music, -1, ms);
}
void audio_music_play_fade_in_at_position(music_t* music, int32_t ms, double pos_s) {
    Mix_FadeInMusicPos(music->music, -1, ms, pos_s);
}

void audio_set_music_position(double pos_s) {
    Mix_SetMusicPosition(pos_s);
}
void audio_rewind_music() {
    Mix_RewindMusic();
}

void audio_sound_set_reverse_stereo(int32_t channel, int32_t flip) {
    Mix_SetReverseStereo(channel, flip);
}

void audio_pause_channel(int32_t channel) {
    Mix_Pause(channel);
}
void audio_pause_music() {
    Mix_PauseMusic();
}
void audio_pause_output() {
    Mix_PauseAudio(1);
}
void audio_resume_channel(int32_t channel) {
    Mix_Resume(channel);
}
void audio_resume_music() {
    Mix_ResumeMusic();
}
void audio_resume_output() {
    Mix_PauseAudio(0);
}

float audio_get_channel_volume(int32_t channel) {
    return ((float)Mix_Volume(channel, -1))*MIX_MAX_VOLUME;
}
float audio_get_master_volume() {
    return ((float)Mix_MasterVolume(-1))*MIX_MAX_VOLUME;
}
float audio_get_sound_volume(sound_t* sound) {
    return ((float)Mix_VolumeChunk(sound->sound, -1))*MIX_MAX_VOLUME;
}
float audio_get_music_volume() {
    return ((float)Mix_VolumeMusic(-1))*MIX_MAX_VOLUME;
}
void audio_set_channel_volume(int32_t channel, float volume) {
    Mix_Volume(channel, (int32_t)(volume*MIX_MAX_VOLUME));
}
void audio_set_master_volume(float volume) {
    Mix_MasterVolume((int32_t)(volume*MIX_MAX_VOLUME));
}
void audio_set_sound_volume(sound_t* sound, float volume) {
    Mix_VolumeChunk(sound->sound, (int32_t)(volume*MIX_MAX_VOLUME));
}
void audio_set_music_volume(float volume) {
    Mix_VolumeMusic((int32_t)(volume*MIX_MAX_VOLUME));
}

void audio_set_sound_distance(int32_t channel, uint8_t distance) {
    Mix_SetDistance(channel, distance);
}
void audio_set_sound_panning(int32_t channel, uint8_t left, uint8_t right) {
    Mix_SetPanning(channel, left, right);
}
void audio_set_sound_position(int32_t channel, int16_t angle, uint8_t distance) {
    Mix_SetPosition(channel, angle, distance);
}

void audio_fade_out_channel(int32_t channel, int32_t ms) {
    Mix_FadeOutChannel(channel, ms);
}
void audio_fade_out_music(int32_t ms) {
    Mix_FadeOutMusic(ms);
}
void audio_halt_channel(int32_t channel) {
    Mix_HaltChannel(channel);
}
void audio_halt_music() {
    Mix_HaltMusic();
}

int32_t audio_is_channel_playing(int32_t channel) {
    return Mix_Playing(channel);
}
int32_t audio_is_music_playing() {
    return Mix_PlayingMusic();
}
int32_t audio_is_channel_paused(int32_t channel) {
    return Mix_Paused(channel);
}
int32_t audio_is_music_paused() {
    return Mix_PausedMusic();
}
int32_t audio_is_channel_fading(int32_t channel) {
    return Mix_FadingChannel(channel);
}
int32_t audio_is_music_fading() {
    return Mix_FadingMusic();
}

static void audio_sound_clean(sound_t* sound) {
    if (sound == NULL) return;

    Mix_FreeChunk(sound->sound);
    free(sound);
}
static void audio_music_clean(music_t* music) {
    if (music == NULL) return;

    Mix_FreeMusic(music->music);
    free(music);
}
void audio_sound_destroy(sound_t* sound) {
    if (sound == NULL) return;

    sounds_amount -= 1;

    // move the last sound at sounds_list to sound's index
    sound_t* last_sound = sounds_list[sounds_amount];
    *((uint64_t*)&last_sound->sound_index) = sound->sound_index;
    sounds_list[sound->sound_index] = last_sound;

    audio_sound_clean(sound);
}
void audio_music_destroy(music_t* music) {
    if (music == NULL) return;

    musics_amount -= 1;

    // move the last music at musics_list to music's index
    music_t* last_music = musics_list[musics_amount];
    *((uint64_t*)&last_music->music_index) = music->music_index;
    musics_list[music->music_index] = last_music;

    audio_music_clean(music);
}
void audio_clean() {
    #ifdef DEBUG_SOFT_MODE
    printf("cleaning %u sounds\n", sounds_amount);
    #endif
    for (uint64_t i = 0; i < sounds_amount; i++) audio_sound_clean(sounds_list[i]);
    sounds_amount = 0;
    
    #ifdef DEBUG_SOFT_MODE
    printf("cleaning %u musics\n", musics_amount);
    #endif
    for (uint64_t i = 0; i < musics_amount; i++) audio_music_clean(musics_list[i]);
    musics_amount = 0;

    Mix_CloseAudio();
    Mix_Quit();
}

void audio_save_sound_to_c_file(sound_t* sound, const char* name, char* c_file_path) {
    FILE* fp;
    fp = fopen(c_file_path, "w");
    if (fp == NULL) {
        #ifdef DEBUG_SOFT_MODE
        printf("failed to open file \"%s\" to save surface \"%s\" to.\n", c_file_path, name);
        #endif
        return;
    }
    
    printf("\nprinting sound for saving\n");

    fprintf(fp,
        ""      "static uint8_t %s_buf[%u] = {"
        ,
        name,
        sound->sound->alen
    );
    for (uint32_t i = 0; i < sound->sound->alen; i++) {
        if (i != 0) fprintf(fp, ", ");
        if (i%20 == 0) fprintf(fp, "\n\t");
        fprintf(fp, "%hhu", sound->sound->abuf[i]);
    }

    fprintf(fp,
        ""      "\n};\n"
        ""      "static Mix_Chunk %s_mix_chunk = (Mix_Chunk){\n"
        "\t"        ".allocated = 0,\n"
        "\t"        ".abuf = %s_buf,\n"
        "\t"        ".alen = %u,\n"
        "\t"        ".volume = %hhu\n"
        ""      "};\n"
        ""      "static sound_t %s_sound = (sound_t){\n"
        "\t"        ".sound_index = -1,\n"
        "\t"        ".sound = &%s_mix_chunk\n"
        ""      "};\n"
        ""      "sound_t* %s = &%s_sound;"
        ,
        name,
        name,
        sound->sound->alen,
        sound->sound->volume,
        name,
        name,
        name,
        name
    );

    fclose(fp);
}