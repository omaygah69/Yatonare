#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "terrain.h"
#include <random>
#include <limits>
#include <algorithm>

Image GenerateNoiseMap(int mapwidth, int mapheight, int seed, float scale, int octaves, float persistence, float lacunarity, Vector2 offset)
{
    std::vector<float> noise_map(mapwidth * mapheight);
    Image image = GenImageColor(mapwidth, mapheight, BLACK);
    Color *pixels = (Color *)image.data;
    
    std::minstd_rand prng(seed);
    std::uniform_real_distribution<float> dist(-100000.0f, 100000.0f);
    std::vector<Vector2> octave_offsets(octaves);

    for(int i = 0; i < octaves; i++){
        float offsetx = dist(prng) + offset.x;
        float offsety = dist(prng) + offset.y;
        octave_offsets[i] = (Vector2){offsetx, offsety};
    }

    if(scale <= 0) scale = 0.001f;
    float max_noise_height = std::numeric_limits<float>::lowest();
    float min_noise_height = std::numeric_limits<float>::max();
    float half_width = mapwidth / 2.0f;
    float half_height = mapheight / 2.0f;


    for(int y = 0; y < mapheight; y++)
    {
        for(int x = 0; x < mapwidth; x++)
        {
            float amplitude = 1;
            float frequency = 1;
            float noise_height = 0;

            for(int i = 0; i < octaves; i++)
            {
                float samplex = (x - half_width) / scale * frequency + octave_offsets[i].x;
                float sampley = (y - half_height) / scale * frequency + octave_offsets[i].y;
                // float sampley = (float)y / mapwidth * scale * frequency;

                float perlin_value = stb_perlin_noise3(samplex, sampley, 0.0f, 0,0,0);
                perlin_value = (perlin_value + 1.0f) * 0.5f;   // remap to 0 - 1
                // I set this to * 2 - 1 map from -1 to 1 but stpperlin already outputs -1 to 1
                // so the image looks very sharp
                noise_height += (perlin_value) * amplitude;
                
                amplitude *= persistence;
                frequency *= lacunarity;
            }

            if(noise_height > max_noise_height) max_noise_height = noise_height;
            if(noise_height < min_noise_height) min_noise_height = noise_height;            

            noise_map[y * mapwidth + x] = noise_height;
        }
    }

    for(int y = 0; y < mapheight; y++)
    {
        for(int x = 0; x < mapwidth; x++)
        {
            float norm = InverseLerp(min_noise_height, max_noise_height, noise_map[y * mapwidth + x]);
            unsigned char c = (unsigned char)(norm * 255);
            pixels[y * mapwidth + x] = (Color){ c, c, c, 255 };
        }
    }

    return image;
}

Image GenerateColorMap(int map_width, int map_height, const Image& noise, std::vector<TerrainType> regions)
{
    std::sort(regions.begin(), regions.end(), [](const TerrainType& a, const TerrainType& b) {
        return a.height < b.height;
    });
    Image color_map = GenImageColor(map_width, map_height, WHITE);
    Color* pixel_colors = (Color*)color_map.data;
    Color* pixel_noise = (Color*)noise.data;
    
    for(int y = 0; y < map_height; y++){
        for(int x = 0; x < map_width; x++){
            float current_height = pixel_noise[y * map_width + x].r / 255.0f;
            for(int i = 0; i < regions.size(); i++){
                if(current_height <= regions[i].height){
                    pixel_colors[y * map_width + x] = regions[i].color;
                    break;
                }
            }
        }
    }

    return color_map;
}

float InverseLerp(float a, float b, float value)
{
    if (a == b) return 0.0f;
    float t = (value - a) / (b - a);
    if (t < 0.0f) return 0.0f;
    if (t > 1.0f) return 1.0f;
    return t;
}

// float PerlinNoise(float x, float y)
// {
//     float n = stb_perlin_noise3(x, y, 0.0f, 0,0,0);
//     // map from -1 to 1
//     n = (n + 1.0f) * 0.5;
//     return n;
// }
