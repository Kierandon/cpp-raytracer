// #include "scene/SceneLoader.h"
// #include "materials/BinaryMaterial.h"
// #include "geometry/Sphere.h"
// #include "geometry/Cylinder.h"
// #include "geometry/Triangle.h"

// #define UNICODE
// #include <windows.h>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <sstream>
// #include <stdexcept>

// // Global variables
// HWND hwnd = NULL;
// SceneLoader scene_manager;
// bool rendering = false;
// HBITMAP hBitmap = NULL;

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// void RenderAndDisplay();
// void HandleKeyInput(WPARAM key);
// void LoadAndDisplayPPM(const char *filename);

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
// {
//     (void)pCmdLine;

//     // Initialise scene
//     SceneConfig config;
//     scene_manager.configure(config);

//     if (__argc > 1)
//     {
//         scene_manager.setup_custom_scene(__argv[1]);
//     }
//     else
//     {
//         scene_manager.setup_default_scene();
//     }

//     // Register window class
//     WNDCLASSEXW wc = {sizeof(WNDCLASSEXW), 0, WindowProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW),
//                       (HBRUSH)(COLOR_WINDOW + 1), NULL, L"RaytracerViewer", NULL};
//     RegisterClassExW(&wc);

//     // Create window
//     hwnd = CreateWindowExW(
//         0, L"RaytracerViewer", L"Raytracer",
//         WS_OVERLAPPEDWINDOW,
//         CW_USEDEFAULT, CW_USEDEFAULT,
//         config.image_width, config.image_height,
//         NULL, NULL, hInstance, NULL);

//     ShowWindow(hwnd, nCmdShow);
//     RenderAndDisplay();

//     // Message loop
//     MSG msg = {};
//     while (GetMessage(&msg, NULL, 0, 0))
//     {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     return 0;
// }

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//     switch (uMsg)
//     {
//     case WM_PAINT:
//     {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(hwnd, &ps);

//         if (hBitmap)
//         {
//             HDC memDC = CreateCompatibleDC(hdc);
//             SelectObject(memDC, hBitmap);

//             BITMAP bmp;
//             GetObject(hBitmap, sizeof(BITMAP), &bmp);
//             BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);

//             DeleteDC(memDC);
//         }

//         EndPaint(hwnd, &ps);
//         return 0;
//     }

//     case WM_KEYDOWN:
//         if (!rendering)
//         {
//             HandleKeyInput(wParam);
//             RenderAndDisplay();
//         }
//         return 0;

//     case WM_DESTROY:
//         PostQuitMessage(0);
//         return 0;
//     }
//     return DefWindowProc(hwnd, uMsg, wParam, lParam);
// }

// void HandleKeyInput(WPARAM key)
// {
//     float move_speed = 0.1f;
//     bool shift_pressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
//     bool ctrl_pressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;

//     switch (key)
//     {
//     // Camera Movement
//     case 'W':
//         scene_manager.config.camera_position.z += move_speed;
//         break;
//     case 'S':
//         scene_manager.config.camera_position.z -= move_speed;
//         break;
//     case 'A':
//         scene_manager.config.camera_position.x -= move_speed;
//         break;
//     case 'D':
//         scene_manager.config.camera_position.x += move_speed;
//         break;
//     case 'Q':
//         scene_manager.config.camera_position.y += move_speed;
//         break;
//     case 'E':
//         scene_manager.config.camera_position.y -= move_speed;
//         break;

//     // Samples per pixel
//     case VK_ADD:
//         if (ctrl_pressed)
//             scene_manager.config.samples_per_pixel *= 2;
//         break;
//     case VK_SUBTRACT:
//         if (ctrl_pressed)
//             scene_manager.config.samples_per_pixel = std::max(1, scene_manager.config.samples_per_pixel / 2);
//         break;

