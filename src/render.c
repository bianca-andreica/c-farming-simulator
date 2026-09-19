#include "render.h"
#include <stdio.h>
#include "game.h"
#include <math.h>
#include <string.h>
#include <stdlib.h> 

bool showNotEnoughMoneyPopup = false;
float popupTimer = 0.0f;
int selectedPlotIndex = -1;

ShopItemUI shopItems[MAX_SHOP_ITEMS];
int totalAnimalItems = 0;
int totalPlantItems = 0;
int selectedInventoryCategory = 0; 
int inventoryPage = 0;
int selectedCropType = -1; 

Font shopfont;

InventoryItem inventory[MAX_INVENTORY_ITEMS] = {0};
int inventoryCount = 0;

int selectedCategory = 0;
int currentPage = 0;

PlacedObject placedObjects[MAX_PLACED_OBJECTS];
int placedObjectCount = 0;

bool placingObject = false;
PlacedObject selectedObjectToPlace;
bool inventoryOpen = false;


extern Plot plots[MAX_PLOTS];

void DrawFarmer(Farmer *farmer)
{
    if (currentZone != ZONE_RELAX)
    {
        DrawTexture(farmer->texture, farmer->position.x, farmer->position.y, WHITE);
    }
}


void DrawAnimals(Animal animals[], int animalCount)
{
    if (currentZone != ZONE_ANIMALS)
        return;

    for (int i = 0; i < animalCount; i++)
{
    if (!animals[i].alive) continue;  

    DrawTexture(animals[i].texture, animals[i].position.x, animals[i].position.y, WHITE);
    DrawAnimalHUD(animals, i, iconFood, &foodStock, shopfont);
}

}


void DrawEggs(Font shopFont)
{
    for (int i = 0; i < eggCount; i++)
    {
        if (!eggs[i].collected)
        {
            DrawTexture(eggs[i].texture, eggs[i].position.x, eggs[i].position.y, WHITE);

            Vector2 mouse = GetGameMousePosition();
            Rectangle eggRect = {
                eggs[i].position.x,
                eggs[i].position.y,
                eggs[i].texture.width,
                eggs[i].texture.height
            };

            if (CheckCollisionPointRec(mouse, eggRect))
            {
                DrawTextEx(shopFont, "Click to Collect", 
                    (Vector2){eggs[i].position.x, eggs[i].position.y - 20},
                    18, 1, WHITE);
            }
        }
        if (eggs[i].showText)
        {
            DrawTextEx(shopFont, "Egg collected!", 
                (Vector2){eggs[i].position.x, eggs[i].position.y - 40},
                18, 1, YELLOW);
        }
    }
}




void DrawZoneArrows()
{
    Vector2 mouse = GetGameMousePosition();

    switch (currentZone)
    {
        case ZONE_PLANTS:
    
        DrawTextureRec(arrowBack, (Rectangle){0, 0, arrowBack.width, arrowBack.height}, (Vector2){downArrowRect2.x, downArrowRect2.y}, WHITE);
    
        if (CheckCollisionPointRec(mouse, downArrowRect2))
        {
    
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentZone = ZONE_COUNT;
            }
        }
        break;
    
    case ZONE_ANIMALS:
        DrawTextureRec(arrowBackStanga, (Rectangle){0, 0, arrowBackStanga.width, arrowBackStanga.height}, (Vector2){downArrowRect3.x, downArrowRect3.y}, WHITE);
    
        if (CheckCollisionPointRec(mouse, downArrowRect3))
        {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentZone = ZONE_COUNT;
            }
        }
        break;
    case ZONE_RELAX:
        DrawTextureRec(arrowBack, (Rectangle){0, 0, arrowBack.width, arrowBack.height}, (Vector2){downArrowRect1.x, downArrowRect1.y}, WHITE);

        if (CheckCollisionPointRec(mouse, downArrowRect1))
        {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentZone = ZONE_COUNT;
            }
        }
        break;

    default:

        DrawTextureRec(animalIcon, (Rectangle){0, 0, animalIcon.width, animalIcon.height}, (Vector2){rightArrowRect.x, rightArrowRect.y}, WHITE);
        if (CheckCollisionPointRec(mouse, rightArrowRect))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) currentZone = ZONE_ANIMALS;
        }

        DrawTextureRec(plantIcon, (Rectangle){0, 0, plantIcon.width, plantIcon.height}, (Vector2){leftArrowRect.x, leftArrowRect.y}, WHITE);
        if (CheckCollisionPointRec(mouse, leftArrowRect))
        {

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) currentZone = ZONE_PLANTS;
        }

        DrawTextureRec(relaxIcon, (Rectangle){0, 0, relaxIcon.width, relaxIcon.height}, (Vector2){upArrowRect.x, upArrowRect.y}, WHITE);
        if (CheckCollisionPointRec(mouse, upArrowRect))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) currentZone = ZONE_RELAX;
        }
        break;
    }
}


