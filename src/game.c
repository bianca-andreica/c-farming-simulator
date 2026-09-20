#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // For rand()
#include "render.h"
#include <string.h>
#include "raylib.h"


#define MAX_LEVELS 5

int levelThresholds[MAX_LEVELS] = { 50, 150, 300, 500, 800 };
int waterStock=10;
int foodStock = 10;
int money = 100;
int energy = 100;
ZoneType currentZone = ZONE_MAIN;

bool showNotEnoughWaterPopup = false;
float waterPopupTimer = 0;
bool waterBoostActive = false;
float waterBoostMultiplier = 1.0f;


bool showLevelUpPopup = false;
float levelUpPopupTimer = 0;

int gameLevel = 1;
int energyUsedLevel = 0;

float energyAccumulator = 0.0f;

Texture2D animalTextures[ANIMAL_TYPE_COUNT];

bool shopOpen = false;
Rectangle shopButtonRect = {0};
Rectangle leftArrowRect = {0};
Rectangle rightArrowRect = {0};
Rectangle upArrowRect = {0};
Rectangle downArrowRect1 = {0};
Rectangle downArrowRect2 = {0};
Rectangle downArrowRect3 = {0};
Rectangle storageRect={0};

Egg eggs[MAX_EGGS] = {0};
int eggCount = 0;
Plot plots[MAX_PLOTS];
Pen pens[PEN_COUNT];
int plotCount = 27;
int penCount = 6;

bool showNotEnoughEnergyPopup = false;
float energyPopupTimer = 0.0f;

Vector2 GetGameMousePosition(void)
{
    float scaleX = (float)GetScreenWidth() / GAME_WIDTH;
    float scaleY = (float)GetScreenHeight() / GAME_HEIGHT;

    float scale = scaleX < scaleY ? scaleX : scaleY;

    float offsetX = (GetScreenWidth() - GAME_WIDTH * scale) / 2.0f;
    float offsetY = (GetScreenHeight() - GAME_HEIGHT * scale) / 2.0f;

    Vector2 mouse = GetMousePosition();

    return (Vector2){
        (mouse.x - offsetX) / scale,
        (mouse.y - offsetY) / scale
    };
}

void InitRectangles(void)
{
    leftArrowRect = (Rectangle){50, GAME_HEIGHT / 2 - 25, 150, 150};
    rightArrowRect = (Rectangle){GAME_WIDTH - 250, GAME_HEIGHT / 2 - 25, 150, 150};
    upArrowRect = (Rectangle){GAME_WIDTH / 2 - 50, 300, 150, 150};
    downArrowRect1 = (Rectangle){GAME_WIDTH / 2 - 25, GAME_HEIGHT - 250, 250, 250};
    downArrowRect2 = (Rectangle){GAME_WIDTH - 300, GAME_HEIGHT / 2, 250, 250};
    downArrowRect3 = (Rectangle){GAME_WIDTH - 1900, GAME_HEIGHT / 2, 250, 250};

    shopButtonRect = (Rectangle){GAME_WIDTH - 170, 900, 150, 150};
}


void InitGame(Farmer *farmer, Plant plants[], int *plantCount, Animal animals[], int *animalCount)
{
    farmer->position = (Vector2){400, 300};
    farmer->texture = LoadTexture("assets/farmer.png");
    farmer->energy = 100;
    farmer->money = 50;
    *plantCount = 0;
    *animalCount = 9;
    InitRectangles();
    InitAnimals(animals);
    InitPlots(plots);
    InitPens(pens);
    InitShopItems();
}


void PlantSeed(Plot *plot)
{

    int baseEnergyCost = 10;
    int energyCost = baseEnergyCost + (gameLevel - 1) * 2;

    int minGrow = 30;      
    int maxGrow = 60;      
    plot->growDuration = (float)(minGrow + rand() % (maxGrow - minGrow + 1));

    plot->isPlanted = true;
    plot->growTimer = 0;
    plot->isFullyGrown = false;
    plot->cropType = rand() % 9;

    energy -= energyCost;
    energyUsedLevel += energyCost;
    if (gameLevel < MAX_LEVELS && energyUsedLevel >= levelThresholds[gameLevel - 1]) {
        gameLevel++;
        showLevelUpPopup = true;
        levelUpPopupTimer = 3.0f;
    }
}


void InitPlots(Plot plots[])
{
    const float plotSize = 80;
    const int columns = 9;
    const int rows = 3;
    const float spacing = 70;

    float totalWidth = columns * plotSize + (columns - 1) * spacing;
    float startX = (GAME_WIDTH - totalWidth) / 2;
    float startY = 500;

    int index = 0;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            plots[index].bounds = (Rectangle){
                startX + col * (plotSize + spacing),
                startY + row * (plotSize + spacing),
                plotSize,
                plotSize};
            plots[index].isPlanted = false;
            plots[index].cropType = 0;
            plots[index].growTimer = 0;
            int minGrow = 300 - (gameLevel * 10); 
            if (minGrow < 60) minGrow = 60;
            int maxGrow = minGrow + 60;
            plots[index].growDuration = (float)(minGrow + rand() % (maxGrow - minGrow + 1));
            plots[index].isFullyGrown = false;
            index++;
        }
    }
}



