#pragma once
#include "raylib.h"
#include <string>
#include <vector>

enum DrawMode
{
    NOISE_MAP,
    COLOR_MAP
};

typedef struct TerrainType
{
    std::string Name;
    float height;
    Color color;
} TerrainType;

typedef struct Terrain
{
    Texture2D texture;
    Mesh mesh;
    Model model;
} Terrain;

Image GenerateNoiseMap(int mapwidth, int mapheight, int seed, float scale, int octaves, float persistence, float lacunarity, Vector2 offset);
float InverseLerp(float a, float b, float value);
Image GenerateColorMap(int map_width, int map_height, const Image& noise, std::vector<TerrainType> regions);
Mesh GenLeveledMesh(Image heightmap, Vector3 size);
