#include "raylib.h"
#include "game.h"
#include "render.h"
#include <stdio.h>

extern void InitRectangles(void);

int main()
{
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Farming Game");
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
    ToggleFullscreen();
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
        SaveGame("savegame.dat", money, energy, foodStock, pens, PEN_COUNT, plots, plotCount);
        break; 
        }
        BeginDrawing();
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
        }

        DrawCurrentZone(currentBackground, animals, animalCount);
     if (IsKeyPressed(KEY_R)) 
     {
        ResetGame(&money, &energy,animals, &animalCount, pens, PEN_COUNT);
     }
        
        UpdateGame(&farmer, plants, &plantCount, animals, &animalCount, plots, plotCount);
        float deltaTime = GetFrameTime();


        UpdateAnimals(deltaTime, animals, animalCount);
        UpdateEggs(deltaTime);
        DrawEggs(shopfont);

        
        if (currentZone != ZONE_RELAX)
        {
            DrawFarmer(&farmer);
        }

        
        DrawAnimals(animals, animalCount);
      
       
        DrawShopButton();

        DrawHUD(foodStock, energy, money);

    
        if (CheckCollisionPointRec(GetMousePosition(), shopButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            shopOpen = !shopOpen; 
        }

        bool clickHandled = false;
        DrawShop(&clickHandled,&shopOpen);
        CheckBarnClick(&clickHandled);
        DrawNotEnoughMoneyPopup(shopfont);

        if (inventoryOpen)
        {
            DrawInventory(&clickHandled, shopfont);
        }
        EndDrawing();
    }
    UnloadGame(&farmer, plants, plantCount, animals, animalCount);
    UnloadTexture(plantZoneBackground);
    UnloadTexture(animalZoneBackground);
    UnloadTexture(relaxZoneBackground);
    UnloadTexture(mainZoneBackground);
    UnloadTextures();
    CloseWindow();
    return 0;
}
