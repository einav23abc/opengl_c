#ifndef AUDIO_H
#define AUDIO_H

// pretty much just a wraper for SDL2_mixer
// https://wiki.libsdl.org/SDL2_mixer/CategoryAPI

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdint.h>
#include <stdio.h>

#define AUDIO_NO_FADING MIX_NO_FADING
#define AUDIO_FADING_OUT MIX_FADING_OUT
#define AUDIO_FADING_IN MIX_FADING_IN

typedef struct {
    const uint64_t sound_index;
    Mix_Chunk* sound;
} sound_t;

typedef struct {
    const uint64_t music_index;
    Mix_Music* music;
} music_t;


extern uint64_t sounds_amount;
extern sound_t* sounds_list[];
extern const uint64_t SOUNDS_MAX_AMOUNT;

extern uint64_t musics_amount;
extern music_t* musics_list[];
extern const uint64_t MUSICS_MAX_AMOUNT;


extern const int32_t AUDIO_RATE;
extern const uint16_t AUDIO_FORMAT;
extern const int32_t AUDIO_CHANNELS_AMOUNT;
extern const int32_t AUDIO_CHUNKSIZE;


void audio_init();

// Reserves channels `0` to `num-1`
// Sounds will not be played in these channels unless channel was specified using `audio_sound_play_at_channel` or `audio_sound_play_fade_in_at_channel`
void audio_reserve_channels(int32_t num);
void audio_set_channel_finished_callback(void(*callback)(int32_t channel));

// * \returns an allocated sound_t or `NULL` on failure
// *
// * All sounds are freed when calling `audio_clean()` but can be freed using `audio_sound_destroy(sound)`
sound_t* audio_sound_load(const char* path);
// * \returns an allocated sound_t or `NULL` on failure
// *
// * \param memory A pointer to a buffer containing WAV file data.
// *
// * All sounds are freed when calling `audio_clean()` but can be freed using `audio_sound_destroy(sound)`
sound_t* audio_sound_load_from_file_buffer(uint8_t* memory);
// * \returns an allocated sound_t or `NULL` on failure
// *
// * \param memory A pointer to a buffer containing the data.
// *
// * \param length The length of the buffer.
// *
// * All sounds are freed when calling `audio_clean()` but can be freed using `audio_sound_destroy(sound)`
sound_t* audio_sound_load_from_buffer(uint8_t* memory, uint32_t length);
// * \returns an allocated music_t or `NULL` on failure
// *
// * All musics are freed when calling `audio_clean()` but can be freed using `audio_music_destroy(music)`
music_t* audio_music_load(const char* path);

// \returns the channel the sound is playing in. or `-1` if not playing
int32_t audio_sound_play(sound_t* sound);
// \returns the channel the sound is playing in. or `-1` if not playing
int32_t audio_sound_play_at_channel(int32_t channel, sound_t* sound);
void audio_music_play(music_t* music);
// \returns the channel the sound is playing in. or `-1` if not playing
int32_t audio_sound_play_fade_in(sound_t* sound, int32_t ms);
// \returns the channel the sound is playing in. or `-1` if not playing
int32_t audio_sound_play_fade_in_at_channel(int32_t channel, sound_t* sound, int32_t ms);
void audio_music_play_fade_in(music_t* music, int32_t ms);
void audio_music_play_fade_in_at_position(music_t* music, int32_t ms, double pos_s);

void audio_set_music_position(double pos_s);
void audio_rewind_music();

void audio_sound_set_reverse_stereo(int32_t channel, int32_t flip);

void audio_pause_channel(int32_t channel);
void audio_pause_music();
void audio_pause_output();
void audio_resume_channel(int32_t channel);
void audio_resume_music();
void audio_resume_output();

// \returns the channel's volume - between `0` and `1`
float audio_get_channel_volume(int32_t channel);
// \returns the master volume of all channels - between `0` and `1`
float audio_get_master_volume();
// \returns the sound's volume - between `0` and `1`
float audio_get_sound_volume(sound_t* sound);
// \returns the playing music's volume - between `0` and `1`
float audio_get_music_volume();
// \param volume between `0` and `1`
void audio_set_channel_volume(int32_t channel, float volume);
// \param volume between `0` and `1`
void audio_set_master_volume(float volume);
// \param volume between `0` and `1`
void audio_set_sound_volume(sound_t* sound, float volume);
// \param volume between `0` and `1`
void audio_set_music_volume(float volume);

void audio_set_sound_distance(int32_t channel, uint8_t distance);
void audio_set_sound_panning(int32_t channel, uint8_t left, uint8_t right);
void audio_set_sound_position(int32_t channel, int16_t angle, uint8_t distance);

void audio_fade_out_channel(int32_t channel, int32_t ms);
void audio_fade_out_music(int32_t ms);
void audio_halt_channel(int32_t channel);
void audio_halt_music();

int32_t audio_is_channel_playing(int32_t channel);
int32_t audio_is_music_playing();
int32_t audio_is_channel_paused(int32_t channel);
int32_t audio_is_music_paused();
// \returns `AUDIO_NO_FADING`, `AUDIO_FADING_OUT` or `AUDIO_FADING_IN`
int32_t audio_is_channel_fading(int32_t channel);
// \returns `AUDIO_NO_FADING`, `AUDIO_FADING_OUT` or `AUDIO_FADING_IN`
int32_t audio_is_music_fading();

void audio_sound_destroy(sound_t* sound);
void audio_music_destroy(music_t* music);
// * Should be called before exiting.
// * Destroys all created sounds and musics.
// * All audio functions will have undefined behavior until `audio_init()` is recalled.
void audio_clean();

/* \brief Save a sound_t into a .c file.
 *
 * Can be used without having to load from a file.
 * 
 * Unlike sounds created by the `audio_sound_load` function, This sound does not need to be freed and will not be freed by the engine.
 * 
 * \param sound The sound to save.
 * 
 * \param name The name that will be given to the sound. This will be `sound_t* name`.
 * 
 * \param c_file_path The path to the c file where the sound will be saved.
 */
void audio_save_sound_to_c_file(sound_t* sound, const char* name, char* c_file_path);

#endif