void DrawCurrentZone(Texture2D background, Animal animals[], int animalCount)
{
    DrawTexturePro(
        background,
        (Rectangle){0, 0, background.width, background.height},
        (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    switch (currentZone)
    {
    case ZONE_PLANTS:
        DrawPlantZone();
        break;

    case ZONE_ANIMALS:
    {
          break;
    }

    case ZONE_RELAX:
        DrawRelaxZone(relaxZoneTexture);
        break;

    case ZONE_MAIN:
       
                Color brightRed = (Color){255, 60, 60, 255};

        DrawTextEx(shopfont, "My Farm",
                (Vector2){GetScreenWidth() / 2 - 100, 50},
                80, 2, brightRed);

        DrawTextEx(shopfont, "Use the arrows to visit different areas of your farm",
                (Vector2){GetScreenWidth() / 2 -350, 130},
                40, 1, brightRed);

        break;

    case ZONE_COUNT:
    
        break;
    }

    DrawZoneArrows();
}

void DrawPlantZone(void)
{
    for (int i = 0; i < plotCount; i++)
    {
        DrawTexture(plotTex, plots[i].bounds.x, plots[i].bounds.y, WHITE);
    }

    Vector2 mouse = GetGameMousePosition();

    for (int i = 0; i < plotCount; i++)
    {

        if (CheckCollisionPointRec(mouse, plots[i].bounds))
        {
            Rectangle highlight = plots[i].bounds;
            highlight.x -= 4;
            highlight.y -= 4;
            highlight.width += 70;
            highlight.height += 70;
            DrawRectangleRec(highlight, Fade(YELLOW, 0.3f));

            if (!plots[i].isPlanted)
            {
                DrawTextEx(shopfont, "Click to plant", (Vector2){plots[i].bounds.x +10, plots[i].bounds.y - 50}, 30, 1, WHITE);
            }
            else if (plots[i].isFullyGrown)
            {
                DrawTextEx(shopfont, "Click to harvest", (Vector2){plots[i].bounds.x+10, plots[i].bounds.y - 50}, 30, 1, WHITE);
            }
        }

        if (plots[i].isPlanted)
        {
            Texture2D tex = plots[i].isFullyGrown ? plantTextures[plots[i].cropType] : genericPlantedTexture;

            DrawTexturePro(
                tex,
                (Rectangle){0, 0, (float)tex.width, (float)tex.height},
                (Rectangle){
                    plots[i].bounds.x + plots[i].bounds.width / 2 - tex.width * 0.2f+30,
                    plots[i].bounds.y + plots[i].bounds.height / 2 - tex.height * 0.2f+30,
                    tex.width * 0.4f,
                    tex.height * 0.4f},
                (Vector2){0, 0},
                0.0f,
                WHITE);
        }

    }
    DrawTexture(waterTowerTex, GetScreenWidth() - 1000, 300, WHITE);
    DrawTexture(toolStorageTex, GetScreenWidth() - 400, 100, WHITE);
    Vector2 towerPos = { GetScreenWidth() - 1000, 300 };
    Rectangle waterTowerRect = { towerPos.x, towerPos.y, waterTowerTex.width, waterTowerTex.height };
    DrawTexture(waterTowerTex, towerPos.x, towerPos.y, WHITE);

    if (CheckCollisionPointRec(mouse, waterTowerRect)) {
        DrawTextEx(shopfont, "Click to water plants", (Vector2){towerPos.x, towerPos.y - 40}, 30, 1, SKYBLUE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (waterStock > 0) {
                waterStock--;
                waterBoostActive = true;
                waterBoostMultiplier = 1.2f;
            } else {
                showNotEnoughWaterPopup = true;
                waterPopupTimer = 2.0f;
            }
        }
    }

}



void DrawRelaxZone(Texture2D background)
{

        DrawTextEx(shopfont, "Relaxation Zone", (Vector2){GetScreenWidth() / 2 - 140, 90}, 50, 1, GREEN);
        DrawTextEx(shopfont, "Recharging Energy...", (Vector2){GetScreenWidth() / 2 - 110, 150}, 40, 1, YELLOW);


        Rectangle barBg = {GetScreenWidth() / 2 - 150, 230, 300, 30};
        float rechargeProgress = (float)energy / 100.0f;


        DrawRectangleRounded(barBg, 0.5f, 8, (Color){30, 30, 30, 200});

        DrawRectangleRounded((Rectangle){barBg.x, barBg.y, barBg.width * rechargeProgress, barBg.height},0.5f, 8, (Color){0, 255, 128, 200});


        DrawRectangleRoundedLines(barBg, 0.5f, 8, WHITE);


        DrawTextEx(shopfont, TextFormat("Energy: %d%%", energy),(Vector2){GetScreenWidth() / 2 - 60, 235}, 20, 1, WHITE);


}


void DrawAnimalHUD(Animal animals[], int index, Texture2D feedIcon, int *foodStock, Font shopfont)
{
    if (!animals[index].alive) return; 

    float barWidth = 120;
    float barHeight = 16;

    float barX = animals[index].position.x;
    float barY = animals[index].position.y - 25;

    Vector2 mousePos = GetGameMousePosition();
    Rectangle animalRect = {
        animals[index].position.x,
        animals[index].position.y,
        animals[index].texture.width,
        animals[index].texture.height};

    bool hoveringAnimal = CheckCollisionPointRec(mousePos, animalRect);


if (hoveringAnimal)
{
    animals[index].isHovered = true;
    animals[index].hoverExpireTime = GetTime() + 1.0; 
}

if (animals[index].isHovered && GetTime() < animals[index].hoverExpireTime)
    {
        
        DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
        float progress = animals[index].timeUntilHungry / animals[index].maxTimeUntilHungry;
        DrawRectangle(barX, barY, barWidth * progress, barHeight, GREEN);

        
        char timeText[32];
        int remainingMinutes = (int)(animals[index].timeUntilHungry * 60);
        int h = remainingMinutes / 60;
        int m = remainingMinutes % 60;
        sprintf(timeText, "%dh %dm", h, m);
        DrawTextEx(shopfont, timeText, (Vector2){barX + 4, barY - 18}, 18, 1, WHITE);

        
        char ageText[32];
        sprintf(ageText, "Age: %.1f days", animals[index].age);
        DrawTextEx(shopfont, ageText, (Vector2){barX + 4, barY + 20}, 18, 1, WHITE);

        float lifespanProgress = animals[index].age / animals[index].lifespan;
        Color lifespanColor = lifespanProgress < 0.5f ? GREEN : (lifespanProgress < 0.8f ? YELLOW : RED);
        DrawRectangle(barX, barY + 40, barWidth, barHeight / 2, DARKGRAY);
        DrawRectangle(barX, barY + 40, barWidth * lifespanProgress, barHeight / 2, lifespanColor);

        if (animals[index].timeUntilHungry <= 0)
        {
            Rectangle feedBtn = {barX + barWidth + 10, barY, 32, 32};
            DrawTextureEx(feedIcon, (Vector2){feedBtn.x, feedBtn.y}, 0, 0.5f, WHITE);
            animals[index].feedButtonRect = feedBtn;

            if (CheckCollisionPointRec(mousePos, feedBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (*foodStock > 0)
                {
                    FeedAnimal(index, foodStock, animals);
                }
            }
        }
        else
        {
            animals[index].feedButtonRect = (Rectangle){0, 0, 0, 0};
        }

        if (animals[index].age > animals[index].lifespan * 0.5f)
        {
            int price = 0;
            if (animals[index].texture.id == chickenTexture.id)
                price = 30 + (int)(animals[index].age * 0.5f);
            else if (animals[index].texture.id == cowTexture.id)
                price = 100 + (int)(animals[index].age * 2.0f);
            else if (animals[index].texture.id == pigTexture.id)
                price = 80 + (int)(animals[index].age * 1.5f);
            else
                price = 70 + (int)(animals[index].age * 1.0f);

            Rectangle sellBtn = {barX + barWidth + 10, barY + 40, 60, 30};
            DrawRectangleRec(sellBtn, YELLOW);
            DrawTextEx(shopfont, "SELL", (Vector2){sellBtn.x + 10, sellBtn.y + 6}, 18, 1, WHITE);
            animals[index].sellButtonRect = sellBtn;

            char priceText[16];
            sprintf(priceText, "$%d", price);
            DrawTextEx(shopfont, priceText, (Vector2){sellBtn.x, sellBtn.y + 34}, 16, 1, YELLOW);

            if (CheckCollisionPointRec(mousePos, sellBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                
                animals[index].alive = false;
                animals[index].respawnTimer = 5.0f; 
                animals[index].age = 0.0f;         
                animals[index].timeUntilHungry = animals[index].maxTimeUntilHungry;
                animals[index].isHungry = false;
                money += price;
            }

        }
        else
        {
            animals[index].sellButtonRect = (Rectangle){0, 0, 0, 0};
        }
    }
    else
{
    animals[index].isHovered = false;
}

    if (CheckCollisionPointRec(mousePos, animalRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (animals[index].timeUntilHungry <= 0 && *foodStock > 0)
        {
            FeedAnimal(index, foodStock, animals);
        }
    }
}


void DrawHUD(int foodStock, int energy, int money)
{
    int startX = 20;
    int startY = 20;
    int spacingY = 110; 

    Rectangle resourcePanel = {startX - 10, startY - 10, 200, 330};
    DrawRectangleRounded((Rectangle){resourcePanel.x + 4, resourcePanel.y + 4, resourcePanel.width, resourcePanel.height},0.2f, 8, (Color){0, 0, 0, 80});
    DrawTexturePro(shopTexture,(Rectangle){0, 0, shopTexture.width, shopTexture.height},resourcePanel,(Vector2){0, 0},0.0f, WHITE);

    DrawRectangleRoundedLines(resourcePanel, 0.2f, 8, (Color){255, 255, 255, 60});


    DrawTextEx(shopfont, "RESOURCES", (Vector2){startX + 40, startY}, 30, 1, (Color){250, 250, 250, 255});
    DrawLine(startX, startY + 28, startX + 160, startY + 28, (Color){255, 255, 255, 40});


    Rectangle foodRect = {startX, startY + 35, 160, 80};
    DrawRectangleRounded(foodRect, 0.2f, 8, (Color){60, 60, 60, 180});

    float foodScale = 0.055f;
    DrawTextureEx(foodIcon, (Vector2){startX + 10, startY + 40}, 0.0f, foodScale, WHITE);

    DrawTextEx(shopfont, "Food", (Vector2){startX + 70, startY + 45}, 25, 1, (Color){230, 230, 230, 255});
    DrawTextEx(shopfont, TextFormat("%d", foodStock), (Vector2){startX + 120, startY + 45}, 22, 1, (Color){255, 220, 100, 255});

    Rectangle foodMeter = {startX + 10, startY + 100, 140, 8};
    float foodRatio = foodStock / 100.0f;
    if (foodRatio > 1.0f) foodRatio = 1.0f;

    DrawRectangleRounded(foodMeter, 0.5f, 4, (Color){80, 80, 80, 180});
    DrawRectangleRounded((Rectangle){foodMeter.x, foodMeter.y, foodMeter.width * foodRatio, foodMeter.height},0.5f, 4, (Color){255, 200, 50, 255});


    Rectangle energyRect = {startX, startY + spacingY + 20, 160, 80};
    DrawRectangleRounded(energyRect, 0.2f, 8, (Color){60, 60, 60, 180});

    float energyScale = 0.13f;
    DrawTextureEx(energyIcon, (Vector2){startX + 10, startY + spacingY + 25}, 0.0f, energyScale, WHITE);

    DrawTextEx(shopfont, "Energy", (Vector2){startX + 60, startY + spacingY + 30}, 23, 1, (Color){230, 230, 230, 255});
    DrawTextEx(shopfont, TextFormat("%d", energy), (Vector2){startX + 120, startY + spacingY + 30}, 22, 1, (Color){50, 255, 120, 255});

    Rectangle energyMeter = {startX + 10, startY + spacingY + 90, 140, 8};
    float energyRatio = energy / 100.0f;

    DrawRectangleRounded(energyMeter, 0.5f, 4, (Color){80, 80, 80, 180});
    DrawRectangleRounded((Rectangle){energyMeter.x, energyMeter.y, energyMeter.width * energyRatio, energyMeter.height},
                         0.5f, 4, (Color){50, 200, 100, 255});

    
    Rectangle moneyRect = {startX, startY + spacingY * 2 + 10, 160, 60};
    DrawRectangleRounded(moneyRect, 0.2f, 8, (Color){60, 60, 60, 180});

    float moneyScale = 0.055f;
    DrawTextureEx(moneyIcon, (Vector2){startX + 10, startY + spacingY * 2 + 15}, 0.0f, moneyScale, WHITE);

    DrawTextEx(shopfont, "Money", (Vector2){startX + 70, startY + spacingY * 2 + 20}, 23, 1, (Color){230, 230, 230, 255});
    DrawTextEx(shopfont, TextFormat("$%d", money), (Vector2){startX + 120, startY + spacingY * 2 + 20}, 22, 1, (Color){255, 215, 0, 255});

    static int lastMoney = 0;
    static float moneyChangeTimer = 0;

    if (money != lastMoney)
    {
        moneyChangeTimer = 1.0f;
        lastMoney = money;
    }

    if (moneyChangeTimer > 0)
    {
        moneyChangeTimer -= GetFrameTime();
        int diff = money - lastMoney;
        Color diffColor = (diff >= 0) ? GREEN : RED;
        DrawTextEx(shopfont,
                   TextFormat("%s%d", (diff >= 0) ? "+" : "", diff),
                   (Vector2){startX + 60, startY + spacingY * 2 + 45},
                   18, 1, diffColor);
    }
    char levelText[32];
    sprintf(levelText, "Nivel %d", gameLevel);


     Rectangle levelPanel = {1700, 15, 170, 40};
    DrawTexturePro(shopTexture,(Rectangle){0, 0, shopTexture.width, shopTexture.height},levelPanel,(Vector2){0, 0},0.0f,WHITE);
    Color playfulYellow = (Color){255, 223, 50, 255}; 
    DrawTextEx(shopfont, levelText, (Vector2){1720, 20}, 28, 2, playfulYellow);


    DrawNotEnoughEnergyPopup(shopfont);
    DrawLevelUpPopup(shopfont);
    DrawNotEnoughWaterPopup(shopfont);



}


void DrawShopButton(void)
{
    DrawTexture(shopIcon, shopButtonRect.x, shopButtonRect.y, WHITE);
}

void DrawShop(bool *clickHandled,bool *shopOpen)
{
    if (!*shopOpen)
        return;

    Vector2 mouse = GetGameMousePosition();

    DrawTexturePro(shopTexture, (Rectangle){0, 0, shopTexture.width, shopTexture.height}, (Rectangle){220, 100, 1500, 600}, (Vector2){0, 0}, 0.0f, WHITE);

    if (shopfont.texture.id != 0)
        DrawTextEx(shopfont, "Shop", (Vector2){850, 105}, 120, 3, (Color){255, 221, 85, 255});
    else
        DrawText("SHOP", 850, 105, 30, RED);

    Rectangle closeButton = {200 + 1520 - 40, 100, 40, 40};  
    DrawRectangleRec(closeButton, RED);
    DrawTextEx(shopfont, "X", (Vector2){closeButton.x + 8, closeButton.y + 4}, 32, 2, WHITE);


    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *shopOpen = false; 
    }
    for (int i = 0; i < 2; i++)
    {
        int x = 250 + i * 350;
        int y = 250;
        DrawShopItem(shopItems[i], x, y, shopfont);
    }
}

void DrawShopItem(ShopItemUI item, int x, int y, Font shopFont)
{
    Vector2 mouse = GetGameMousePosition();

    DrawRectangleRounded((Rectangle){x, y, 300, 420}, 0.1f, 10, RAYWHITE);
    DrawRectangleRoundedLines((Rectangle){x, y, 300, 420}, 0.1f, 10, LIGHTGRAY);

    DrawTexture(item.icon, x + 15, y + 10, WHITE);

    DrawTextEx(shopFont, item.name, (Vector2){x + 10, y + 285}, 22, 1, BLACK);

    char priceStr[32];
    sprintf(priceStr, "%d $", item.price);
    DrawTextEx(shopFont, priceStr, (Vector2){x + 10, y + 310}, 20, 1, DARKGREEN);

    DrawTextEx(shopFont, "-", (Vector2){x + 10, y + 335}, 20, 1, RED);
    DrawRectangle(x + 35, y + 335, 40, 25, LIGHTGRAY);
    char qStr[8];
    sprintf(qStr, "%d", *item.quantity);
    DrawTextEx(shopFont, qStr, (Vector2){x + 45, y + 338}, 16, 1, BLACK);
    DrawTextEx(shopFont, "+", (Vector2){x + 85, y + 335}, 20, 1, GREEN);

    item.buyButton = (Rectangle){x + 20, y + 370, 120, 25};
    DrawRectangleRec(item.buyButton, ORANGE);
    DrawTextEx(shopFont, "Buy", (Vector2){item.buyButton.x + 15, item.buyButton.y + 5}, 20, 1, WHITE);

    if (CheckCollisionPointRec(mouse, (Rectangle){x + 10, y + 335, 20, 20}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (*item.quantity > 0)
            (*item.quantity)--;
    }
    if (CheckCollisionPointRec(mouse, (Rectangle){x + 85, y + 335, 20, 20}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        (*item.quantity)++;
    }
   if (CheckCollisionPointRec(mouse, item.buyButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
{
    int qty = *(item.quantity);
    int totalCost = item.price * qty;

    if (money >= totalCost)
    {
        money -= totalCost;

        if (strcmp(item.name, "Mancare") == 0)
            foodStock += qty;
        else if (strcmp(item.name, "Apa") == 0)
            waterStock += qty;

        AddToInventory(item);
        printf("Ai cumparat %s x%d!\n", item.name, qty);
    }
    else
    {
        showNotEnoughMoneyPopup = true;
        popupTimer = 2.0f;
    }
}


}

void InitShopItems(void)
{
    
        static int QtyEssentials[2] = {1, 1};

        shopItems[0] = (ShopItemUI){foodTexture, "Mancare", 5, &QtyEssentials[0], {0}};
        shopItems[1] = (ShopItemUI){waterTexture, "Apa", 3, &QtyEssentials[1], {0}};

}

bool PointInTriangle(Vector2 pt, Vector2 v1, Vector2 v2, Vector2 v3)
{
    float area = fabs((v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y)) / 2.0);
    float a = fabs((pt.x * (v2.y - v3.y) + v2.x * (v3.y - pt.y) + v3.x * (pt.y - v2.y)) / 2.0);
    float b = fabs((v1.x * (pt.y - v3.y) + pt.x * (v3.y - v1.y) + v3.x * (v1.y - pt.y)) / 2.0);
    float c = fabs((v1.x * (v2.y - pt.y) + v2.x * (pt.y - v1.y) + pt.x * (v1.y - v2.y)) / 2.0);

    return fabs(area - (a + b + c)) < 0.1f;
}

void AddToInventory(ShopItemUI item)
{
    for (int i = 0; i < inventoryCount; i++)
    {
        if (strcmp(inventory[i].name, item.name) == 0)
        {
            inventory[i].quantity += 1;
            return;
        }
    }
    if (inventoryCount < MAX_INVENTORY_ITEMS)
    {
        inventory[inventoryCount].name = item.name;
        inventory[inventoryCount].icon = item.icon;
        inventory[inventoryCount].quantity = 1;
        inventoryCount++;
    }
}


void DrawNotEnoughMoneyPopup(Font shopfont)
{
    if (!showNotEnoughMoneyPopup)
        return;

    DrawRectangle(700, 400, 500, 200, (Color){255, 230, 180, 255});
    DrawRectangleLines(700, 400, 500, 200, RED);

    DrawTextEx(shopfont, "Nu ai destui bani!", (Vector2){750, 450}, 32, 2, RED);

    Rectangle closeButton = {1150, 410, 40, 40};
    DrawRectangleRec(closeButton, (Color){200, 0, 0, 255});
    DrawTextEx(shopfont, "X", (Vector2){1160, 415}, 24, 1, WHITE);

    Vector2 mouse = GetGameMousePosition();
    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showNotEnoughMoneyPopup = false;
    }

    popupTimer -= GetFrameTime();
    if (popupTimer <= 0)
    {
        showNotEnoughMoneyPopup = false;
    }
}


void DrawInventory(bool *clickHandled, Font shopfont)
{
    if (!inventoryOpen)
        return;

    Vector2 mouse = GetGameMousePosition();
    
    DrawTexturePro(shopTexture, (Rectangle){0, 0, shopTexture.width, shopTexture.height}, (Rectangle){220, 100, 1500, 600}, (Vector2){0, 0}, 0.0f, WHITE);

    DrawTextEx(shopfont, "Inventar", (Vector2){850, 105}, 120, 3, (Color){255, 221, 85, 255});

    

    Rectangle closeButton = {200 + 1520 - 40, 100, 40, 40};
    DrawRectangleRec(closeButton, RED);
    DrawTextEx(shopfont, "X", (Vector2){closeButton.x + 8, closeButton.y + 4}, 32, 2, WHITE);
    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        inventoryOpen = false;
    }

    InventoryItem *filteredItems[100];
    int filteredCount = 0;
    for (int i = 0; i < inventoryCount; i++) {
        if ((selectedInventoryCategory == 0 && inventory[i].type == ITEM_ANIMAL) ||
            (selectedInventoryCategory == 1 && inventory[i].type == ITEM_PLANT)) {
            filteredItems[filteredCount++] = &inventory[i];
        }
    }

    int itemsPerPage = 4;
    int start = inventoryPage * itemsPerPage;
    int end = start + itemsPerPage;
    if (end > filteredCount)
        end = filteredCount;

    
    for (int i = start; i < end; i++) {
        int x = 250 + (i - start) * 350;
        int y = 250;
        InventoryItem *item = filteredItems[i];

DrawRectangleRounded((Rectangle){x, y, 300, 420}, 0.1f, 10, RAYWHITE);
DrawRectangleRoundedLines((Rectangle){x, y, 300, 420}, 0.1f, 10, LIGHTGRAY);

DrawTexture(item->icon, x + 15, y + 10, WHITE);

DrawTextEx(shopfont, item->name, (Vector2){x + 10, y + 300}, 30, 1, BLACK);

char quantityStr[32];

if (strcmp(item->name, "Mancare") == 0)
    sprintf(quantityStr, "x%d", foodStock);
else if (strcmp(item->name, "Apa") == 0)
    sprintf(quantityStr, "x%d", waterStock);
else
    sprintf(quantityStr, "x%d", item->quantity);

DrawTextEx(shopfont, quantityStr, (Vector2){x + 10, y + 330}, 30, 1, DARKGREEN);


    DrawTexture(arrowIconLeft, 150, 390, WHITE);
    DrawTexture(arrowIconRight, 1680, 390, WHITE);

    Rectangle leftArrowHitbox = {150, 390, 60, 60};
    Rectangle rightArrowHitbox = {1680, 390, 60, 60};

    if (!(*clickHandled) && CheckCollisionPointRec(mouse, leftArrowHitbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && inventoryPage > 0) {
        inventoryPage--;
        *clickHandled = true;
    }

    if (!(*clickHandled) && CheckCollisionPointRec(mouse, rightArrowHitbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (inventoryPage + 1) * itemsPerPage < filteredCount) {
        inventoryPage++;
        *clickHandled = true;
    }
}
}

void CheckBarnClick(bool *clickHandled)
{
    if (currentZone != ZONE_MAIN)
        return;

    Vector2 mouse = GetGameMousePosition();

    Rectangle barnHitbox = {1200, 220, 700, 300};

    if (CheckCollisionPointRec(mouse, barnHitbox))
    {
        DrawTextEx(
            shopfont,
            "Hambar - Check your stocks!",
            (Vector2){1250, 530},
            30,
            1,
            WHITE
        );

        if (!(*clickHandled) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            inventoryOpen = true;
            *clickHandled = true;
        }
    }
}

void SaveGame(const char* filename, int money, int energy, int food,
              Pen pens[], int penCount,
              Plot plots[], int plotCount) {
    FILE* file = fopen(filename, "wb");
    if (!file) return;

    fwrite(&money, sizeof(int), 1, file);
    fwrite(&energy, sizeof(int), 1, file);
    fwrite(&food, sizeof(int), 1, file);


    fwrite(pens, sizeof(Pen), penCount, file);
    fwrite(plots, sizeof(Plot), plotCount, file);

    fclose(file);
}
void LoadGame(const char* filename, int* money, int* energy, int* food,
              Pen pens[], int penCount,
              Plot plots[], int plotCount) {
    FILE* file = fopen(filename, "rb");
    if (!file) return;

    fread(money, sizeof(int), 1, file);
    fread(energy, sizeof(int), 1, file);
    fread(food, sizeof(int), 1, file);

    fread(pens, sizeof(Pen), penCount, file);
    fread(plots, sizeof(Plot), plotCount, file);

    fclose(file);
}

