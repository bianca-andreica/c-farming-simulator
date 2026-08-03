#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#define MAX_SHOP_ITEMS 9
#define MAX_PLACED_OBJECTS 100
#define MAX_CROPS 9
typedef struct
{
    Texture2D icon;
    char *name;
    int price;
    int *quantity;
    Rectangle buyButton;
} ShopItemUI;

typedef struct
{
    Texture2D texture;
    Vector2 position;
    const char *name;
} PlacedObject;

extern int selectedPlotIndex ;
extern int selectedQuantity ;
extern int selectedItem ;
extern PlacedObject placedObjects[MAX_PLACED_OBJECTS];
extern int placedObjectCount;
extern ShopItemUI shopItems[MAX_SHOP_ITEMS];
extern ShopItemUI plantItems[MAX_SHOP_ITEMS];
extern int totalAnimalItems;
extern int totalPlantItems;
extern int selectedCropType;
extern bool inventoryOpen;
extern bool placingObject;
extern Font shopfont;
extern PlacedObject selectedObjectToPlace;
extern Rectangle downArrowRect;
extern int selectedInventoryCategory;
extern int inventoryPage;

void InitShopItems(void);

void DrawFarmer(Farmer *farmer);
void DrawAnimals(Animal animals[], int animalCount);
void DrawAnimalHUD(Animal animals[], int index, Texture2D feedIcon, int *foodStock,Font shopfont);
void DrawPlots(Plot plots[], int plotCount, Texture2D plantIcon);
void DrawEggs(Font shopfont);
void DrawZoneArrows(void);
void DrawCurrentZone(Texture2D background, Animal animals[], int animalCount);
void DrawRelaxZone(Texture2D background);
void DrawPlantZone(void);
void DrawShopButton(void);
void DrawShopItem(ShopItemUI item, int x, int y, Font shopFont);
void InitShopItems(void);
bool PointInTriangle(Vector2 pt, Vector2 v1, Vector2 v2, Vector2 v3);
void AddToInventory(ShopItemUI item);
void DrawNotEnoughMoneyPopup(Font shopfont);
void CheckBarnClick(bool *clickHandled);
void DrawInventory(bool *clickHandled, Font shopfont);
void DrawShop(bool *clickHandled,bool *shopOpen);
void DrawHUD(int foodStock, int energy, int money);
void SaveGame(const char* filename, int money, int energy, int food,Pen pens[], int penCount,Plot plots[], int plotCount);
void LoadGame(const char* filename, int* money, int* energy, int* food,Pen pens[], int penCount,Plot plots[], int plotCount);
#endif
