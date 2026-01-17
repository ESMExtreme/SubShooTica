#ifndef ENEMYCONFIG_H
#define ENEMYCONFIG_H

#include <string>
#include <vector>

// Typ przeciwnika
enum class EnemyType {
    Shape,      // Kwadrat z kolorem
    Texture     // Sprite z teksturą
};

// Konfiguracja pojedynczego typu przeciwnika
struct EnemyTypeConfig {
    EnemyType type = EnemyType::Shape;
    std::string texturePath = "";
    int hp = 1;
    float speed = 100.0f;
    float sizeX = 30.0f;
    float sizeY = 30.0f;
    int colorR = 0;
    int colorG = 255;
    int colorB = 0;
    int scrapReward = 10;
    int spawnWeight = 1; // Waga spawnu (większa = częściej)
};

// Konfiguracja poziomu
struct EnemyConfig {
    float spawnInterval = 2.0f;
    int maxEnemies = 10;
    std::vector<EnemyTypeConfig> enemyTypes; // Różne typy przeciwników

    // Stara struktura dla kompatybilności wstecznej
    int hp = 1;
    float speed = 100.0f;
    float sizeX = 30.0f;
    float sizeY = 30.0f;
    int colorR = 0;
    int colorG = 255;
    int colorB = 0;
    int scrapReward = 10;
};

#endif // ENEMYCONFIG_H

