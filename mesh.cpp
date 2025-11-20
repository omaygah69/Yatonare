#include "terrain.h"

Mesh GenLeveledMesh(Image heightmap, Vector3 size) {
    Color *pixels = (Color *)heightmap.data;
    int w = heightmap.width;
    int h = heightmap.height;
    // Flatten height: for any pixel whose normalized brightness <= sea level,
    // set it to exactly sea level (or you could set to zero)
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = y * w + x;
            float norm = pixels[i].r / 255.0f;
            if (norm < 0.4f) {
                unsigned char v = (unsigned char)(0.4f * 255.0f);
                pixels[i].r = v;
                pixels[i].g = v;
                pixels[i].b = v;
                // optionally leave alpha or set it too
            }
        }
    }
    Mesh mesh = GenMeshHeightmap(heightmap, size); 
    return mesh;
}
