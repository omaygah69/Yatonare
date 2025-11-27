#include "raylib.h"
#include "terrain.h"
#include "raygui.h"                
#include "gui.h"

int main()
{
    const int width = 1280;
    const int height = 720;
    InitWindow(width, height, "cs2");

    bool auto_update = false;
    int map_width = 100;
    int map_height = 100;
    DrawMode draw_mode = COLOR_MAP;
    float noise_scale = 35.0f;
    int octaves = 5;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    int seed = 0;
    Vector2 offset = { 0.0f, 0.0f};
    std::vector<TerrainType> regions;
    Vector3 model_position = { -8.0f, 0.0f, -8.0f }; 

    regions.push_back({"DarkWater", 0.3f, {54, 103, 199, 255}});
    regions.push_back({"Water", 0.4f, BLUE});
    regions.push_back({"Sand", 0.46f, BEIGE});
    regions.push_back({"Grass", 0.55f, LIME});
    regions.push_back({"Trees", 0.6f, DARKGREEN});
    regions.push_back({"Forrest", 0.7f, { 50, 63, 47, 255 }});
    regions.push_back({"Mountain", 0.8f, DARKBROWN});
    regions.push_back({"Dagestan", 0.9f, { 50, 50, 47, 255 }});
    regions.push_back({"Siberia", 1.0f, RAYWHITE});

    Image noise = GenerateNoiseMap(map_width, map_height, seed, noise_scale,
                                   octaves, persistence, lacunarity, offset);
    Image color_map = GenerateColorMap(map_width, map_height, noise, regions);
    Texture2D texture = LoadTextureFromImage(noise);
    Terrain terrain = { 0 };
    Vector3 mesh_size = {1000, 200, 1000};
    terrain.texture = LoadTextureFromImage(color_map);
    terrain.mesh = GenLeveledMesh(noise, mesh_size);
    terrain.model = LoadModelFromMesh(terrain.mesh);
    terrain.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture

    // SetTextureFilter(texture, ICON_FILTER_POINT); // or FILTER_TRILINEAR
    Camera camera = { 0 };
    camera.position = (Vector3){ mesh_size.x / 2, mesh_size.y + 100.0f, mesh_size.z / 2 };          
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };          
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };              
    camera.fovy = 50.0f;                                    
    camera.projection = CAMERA_PERSPECTIVE;          
    
    bool paused = false;
    // UnloadImage(color_map);
    // UnloadImage(noise);
    DisableCursor();
    while(!WindowShouldClose())
    {
        int side_bar_width = GetScreenWidth() / 5;
        int side_bar_pos = GetScreenWidth() - side_bar_width;
        float slider_pos = side_bar_pos + 5.0f;
        auto_update = false;
        if (IsKeyPressed(KEY_F)) {
            paused = !paused;
            if (paused) {
                ShowCursor();
            } else {
                DisableCursor();
            }
        }
        if(!paused)
            UpdateCamera(&camera, CAMERA_FREE);

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);
        DrawModel(terrain.model, model_position, 1.0f, RAYWHITE);
        EndMode3D();
        // DrawTexture(texture, posX, posY, WHITE);
        DrawTextureEx(texture, (Vector2){0, 0}, 0.0f, 1.0f, WHITE);
        DrawRectangle(side_bar_pos, 0, GetScreenWidth(), GetScreenHeight() / 2, (Color){ 232, 232, 232, 255 });

        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 40, 120, 20 }, "Scale", &noise_scale, 0.0f, 100.0f);
        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 80, 120, 20 }, "Lacunarity", &lacunarity, 0.0f, 10.0f);
        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 120, 120, 20 }, "Persistence", &persistence, 0.0f, 5.0f);
        auto_update |= GuiSliderInt((Rectangle){ slider_pos + 100, 160, 120, 20 }, "Seed", &seed, 1, 10);
        auto_update |= GuiSliderInt((Rectangle){ slider_pos + 100, 200, 120, 20 }, "Octaves", &octaves, 1, 10);
        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 240, 120, 20 }, "Offsetx", &offset.x, 0.0f, 5.0f);
        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 280, 120, 20 }, "Offsety", &offset.y, 0.0f, 5.0f);
        auto_update |= GuiSliderFloat((Rectangle){ slider_pos + 100, 320, 120, 20 }, "MeshHeight", &mesh_size.y, 0.0f, 300.0f);
        
        if(auto_update)
        {
            UnloadTexture(texture);
            UnloadTexture(terrain.texture);  
            UnloadImage(noise);
            noise = GenerateNoiseMap(map_width, map_height, seed, noise_scale, octaves, persistence, lacunarity, offset);
            if(draw_mode == NOISE_MAP){
                texture = LoadTextureFromImage(noise);
            }
            if(draw_mode == COLOR_MAP){
                color_map = GenerateColorMap(map_width, map_height, noise, regions);
                texture = LoadTextureFromImage(color_map);
                terrain.texture = texture;
                terrain.mesh = GenLeveledMesh(noise, mesh_size);
                terrain.model = LoadModelFromMesh(terrain.mesh);
                terrain.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; 
                UnloadImage(color_map);
            }
        }
        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadTexture(terrain.texture);
    UnloadModel(terrain.model);
    CloseWindow();
    return 0;
}