void InitPens(Pen pens[]) {

    pens[0].bounds = (Rectangle){1600, 400, 200, 200};
    pens[0].animalType = ANIMAL_CAT; 
    pens[1].bounds = (Rectangle){200, 550, 200, 200};
    pens[1].animalType = ANIMAL_CHICKEN; 
    pens[2].bounds = (Rectangle){300, 800, 200, 200};
    pens[2].animalType = ANIMAL_COW; 

    pens[3].bounds = (Rectangle){1600, 650, 200, 200};
    pens[3].animalType = ANIMAL_DOG; 
    pens[4].bounds = (Rectangle){1000, 800, 200, 200};
    pens[4].animalType = ANIMAL_DUCK; 
    pens[5].bounds = (Rectangle){1400, 800, 200, 200};
    pens[5].animalType = ANIMAL_HORSE; 
    pens[6].bounds = (Rectangle){1250, 350, 200, 200};
    pens[6].animalType = ANIMAL_PIG; 
    pens[7].bounds = (Rectangle){900, 500, 200, 200};
    pens[7].animalType = ANIMAL_RABBIT; 
    pens[8].bounds = (Rectangle){1250, 600, 200, 200};
    pens[8].animalType = ANIMAL_SHEEP; 

    for (int i = 0; i < 9; i++) {
        pens[i].isOccupied = false;
        pens[i].animalIndex = -1;
        pens[i].inventoryIndex = -1;

    }
}


void HandlePlotPlanting(Plot plots[], int plotCount)
{
    if (currentZone != ZONE_PLANTS) return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetGameMousePosition();
        for (int i = 0; i < plotCount; i++)
        {
            if (CheckCollisionPointRec(mouse, plots[i].bounds))
            {
                int energyCost = 10 + (gameLevel - 1) * 4;

                if (!plots[i].isPlanted)
                {
                    if (energy >= energyCost)
                    {
                        PlantSeed(&plots[i]);
                    }
                    else
                    {
                        showNotEnoughEnergyPopup = true;
                        energyPopupTimer = 2.5f;
                    }
                }
                else if (plots[i].isFullyGrown)
                {
                    int baseMoney = 10;
                    int moneyReward = baseMoney - (gameLevel - 1);
                    if (moneyReward < 1) moneyReward = 1;

                    money += moneyReward;
                    plots[i].isPlanted = false;
                    plots[i].isFullyGrown = false;
                    plots[i].growTimer = 0;
                }

                break; 
            }
        }
    }
}




void UpdatePlantGrowth(Plot plots[], int plotCount, float deltaTime)
{
    for (int i = 0; i < plotCount; i++)
    {
        if (plots[i].isPlanted && !plots[i].isFullyGrown)
        {
            float growthRate = waterBoostActive ? waterBoostMultiplier : 1.0f;
            plots[i].growTimer += deltaTime * growthRate;
            if (plots[i].growTimer >= plots[i].growDuration)
            {
                plots[i].isFullyGrown = true;
            }
        }
    }
}


void UpdateGame(Farmer *farmer, Plant plants[], int *plantCount, Animal animals[], int *animalCount,Plot plots[], int plotCount)
{
    if (IsKeyDown(KEY_RIGHT))
        farmer->position.x += 2;
    if (IsKeyDown(KEY_LEFT))
        farmer->position.x -= 2;
    if (IsKeyDown(KEY_DOWN))
        farmer->position.y += 2;
    if (IsKeyDown(KEY_UP))
        farmer->position.y -= 2;

    Vector2 mouse = GetGameMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        
        if (currentZone == ZONE_MAIN)
        {
            if (CheckCollisionPointRec(mouse, leftArrowRect))
            {
                ChangeZone(ZONE_PLANTS);
            }
            else if (CheckCollisionPointRec(mouse, rightArrowRect))
            {
                ChangeZone(ZONE_ANIMALS);
            }
            else if (CheckCollisionPointRec(mouse, upArrowRect))
            {
                ChangeZone(ZONE_RELAX);
            }
        }
        else if ((currentZone == ZONE_RELAX && CheckCollisionPointRec(mouse, downArrowRect1)) ||
         (currentZone == ZONE_PLANTS && CheckCollisionPointRec(mouse, downArrowRect2)) ||
         (currentZone == ZONE_ANIMALS && CheckCollisionPointRec(mouse, downArrowRect3)))
{
    ChangeZone(ZONE_MAIN);
}

    }

    switch (currentZone)
    {
    case ZONE_PLANTS:
        UpdatePlantGrowth(plots, plotCount, GetFrameTime());
        HandlePlotPlanting(plots, plotCount);
        break;

    case ZONE_ANIMALS:
        UpdateEggs(GetFrameTime());
        DrawAnimals(animals, *animalCount);
        break;

    case ZONE_RELAX:
        RechargeEnergy(farmer, GetFrameTime());
        break;

    case ZONE_MAIN:
        
        break;

    case ZONE_COUNT:
        
        break;
    }
    UpdateAnimals(GetFrameTime(), animals, *animalCount);
}


