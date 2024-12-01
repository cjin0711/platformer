#include "Menu.h"
#include "Utility.h"
#include <windows.h>

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 11
#define ENEMY_COUNT 1

constexpr char FONTSHEET_FILEPATH[] = "assets/font1.png";

GLuint g_font_texture_id_Menu;

// DATA FOR MAP
unsigned int MENU_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};


void Menu::initialise()
{

    // ----- MAP ----- //
    GLuint map_texture_id = Utility::load_texture("assets/new2_tileset.png");

    g_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 4, 1);

    // ----- FONT ----- //
    g_font_texture_id_Menu = Utility::load_texture(FONTSHEET_FILEPATH);

    // ----- BACKGROUND MUSIC ----- //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    g_state.music = Mix_LoadMUS("assets/goblins.mp3");
    Mix_PlayMusic(g_state.music, -1);
    // Volume
    Mix_VolumeMusic(64.0f);

    g_state.player = new Entity();
    g_state.player->deactivate();
}

void Menu::update(float delta_time) {

    g_state.player->update(delta_time, g_state.player, g_state.enemies, ENEMY_COUNT, g_state.map);
}


void Menu::render(ShaderProgram* g_shader_program)
{
    g_state.map->render(g_shader_program);

    Utility::draw_text(g_shader_program, g_font_texture_id_Menu, "MARIO ADVENTURE", 0.5f, 0.01f,
        glm::vec3(-3.5f, 0.0f, 0.0f));

    Utility::draw_text(g_shader_program, g_font_texture_id_Menu, "Press Enter", 0.3f, 0.01f,
        glm::vec3(-1.5f, -2.0f, 0.0f));
    
}