//     // FOV control
//     case VK_UP:
//         if (!shift_pressed)
//             scene_manager.config.fov = std::min(120.0f, scene_manager.config.fov + 5.0f);
//         break;
//     case VK_DOWN:
//         if (!shift_pressed)
//             scene_manager.config.fov = std::max(10.0f, scene_manager.config.fov - 5.0f);
//         break;

//     // Toggle features
//     case 'B':
//         scene_manager.config.use_bvh = !scene_manager.config.use_bvh;
//         break;
//     case 'N':
//         scene_manager.config.use_denoiser = !scene_manager.config.use_denoiser;
//         break;
//     case 'T':
//         scene_manager.config.use_tone_mapping = !scene_manager.config.use_tone_mapping;
//         break;
//     case 'G':
//         scene_manager.config.use_gradient = !scene_manager.config.use_gradient;
//         break;
//     case 'F':
//         scene_manager.config.use_dof = !scene_manager.config.use_dof;
//         break;

//     // DOF controls
//     case VK_LEFT:
//         if (shift_pressed)
//             scene_manager.config.aperture = std::max(0.0f, scene_manager.config.aperture - 0.1f);
//         break;
//     case VK_RIGHT:
//         if (shift_pressed)
//             scene_manager.config.aperture = scene_manager.config.aperture + 0.1f;
//         break;
//     }

//     // Print current settings
//     std::cout << "Camera position: " << scene_manager.config.camera_position << "\n"
//               << "FOV: " << scene_manager.config.fov << "°\n"
//               << "Samples per pixel: " << scene_manager.config.samples_per_pixel << "\n"
//               << "Features - BVH: " << scene_manager.config.use_bvh
//               << " Denoiser: " << scene_manager.config.use_denoiser
//               << " DoF: " << scene_manager.config.use_dof << std::endl;

//     scene_manager.configure(scene_manager.config);
// }

// void RenderAndDisplay()
// {
//     rendering = true;
//     scene_manager.render("output.ppm");
//     rendering = false;

//     // Load and display the PPM file
//     LoadAndDisplayPPM("output.ppm");
//     InvalidateRect(hwnd, NULL, TRUE);
// }

// void LoadAndDisplayPPM(const char *filename)
// {
//     std::ifstream file(filename);
//     if (!file)
//         throw std::runtime_error("Failed to open PPM file.");

//     std::string header;
//     int width, height, maxVal;

//     // Read and validate the PPM header
//     file >> header;
//     if (header != "P3")
//         throw std::runtime_error("Invalid PPM format.");

//     file >> width >> height >> maxVal;

//     if (maxVal != 255)
//         throw std::runtime_error("Unsupported max value in PPM file.");

//     // Read pixel data
//     std::vector<unsigned char> pixels(width * height * 3);
//     for (int i = 0; i < width * height * 3; i += 3)
//     {
//         int r, g, b;
//         file >> r >> g >> b;

//         // Store pixels in BGR order for Windows bitmaps
//         pixels[i] = static_cast<unsigned char>(b);     // Blue
//         pixels[i + 1] = static_cast<unsigned char>(g); // Green
//         pixels[i + 2] = static_cast<unsigned char>(r); // Red
//     }

//     if (!file)
//         throw std::runtime_error("Failed to read PPM pixel data.");

//     // Create a bitmap
//     BITMAPINFO bmi = {};
//     bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//     bmi.bmiHeader.biWidth = width;
//     bmi.bmiHeader.biHeight = -height; // Negative for top-down bitmap
//     bmi.bmiHeader.biPlanes = 1;
//     bmi.bmiHeader.biBitCount = 24;
//     bmi.bmiHeader.biCompression = BI_RGB;

//     HDC hdc = GetDC(hwnd);
//     if (hBitmap)
//     {
//         DeleteObject(hBitmap);
//     }
//     hBitmap = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, pixels.data(), &bmi, DIB_RGB_COLORS);
//     ReleaseDC(hwnd, hdc);
// }
