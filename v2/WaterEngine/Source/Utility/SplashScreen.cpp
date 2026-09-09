// =============================================================================
// Water Engine v2.2.4
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/SplashScreen.h"
#include "Core/EngineConfig.h"
#include "Utility/Math.h"

#if defined(NDEBUG) && defined(WIN32)

#include <windows.h>
#include <physfs.h>
#include <cstdint>

namespace
{
    // Keeps Windows from marking the process unresponsive during the blocking loop
    void PumpMessages()
    {
        MSG Msg;
        while (PeekMessageW(&Msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            TranslateMessage(&Msg);
            DispatchMessageW(&Msg);
        }
    }

    LRESULT CALLBACK SplashWndProc(HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam)
    {
        if (Msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
        return DefWindowProcW(Hwnd, Msg, WParam, LParam);
    }
}

namespace we
{
    void ShowSplash()
    {
        if (SplashConfig::TexturePath[0] == '\0') return;

        // Mount the pak here so the splash loads before ResourceSubsystem runs.
        // That subsystem checks PHYSFS_isInit() and skips re-initializing.
        if (!PHYSFS_init(nullptr) || !PHYSFS_mount(ASSET_PACK_PATH, "/", 1))
            return;

        PHYSFS_File* PakFile = PHYSFS_openRead(SplashConfig::TexturePath);
        if (!PakFile) return;

        const PHYSFS_sint64 FileLen = PHYSFS_fileLength(PakFile);
        if (FileLen <= 0) { PHYSFS_close(PakFile); return; }

        string PngData;
        PngData.resize(static_cast<size_t>(FileLen));
        PHYSFS_readBytes(PakFile, PngData.data(), static_cast<PHYSFS_uint64>(FileLen));
        PHYSFS_close(PakFile);

        sf::Image Img;
        if (!Img.loadFromMemory(reinterpret_cast<const std::uint8_t*>(PngData.data()), PngData.size()))
            return;

        const sf::Vector2u ImgSize = Img.getSize();
        const LONG W = static_cast<LONG>(ImgSize.x);
        const LONG H = static_cast<LONG>(ImgSize.y);
        if (W <= 0 || H <= 0) return;

        const std::uint8_t* Rgba = Img.getPixelsPtr();

        // =====================================================================
        // Source Bitmap
        // =====================================================================
        // UpdateLayeredWindow needs a 32-bit BGRA DIB with premultiplied alpha
        HDC ScreenDC = GetDC(nullptr);
        HDC MemDC = CreateCompatibleDC(ScreenDC);

        BITMAPINFOHEADER Bih;
        ZeroMemory(&Bih, sizeof(Bih));
        Bih.biSize = sizeof(Bih);
        Bih.biWidth = W;
        Bih.biHeight = -H;  // Negative = top-down scan order
        Bih.biPlanes = 1;
        Bih.biBitCount = 32;
        Bih.biCompression = BI_RGB;

        void* DibBits = nullptr;
        HBITMAP Bmp = CreateDIBSection(ScreenDC, reinterpret_cast<BITMAPINFO*>(&Bih),
                                       DIB_RGB_COLORS, &DibBits, nullptr, 0);
        ReleaseDC(nullptr, ScreenDC);

        if (!Bmp) { DeleteDC(MemDC); return; }

        HGDIOBJ OldBmp = SelectObject(MemDC, static_cast<HGDIOBJ>(Bmp));

        // RGBA (SFML) to premultiplied BGRA (Win32)
        BYTE* Dst = static_cast<BYTE*>(DibBits);
        const LONG PixelCount = W * H;
        for (LONG i = 0; i < PixelCount; ++i)
        {
            const DWORD R = Rgba[i * 4 + 0];
            const DWORD G = Rgba[i * 4 + 1];
            const DWORD B = Rgba[i * 4 + 2];
            const DWORD A = Rgba[i * 4 + 3];
            Dst[i * 4 + 0] = static_cast<BYTE>(B * A / 255u);
            Dst[i * 4 + 1] = static_cast<BYTE>(G * A / 255u);
            Dst[i * 4 + 2] = static_cast<BYTE>(R * A / 255u);
            Dst[i * 4 + 3] = static_cast<BYTE>(A);
        }

        // =====================================================================
        // Layered Window
        // =====================================================================
        const int ScreenW = GetSystemMetrics(SM_CXSCREEN);
        const int ScreenH = GetSystemMetrics(SM_CYSCREEN);

        HINSTANCE ModuleInst = GetModuleHandleW(nullptr);

        WNDCLASSEXW Wc;
        ZeroMemory(&Wc, sizeof(Wc));
        Wc.cbSize = sizeof(Wc);
        Wc.lpfnWndProc = &SplashWndProc;
        Wc.hInstance = ModuleInst;
        Wc.lpszClassName = L"WESplash";
        RegisterClassExW(&Wc);

        // LAYERED = per-pixel alpha, TOOLWINDOW = off the taskbar, TOPMOST = above all
        HWND SplashWindow = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
            L"WESplash", L"",
            WS_POPUP,
            (ScreenW - static_cast<int>(W)) / 2,
            (ScreenH - static_cast<int>(H)) / 2,
            static_cast<int>(W), static_cast<int>(H),
            nullptr, nullptr, ModuleInst, nullptr);

        if (SplashWindow)
        {
            ShowWindow(SplashWindow, SW_SHOW);

            // Second full-size DIB we StretchBlt into each frame. UpdateLayeredWindow
            // only reads the top-left region, so the rest is never consumed.
            HDC ScaledDC = CreateCompatibleDC(nullptr);

            void* ScaledBits = nullptr;
            HDC TmpDC = GetDC(nullptr);
            HBITMAP ScaledBmp = CreateDIBSection(TmpDC, reinterpret_cast<BITMAPINFO*>(&Bih),
                                                 DIB_RGB_COLORS, &ScaledBits, nullptr, 0);
            ReleaseDC(nullptr, TmpDC);

            HGDIOBJ OldScaledBmp = SelectObject(ScaledDC, static_cast<HGDIOBJ>(ScaledBmp));

            // COLORONCOLOR drops whole pixels instead of averaging, which would
            // blend the premultiplied alpha channel into the color channels
            SetStretchBltMode(ScaledDC, COLORONCOLOR);

            // Alpha is a global multiplier on top of per-pixel alpha, Scale is a
            // fraction of the full image size
            auto UpdateFrame = [&](BYTE Alpha, float Scale)
            {
                const LONG ScaledW = Max(1L, static_cast<LONG>(static_cast<float>(W) * Scale));
                const LONG ScaledH = Max(1L, static_cast<LONG>(static_cast<float>(H) * Scale));

                StretchBlt(ScaledDC, 0, 0, static_cast<int>(ScaledW), static_cast<int>(ScaledH),
                           MemDC, 0, 0, static_cast<int>(W), static_cast<int>(H), SRCCOPY);

                POINT SrcPos;
                SrcPos.x = 0;
                SrcPos.y = 0;

                SIZE Size;
                Size.cx = ScaledW;
                Size.cy = ScaledH;

                POINT DstPos;
                DstPos.x = (ScreenW - static_cast<int>(ScaledW)) / 2;
                DstPos.y = (ScreenH - static_cast<int>(ScaledH)) / 2;

                BLENDFUNCTION Blend;
                ZeroMemory(&Blend, sizeof(Blend));
                Blend.BlendOp = AC_SRC_OVER;
                Blend.BlendFlags = 0;
                Blend.SourceConstantAlpha = Alpha;
                Blend.AlphaFormat = AC_SRC_ALPHA;

                HDC FrameDC = GetDC(nullptr);
                UpdateLayeredWindow(SplashWindow, FrameDC, &DstPos, &Size,
                                    ScaledDC, &SrcPos, 0, &Blend, ULW_ALPHA);
                ReleaseDC(nullptr, FrameDC);
            };

            // =================================================================
            // Animation
            // =================================================================
            constexpr float FadeIn = SplashConfig::FadeInTime;
            constexpr float Hold = SplashConfig::HoldTime;
            constexpr float FadeOut = SplashConfig::FadeOutTime;
            constexpr float StartScale = SplashConfig::StartScale;
            constexpr float Total = FadeIn + Hold + FadeOut;

            const ULONGLONG StartTick = GetTickCount64();
            for (;;)
            {
                PumpMessages();

                const float T = static_cast<float>(GetTickCount64() - StartTick) * 0.001f;
                if (T >= Total) break;

                BYTE Alpha;
                float Scale;

                if (T < FadeIn)
                {
                    // Alpha and scale share the ease so the logo blooms open
                    const float Eased = SmoothStep(0.0f, 1.0f, T / FadeIn);
                    Alpha = static_cast<BYTE>(255.0f * Eased);
                    Scale = Lerp(StartScale, 1.0f, Eased);
                }
                else if (T < FadeIn + Hold)
                {
                    Alpha = 255;
                    Scale = 1.0f;
                }
                else
                {
                    // Alpha only, no shrink on exit
                    Alpha = static_cast<BYTE>(255.0f * SmoothStep(1.0f, 0.0f, (T - FadeIn - Hold) / FadeOut));
                    Scale = 1.0f;
                }

                UpdateFrame(Alpha, Scale);
                Sleep(16);  // ~60 fps
            }

            UpdateFrame(0, 1.0f);  // Avoids a flash on teardown
            DestroyWindow(SplashWindow);
            PumpMessages();

            SelectObject(ScaledDC, OldScaledBmp);
            DeleteObject(static_cast<HGDIOBJ>(ScaledBmp));
            DeleteDC(ScaledDC);
        }

        SelectObject(MemDC, OldBmp);
        DeleteObject(static_cast<HGDIOBJ>(Bmp));
        DeleteDC(MemDC);
        UnregisterClassW(L"WESplash", ModuleInst);
    }
}

#endif