void UnloadGame(Farmer *farmer, Plant plants[], int plantCount, Animal animals[], int animalCount)
{
    UnloadTexture(farmer->texture);
    for (int i = 0; i < plantCount; i++)
    {
        UnloadTexture(plants[i].texture);
    }
    for (int i = 0; i < animalCount; i++)
    {
        UnloadTexture(animals[i].texture);
    }
}

Texture2D shopTexture;
Texture2D relaxZoneTexture;

Texture2D cowTexture;
Texture2D chickenTexture;
Texture2D pigTexture;
Texture2D sheepTexture;
Texture2D cockTexture;
Texture2D chickenBabyTexture;
Texture2D satanaTexture;
Texture2D brownCowTexture;
Texture2D eggTexture;

Texture2D foodTexture;
Texture2D waterTexture;

Texture2D genericPlantedTexture;

Texture2D foodIcon;
Texture2D energyIcon;
Texture2D moneyIcon;
Texture2D arrowIconLeft;
Texture2D arrowIconRight;
Texture2D arrowIconUp;
Texture2D arrowIconDown;
Texture2D iconFood;
Texture2D plantIcon;
Texture2D animalIcon;
Texture2D relaxIcon;
Texture2D shopIcon;
Texture2D sellIcon;
Texture2D collectIcon;


Texture2D arrowBackStanga;
Texture2D dogShop;
Texture2D chickenShop;
Texture2D rabbitShop;
Texture2D sheepShop;
Texture2D catShop;
Texture2D duckShop;
Texture2D cowShop;
Texture2D pigShop;
Texture2D horseShop;

Texture2D capsuneShop;
Texture2D dafinShop;
Texture2D grauShop;
Texture2D mazareShop;
Texture2D morcovShop;
Texture2D porumbShop;
Texture2D spanacShop;
Texture2D telinaShop;
Texture2D varzaShop;

Texture2D woodPanelTexture;
Texture2D arrowBack;

Texture2D plotTex;
Texture2D waterTowerTex;
Texture2D toolStorageTex;
Texture2D dogHome;
Texture2D horseHome;
Texture2D chickenHome;
Texture2D rabbitHome;
Texture2D sheepHome;
Texture2D catHome;
Texture2D pigHome;
Texture2D duckHome;
Texture2D cowHome;

Texture2D plantTextures[9];
Texture2D penTextures[9];

