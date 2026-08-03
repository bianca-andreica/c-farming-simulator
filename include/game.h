#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define MAX_PLANTS 50
#define MAX_ANIMALS 20
#define MAX_PLOTS 27
#define MAX_PENS 9
#define MAX_INVENTORY_ITEMS 100
#define MAX_EGGS 20
#define PEN_COUNT ANIMAL_TYPE_COUNT


typedef enum
{
    ZONE_PLANTS,
    ZONE_ANIMALS,
    ZONE_RELAX,
    ZONE_MAIN, 
    ZONE_COUNT
} ZoneType;

typedef enum {
    ANIMAL_CAT,
    ANIMAL_CHICKEN,
    ANIMAL_COW,
    ANIMAL_DOG,
    ANIMAL_DUCK,
    ANIMAL_HORSE,
    ANIMAL_PIG,
    ANIMAL_RABBIT,
    ANIMAL_SHEEP,
    ANIMAL_TYPE_COUNT
} AnimalType;



extern int foodStock;
extern int plotCount;
extern int penCount;
extern ZoneType currentZone;

extern Rectangle shopButtonRect;
extern Rectangle leftArrowRect;
extern Rectangle rightArrowRect;
extern Rectangle upArrowRect;
extern Rectangle downArrowRect1;
extern Rectangle downArrowRect2;
extern Rectangle downArrowRect3;
extern Texture2D animalTextures[];

typedef struct
{
    Vector2 position;
    Texture2D texture;
    int energy;
    int money;
} Farmer;

typedef struct
{
    Vector2 position;
    Texture2D texture;
    int growthTime;
    int energyCost;
    int value;
    bool isGrown;
} Plant;

typedef struct
{
    Vector2 position;
    Texture2D texture;
    float timeUntilHungry;
    float maxTimeUntilHungry;
    float lifespan;    
    float age;         
    bool canLayEggs;   
    float eggLayTimer; 
    Rectangle feedButtonRect;
    Rectangle sellButtonRect;
    bool isHungry;
    float respawnTimer;
    bool alive;
    bool isVisible;
    bool isHovered;
    double hoverExpireTime;

} Animal;

typedef struct
{
    Vector2 position;
    Texture2D texture;
    float freshness; 
    bool collected;
    Rectangle collectRect;
    float textTimer;
    bool showText;
} Egg;

typedef struct
{
    Rectangle bounds;
    bool isPlanted;
    int cropType;
    float growTimer;     
    float growDuration;  
    bool isFullyGrown;
    int currentGrowthStage; 
} Plot;

typedef struct
{
    Rectangle bounds;
    bool isOccupied;
    int animalIndex;
    int inventoryIndex;
    AnimalType animalType;
} Pen;


typedef enum { ITEM_ANIMAL, ITEM_PLANT } ItemType;

typedef struct {
    Texture2D icon;
    const char *name;
    int quantity;
    ItemType type;
} InventoryItem;


extern int gameLevel;        
extern int energyUsedLevel;  
extern float energyAccumulator;

extern bool showNotEnoughEnergyPopup;
extern float energyPopupTimer;

extern bool showLevelUpPopup ;
extern float levelUpPopupTimer ;
extern bool shopOpen;

extern int selectedCategory;
extern int currentPage;
extern Plot plots[];
extern Texture2D shopTexture;
extern Texture2D relaxZoneTexture;


extern Texture2D cowTexture;
extern Texture2D chickenTexture;
extern Texture2D pigTexture;
extern Texture2D sheepTexture;
extern Texture2D cockTexture;
extern Texture2D chickenBabyTexture;
extern Texture2D satanaTexture;
extern Texture2D brownCowTexture;
extern Texture2D eggTexture;
extern Texture2D arrowBack;

extern Texture2D foodIcon;
extern Texture2D energyIcon;
extern Texture2D moneyIcon;
extern Texture2D iconFood;
extern Texture2D plantIcon;
extern Texture2D animalIcon;
extern Texture2D relaxIcon;
extern Texture2D shopIcon;
extern Texture2D sellIcon;
extern Texture2D collectIcon;


extern Texture2D arrowIconRight;
extern Texture2D arrowIconLeft;
extern Texture2D arrowIconUp;
extern Texture2D arrowIconDown;
extern Texture2D woodPanelTexture;


extern Texture2D dogShop;
extern Texture2D chickenShop;
extern Texture2D rabbitShop;
extern Texture2D sheepShop;
extern Texture2D catShop;
extern Texture2D duckShop;
extern Texture2D cowShop;
extern Texture2D pigShop;
extern Texture2D horseShop;

extern Texture2D capsuneShop;
extern Texture2D dafinShop;
extern Texture2D grauShop;
extern Texture2D mazareShop;
extern Texture2D morcovShop;
extern Texture2D porumbShop;
extern Texture2D spanacShop;
extern Texture2D telinaShop;
extern Texture2D varzaShop;

extern bool showNotEnoughWaterPopup;
extern float waterPopupTimer;
extern bool waterBoostActive;
extern float waterBoostMultiplier;



extern Texture2D foodTexture;
extern Texture2D waterTexture;
extern int waterStock;
extern int money;
extern InventoryItem inventory[MAX_INVENTORY_ITEMS];
extern int inventoryCount;
extern Pen pens[];
extern int energy;
extern Egg eggs[MAX_EGGS];
extern int eggCount;
extern Texture2D genericPlantedTexture;

extern Texture2D plotTex;
extern Texture2D waterTowerTex;
extern Texture2D toolStorageTex;
extern Texture2D dogHome;
extern Texture2D horseHome;
extern Texture2D chickenHome;
extern Texture2D rabbitHome;
extern Texture2D sheepHome;
extern Texture2D catHome;
extern Texture2D pigHome;
extern Texture2D duckHome;
extern Texture2D cowHome;
extern Texture2D arrowBackStanga;
extern Texture2D plantTextures[]; 
extern Texture2D penTextures[];
void InitRectangles(void);
void InitGame(Farmer *farmer, Plant plants[], int *plantCount, Animal animals[], int *animalCount);
void UpdateGame(Farmer *farmer, Plant plants[], int *plantCount,Animal animals[], int *animalCount, Plot plots[], int plotCount);
void UnloadGame(Farmer *farmer, Plant plants[], int plantCount, Animal animals[], int animalCount);
void InitTextures();
void UnloadTextures();
void InitAnimals(Animal animals[]);
void UpdateAnimals(float deltaTime, Animal animals[], int animalCount);
void FeedAnimal(int animalIndex, int *foodStock, Animal animals[]);
void SellAnimal(int animalIndex, Animal animals[]);
void InitPlots(Plot plots[]);
void InitPens(Pen pens[]);
void HandlePlotPlanting(Plot plots[], int plotCount);
void UpdateEggs(float deltaTime);
void ChangeZone(ZoneType newZone);
void RechargeEnergy(Farmer *farmer, float deltaTime);
void UseEnergy(int amount);
void UpdatePlantGrowth(Plot plots[], int plotCount, float deltaTime);
void ResetGame(int *money, int *energy, Animal animals[], int *animalCount, Pen pens[], int penCount);
void PlantSeed(Plot *plot);
void DrawNotEnoughEnergyPopup(Font shopfont);
void DrawLevelUpPopup(Font shopfont);
void DrawNotEnoughWaterPopup(Font shopfont);
#endif
