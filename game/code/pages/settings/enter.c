#include "page.h"
#include "../../game.h"
#include "../../ui_lists.h"
#include "../../alerts.h"


settings_t settings = (settings_t){
    .master_volume = 1,
    .music_volume = 1,
    .sound_volume = 1,
};


static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id);
static void audio_settings_button_callback(int32_t ui_list_id, int32_t button_id);
void volume_change_slider_callback(int32_t ui_list_id, int32_t button_id);


void enter_settings() {
    close_all_ui_lists();
    close_all_alerts();
    
    int32_t ui_list_id;

    // main menu
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_ - (strlen("Main menu")*letters_font.letter_width + 5*2) - 1,
        _OUTPORT_HEIGHT_ - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &main_menu_button_callback,
                .string = "Main menu",
                .font = &letters_font,
                .padding = 5,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );

    // settings main tab
    ui_list_id = new_ui_list(
        _OUTPORT_WIDTH_*0.5 - (strlen("Audio")*big_letters_font.letter_width + 4*2) - 128*0.5 - 1,
        _OUTPORT_HEIGHT_*0.5 + (big_letters_font.letter_height + 4*2)*0.5 - 1,
        1,
        &nine_slice2
    );
    make_ui_list_permenant(ui_list_id);
    ui_list_add_element(
        ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = &audio_settings_button_callback,
                .string = "Audio",
                .font = &big_letters_font,
                .padding = 4,
                .nslice = NULL,
                .hover_nslice = &nine_slice3,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
}

static void main_menu_button_callback(int32_t ui_list_id, int32_t button_id) {
    switch_page(PAGE_MAIN_MENU);
}
static void audio_settings_button_callback(int32_t ui_list_id, int32_t button_id) {
    // audio settings list
    int32_t new_ui_list_id = new_ui_list_child(
        ui_list_id,
        (strlen("Audio")*big_letters_font.letter_width + 4*2 + 1) + 6 - 1,
        1 - 6,
        6,
        &nine_slice1
    );

    // music volume
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_SLIDER,
            .slider = {
                .value = &(settings.music_volume),
                .callback = &volume_change_slider_callback,
                .width = 128,
                .height = 7,
                .padding = 5,
                .nslice = NULL,
                .pin_width = 11,
                .pin_height = 11,
                .pin_nslice = &nine_slice_slider_pin,
                .full_nslice = &nine_slice_slider_full,
                .empty_nslice = &nine_slice_slider_empty,
                
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = " Music volume:",
                .font = &letters_font,
                .padding = 0,
                .nslice = NULL,
                .hover_nslice = NULL,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );

    // sounds volume
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_SLIDER,
            .slider = {
                .value = &(settings.sound_volume),
                .callback = &volume_change_slider_callback,
                .width = 128,
                .height = 7,
                .padding = 5,
                .nslice = NULL,
                .pin_width = 11,
                .pin_height = 11,
                .pin_nslice = &nine_slice_slider_pin,
                .full_nslice = &nine_slice_slider_full,
                .empty_nslice = &nine_slice_slider_empty,
                
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = " Sound effects volume:",
                .font = &letters_font,
                .padding = 0,
                .nslice = NULL,
                .hover_nslice = NULL,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );


    // master volume
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_SLIDER,
            .slider = {
                .value = &(settings.master_volume),
                .callback = &volume_change_slider_callback,
                .width = 128,
                .height = 7,
                .padding = 5,
                .nslice = NULL,
                .pin_width = 11,
                .pin_height = 11,
                .pin_nslice = &nine_slice_slider_pin,
                .full_nslice = &nine_slice_slider_full,
                .empty_nslice = &nine_slice_slider_empty,
                
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );
    ui_list_add_element(
        new_ui_list_id,
        (ui_list_element_t){
            .type = ELEMENT_TYPE_BUTTON,
            .button = {
                .callback = NULL,
                .string = " Master volume:",
                .font = &letters_font,
                .padding = 0,
                .nslice = NULL,
                .hover_nslice = NULL,
                .info_string = "",
                .info_string_font = NULL,
                .info_string_padding = 0,
                .info_string_nslice = NULL
            }
        }
    );

    make_ui_list_safe(new_ui_list_id);
}

void volume_change_slider_callback(int32_t ui_list_id, int32_t button_id) {
    audio_set_master_volume(settings.master_volume);
    audio_set_music_volume(settings.music_volume*settings.master_volume);
    audio_set_channel_volume(-1, settings.sound_volume);
}