void InitTextures()
{
    // Load egg texture
    Image eggImage = LoadImage("assets/egg.png");
    ImageResize(&eggImage, 32, 32);
    eggTexture = LoadTextureFromImage(eggImage);
    UnloadImage(eggImage);

    // Load plant textures
    Image img;
img = LoadImage("assets/seeds/wheat.png"); ImageResize(&img, 450, 450); plantTextures[0] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/corn.png"); ImageResize(&img, 450, 450); plantTextures[1] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/carrot.png"); ImageResize(&img, 450, 450); plantTextures[2] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/tomato.png"); ImageResize(&img, 450, 450); plantTextures[3] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/lettuce.png"); ImageResize(&img, 450, 450); plantTextures[4] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/potato.png"); ImageResize(&img, 450, 450); plantTextures[5] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/strawberry.png"); ImageResize(&img, 450, 450); plantTextures[6] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/onion.png"); ImageResize(&img, 450, 450); plantTextures[7] = LoadTextureFromImage(img); UnloadImage(img);
img = LoadImage("assets/seeds/cabbage.png"); ImageResize(&img, 450, 450); plantTextures[8] = LoadTextureFromImage(img); UnloadImage(img);

    genericPlantedTexture = LoadTexture("assets/seeds/baby_plant.png");


    penTextures[ANIMAL_CAT] = LoadTexture("assets/animal_home/cat_home.png");
    penTextures[ANIMAL_CHICKEN] = LoadTexture("assets/animal_home/chicken_home.png");
    penTextures[ANIMAL_COW] = LoadTexture("assets/animal_home/cow_home.png");
    penTextures[ANIMAL_DOG] = LoadTexture("assets/animal_home/dog_home.png");
    penTextures[ANIMAL_DUCK] = LoadTexture("assets/animal_home/duck_home.png");
    penTextures[ANIMAL_HORSE] = LoadTexture("assets/animal_home/horse_home.png");
    penTextures[ANIMAL_PIG] = LoadTexture("assets/animal_home/pig_home.png");
    penTextures[ANIMAL_RABBIT] = LoadTexture("assets/animal_home/rabbit_home.png");
    penTextures[ANIMAL_SHEEP] = LoadTexture("assets/animal_home/sheep_home.png");\
    
    foodIcon = LoadTexture("assets/icons/icon_food.png");
    energyIcon = LoadTexture("assets/icons/icon_energy.png");
    moneyIcon = LoadTexture("assets/icons/icon_money.png");

    Image arrowLeftImg = LoadImage("assets/arrows/icon_arrow_left.png");
    ImageResize(&arrowLeftImg, 100, 100);
    arrowIconLeft = LoadTextureFromImage(arrowLeftImg);
    UnloadImage(arrowLeftImg);

    Image arrowRightImg = LoadImage("assets/arrows/icon_arrow_right.png");
    ImageResize(&arrowRightImg, 100, 100);
    arrowIconRight = LoadTextureFromImage(arrowRightImg);
    UnloadImage(arrowRightImg);


    Image food = LoadImage("assets/icons/iconFood.png");
    ImageResize(&food, 270, 270);
    foodTexture = LoadTextureFromImage(food);
    UnloadImage(food);


    Image water = LoadImage("assets/icons/iconWater.png");
    ImageResize(&water, 270, 270);
    waterTexture = LoadTextureFromImage(water);
    UnloadImage(water);


    Image arrowBackImg = LoadImage("assets/arrows/back.png.png");
    ImageResize(&arrowBackImg, 250, 250);
    arrowBack = LoadTextureFromImage(arrowBackImg);
    UnloadImage(arrowBackImg);

    Image arrowBackStangaImg = LoadImage("assets/arrows/back_stanga.png");
    ImageResize(&arrowBackStangaImg, 250, 250);
    arrowBackStanga = LoadTextureFromImage(arrowBackStangaImg);
    UnloadImage(arrowBackStangaImg);

    Image plantIconImg = LoadImage("assets/arrows/arrow_plant_zone.png");
    ImageResize(&plantIconImg, 150, 150);
    plantIcon = LoadTextureFromImage(plantIconImg);
    UnloadImage(plantIconImg);

    Image animalIconImg = LoadImage("assets/arrows/arrow_animal_zone.png");
    ImageResize(&animalIconImg, 150, 150);
    animalIcon = LoadTextureFromImage(animalIconImg);
    UnloadImage(animalIconImg);

    Image relaxIconImg = LoadImage("assets/arrows/arrow_relax_zone.png");
    ImageResize(&relaxIconImg, 150, 150);
    relaxIcon = LoadTextureFromImage(relaxIconImg);
    UnloadImage(relaxIconImg);

    Image sellIconImg = LoadImage("assets/sell_icon.png");
    ImageResize(&sellIconImg, 32, 32);
    sellIcon = LoadTextureFromImage(sellIconImg);
    UnloadImage(sellIconImg);

    Image collectIconImg = LoadImage("assets/collect_icon.png");
    ImageResize(&collectIconImg, 32, 32);
    collectIcon = LoadTextureFromImage(collectIconImg);
    UnloadImage(collectIconImg);

    Image woodImage = LoadImage("assets/textures/wood_textures.png");
    ImageResize(&woodImage, 180, 40);
    woodPanelTexture = LoadTextureFromImage(woodImage);
    UnloadImage(woodImage);

    Image feedImg = LoadImage("assets/iconFood.png");
    ImageResize(&feedImg, 64, 64);
    iconFood = LoadTextureFromImage(feedImg);
    UnloadImage(feedImg);

    Image shopIconImage = LoadImage("assets/icons/icon_shop.png");
    if (shopIconImage.data == NULL)
    {
        printf("Error: Could not load shop icon image\n");
    }
    ImageResize(&shopIconImage, shopIconImage.width / 3, shopIconImage.height / 3);
    shopIcon = LoadTextureFromImage(shopIconImage);
    if (shopIcon.id == 0)
    {
        printf("Error: Could not create shop icon texture\n");
    }
    UnloadImage(shopIconImage);
    relaxZoneTexture = LoadTexture("assets/backgrounds/background_relax.png");

    Image shopBackgroundImage = LoadImage("assets/textures/wood_textures.png");
    if (shopBackgroundImage.data == NULL)
    {
        printf("Error: Could not load shop background image\n");
    }
    ImageResize(&shopBackgroundImage, 1500, 600);
    shopTexture = LoadTextureFromImage(shopBackgroundImage);
    if (shopTexture.id == 0)
    {
        printf("Error: Could not create shop background texture\n");
    }
    UnloadImage(shopBackgroundImage);

    Image capsuneImage = LoadImage("assets/shop/plante/capsuni.png");
    ImageResize(&capsuneImage, capsuneImage.width / 1.2, capsuneImage.height / 1.2);
    capsuneShop = LoadTextureFromImage(capsuneImage);
    UnloadImage(capsuneImage);

    Image dafinImage = LoadImage("assets/shop/plante/dafin.png");
    ImageResize(&dafinImage, dafinImage.width / 1.2, dafinImage.height / 1.2);
    dafinShop = LoadTextureFromImage(dafinImage);
    UnloadImage(dafinImage);

    Image grauImage = LoadImage("assets/shop/plante/grau.png");
    ImageResize(&grauImage, grauImage.width / 1.2, grauImage.height / 1.2);
    grauShop = LoadTextureFromImage(grauImage);
    UnloadImage(grauImage);

    Image mazareImage = LoadImage("assets/shop/plante/mazare.png");
    ImageResize(&mazareImage, mazareImage.width / 1.2, mazareImage.height / 1.2);
    mazareShop = LoadTextureFromImage(mazareImage);
    UnloadImage(mazareImage);

    Image morcovImage = LoadImage("assets/shop/plante/morcov.png");
    ImageResize(&morcovImage, morcovImage.width / 1.2, morcovImage.height / 1.2);
    morcovShop = LoadTextureFromImage(morcovImage);
    UnloadImage(morcovImage);

    Image porumbImage = LoadImage("assets/shop/plante/porumb.png");
    ImageResize(&porumbImage, porumbImage.width / 1.2, porumbImage.height / 1.2);
    porumbShop = LoadTextureFromImage(porumbImage);
    UnloadImage(porumbImage);

    Image spanacImage = LoadImage("assets/shop/plante/spanac.png");
    ImageResize(&spanacImage, spanacImage.width / 1.2, spanacImage.height / 1.2);
    spanacShop = LoadTextureFromImage(spanacImage);
    UnloadImage(spanacImage);

    Image telinaImage = LoadImage("assets/shop/plante/telina.png");
    ImageResize(&telinaImage, telinaImage.width / 1.2, telinaImage.height / 1.2);
    telinaShop = LoadTextureFromImage(telinaImage);
    UnloadImage(telinaImage);

    Image varzaImage = LoadImage("assets/shop/plante/varza.png");
    ImageResize(&varzaImage, varzaImage.width / 1.2, varzaImage.height / 1.2);
    varzaShop = LoadTextureFromImage(varzaImage);
    UnloadImage(varzaImage);

    Image dogShopImage = LoadImage("assets/shop/animale/caine.png");
    ImageResize(&dogShopImage, dogShopImage.width / 1.2, dogShopImage.height / 1.2);
    dogShop = LoadTextureFromImage(dogShopImage);
    UnloadImage(dogShopImage);

    Image horseShopImage = LoadImage("assets/shop/animale/cal.png");
    ImageResize(&horseShopImage, horseShopImage.width / 1.2, horseShopImage.height / 1.2);
    horseShop = LoadTextureFromImage(horseShopImage);
    UnloadImage(horseShopImage);

    Image chickenShopImage = LoadImage("assets/shop/animale/gaina.png");
    ImageResize(&chickenShopImage, chickenShopImage.width / 1.2, chickenShopImage.height / 1.2);
    chickenShop = LoadTextureFromImage(chickenShopImage);
    UnloadImage(chickenShopImage);

    Image rabbitShopImage = LoadImage("assets/shop/animale/iepure.png");
    ImageResize(&rabbitShopImage, rabbitShopImage.width / 1.2, rabbitShopImage.height / 1.2);
    rabbitShop = LoadTextureFromImage(rabbitShopImage);
    UnloadImage(rabbitShopImage);

    Image sheepShopImage = LoadImage("assets/shop/animale/oaie.png");
    ImageResize(&sheepShopImage, sheepShopImage.width / 1.2, sheepShopImage.height / 1.2);
    sheepShop = LoadTextureFromImage(sheepShopImage);
    UnloadImage(sheepShopImage);

    Image catShopImage = LoadImage("assets/shop/animale/pisica.png");
    ImageResize(&catShopImage, catShopImage.width / 1.2, catShopImage.height / 1.2);
    catShop = LoadTextureFromImage(catShopImage);
    UnloadImage(catShopImage);

    Image pigShopImage = LoadImage("assets/shop/animale/porc.png");
    ImageResize(&pigShopImage, pigShopImage.width / 1.2, pigShopImage.height / 1.2);
    pigShop = LoadTextureFromImage(pigShopImage);
    UnloadImage(pigShopImage);

    Image duckShopImage = LoadImage("assets/shop/animale/rata.png");
    ImageResize(&duckShopImage, duckShopImage.width / 1.2, duckShopImage.height / 1.2);
    duckShop = LoadTextureFromImage(duckShopImage);
    UnloadImage(duckShopImage);

    Image plotImage = LoadImage("assets/plot.PNG");
    ImageResize(&plotImage, 150, 150);
    plotTex = LoadTextureFromImage(plotImage);
    UnloadImage(plotImage);

    Image waterTowerImage = LoadImage("assets/water_tower.png");
    ImageResize(&waterTowerImage, 150, 200);
    waterTowerTex = LoadTextureFromImage(waterTowerImage);
    UnloadImage(waterTowerImage);

    Image toolStorageImage = LoadImage("assets/tool_storage.png");
    ImageResize(&toolStorageImage, 150, 200);
    toolStorageTex = LoadTextureFromImage(toolStorageImage);
    UnloadImage(toolStorageImage);

    Image dogHomeImage = LoadImage("assets/animal_home/dog_home.png");
    ImageResize(&dogHomeImage, 200, 150);
    dogHome = LoadTextureFromImage(dogHomeImage);
    UnloadImage(dogHomeImage);

    Image horseHomeImage = LoadImage("assets/animal_home/horse_home.png");
    ImageResize(&horseHomeImage, 200, 150);
    horseHome = LoadTextureFromImage(horseHomeImage);
    UnloadImage(horseHomeImage);

    Image chickenHomeImage = LoadImage("assets/animal_home/chicken_home.png");
    ImageResize(&chickenHomeImage, 200, 150);
    chickenHome = LoadTextureFromImage(chickenHomeImage);
    UnloadImage(chickenHomeImage);

    Image rabbitHomeImage = LoadImage("assets/animal_home/rabbit_home.png");
    ImageResize(&rabbitHomeImage, 200, 150);
    rabbitHome = LoadTextureFromImage(rabbitHomeImage);
    UnloadImage(rabbitHomeImage);

    Image sheepHomeImage = LoadImage("assets/animal_home/sheep_home.png");
    ImageResize(&sheepHomeImage, 200, 150);
    sheepHome = LoadTextureFromImage(sheepHomeImage);
    UnloadImage(sheepHomeImage);

    Image catHomeImage = LoadImage("assets/animal_home/cat_home.png");
    ImageResize(&catHomeImage, 200, 150);
    catHome = LoadTextureFromImage(catHomeImage);
    UnloadImage(catHomeImage);

    Image pigHomeImage = LoadImage("assets/animal_home/pig_home.png");
    ImageResize(&pigHomeImage, 200, 150);
    pigHome = LoadTextureFromImage(pigHomeImage);
    UnloadImage(pigHomeImage);

    Image duckHomeImage = LoadImage("assets/animal_home/duck_home.png");
    ImageResize(&duckHomeImage, 200, 150);
    duckHome = LoadTextureFromImage(duckHomeImage);
    UnloadImage(duckHomeImage);

    Image cowHomeImage = LoadImage("assets/animal_home/cow_home.png");
    ImageResize(&cowHomeImage, 200, 150);
    cowHome = LoadTextureFromImage(cowHomeImage);
    UnloadImage(cowHomeImage);
}


