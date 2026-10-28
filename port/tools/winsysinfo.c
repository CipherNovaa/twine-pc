/* The USB test kit's system report (port/usbkit, built by make -C port usbkit): Windows' version, OpenGL's (a WGL
 * context on a hidden window), what SDL sees (video driver, displays and their modes, controllers) and the GPU device
 * the hardware renderer would get (SDL's GPU API on Vulkan, as gpu_hw.c asks for it). Prints to stdout. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <SDL3/SDL.h>

typedef LONG(WINAPI *RtlGetVersionFn)(OSVERSIONINFOW *);

static void windows_version(void) {
    OSVERSIONINFOW v;
    RtlGetVersionFn get = (RtlGetVersionFn)(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlGetVersion");
    const char *(*wine)(void) = (const char *(*)(void))(void *)GetProcAddress(GetModuleHandleA("ntdll.dll"),
                                                                               "wine_get_version");
    memset(&v, 0, sizeof(v));
    v.dwOSVersionInfoSize = sizeof(v);
    if (get && !get(&v))
        printf("windows: %lu.%lu build %lu\n", v.dwMajorVersion, v.dwMinorVersion, v.dwBuildNumber);
    if (wine)
        printf("windows: Wine %s\n", wine());
}

static void opengl(void) {
    typedef const unsigned char *(WINAPI * GetString)(unsigned);
    WNDCLASSA wc;
    HWND w;
    HDC dc;
    HGLRC rc;
    PIXELFORMATDESCRIPTOR pfd;
    HMODULE gl = LoadLibraryA("opengl32.dll");
    GetString gs = gl ? (GetString)(void *)GetProcAddress(gl, "glGetString") : NULL;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = "twine_sysinfo";
    RegisterClassA(&wc);
    w = CreateWindowA("twine_sysinfo", "", WS_OVERLAPPEDWINDOW, 0, 0, 16, 16, NULL, NULL, wc.hInstance, NULL);
    if (!w || !gs) {
        printf("opengl: no window or opengl32.dll\n");
        return;
    }
    dc = GetDC(w);
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
    rc = wglCreateContext(dc);
    if (rc && wglMakeCurrent(dc, rc)) {
        printf("opengl: %s\nopengl: %s\nopengl: %s\n", gs(0x1F02 /* VERSION */), gs(0x1F01 /* RENDERER */),
               gs(0x1F00 /* VENDOR */));
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(rc);
    } else
        printf("opengl: no context (%lu)\n", GetLastError());
    ReleaseDC(w, dc);
    DestroyWindow(w);
}

static void sdl(void) {
    int i, n;
    SDL_DisplayID *d;
    SDL_JoystickID *j;
    SDL_GPUDevice *gpu;
    printf("sdl: %d.%d.%d\n", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("sdl: init: %s\n", SDL_GetError());
        return;
    }
    printf("sdl: video driver %s\n", SDL_GetCurrentVideoDriver());
    d = SDL_GetDisplays(&n);
    for (i = 0; d && i < n; i++) {
        const SDL_DisplayMode *m = SDL_GetCurrentDisplayMode(d[i]);
        printf("display %d: %s, %dx%d at %.2f Hz, scale %.2f\n", i + 1, SDL_GetDisplayName(d[i]), m ? m->w : 0,
               m ? m->h : 0, m ? m->refresh_rate : 0.0f, SDL_GetDisplayContentScale(d[i]));
    }
    SDL_free(d);
    j = SDL_GetJoysticks(&n);
    printf("controllers: %d\n", n);
    for (i = 0; j && i < n; i++)
        printf("controller %d: %s%s\n", i + 1, SDL_GetJoystickNameForID(j[i]),
               SDL_IsGamepad(j[i]) ? " (gamepad)" : "");
    SDL_free(j);
    gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
    if (gpu) {
        printf("gpu: Vulkan device for the hardware renderer: driver %s\n", SDL_GetGPUDeviceDriver(gpu));
        SDL_DestroyGPUDevice(gpu);
    } else
        printf("gpu: no Vulkan device for the hardware renderer: %s\n", SDL_GetError());
    SDL_Quit();
}

int main(void) {
    windows_version();
    opengl();
    sdl();
    return 0;
}
