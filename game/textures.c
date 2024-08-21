#include "textures.h"

#ifndef TEXTURES_FROM_FILES
#include "./textures/floor_texture_surface.c"
#include "./textures/global_texture_surface.c"
#include "./textures/tile_texture_surface.c"
#include "./textures/field_tile_texture_surface.c"
#include "./textures/forest_tile_texture_surface.c"
#include "./textures/mine_tile_texture_surface.c"
#include "./textures/house_tile_texture_surface.c"
#include "./textures/barracks_tile_texture_surface.c"
#include "./textures/attack_effect_texture_surface.c"
#include "./textures/letters_font_texture_surface.c"
#include "./textures/big_letters_font_texture_surface.c"
#include "./textures/nine_slice1_texture_surface.c"
#include "./textures/nine_slice2_texture_surface.c"
#include "./textures/nine_slice3_texture_surface.c"
#include "./textures/menu_background_scroll_texture_surface.c"
#endif
texture_t* floor_texture;
texture_t* global_texture;
texture_t* tile_texture;
texture_t* field_tile_texture;
texture_t* forest_tile_texture;
texture_t* mine_tile_texture;
texture_t* house_tile_texture;
texture_t* barracks_tile_texture;
texture_t* attack_effect_texture;
texture_t* letters_font_texture;
texture_t* big_letters_font_texture;
texture_t* nine_slice1_texture;
texture_t* nine_slice2_texture;
texture_t* nine_slice3_texture;
texture_t* menu_background_scroll_texture;

void game_textures_init() {
    #ifdef TEXTURES_FROM_FILES
    floor_texture = load_texture("./game/textures/floor.png");
    global_texture = load_texture("./game/textures/global_texture.png");
    tile_texture = load_texture("./game/textures/tile_texture.png");
    field_tile_texture = load_texture("./game/textures/field_tile_texture.png");
    forest_tile_texture = load_texture("./game/textures/forest_tile_texture.png");
    mine_tile_texture = load_texture("./game/textures/mine_tile_texture.png");
    house_tile_texture = load_texture("./game/textures/house_tile_texture.png");
    barracks_tile_texture = load_texture("./game/textures/barracks_tile_texture.png");
    attack_effect_texture = load_texture("./game/textures/attack_effect.png");

    letters_font_texture = load_texture("./game/textures/font.png");
    big_letters_font_texture = load_texture("./game/textures/font_big.png");
    nine_slice1_texture = load_texture("./game/textures/nslice1.png");;
    nine_slice2_texture = load_texture("./game/textures/nslice2.png");
    nine_slice3_texture = load_texture("./game/textures/nslice3.png");
    menu_background_scroll_texture = load_texture("./game/textures/menu_background_scroll.png");

    save_surface_to_c_file("./game/textures/floor.png", "floor_texture_surface", "./game/textures/floor_texture_surface.c");
    save_surface_to_c_file("./game/textures/global_texture.png", "global_texture_surface", "./game/textures/global_texture_surface.c");
    save_surface_to_c_file("./game/textures/tile_texture.png", "tile_texture_surface", "./game/textures/tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/field_tile_texture.png", "field_tile_texture_surface", "./game/textures/field_tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/forest_tile_texture.png", "forest_tile_texture_surface", "./game/textures/forest_tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/mine_tile_texture.png", "mine_tile_texture_surface", "./game/textures/mine_tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/house_tile_texture.png", "house_tile_texture_surface", "./game/textures/house_tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/barracks_tile_texture.png", "barracks_tile_texture_surface", "./game/textures/barracks_tile_texture_surface.c");
    save_surface_to_c_file("./game/textures/attack_effect.png", "attack_effect_texture_surface", "./game/textures/attack_effect_texture_surface.c");

    save_surface_to_c_file("./game/textures/font.png", "letters_font_texture_surface", "./game/textures/letters_font_texture_surface.c");
    save_surface_to_c_file("./game/textures/font_big.png", "big_letters_font_texture_surface", "./game/textures/big_letters_font_texture_surface.c");
    save_surface_to_c_file("./game/textures/nslice1.png", "nine_slice1_texture_surface", "./game/textures/nine_slice1_texture_surface.c");
    save_surface_to_c_file("./game/textures/nslice2.png", "nine_slice2_texture_surface", "./game/textures/nine_slice2_texture_surface.c");
    save_surface_to_c_file("./game/textures/nslice3.png", "nine_slice3_texture_surface", "./game/textures/nine_slice3_texture_surface.c");
    save_surface_to_c_file("./game/textures/menu_background_scroll.png", "menu_background_scroll_texture_surface", "./game/textures/menu_background_scroll_texture_surface.c");
    #else
    floor_texture = load_texture_from_surface(floor_texture_surface);
    global_texture = load_texture_from_surface(global_texture_surface);
    tile_texture = load_texture_from_surface(tile_texture_surface);
    field_tile_texture = load_texture_from_surface(field_tile_texture_surface);
    forest_tile_texture = load_texture_from_surface(forest_tile_texture_surface);
    mine_tile_texture = load_texture_from_surface(mine_tile_texture_surface);
    house_tile_texture = load_texture_from_surface(house_tile_texture_surface);
    barracks_tile_texture = load_texture_from_surface(barracks_tile_texture_surface);
    attack_effect_texture = load_texture_from_surface(attack_effect_texture_surface);

    letters_font_texture = load_texture_from_surface(letters_font_texture_surface);
    big_letters_font_texture = load_texture_from_surface(big_letters_font_texture_surface);
    nine_slice1_texture = load_texture_from_surface(nine_slice1_texture_surface);
    nine_slice2_texture = load_texture_from_surface(nine_slice2_texture_surface);
    nine_slice3_texture = load_texture_from_surface(nine_slice3_texture_surface);
    menu_background_scroll_texture = load_texture_from_surface(menu_background_scroll_texture_surface);

    #endif
}