void InitAnimals(Animal animals[])
{

    Texture2D animalTextures[] = {
        LoadTexture("assets/cow.png"),
        LoadTexture("assets/sheep.png"),
        LoadTexture("assets/chicken.png"),
        LoadTexture("assets/cat.png"),
        LoadTexture("assets/dog.png"),
        LoadTexture("assets/duck.png"),
        LoadTexture("assets/horse.png"),
        LoadTexture("assets/pig.png"),
        LoadTexture("assets/satana.png")
    };

    Vector2 animalPositions[] = {
        {1500, 920}, {1500, 700}, {400, 600}, {1700, 390}, {1750, 700},
        {1100, 900}, {300, 800}, {1400, 500}, {1000, 700}
    };

    for (int i = 0; i < 9; i++)
    {
        animals[i].texture = animalTextures[i];
        animals[i].position = animalPositions[i];
        animals[i].age = 0.0f;
        animals[i].lifespan = 10.0f + GetRandomValue(5, 10); 
        animals[i].timeUntilHungry = 60.0f; 
        animals[i].maxTimeUntilHungry = 60.0f;
        animals[i].isHungry = false;
        animals[i].eggLayTimer = 12.0f + GetRandomValue(0, 12);
        animals[i].respawnTimer = 0;
        animals[i].alive = true;
        animals[i].canLayEggs = (i == 2 || i == 5); 

    }
}

