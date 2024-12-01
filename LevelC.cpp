#include "LevelC.h"
#include "Utility.h"
#include <windows.h>

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 11
#define ENEMY_COUNT 1

const char SPRITESHEET_FILEPATH[] = "assets/mario.png";
constexpr char FONTSHEET_FILEPATH[] = "assets/font1.png";

GLuint g_font_texture_id_C;

// DATA FOR MAP
unsigned int LEVELC_DATA[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3,
    3, 0, 0, 0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3,
    3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3
};

LevelC::~LevelC()
{
    delete[] g_state.enemies;
    delete    g_state.player;
    delete    g_state.map;
    delete   g_state.fireball;
    Mix_FreeChunk(g_state.jump_sfx);
    Mix_FreeMusic(g_state.music);
}

void LevelC::initialise()
{

    // ----- MAP ----- //
    GLuint map_texture_id = Utility::load_texture("assets/new2_tileset.png");

    g_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 4, 1);

    // ----- FONT ----- //
    g_font_texture_id_C = Utility::load_texture(FONTSHEET_FILEPATH);

    // ----- PLAYER ----- //
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    int player_walking_animation[3][4] =
    {
        { 4, 5, 6, 7 },
        { 0, 1, 2, 3 },
        { 8, 9, 10, 11 }
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -18.0f, 0.0f);

    g_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        3.5f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        3,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        0.75f,                      // width
        1.0f,                       // height
        PLAYER
    );

    // Initial Position
    g_state.player->set_position(glm::vec3(1.0f, -5.0f, 0.0f));
    g_state.player->set_start_position(glm::vec3(1.0f, -5.0f, 0.0f));

    // Lives
    //g_state.player->set_num_lives(g_state.player->get_num_lives());


    // Jumping
    g_state.player->set_jumping_power(10.0f);


    // ----- ENEMY: KOOPA (Count: 1) ----- //
    GLuint goomba_texture_id = Utility::load_texture("assets/goomba.png");

    g_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        g_state.enemies[i] = Entity(
            goomba_texture_id,    // texture id
            1.0f,               // speed
            0.75f,              // width
            0.75f,               // height
            ENEMY,              // entity type
            GUARD,              // AI type
            IDLE                // initial AI state
        );
    }

    g_state.enemies[0].set_position(glm::vec3(7.0f, -2.0f, 0.0f));
    g_state.enemies[0].set_acceleration(glm::vec3(0.0f, -5.0f, 0.0f));

    // ----- BACKGROUND MUSIC ----- //
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    //g_state.music = Mix_LoadMUS("assets/goblins.mp3");
    //Mix_PlayMusic(g_state.music, -1);
    //// Volume
    //Mix_VolumeMusic(64.0f);

    //// ----- JUMP SOUND EFFECT ----- //
    //g_state.jump_sfx = Mix_LoadWAV("assets/jump.wav");
}

void LevelC::update(float delta_time)
{
    g_state.player->update(delta_time, g_state.player, g_state.enemies, ENEMY_COUNT, g_state.map);

    // COLLISION LOGIC
    if (g_state.player->get_lose_life()) {
        num_lives--;
    }
    if (num_lives == 0) {
        g_state.player->set_touched();
    }

    // KILL LOGIC
    if (g_state.player->get_killed_enemy()) {
        kill_count++;
    }

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        g_state.enemies[i].update(delta_time, g_state.player, g_state.player, 1, g_state.map);
    }
}


void LevelC::render(ShaderProgram* g_shader_program)
{
    g_state.map->render(g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        g_state.enemies[i].render(g_shader_program);

    if (g_state.player) {
        g_state.player->render(g_shader_program);

        //// Tutorial Text
        //Utility::draw_text(g_shader_program, g_font_texture_id_C, "Press C to Fireball", 0.25f, 0.01f,
        //    glm::vec3(2.5f, -6.5f, 0.0f));

        //Utility::draw_text(g_shader_program, g_font_texture_id_C, std::to_string(kill_count), 0.25f, 0.01f,
        //    glm::vec3(g_state.player->get_position().x, g_state.player->get_position().y + 1.0f, 0.0f));

        Utility::draw_text(g_shader_program, g_font_texture_id_C, "Lives: " + std::to_string(num_lives), 0.2f, 0.01f,
            glm::vec3(g_state.player->get_position().x - 0.65f, g_state.player->get_position().y + 1.0f, 0.0f));
        
        // Conditional Win / Lose Text
        if (kill_count == 3) {
            Utility::draw_text(g_shader_program, g_font_texture_id_C, "YOU WIN!", 0.5f, 0.05f,
                glm::vec3(g_state.player->get_position().x - 2.0f, g_state.player->get_position().y + 2.0f, 0.0f));
        }
        if (kill_count < 3 && g_state.player->get_touched()) {
            Utility::draw_text(g_shader_program, g_font_texture_id_C, "YOU LOSE!", 0.5f, 0.05f,
                glm::vec3(g_state.player->get_position().x - 2.0f, g_state.player->get_position().y + 2.0f, 0.0f));
        }
    }
}

