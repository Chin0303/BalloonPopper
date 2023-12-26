#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <stdlib.h>
#include <asndlib.h>
#include <vector>
#include <ctime>
#include <string>

#include "oggplayer.h"
#include "hand_png.h"
#include "balloon_pop_ogg.h"

#include "blue_balloon_png.h"
#include "green_balloon_png.h"
#include "red_balloon_png.h"
#include "golden_balloon_png.h"
#include "purple_balloon_png.h"

#define White 0xFFFFFFFF

#define click()    (WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_A)

int main() {
    GRRLIB_Init();
    WPAD_Init();
    ASND_Init();

    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    GRRLIB_texImg *tex_hand_png = GRRLIB_LoadTexture(hand_png);

    GRRLIB_texImg *blue_balloon_texture = GRRLIB_LoadTexture(blue_balloon_png);
    GRRLIB_texImg *red_balloon_texture = GRRLIB_LoadTexture(red_balloon_png);
    GRRLIB_texImg *green_balloon_texture = GRRLIB_LoadTexture(green_balloon_png);
    GRRLIB_texImg *purple_balloon_texture = GRRLIB_LoadTexture(purple_balloon_png);
    GRRLIB_texImg *golden_balloon_texture = GRRLIB_LoadTexture(golden_balloon_png);

    GRRLIB_texImg* balloonTextures[] = {
        blue_balloon_texture,
        red_balloon_texture,
        green_balloon_texture,
        purple_balloon_texture,
        golden_balloon_texture
    };

    int x_spawnedballoon = 0;
    int y_spawnedballoon = 0;
    int x_hand = 0;
    int y_hand = 0;

    float scale_factor = 0.5;

    std::time_t lastBalloonSpawnTime = std::time(nullptr);
    int playerScore = 0;

    GRRLIB_texImg* currentBalloon = nullptr;

    while (1) {
        WPAD_ScanPads();
        ir_t ir;
        WPAD_IR(WPAD_CHAN_0, &ir);

        std::time_t currentTime = std::time(nullptr);

        if (currentTime - lastBalloonSpawnTime >= 1.5f) {
            x_spawnedballoon = std::rand() % 641;
            y_spawnedballoon = std::rand() % 481;
            lastBalloonSpawnTime = currentTime;

            currentBalloon = balloonTextures[std::rand() % (sizeof(balloonTextures) / sizeof(balloonTextures[0]))];
        }

        if (click() && currentBalloon &&
            ir.x >= x_spawnedballoon && ir.x <= x_spawnedballoon + currentBalloon->w &&
            ir.y >= y_spawnedballoon && ir.y <= y_spawnedballoon + currentBalloon->h) {
            
            PlayOgg(balloon_pop_ogg, balloon_pop_ogg_size, 0, 0);
            
            if (currentBalloon == golden_balloon_texture) {
                playerScore += 5;
            } else {
                playerScore += 1;
            }
            currentBalloon = nullptr;
        }

        x_hand = ir.x - (int)(tex_hand_png->w * scale_factor / 2);
        y_hand = ir.y - (int)(tex_hand_png->h * scale_factor / 2);

        GRRLIB_FillScreen(White);

        if (currentBalloon) {
            GRRLIB_DrawImg(x_spawnedballoon, y_spawnedballoon,
                           currentBalloon, 0, 1, 1, White);
        }

        GRRLIB_DrawImg(x_hand, y_hand, tex_hand_png, 0, scale_factor, scale_factor, White);
        GRRLIB_Render();

        if(WPAD_ButtonsDown(WPAD_CHAN_0) & WPAD_BUTTON_HOME)
            break;
    }

    GRRLIB_FreeTexture(blue_balloon_texture);
    GRRLIB_FreeTexture(red_balloon_texture);
    GRRLIB_FreeTexture(green_balloon_texture);
    GRRLIB_FreeTexture(purple_balloon_texture);
    GRRLIB_FreeTexture(golden_balloon_texture);

    StopOgg();
    GRRLIB_FreeTexture(tex_hand_png);
    GRRLIB_Exit();
    return 0;
}
// best i could do

// TODO: add score system with TTF
// TODO: add background
// TODO: make it fun