void UpdateAnimals(float deltaTime, Animal animals[], int animalCount)
{    
    float animalDeltaTime = deltaTime;

    for (int i = 0; i < animalCount; i++)
    {
        if (!animals[i].alive)
{
    animals[i].respawnTimer -= animalDeltaTime;
    if (animals[i].respawnTimer <= 0)
    {
        animals[i].alive = true;
        animals[i].age = 0;
        animals[i].timeUntilHungry = animals[i].maxTimeUntilHungry;
        animals[i].isHungry = false;
    }

    continue; 
}


        animals[i].timeUntilHungry -= animalDeltaTime;
        if (animals[i].timeUntilHungry <= 0)
        {
            animals[i].isHungry = true;
            animals[i].timeUntilHungry = 0;
        }

        animals[i].age += animalDeltaTime / 24.0f;

        if (animals[i].canLayEggs && !animals[i].isHungry)
        {
            animals[i].eggLayTimer -= animalDeltaTime;
            if (animals[i].eggLayTimer <= 0 && eggCount < MAX_EGGS)
            {
                eggs[eggCount++] = (Egg){
                    .position = {animals[i].position.x + 20, animals[i].position.y + 20},
                    .texture = eggTexture,
                    .freshness = 24.0f,
                    .collected = false,
                    .textTimer = 0,
                    .showText = false
                };
                animals[i].eggLayTimer = 12.0f + (rand() % 12);
            }
        }


    }
    
}

