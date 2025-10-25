#include <raylib.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main() {
    Vector2 Window = {900, 600};
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(Window.x, Window.y, "Model Visualizer");
    SetTargetFPS(30);

    Model model = LoadModel("Models/SpikeBall.obj");
    Texture2D texture = LoadTexture("Textures/RandomTexture.jpg");
    model.materials[0].maps[0].texture = texture;

    Image backgroundImage = LoadImage("Images/BackgroundImage.jpg");
    ImageResize(&backgroundImage, Window.x, backgroundImage.height * (Window.x / backgroundImage.width));
    Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);

    Font mainFont = LoadFontEx("Fonts/MainFont.ttf", 100, 0, 250);
    GenTextureMipmaps(&mainFont.texture);
    SetTextureFilter(mainFont.texture, TEXTURE_FILTER_BILINEAR);
    GuiSetFont(mainFont);

    Vector3 modelPos = {0, 0, 0};
    Vector3 modelAxes = {0, 0, 0};
    Vector3 modelScale = {0.5f, 0.5f, 0.5f};

    Camera3D camera = {0};
    camera.position = (Vector3){0, 0, 5};
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 60;
    camera.projection = CAMERA_PERSPECTIVE;

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt((Color){255, 255, 255, 240}));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 12);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 0);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt((Color){255, 255, 255, 10}));
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt((Color){255, 255, 255, 250}));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt((Color){255, 255, 255, 40}));
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt((Color){255, 255, 255, 40}));
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt((Color){255, 255, 255, 200}));
    GuiSetFont(mainFont);

    float rotationAngle = 0;
    bool toggleWires = true;
    char cameraMode = 0;
    const char *textfile = LoadFileText("float.c");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureV(backgroundTexture, (Vector2){0, 0}, WHITE);

        const char *infoFPS = TextFormat("%i fps", GetFPS());
        const char *infoScreen = TextFormat("%ix%i Screen", GetScreenWidth(), GetScreenHeight());
        const char *infoRender = TextFormat("%ix%i Render", GetRenderWidth(), GetRenderHeight());
        const char *infoObj = TextFormat("%s", textfile);

        DrawTextEx(mainFont, "Texture loaded successfully", (Vector2){10, 130}, 12, 0, WHITE);
        DrawTextEx(mainFont, model.meshCount == 0 ? "Invalid Model" : "Model loaded successfully",
                   (Vector2){10, 150}, 12, 0, model.meshCount == 0 ? RED : WHITE);
        DrawTextEx(mainFont, backgroundImage.width == 0 || backgroundImage.height == 0 ? "Invalid Image" : "Image loaded successfully",
                   (Vector2){10, 170}, 12, 0, backgroundImage.width == 0 || backgroundImage.height == 0 ? RED : WHITE);
        DrawTextEx(mainFont, mainFont.texture.id == 0 ? "Invalid Font" : "Font loaded successfully",
                   (Vector2){10, 190}, 12, 0, mainFont.texture.id == 0 ? RED : WHITE);

        DrawTextEx(mainFont, infoFPS, (Vector2){10, 10}, 50, 0, WHITE);
        DrawTextEx(mainFont, infoScreen, (Vector2){200, 24}, 14, 0, WHITE);
        DrawTextEx(mainFont, infoRender, (Vector2){200, 44}, 14, 0, WHITE);
        DrawTextEx(mainFont, infoObj, (Vector2){10, 220}, 10, 0, (Color){255, 255, 255, 200});

        switch (cameraMode) {
            case 0: DrawTextEx(mainFont, "Camera Mode: CAMERA_ORBITAL", (Vector2){600, 20}, 12, 0, WHITE); break;
            case 1: DrawTextEx(mainFont, "Camera Mode: CAMERA_FREE", (Vector2){600, 20}, 12, 0, WHITE); break;
            case 2: DrawTextEx(mainFont, "Camera Mode: CAMERA_CUSTOM", (Vector2){600, 20}, 12, 0, WHITE); break;
            case 3: DrawTextEx(mainFont, "Camera Mode: CAMERA_FIRST_PERSON", (Vector2){600, 20}, 12, 0, WHITE); break;
            case 4: DrawTextEx(mainFont, "Camera Mode: CAMERA_THIRD_PERSON", (Vector2){600, 20}, 12, 0, WHITE); break;
        }

        BeginMode3D(camera);
        if (toggleWires)
            DrawModelEx(model, modelPos, modelAxes, rotationAngle, modelScale, WHITE);
        else
            DrawModelWiresEx(model, modelPos, modelAxes, rotationAngle, modelScale, WHITE);
        EndMode3D();

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z)) GuiButton((Rectangle){10, 80, 50, 30}, GuiIconText(ICON_ARROW_UP, "W/Z"));
        if (IsKeyDown(KEY_S)) GuiButton((Rectangle){10, 80, 50, 30}, GuiIconText(ICON_ARROW_DOWN, "S"));
        if (IsKeyDown(KEY_D)) GuiButton((Rectangle){10, 80, 50, 30}, GuiIconText(ICON_ARROW_RIGHT, "D"));
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) GuiButton((Rectangle){10, 80, 50, 30}, GuiIconText(ICON_ARROW_LEFT, "Q/A"));
        if (IsKeyDown(KEY_UP)) GuiButton((Rectangle){70, 80, 100, 30}, GuiIconText(ICON_ARROW_UP, "UP ARROW"));
        if (IsKeyDown(KEY_DOWN)) GuiButton((Rectangle){70, 80, 100, 30}, GuiIconText(ICON_ARROW_DOWN, "DOWN ARROW"));
        if (IsKeyDown(KEY_RIGHT)) GuiButton((Rectangle){70, 80, 100, 30}, GuiIconText(ICON_ARROW_RIGHT, "RIGHT ARROW"));
        if (IsKeyDown(KEY_LEFT)) GuiButton((Rectangle){70, 80, 100, 30}, GuiIconText(ICON_ARROW_LEFT, "LEFT ARROW"));

        if (GuiButton((Rectangle){600, 500, 120, 30}, GuiIconText(ICON_REREDO_FILL, "Reset"))) {
            modelPos = (Vector3){0, 0, 0};
            modelAxes = (Vector3){0, 0, 0};
            modelScale = (Vector3){0.5f, 0.5f, 0.5f};
            camera.position = (Vector3){0, 0, 5};
            camera.target = (Vector3){0, 0, 0};
            camera.up = (Vector3){0, 1, 0};
            rotationAngle = 0;
            camera.fovy = 60;
        }

        if (GuiButton((Rectangle){730, 500, 120, 30}, GuiIconText(ICON_REPEAT, "Show Wires")))
            toggleWires = !toggleWires;

            if (GuiButton((Rectangle){600, 550, 250, 30}, GuiIconText(ICON_CAMERA, "Camera")))
                cameraMode = (cameraMode + 1) % 5;

                GuiSliderBar((Rectangle){600, 100, 200, 10}, "Position X", TextFormat("%.2ff", modelPos.x), &modelPos.x, -10, 10);
                GuiSliderBar((Rectangle){600, 115, 200, 10}, "Position Y", TextFormat("%.2ff", modelPos.y), &modelPos.y, -10, 10);
                GuiSliderBar((Rectangle){600, 130, 200, 10}, "Position Z", TextFormat("%.2ff", modelPos.z), &modelPos.z, -10, 10);
                GuiSliderBar((Rectangle){600, 155, 200, 10}, "Rotation Axe X", TextFormat("%.2ff", modelAxes.x), &modelAxes.x, -10, 10);
                GuiSliderBar((Rectangle){600, 170, 200, 10}, "Rotation Axe Y", TextFormat("%.2ff", modelAxes.y), &modelAxes.y, -10, 10);
                GuiSliderBar((Rectangle){600, 185, 200, 10}, "Rotation Axe Z", TextFormat("%.2ff", modelAxes.z), &modelAxes.z, -10, 10);
                GuiSliderBar((Rectangle){600, 210, 200, 10}, "Scale X", TextFormat("%.2ff", modelScale.x), &modelScale.x, -10, 10);
                GuiSliderBar((Rectangle){600, 225, 200, 10}, "Scale Y", TextFormat("%.2ff", modelScale.y), &modelScale.y, -10, 10);
                GuiSliderBar((Rectangle){600, 240, 200, 10}, "Scale Z", TextFormat("%.2ff", modelScale.z), &modelScale.z, -10, 10);
                GuiSliderBar((Rectangle){600, 265, 200, 10}, "Camera Position X", TextFormat("%.2ff", camera.position.x), &camera.position.x, -10, 10);
                GuiSliderBar((Rectangle){600, 280, 200, 10}, "Camera Position Y", TextFormat("%.2ff", camera.position.y), &camera.position.y, -10, 10);
                GuiSliderBar((Rectangle){600, 295, 200, 10}, "Camera Position Z", TextFormat("%.2ff", camera.position.z), &camera.position.z, -10, 10);
                GuiSliderBar((Rectangle){600, 310, 200, 10}, "Camera Target X", TextFormat("%.2ff", camera.target.x), &camera.target.x, -10, 10);
                GuiSliderBar((Rectangle){600, 325, 200, 10}, "Camera Target Y", TextFormat("%.2ff", camera.target.y), &camera.target.y, -10, 10);
                GuiSliderBar((Rectangle){600, 340, 200, 10}, "Camera Target Z", TextFormat("%.2ff", camera.target.z), &camera.target.z, -10, 10);
                GuiSliderBar((Rectangle){600, 365, 200, 10}, "Camera Up X", TextFormat("%.2ff", camera.up.x), &camera.up.x, -10, 10);
                GuiSliderBar((Rectangle){600, 380, 200, 10}, "Camera Up Y", TextFormat("%.2ff", camera.up.y), &camera.up.y, -10, 10);
                GuiSliderBar((Rectangle){600, 395, 200, 10}, "Camera Up Z", TextFormat("%.2ff", camera.up.z), &camera.up.z, -10, 10);
                GuiSliderBar((Rectangle){600, 420, 200, 10}, "FOVY", TextFormat("%.2ff", camera.fovy), &camera.fovy, -100, 100);
                GuiSliderBar((Rectangle){600, 445, 200, 10}, "Rotation Angle", TextFormat("%.2ff", rotationAngle), &rotationAngle, 0, 360);

                EndDrawing();

                switch (cameraMode) {
                    case 0: UpdateCamera(&camera, CAMERA_ORBITAL); break;
                    case 1: UpdateCamera(&camera, CAMERA_FREE); break;
                    case 2: UpdateCamera(&camera, CAMERA_CUSTOM); break;
                    case 3: UpdateCamera(&camera, CAMERA_FIRST_PERSON); break;
                    case 4: UpdateCamera(&camera, CAMERA_THIRD_PERSON); break;
                }
    }

    UnloadTexture(backgroundTexture);
    UnloadModel(model);
    UnloadImage(backgroundImage);
    UnloadFont(mainFont);
    CloseWindow();
    return 0;
}
