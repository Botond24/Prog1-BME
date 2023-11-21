#include "main.h"

int main(int argc, char** argv) {
    char *theme_file = "theme.ini";
    char *input_file = "test.c";
    char *output_file = "test";
    theme_t *theme = &default_theme;
    printf("%u %u %u", theme->main_.background.r, theme->main_.background.g, theme->main_.background.b);
    file_type_e output_type;
    file_type_e input_type;
    node_t *root = NULL;
    int quit = 0;
    HWND windowRef;
    SDL_Event event;

    if (argc != 1){
        if(init_console(argc, argv, theme_file, output_file, input_file) == -1) return -1;
        output_type = ends_with(output_file);
        input_type = ends_with(input_file);
    }
    if (read_ini(theme_file, theme) == -1) theme = &default_theme;
    SDL_Window *window = SDL_CreateWindow("Source to Flow", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    windowRef = GetHwnd(window);
    ActivateMenu(windowRef);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *surface = SDL_CreateRGBSurface(1,640,480,32,0,0,0,0);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    if (input_type == file_type_c){
        root = read_source(input_file);
    }
    if (output_type == file_type_c){
        fprintf(stderr,"HOW");
    }
    while (!quit) {
        SDL_PollEvent(&event);
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
                            ;
                            temp = file_open_dialog(windowRef,L"Source file",L"*.c,*.h");
                            if (temp == NULL) break;
                            free(input_file);
                            input_file = temp;
                            input_type = ends_with(input_file);
                            root = read_source(input_file);
                            //printf("%s", input_file);
                            break;
                        case ID_SAVE_FLOW:
                            ;
                            temp = file_save_dialog(windowRef);
                            if (temp == NULL) break;
                            free(output_file);
                            output_file = temp;
                            output_type = ends_with(output_file);
                            //printf("%s", output_file);
                            break;
                        case ID_LOAD_THEME:
                            temp = file_open_dialog(windowRef,L"Theme file",L"*.ini");
                            if (temp == NULL) break;
                            free(theme_file);
                            theme_file = temp;
                            if (read_ini(theme_file, theme) == -1){
                                theme = &default_theme;
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Theme error", "Couldn't load theme", window);
                                break;
                            }
                            break;
                        case ID_RESET_THEME:
                            theme = &default_theme;
                            //TODO: redraw call
                            break;
                        case ID_ZOOM_IN:
                            break;
                        case ID_ZOOM_OUT:
                            break;
                        case ID_ZOOM_RESET:
                            break;
                        default:
                            break;
                    }
                }
                break;
        }

        //SDL_SetRenderDrawColor(renderer, theme->main_.background.r, theme->main_.background.g, theme->main_.background.b, theme->main_.background.a);
        //SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //free(theme);
    //free(input_file);
    //free(output_file);
    free(root);
    return 0;
}

HWND GetHwnd(SDL_Window *window){
    SDL_SysWMinfo windowInfo;
    if(!SDL_GetWindowWMInfo(window,&windowInfo)) return NULL;
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
    AppendMenu(hView, MF_STRING, ID_ZOOM_IN, "Zoom In");
    AppendMenu(hView, MF_STRING, ID_ZOOM_OUT, "Zoom Out");
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