void UpdateEggs(float deltaTime)
{
    for (int i = 0; i < eggCount; i++)
    {
        if (!eggs[i].collected)
        {
            eggs[i].freshness -= deltaTime;

            Vector2 mouse = GetGameMousePosition();
            Rectangle eggRect = {
                eggs[i].position.x,
                eggs[i].position.y,
                eggs[i].texture.width,
                eggs[i].texture.height
            };

            if (CheckCollisionPointRec(mouse, eggRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                eggs[i].collected = true;
                money += 10;
                eggs[i].showText = true;
                eggs[i].textTimer = 2.0f;
            }
        }
        else if (eggs[i].showText)
        {
            eggs[i].textTimer -= deltaTime;
            if (eggs[i].textTimer <= 0)
            {
                eggs[i].showText = false;
            }
        }
    }
}


void FeedAnimal(int animalIndex, int *foodStock, Animal animals[])
{
    if (*foodStock > 0 && animalIndex >= 0)
    {
        animals[animalIndex].timeUntilHungry = animals[animalIndex].maxTimeUntilHungry;
        animals[animalIndex].isHungry = false;
        (*foodStock)--;
    }
    if (animals[animalIndex].canLayEggs && eggCount < MAX_EGGS)
        {
            eggs[eggCount++] = (Egg){
                .position = {
                animals[animalIndex].position.x + 70,
                animals[animalIndex].position.y + 80
                },

                .texture = eggTexture,
                .freshness = 24.0f,
                .collected = false,
                .textTimer = 0,
                .showText = false
            };
        }
}

void SellAnimal(int index, Animal animals[])
{
    if (animals[index].alive)
    {
        int value = 50 + (int)(animals[index].age * 5); 
        money += value;

        animals[index].alive = false;
        animals[index].respawnTimer = 60.0f;
    }
}


void ChangeZone(ZoneType newZone)
{
    if (newZone >= 0 && newZone < ZONE_COUNT)
    {
        currentZone = newZone;
        if (newZone == ZONE_PLANTS || newZone == ZONE_ANIMALS || newZone == ZONE_RELAX)
        {
            
        }
    }
}

void RechargeEnergy(Farmer *farmer, float deltaTime)
{
    if (currentZone == ZONE_RELAX)
    {
        energyAccumulator += deltaTime * 5.0f;  
        if (energyAccumulator >= 1.0f)         
        {
            int energyToAdd = (int)energyAccumulator;
            energy += energyToAdd;
            energyAccumulator -= energyToAdd;   

            if (energy > 100)
                energy = 100;
        }
    }
}

void UseEnergy(int amount)
{
    energy -= amount;
    if (energy < 0)
    {
        energy = 0;
    }
}

void UnloadTextures()
{
    UnloadTexture(cowTexture);
    UnloadTexture(chickenTexture);
    UnloadTexture(pigTexture);
    UnloadTexture(sheepTexture);
    UnloadTexture(cockTexture);
    UnloadTexture(chickenBabyTexture);
    UnloadTexture(satanaTexture);
    UnloadTexture(brownCowTexture);
    UnloadTexture(eggTexture);

    for (int i = 0; i < 9; i++)
    {
        UnloadTexture(plantTextures[i]);
    }

    UnloadTexture(relaxZoneTexture);
    UnloadTexture(foodIcon);
    UnloadTexture(energyIcon);
    UnloadTexture(moneyIcon);
    UnloadTexture(iconFood);
    UnloadTexture(plantIcon);
    UnloadTexture(animalIcon);
    UnloadTexture(relaxIcon);
    UnloadTexture(shopIcon);
    UnloadTexture(sellIcon);
    UnloadTexture(collectIcon);

    UnloadTexture(arrowIconLeft);
    UnloadTexture(arrowIconRight);
    UnloadTexture(arrowIconUp);
    UnloadTexture(arrowIconDown);
    UnloadTexture(woodPanelTexture);

    UnloadTexture(dogShop);
    UnloadTexture(chickenShop);
    UnloadTexture(rabbitShop);
    UnloadTexture(sheepShop);
    UnloadTexture(catShop);
    UnloadTexture(duckShop);
    UnloadTexture(cowShop);
    UnloadTexture(pigShop);
    UnloadTexture(horseShop);

    UnloadTexture(capsuneShop);
    UnloadTexture(dafinShop);
    UnloadTexture(grauShop);
    UnloadTexture(mazareShop);
    UnloadTexture(morcovShop);
    UnloadTexture(porumbShop);
    UnloadTexture(spanacShop);
    UnloadTexture(telinaShop);
    UnloadTexture(varzaShop);

    UnloadTexture(shopTexture);

    UnloadTexture(plotTex);
    UnloadTexture(waterTowerTex);
    UnloadTexture(toolStorageTex);
    UnloadTexture(dogHome);
    UnloadTexture(horseHome);
    UnloadTexture(chickenHome);
    UnloadTexture(rabbitHome);
    UnloadTexture(sheepHome);
    UnloadTexture(catHome);
    UnloadTexture(pigHome);
    UnloadTexture(duckHome);
    UnloadTexture(cowHome);
}
void ResetGame(int *money, int *energy,Animal animals[], int *animalCount, Pen pens[], int penCount) {
    *money = 100;
    *energy = 100;
    for (int i = 0; i < penCount; i++) {
        pens[i].isOccupied = false;
        pens[i].animalIndex = -1;
    }

    printf("Jocul a fost resetat la starea inițială.\n");
}

void DrawNotEnoughEnergyPopup(Font shopfont)
{
    if (!showNotEnoughEnergyPopup)
        return;

    DrawRectangle(700, 400, 500, 200, (Color){200, 255, 200, 255});
    DrawRectangleLines(700, 400, 500, 200, GREEN);

    DrawTextEx(shopfont, "Nu ai destula energie!", (Vector2){730, 450}, 32, 2, DARKGREEN);

    Rectangle closeButton = {1150, 410, 40, 40};
    DrawRectangleRec(closeButton, (Color){0, 150, 0, 255});
    DrawTextEx(shopfont, "X", (Vector2){1160, 415}, 24, 1, WHITE);

    Vector2 mouse = GetGameMousePosition();
    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showNotEnoughEnergyPopup = false;
    }

    energyPopupTimer -= GetFrameTime();
    if (energyPopupTimer <= 0)
    {
        showNotEnoughEnergyPopup = false;
    }
}

