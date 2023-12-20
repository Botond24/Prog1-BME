#include "main.h"

int main(int argc, char** argv) {
    debugmalloc_log_file("debugmalloc.log");
    char *theme_file = malloc(1);
    char *input_file = malloc(1);
    char *output_file = malloc(1);
    file_type_e output_type;

    node_t *root = NULL;
    box_t *boxes = (box_t*)malloc(sizeof(box_t));
    int box_count = 0;
    graphics_t graphics;
    graphics.theme = &default_theme;

    HWND windowRef;
    SDL_Event event;
    int quit = 0;

    if (argc != 1){
        if(init_console(argc, argv, theme_file, output_file, input_file) == -1) return -1;
        output_type = ends_with(output_file);
    }

    //if (read_ini(theme_file, graphics.theme) == -1) graphics.theme = &default_theme;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Source to Flow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (initGraphics(&graphics, window) == -1) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
    }
    windowRef = GetHwnd(window);
    SDL_Texture *texture = SDL_CreateTexture(graphics.renderer, graphics.pixel_format, SDL_TEXTUREACCESS_TARGET, 640, 480);
    SDL_SetRenderTarget(graphics.renderer, texture);
    ActivateMenu(windowRef);

    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    redraw(&graphics, boxes, box_count);


    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                event.type = SDL_QUIT;
                SDL_PushEvent(&event);
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_SYSWMEVENT:
                if (event.syswm.msg->msg.win.msg == WM_COMMAND){
                    char* temp;
                    switch (event.syswm.msg->msg.win.wParam){
                        case ID_EXIT:
                            quit = 1;
                            break;
                        case ID_OPEN_FILE:
                            temp = file_open_dialog(windowRef,L"Source file",L"*.c");
                            if (temp == NULL) break;
                            //free(input_file);
                            input_file = temp;
                            root = read_source(input_file);
                            box_count = initBoxes(&graphics, root, boxes);
                            //printf("%s", input_file);
                            break;
                        case ID_SAVE_FLOW:
                            temp = file_save_dialog(windowRef);
                            if (temp == NULL) break;
                            free(output_file);
                            output_file = temp;
                            output_file[strlen(output_file)-4] = '\0';
                            //printf("%s", output_file);
                            output_type = ends_with(output_file);
                            saveToFile(&graphics, output_file, output_type);
                            //printf("%s", output_file);
                            break;
                        case ID_LOAD_THEME:
                            temp = file_open_dialog(windowRef,L"Theme file",L"*.ini");
                            if (temp == NULL) break;
                            free(theme_file);
                            theme_file = temp;
                            if (read_ini(theme_file, graphics.theme) == -1){
                                graphics.theme = &default_theme;
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Theme error", "Couldn't load theme", window);
                                break;
                            }
                            break;
                        case ID_RESET_THEME:
                            graphics.theme = &default_theme;
                            //TODO: redraw call
                            break;
                        case ID_ZOOM_IN:
                            graphics.scale += 0.1;
                            SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                            break;
                        case ID_ZOOM_OUT:
                            graphics.scale -= 0.1;
                            SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                            break;
                        case ID_ZOOM_RESET:
                            graphics.scale = 1;
                            SDL_RenderSetScale(graphics.renderer, 1, 1);
                            break;
                        default:
                            break;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS){
                    graphics.scale += 0.1;
                    SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                }
                else if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS){
                    graphics.scale -= 0.1;
                    SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                }
                break;
            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0){
                    graphics.scale += 0.1;
                    SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                }
                else if (event.wheel.y < 0){
                    graphics.scale -= 0.1;
                    SDL_RenderSetScale(graphics.renderer, graphics.scale, graphics.scale);
                }
            default:
                break;
        }
        redraw(&graphics, boxes, box_count);
        //SDL_SetRenderDrawColor(renderer, theme->main_.background.r, theme->main_.background.g, theme->main_.background.b, theme->main_.background.a);
        //SDL_RenderClear(renderer);
        SDL_RenderPresent(graphics.renderer);
    }
    SDL_DestroyRenderer(graphics.renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    freeBoxes(boxes, box_count);
    free(theme_file);
    free(input_file);
    free(output_file);
    //free(theme);
    //free(input_file);
    //free(output_file);
    freeLinkedList(root);
    return 0;
}

