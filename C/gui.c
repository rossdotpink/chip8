#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
// #define RAYGUI_CUSTOM_ICONS   // It requires providing gui_icons.h in the
// same directory #include "gui_icons.h"    // External icons data provided, it
// can be generated with rGuiIcons tool
#include <string.h>  // Required for: strcpy()

#include "raygui.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
  // CHIP8 start time

  chip8_t* system = init_chip8();
  log_trace("main() system = %p", system);

  load_rom_file(argv[1], system);

  log_trace("main() - DT         = %u!!", system->delay_timer);
  log_trace("main() - PC         = %u!!", system->program_counter);
  log_trace("main() - MEM_OFFSET = %d!!", MEM_OFFSET);

  log_trace("0x%03x\t%02x%02x", (system->program_counter),
            system->memory[MEM_OFFSET], system->memory[MEM_OFFSET + 1]);

  loop(system);

  // for(int i = 0; i < system->current_rom_size; i += 2) {
  //   log_trace("0x%03x\t%02x%02x", (MEM_OFFSET+(i*4)),
  //   system->memory[MEM_OFFSET+i], system->memory[MEM_OFFSET+i+1]);
  // }

  return OK;

  // Initialization
  //---------------------------------------------------------------------------------------
  const int screenWidth = 690;
  const int screenHeight = 560;

  InitWindow(screenWidth, screenHeight, "raygui - controls test suite");
  SetExitKey(0);

  // GUI controls initialization
  //----------------------------------------------------------------------------------
  int dropdownBox000Active = 0;
  bool dropDown000EditMode = false;

  int dropdownBox001Active = 0;
  bool dropDown001EditMode = false;

  int spinner001Value = 0;
  bool spinnerEditMode = false;

  int valueBox002Value = 0;
  bool valueBoxEditMode = false;

  char textBoxText[64] = "Text box";
  bool textBoxEditMode = false;

  int listViewScrollIndex = 0;
  int listViewActive = -1;

  int listViewExScrollIndex = 0;
  int listViewExActive = 2;
  int listViewExFocus = -1;
  const char* listViewExList[8] = {"This", "is",      "a",        "list view",
                                   "with", "disable", "elements", "amazing!"};

  char multiTextBoxText[256] = "Multi text box";
  bool multiTextBoxEditMode = false;
  Color colorPickerValue = RED;

  float sliderValue = 50.0f;
  float sliderBarValue = 60;
  float progressValue = 0.4f;

  bool forceSquaredChecked = false;

  float alphaValue = 0.5f;

  int comboBoxActive = 1;

  int toggleGroupActive = 0;

  Vector2 viewScroll = {0, 0};
  //----------------------------------------------------------------------------------

  // Custom GUI font loading
  // Font font = LoadFontEx("fonts/rainyhearts16.ttf", 12, 0, 0);
  // GuiSetFont(font);

  bool exitWindow = false;
  bool showMessageBox = false;

  char textInput[256] = {0};
  bool showTextInputBox = false;

  char textInputFileName[256] = {0};

  SetTargetFPS(60);
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!exitWindow)  // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    exitWindow = WindowShouldClose();

    if (IsKeyPressed(KEY_ESCAPE)) showMessageBox = !showMessageBox;

    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
      showTextInputBox = true;

    if (IsFileDropped()) {
      FilePathList droppedFiles = LoadDroppedFiles();

      if ((droppedFiles.count > 0) &&
          IsFileExtension(droppedFiles.paths[0], ".rgs"))
        GuiLoadStyle(droppedFiles.paths[0]);

      UnloadDroppedFiles(droppedFiles);  // Clear internal buffers
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // raygui: controls drawing
    //----------------------------------------------------------------------------------
    // Check all possible events that require GuiLock
    if (dropDown000EditMode || dropDown001EditMode) GuiLock();

    // First GUI column
    // GuiSetStyle(CHECKBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiCheckBox((Rectangle){25, 108, 15, 15}, "FORCE CHECK!",
                &forceSquaredChecked);

    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    // GuiSetStyle(VALUEBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiSpinner((Rectangle){25, 135, 125, 30}, NULL, &spinner001Value, 0,
                   100, spinnerEditMode))
      spinnerEditMode = !spinnerEditMode;
    if (GuiValueBox((Rectangle){25, 175, 125, 30}, NULL, &valueBox002Value, 0,
                    100, valueBoxEditMode))
      valueBoxEditMode = !valueBoxEditMode;
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    if (GuiTextBox((Rectangle){25, 215, 125, 30}, textBoxText, 64,
                   textBoxEditMode))
      textBoxEditMode = !textBoxEditMode;

    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    if (GuiButton((Rectangle){25, 255, 125, 30},
                  GuiIconText(ICON_FILE_SAVE, "Save File")))
      showTextInputBox = true;

    GuiGroupBox((Rectangle){25, 310, 125, 150}, "STATES");
    // GuiLock();
    GuiSetState(STATE_NORMAL);
    if (GuiButton((Rectangle){30, 320, 115, 30}, "NORMAL")) {
    }
    GuiSetState(STATE_FOCUSED);
    if (GuiButton((Rectangle){30, 355, 115, 30}, "FOCUSED")) {
    }
    GuiSetState(STATE_PRESSED);
    if (GuiButton((Rectangle){30, 390, 115, 30}, "#15#PRESSED")) {
    }
    GuiSetState(STATE_DISABLED);
    if (GuiButton((Rectangle){30, 425, 115, 30}, "DISABLED")) {
    }
    GuiSetState(STATE_NORMAL);
    // GuiUnlock();

    GuiComboBox((Rectangle){25, 470, 125, 30}, "ONE;TWO;THREE;FOUR",
                &comboBoxActive);

    // NOTE: GuiDropdownBox must draw after any other control that can be
    // covered on unfolding
    GuiUnlock();

    if (showMessageBox) {
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                    Fade(RAYWHITE, 0.8f));
      int result = GuiMessageBox(
          (Rectangle){(float)GetScreenWidth() / 2 - 125,
                      (float)GetScreenHeight() / 2 - 50, 250, 100},
          GuiIconText(ICON_EXIT, "Close Window"), "Do you really want to exit?",
          "Yes;No");

      if ((result == 0) || (result == 2))
        showMessageBox = false;
      else if (result == 1)
        exitWindow = true;
    }

    if (showTextInputBox) {
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                    Fade(RAYWHITE, 0.8f));
      int result = GuiTextInputBox(
          (Rectangle){(float)GetScreenWidth() / 2 - 120,
                      (float)GetScreenHeight() / 2 - 60, 240, 140},
          "Save", GuiIconText(ICON_FILE_SAVE, "Save file as..."), "Ok;Cancel",
          textInput, 255, NULL);

      if (result == 1) {
        // TODO: Validate textInput value and save

        strcpy(textInputFileName, textInput);
      }

      if ((result == 0) || (result == 1) || (result == 2)) {
        showTextInputBox = false;
        strcpy(textInput, "\0");
      }
    }
    //----------------------------------------------------------------------------------

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