void DrawLevelUpPopup(Font shopfont)
{
    if (!showLevelUpPopup)
        return;

    
    DrawRectangle(650, 380, 600, 220, (Color){255, 240, 150, 255});  
    DrawRectangleLines(650, 380, 600, 220, (Color){255, 140, 0, 255}); 

    DrawTextEx(shopfont, "* Ai urcat un nivel! *", (Vector2){720, 420}, 36, 3, (Color){255, 100, 0, 255});

    DrawTextEx(shopfont, "Felicitari, fermierule harnic!", (Vector2){700, 470}, 28, 2, (Color){150, 50, 0, 255});

    Rectangle closeButton = {1200, 390, 40, 40};
    DrawRectangleRec(closeButton, (Color){200, 0, 0, 255});
    DrawTextEx(shopfont, "X", (Vector2){1210, 395}, 24, 2, WHITE);

    Vector2 mouse = GetGameMousePosition();
    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showLevelUpPopup = false;
    }
    
    levelUpPopupTimer -= GetFrameTime();
    if (levelUpPopupTimer <= 0)
    {
        showLevelUpPopup = false;
    }
}



void DrawNotEnoughWaterPopup(Font shopfont)
{
    if (!showNotEnoughWaterPopup)
        return;

    DrawRectangle(700, 400, 500, 200, (Color){200, 200, 255, 255});
    DrawRectangleLines(700, 400, 500, 200, BLUE);

    DrawTextEx(shopfont, "Nu ai destula apa!", (Vector2){730, 450}, 32, 2, DARKBLUE);

    Rectangle closeButton = {1150, 410, 40, 40};

    DrawRectangleRec(closeButton, (Color){0, 0, 150, 255});
    DrawTextEx(shopfont, "X", (Vector2){1160, 415}, 24, 1, WHITE);

    Vector2 mouse = GetGameMousePosition();
    if (CheckCollisionPointRec(mouse, closeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showNotEnoughWaterPopup = false;
    }

    waterPopupTimer -= GetFrameTime();
    if (waterPopupTimer <= 0)
    {
        showNotEnoughWaterPopup = false;
    }
}
