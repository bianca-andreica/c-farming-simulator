#include "raylib.h"
#include "game.h"
#include "render.h"
#include <stdio.h>

extern void InitRectangles(void);

int main()
{
    // 1. Setezi flag-ul de Fullscreen
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    
    // 2. Deschizi fereastra mai întâi (pui 0, 0 ca să ia automat rezoluția nativă a monitorului)
    InitWindow(0, 0, "Farming Game");
    RenderTexture2D gameTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    // 3. ACUM măsori dimensiunile! (Fiind deja inițializat, nu va mai da 0 x 0)
    int monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);

    printf("Screen: %d x %d\n", GetScreenWidth(), GetScreenHeight());
    printf("Monitor %d: %d x %d\n", monitor, monitorWidth, monitorHeight);
    Farmer farmer;
    Animal animals[MAX_ANIMALS];
    LoadGame("savegame.dat", &money, &energy, &foodStock, pens, PEN_COUNT, plots, plotCount);
    shopfont = LoadFontEx("assets/Fonts/Chewy-Regular.ttf", 128, 0, 0);
    if (shopfont.texture.id == 0)
    {
        printf("Eroare: Fontul Chewy nu s-a încărcat corect!\n");
    }
    InitTextures();
    InitRectangles();
    SetTargetFPS(60);

    Texture2D plantZoneBackground = LoadTexture("assets/backgrounds/background_plant.png");
    Texture2D animalZoneBackground = LoadTexture("assets/backgrounds/background_animals.png");
    Texture2D relaxZoneBackground = LoadTexture("assets/backgrounds/background_relax.png");
    Texture2D mainZoneBackground = LoadTexture("assets/backgrounds/backgroung_princip.png");

    if (plantZoneBackground.id == 0)
    {
        printf("Eroare: Nu s-a putut încarca imaginea de fundal pentru zona de plantat!\n");
    }

    if (animalZoneBackground.id == 0)
    {
        printf("Eroare: Nu s-a putut încarca imaginea de fundal pentru zona de animale!\n");
    }

    if (relaxZoneBackground.id == 0)
    {
        printf("Eroare: Nu s-a putut încarca imaginea de fundal pentru zona de relaxare!\n");
    }

    if (mainZoneBackground.id == 0)
    {
        printf("Eroare: Nu s-a putut încarca imaginea de fundal pentru zona principala!\n");
    }
    Plant plants[MAX_PLANTS];
    int plantCount = 0;
    int animalCount = 0;
    InitGame(&farmer, plants, &plantCount, animals, &animalCount);

    while (true)
{
    if (WindowShouldClose())
    {
        SaveGame(
            "savegame.dat",
            money,
            energy,
            foodStock,
            pens,
            PEN_COUNT,
            plots,
            plotCount
        );
        break;
    }

    float deltaTime = GetFrameTime();

    // ==========================================
    // 1. UPDATE LOGIC
    // ==========================================

    if (IsKeyPressed(KEY_R))
    {
        ResetGame(
            &money,
            &energy,
            animals,
            &animalCount,
            pens,
            PEN_COUNT
        );
    }

    UpdateGame(
        &farmer,
        plants,
        &plantCount,
        animals,
        &animalCount,
        plots,
        plotCount
    );

    UpdateAnimals(
        deltaTime,
        animals,
        animalCount
    );

    if (currentZone == ZONE_ANIMALS)
    {
        UpdateEggs(deltaTime);
    }

    // ==========================================
    // 2. DRAW
    // ==========================================

    BeginTextureMode(gameTarget);

    ClearBackground(RAYWHITE);

    Texture2D currentBackground;

    switch (currentZone)
    {
        case ZONE_PLANTS:
            currentBackground = plantZoneBackground;
            break;

        case ZONE_ANIMALS:
            currentBackground = animalZoneBackground;
            break;

        case ZONE_RELAX:
            currentBackground = relaxZoneTexture;
            break;

        default:
            currentBackground = mainZoneBackground;
            break;
    }

    // Fundalul este desenat DUPA ce zona a fost schimbata
    DrawCurrentZone(
        currentBackground,
        animals,
        animalCount
    );

    // Fermier
    if (currentZone != ZONE_RELAX)
    {
        DrawFarmer(&farmer);
    }

    // Animalele
    DrawAnimals(
        animals,
        animalCount
    );

    // Ouale - DUPA animale
    if (currentZone == ZONE_ANIMALS)
    {
        DrawEggs(shopfont);
    }

    // Shop
    DrawShopButton();

    // HUD
    DrawHUD(
        foodStock,
        energy,
        money
    );

    // Click shop
    if (CheckCollisionPointRec(
            GetGameMousePosition(),
            shopButtonRect
        ) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        shopOpen = !shopOpen;
    }

    bool clickHandled = false;

    DrawShop(
        &clickHandled,
        &shopOpen
    );

    CheckBarnClick(
        &clickHandled
    );

    DrawNotEnoughMoneyPopup(shopfont);

    if (inventoryOpen)
    {
        DrawInventory(
            &clickHandled,
            shopfont
        );
    }

    EndTextureMode();


    // ==========================================
    // 3. DISPLAY FINAL
    // ==========================================

    BeginDrawing();

    ClearBackground(BLACK);

    float scaleX = (float)GetScreenWidth() / GAME_WIDTH;
    float scaleY = (float)GetScreenHeight() / GAME_HEIGHT;

    float scale = scaleX < scaleY ? scaleX : scaleY;

    float destWidth = GAME_WIDTH * scale;
    float destHeight = GAME_HEIGHT * scale;

    float destX = (GetScreenWidth() - destWidth) / 2.0f;
    float destY = (GetScreenHeight() - destHeight) / 2.0f;

    DrawTexturePro(
        gameTarget.texture,
        (Rectangle){
            0,
            0,
            (float)gameTarget.texture.width,
            -(float)gameTarget.texture.height
        },
        (Rectangle){
            destX,
            destY,
            destWidth,
            destHeight
        },
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    EndDrawing();
}
    UnloadGame(&farmer, plants, plantCount, animals, animalCount);
    UnloadTexture(plantZoneBackground);
    UnloadTexture(animalZoneBackground);
    UnloadTexture(relaxZoneBackground);
    UnloadTexture(mainZoneBackground);
    UnloadTextures();
    UnloadRenderTexture(gameTarget);
    CloseWindow();
    return 0;
}