HWND GetHwnd(SDL_Window *window){
    ///NEEDED FOR SOME REASON
    SDL_version *ver = malloc(sizeof(SDL_version));
    SDL_GetVersion(ver);
    //printf("%d.%d.%d",ver->major,ver->minor,ver->patch);
    SDL_SysWMinfo windowInfo;
    int success = SDL_GetWindowWMInfo(window,&windowInfo);
    //printf("%d.%d.%d",windowInfo.version.major,windowInfo.version.minor,windowInfo.version.patch);
    if (!success){
        printf("SDL_GetWindowWMInfo failed");
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
    }
    free(ver);
    return windowInfo.info.win.window;
}

void ActivateMenu(HWND windowRef)
{
    HMENU hMenuBar = CreateMenu();
    HMENU hFile = CreateMenu();
    HMENU hView = CreateMenu();

    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, "File");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hView, "View");
    AppendMenu(hMenuBar, MF_STRING, ID_EXIT, "Exit");

    AppendMenu(hFile, MF_STRING, ID_OPEN_FILE, "Open File");
    AppendMenu(hFile, MF_STRING, ID_SAVE_FLOW, "Save flowchart");

    AppendMenu(hView, MF_STRING, ID_LOAD_THEME, "Load Theme");
    AppendMenu(hView, MF_STRING, ID_RESET_THEME, "Reset Theme");
    AppendMenu(hView, MF_STRING, ID_ZOOM_IN, "Zoom In (+)");
    AppendMenu(hView, MF_STRING, ID_ZOOM_OUT, "Zoom Out (-)");
    AppendMenu(hView, MF_STRING, ID_ZOOM_RESET, "Zoom Reset");

    SetMenu(windowRef, hMenuBar);
}

char* file_open_dialog(HWND windowRef, const wchar_t *name, const wchar_t *file_spec)
{
    char *file_path = NULL;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                                      COINIT_DISABLE_OLE1DDE);
    COMDLG_FILTERSPEC filterspec = {name, file_spec};
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                              &IID_IFileOpenDialog, (void**)(&pFileOpen));
        if (SUCCEEDED(hr))
        {
            pFileOpen->lpVtbl->SetFileTypes(pFileOpen, 1, &filterspec);
            // Show the Open dialog box.
            hr = pFileOpen->lpVtbl->Show(pFileOpen, windowRef);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->lpVtbl->GetResult(pFileOpen, &pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->lpVtbl->GetDisplayName(pItem,SIGDN_FILESYSPATH,&pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        file_path = (char *)malloc(lstrlenW(pszFilePath) + 1);
                        wcstombs(file_path, pszFilePath, lstrlenW(pszFilePath) + 1);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->lpVtbl->Release((IShellItem *) &pItem);
                }
            }
            pFileOpen->lpVtbl->Release((IFileOpenDialog *) &pFileOpen);
        }
        CoUninitialize();
    }
    return file_path;
}

char* file_save_dialog(HWND windowRef)
{
    char *file_path = NULL;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                                      COINIT_DISABLE_OLE1DDE);
    COMDLG_FILTERSPEC filterspec[2] = {{L"png", L"*.png"}, {L"jpg", L"*.jpg"}}; //TODO: add md later
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog *pFileSave;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                              &IID_IFileSaveDialog, (void**)(&pFileSave));
        if (SUCCEEDED(hr))
        {
            pFileSave->lpVtbl->SetFileTypes(pFileSave, 2, filterspec);
            pFileSave->lpVtbl->SetFileName(pFileSave, L"Flowchart.png");
            // Show the Open dialog box.
            hr = pFileSave->lpVtbl->Show(pFileSave, windowRef);
            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                unsigned int i;
                pFileSave->lpVtbl->GetFileTypeIndex(pFileSave, &i);
                hr = pFileSave->lpVtbl->GetResult(pFileSave, &pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->lpVtbl->GetDisplayName(pItem,SIGDN_FILESYSPATH,&pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        file_path = (char *)malloc(lstrlenW(pszFilePath) + 5);
                        wcstombs(file_path, pszFilePath, lstrlenW(pszFilePath) + 1);
                        strcat(file_path, i == 1 ? ".png" : ".jpg");
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->lpVtbl->Release((IShellItem *) &pItem);
                }
            }
            pFileSave->lpVtbl->Release((IFileSaveDialog *) &pFileSave);
        }
        CoUninitialize();
    }
    return file_path;
}
