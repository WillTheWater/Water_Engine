// =============================================================================
// Water Engine v2.1.1
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Utility/SplashScreen.h"
#include "Core/EngineConfig.h"

#if defined(NDEBUG) && defined(WIN32)

#include <windows.h>
#include <physfs.h>
#include <cstdint>

namespace
{
    static void PumpMessages()
    {
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    // -------------------------------------------------------------------------
    // Minimal window
    // -------------------------------------------------------------------------
    static LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
        return DefWindowProcW(hwnd, msg, wp, lp);
    }

    // -------------------------------------------------------------------------
    // Smooth step ease curve
    // -------------------------------------------------------------------------
    static float SmoothStep(float t)
    {
        if (t <= 0.f) return 0.f;
        if (t >= 1.f) return 1.f;
        return t * t * (3.f - 2.f * t);
    }

}

namespace we
{
    void ShowSplash()
    {
        // ------------------------------------------------------------------
        // An empty TexturePath skips splash
        // ------------------------------------------------------------------
        if (SplashConfig::TexturePath[0] == '\0') return;

        // ------------------------------------------------------------------
        // Mount Content.pak early via PhysicsFS so the splash image can be
        // read from the same pak as all other game assets.
        // The ResourceSubsystem checks PHYSFS_isInit() on startup and skips
        // re initialising if already done
        // ------------------------------------------------------------------
        if (!PHYSFS_init(nullptr) || !PHYSFS_mount(ASSET_PACK_PATH, "/", 1))
            return;

        // ------------------------------------------------------------------
        // Read the PNG bytes from the pak, then decode into sf::Image
        // ------------------------------------------------------------------
        PHYSFS_File* pakFile = PHYSFS_openRead(SplashConfig::TexturePath);
        if (!pakFile) return;

        const PHYSFS_sint64 fileLen = PHYSFS_fileLength(pakFile);
        if (fileLen <= 0) { (void)PHYSFS_close(pakFile); return; }

        string pngData;
        pngData.resize(static_cast<size_t>(fileLen));
        PHYSFS_readBytes(pakFile, pngData.data(), static_cast<PHYSFS_uint64>(fileLen));
        (void)PHYSFS_close(pakFile);

        sf::Image img;
        if (!img.loadFromMemory(reinterpret_cast<const std::uint8_t*>(pngData.data()), pngData.size()))
            return;

        const sf::Vector2u imgSz = img.getSize();
        const LONG W = static_cast<LONG>(imgSz.x);
        const LONG H = static_cast<LONG>(imgSz.y);
        if (W <= 0 || H <= 0) return;

        const std::uint8_t* rgba = img.getPixelsPtr();

        HDC screenDC = GetDC(nullptr);
        HDC memDC = CreateCompatibleDC(screenDC);

        BITMAPINFOHEADER bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.biSize = sizeof(bi);
        bi.biWidth = W;
        bi.biHeight = -H;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;

        void* dibBits = nullptr;
        HBITMAP hBmp = CreateDIBSection(screenDC,
            reinterpret_cast<BITMAPINFO*>(&bi),
            DIB_RGB_COLORS, &dibBits, nullptr, 0);
        ReleaseDC(nullptr, screenDC);

        if (!hBmp) { DeleteDC(memDC); return; }

        HGDIOBJ oldBmp = SelectObject(memDC, static_cast<HGDIOBJ>(hBmp));

        // Convert RGBA (SFML) -> BGRA premultiplied (Win32 DIB)
        BYTE* dst = static_cast<BYTE*>(dibBits);
        const LONG pixelCount = W * H;
        for (LONG i = 0; i < pixelCount; ++i)
        {
            const DWORD r = rgba[i * 4 + 0];
            const DWORD g = rgba[i * 4 + 1];
            const DWORD b = rgba[i * 4 + 2];
            const DWORD a = rgba[i * 4 + 3];
            dst[i * 4 + 0] = static_cast<BYTE>(b * a / 255u);  // B premul
            dst[i * 4 + 1] = static_cast<BYTE>(g * a / 255u);  // G premul
            dst[i * 4 + 2] = static_cast<BYTE>(r * a / 255u);  // R premul
            dst[i * 4 + 3] = static_cast<BYTE>(a);
        }

        // ------------------------------------------------------------------
        // Center on primary monitor
        // ------------------------------------------------------------------
        const int scrW = GetSystemMetrics(SM_CXSCREEN);
        const int scrH = GetSystemMetrics(SM_CYSCREEN);
        const int posX = (scrW - static_cast<int>(W)) / 2;
        const int posY = (scrH - static_cast<int>(H)) / 2;

        // ------------------------------------------------------------------
        // Register a minimal window class and create the layered popup.
        //   WS_EX_LAYERED    : enables UpdateLayeredWindow per-pixel alpha
        //   WS_EX_TOOLWINDOW : keeps it off the taskbar
        //   WS_EX_TOPMOST    : floats above all normal windows
        // ------------------------------------------------------------------
        HINSTANCE hInst = GetModuleHandleW(nullptr);

        WNDCLASSEXW wc;
        ZeroMemory(&wc, sizeof(wc));
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = &SplashWndProc;
        wc.hInstance = hInst;
        wc.lpszClassName = L"WESplash";
        (void)RegisterClassExW(&wc);

        HWND hwnd = CreateWindowExW(
            WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
            L"WESplash", L"",
            WS_POPUP,
            posX, posY,
            static_cast<int>(W), static_cast<int>(H),
            nullptr, nullptr, hInst, nullptr);

        if (hwnd)
        {
            ShowWindow(hwnd, SW_SHOW);

            // Scaled DC
            HDC scaledDC = CreateCompatibleDC(nullptr);

            BITMAPINFOHEADER biScaled;
            ZeroMemory(&biScaled, sizeof(biScaled));
            biScaled.biSize = sizeof(biScaled);
            biScaled.biWidth = W;
            biScaled.biHeight = -H;
            biScaled.biPlanes = 1;
            biScaled.biBitCount = 32;
            biScaled.biCompression = BI_RGB;

            void* scaledBits = nullptr;
            HDC     tmpDC = GetDC(nullptr);
            HBITMAP scaledBmp = CreateDIBSection(tmpDC,
                reinterpret_cast<BITMAPINFO*>(&biScaled),
                DIB_RGB_COLORS, &scaledBits, nullptr, 0);
            ReleaseDC(nullptr, tmpDC);

            HGDIOBJ oldScaledBmp = SelectObject(scaledDC, static_cast<HGDIOBJ>(scaledBmp));
            SetStretchBltMode(scaledDC, COLORONCOLOR);

            // UpdateFrame
            auto UpdateFrame = [&](BYTE alpha, float scale)
                {
                    const LONG sW = std::max(1L, static_cast<LONG>(static_cast<float>(W) * scale));
                    const LONG sH = std::max(1L, static_cast<LONG>(static_cast<float>(H) * scale));

                    (void)StretchBlt(scaledDC,
                        0, 0, static_cast<int>(sW), static_cast<int>(sH),
                        memDC,
                        0, 0, static_cast<int>(W), static_cast<int>(H),
                        SRCCOPY);

                    const int px = (scrW - static_cast<int>(sW)) / 2;
                    const int py = (scrH - static_cast<int>(sH)) / 2;

                    POINT ptSrc;
                    ptSrc.x = 0;
                    ptSrc.y = 0;

                    SIZE sz;
                    sz.cx = sW;
                    sz.cy = sH;

                    POINT ptDst;
                    ptDst.x = px;
                    ptDst.y = py;

                    BLENDFUNCTION blend;
                    ZeroMemory(&blend, sizeof(blend));
                    blend.BlendOp = AC_SRC_OVER;
                    blend.BlendFlags = 0;
                    blend.SourceConstantAlpha = alpha;
                    blend.AlphaFormat = AC_SRC_ALPHA;

                    HDC scrDC = GetDC(nullptr);
                    (void)UpdateLayeredWindow(hwnd, scrDC,
                        &ptDst, &sz,
                        scaledDC, &ptSrc,
                        0, &blend, ULW_ALPHA);
                    ReleaseDC(nullptr, scrDC);
                };

            // ------------------------------------------------------------------
            // Animation loop : timing and curves from SplashConfig.
            //
            // Fade-in : alpha 0->255 with smooth-step ease, scale 20%->100%
            // Hold    : full alpha, full scale
            // Fade-out: alpha 255->0 with smooth-step ease
            // ------------------------------------------------------------------
            constexpr float kFadeIn = SplashConfig::FadeInTime;
            constexpr float kHold = SplashConfig::HoldTime;
            constexpr float kFadeOut = SplashConfig::FadeOutTime;
            constexpr float kTotal = kFadeIn + kHold + kFadeOut;

            const ULONGLONG t0 = GetTickCount64();
            for (;;)
            {
                PumpMessages();

                const float t = static_cast<float>(GetTickCount64() - t0) * 0.001f;
                if (t >= kTotal) break;

                BYTE  alpha;
                float scale;

                if (t < kFadeIn)
                {
                    const float eased = SmoothStep(t / kFadeIn);
                    alpha = static_cast<BYTE>(255.f * eased);
                    scale = 0.2f + 0.8f * eased;   // 20% -> 100%
                }
                else if (t < kFadeIn + kHold)
                {
                    alpha = 255;
                    scale = 1.0f;
                }
                else
                {
                    alpha = static_cast<BYTE>(255.f * SmoothStep(1.f - (t - kFadeIn - kHold) / kFadeOut));
                    scale = 1.0f;
                }

                UpdateFrame(alpha, scale);
                Sleep(16);
            }

            // Cleanup
            UpdateFrame(0, 1.0f);
            (void)DestroyWindow(hwnd);
            PumpMessages();
            (void)SelectObject(scaledDC, oldScaledBmp);
            (void)DeleteObject(static_cast<HGDIOBJ>(scaledBmp));
            (void)DeleteDC(scaledDC);
        }

        // ------------------------------------------------------------------
        // GDI cleanup
        // ------------------------------------------------------------------
        (void)SelectObject(memDC, oldBmp);
        (void)DeleteObject(static_cast<HGDIOBJ>(hBmp));
        (void)DeleteDC(memDC);
        (void)UnregisterClassW(L"WESplash", hInst);
    }
}

#endif