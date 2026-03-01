// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/Editor.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/WaterEngine.h"
#include "Framework/World/World.h"
#include "Framework/World/Actor.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Input/EditorEventHandler.h"
#include "Utility/Log.h"

// ImGui-SFML
#include <imgui.h>
#include <imgui-SFML.h>

namespace we
{
    Editor::Editor(EngineSubsystem& InSubsystem)
        : Subsystem{ InSubsystem }
    {
    }

    Editor::~Editor()
    {
    }

    void Editor::Initialize()
    {
        LOG("Editor initialized");
    }

    void Editor::Shutdown()
    {
        LOG("Editor shutdown");
    }

    // ============================================================================
    // Mode Transitions - called by WaterEngine
    // ============================================================================

    void Editor::OnPlay()
    {
        SaveWorldState();
        
        // Reset camera to default view for gameplay
        Subsystem.Render->SetEditorCameraOffset({ 0, 0 });
        Subsystem.Render->SetEditorZoom(1.0f);
        
        if (Subsystem.World->GetCurrentWorld())
        {
            Subsystem.World->GetCurrentWorld()->BeginPlayGlobal();
        }

        LOG("Editor: Entered Play mode");
    }

    void Editor::OnStop()
    {
        RestoreWorldState();
        LOG("Editor: Stopped (Edit mode)");
    }

    // ============================================================================
    // World State Snapshot
    // ============================================================================

    void Editor::SaveWorldState()
    {
        WorldSnapshot.clear();
        // TODO: Implement world state serialization for play/stop
    }

    void Editor::RestoreWorldState()
    {
        // TODO: Implement world state deserialization for play/stop
    }

    // ============================================================================
    // Main UI
    // ============================================================================

    void Editor::DrawUI(bool bIsPlaying)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 workPos = viewport->Pos;
        ImVec2 workSize = viewport->Size;

        const float menuBarHeight = 25.0f;
        const float remainingHeight = workSize.y - menuBarHeight;

        // === TOP MENU BAR ===
        ImGui::SetNextWindowPos(workPos);
        ImGui::SetNextWindowSize(ImVec2(workSize.x, menuBarHeight));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("##MenuBar", nullptr, 
            ImGuiWindowFlags_NoTitleBar | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_MenuBar);
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Level")) { }
                if (ImGui::MenuItem("Open Level")) { }
                if (ImGui::MenuItem("Save Level")) { }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) { }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) { }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) { }
                ImGui::Separator();
                if (ImGui::MenuItem("Settings")) { }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::SeparatorText("UI Scale");
                if (ImGui::Button("1x", ImVec2(40, 0))) { FontScale = 1.0f; ImGui::GetIO().FontGlobalScale = 1.0f; }
                ImGui::SameLine();
                if (ImGui::Button("1.5x", ImVec2(40, 0))) { FontScale = 1.5f; ImGui::GetIO().FontGlobalScale = 1.5f; }
                ImGui::SameLine();
                if (ImGui::Button("2x", ImVec2(40, 0))) { FontScale = 2.0f; ImGui::GetIO().FontGlobalScale = 2.0f; }
                
                ImGui::SeparatorText("Grid");
                bool gridEnabled = Subsystem.Render->IsGridEnabled();
                if (ImGui::Checkbox("Show Grid", &gridEnabled))
                {
                    Subsystem.Render->SetGridEnabled(gridEnabled);
                }
                
                bool originEnabled = Subsystem.Render->IsGridOriginEnabled();
                if (ImGui::Checkbox("Show Origin", &originEnabled))
                {
                    Subsystem.Render->SetGridOriginEnabled(originEnabled);
                }
                
                if (ImGui::BeginMenu("Grid Colors"))
                {
                    auto gridColor = Subsystem.Render->GetGridColor();
                    float gridColorF[4] = { gridColor.r / 255.0f, gridColor.g / 255.0f, gridColor.b / 255.0f, gridColor.a / 255.0f };
                    if (ImGui::ColorEdit4("Grid Lines", gridColorF))
                    {
                        Subsystem.Render->SetGridColor(color(
                            static_cast<uint8>(gridColorF[0] * 255),
                            static_cast<uint8>(gridColorF[1] * 255),
                            static_cast<uint8>(gridColorF[2] * 255),
                            static_cast<uint8>(gridColorF[3] * 255)));
                    }

                    auto xAxisColor = Subsystem.Render->GetGridXAxisColor();
                    float xAxisColorF[4] = { xAxisColor.r / 255.0f, xAxisColor.g / 255.0f, xAxisColor.b / 255.0f, xAxisColor.a / 255.0f };
                    if (ImGui::ColorEdit4("X Axis", xAxisColorF))
                    {
                        Subsystem.Render->SetGridXAxisColor(color(
                            static_cast<uint8>(xAxisColorF[0] * 255),
                            static_cast<uint8>(xAxisColorF[1] * 255),
                            static_cast<uint8>(xAxisColorF[2] * 255),
                            static_cast<uint8>(xAxisColorF[3] * 255)));
                    }

                    auto yAxisColor = Subsystem.Render->GetGridYAxisColor();
                    float yAxisColorF[4] = { yAxisColor.r / 255.0f, yAxisColor.g / 255.0f, yAxisColor.b / 255.0f, yAxisColor.a / 255.0f };
                    if (ImGui::ColorEdit4("Y Axis", yAxisColorF))
                    {
                        Subsystem.Render->SetGridYAxisColor(color(
                            static_cast<uint8>(yAxisColorF[0] * 255),
                            static_cast<uint8>(yAxisColorF[1] * 255),
                            static_cast<uint8>(yAxisColorF[2] * 255),
                            static_cast<uint8>(yAxisColorF[3] * 255)));
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();
        ImGui::PopStyleVar();

        // === RESIZE HANDLES ===
        // Draw these LAST so they appear on top of everything
        // Vertical resize handle between left panels and Details
        float resizeHandleWidth = 8.0f;
        float leftPanelWidth = workSize.x - DetailsPanelWidth - resizeHandleWidth;
        
        // Draw vertical splitter on top
        float splitterX = workPos.x + leftPanelWidth + resizeHandleWidth * 0.5f;
        ImDrawList* fgDrawList = ImGui::GetForegroundDrawList();
        
        // Visible line
        fgDrawList->AddLine(
            ImVec2(splitterX, workPos.y + menuBarHeight + 5),
            ImVec2(splitterX, workPos.y + workSize.y - 5),
            IM_COL32(100, 100, 100, 150), 2.0f);
        
        // Grip dots
        float centerY = workPos.y + menuBarHeight + remainingHeight * 0.5f;
        for (int i = -2; i <= 2; i++)
        {
            fgDrawList->AddCircleFilled(
                ImVec2(splitterX, centerY + i * 15), 3.0f,
                IM_COL32(150, 150, 150, 200));
        }
        
        // Invisible button for interaction (on top)
        ImGui::SetNextWindowPos(ImVec2(workPos.x + leftPanelWidth, workPos.y + menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(resizeHandleWidth, remainingHeight));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin("##VSplitter", nullptr, 
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        
        if (ImGui::IsWindowHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                bResizingDetails = true;
            }
        }
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            bResizingDetails = false;
        }
        
        if (bResizingDetails)
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            float newWidth = workSize.x - (mousePos.x - workPos.x);
            DetailsPanelWidth = std::max(150.0f, std::min(500.0f, newWidth));
        }
        ImGui::End();
        ImGui::PopStyleColor();

        // Horizontal resize handle between Viewport and World
        float viewportHeight = remainingHeight * ViewportHeightRatio;
        float worldHeight = remainingHeight - viewportHeight - resizeHandleWidth;
        
        // Draw horizontal splitter on top
        float splitterY = workPos.y + menuBarHeight + viewportHeight + resizeHandleWidth * 0.5f;
        
        // Visible line
        fgDrawList->AddLine(
            ImVec2(workPos.x + 5, splitterY),
            ImVec2(workPos.x + leftPanelWidth - 5, splitterY),
            IM_COL32(100, 100, 100, 150), 2.0f);
        
        // Grip dots
        float centerX = workPos.x + leftPanelWidth * 0.5f;
        for (int i = -2; i <= 2; i++)
        {
            fgDrawList->AddCircleFilled(
                ImVec2(centerX + i * 15, splitterY), 3.0f,
                IM_COL32(150, 150, 150, 200));
        }
        
        // Invisible button for interaction
        ImGui::SetNextWindowPos(ImVec2(workPos.x, workPos.y + menuBarHeight + viewportHeight));
        ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, resizeHandleWidth));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin("##HSplitter", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
        
        if (ImGui::IsWindowHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                bResizingViewport = true;
            }
        }
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            bResizingViewport = false;
        }
        
        if (bResizingViewport)
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            float newRatio = (mousePos.y - workPos.y - menuBarHeight) / remainingHeight;
            ViewportHeightRatio = std::max(0.2f, std::min(0.8f, newRatio));
        }
        ImGui::End();
        ImGui::PopStyleColor();

        // === LAYOUT ===
        // Recalculate after potential resize
        leftPanelWidth = workSize.x - DetailsPanelWidth - resizeHandleWidth;
        viewportHeight = remainingHeight * ViewportHeightRatio;
        worldHeight = remainingHeight - viewportHeight - resizeHandleWidth;

        // VIEWPORT (upper left)
        ImGui::SetNextWindowPos(ImVec2(workPos.x, workPos.y + menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, viewportHeight));
        DrawViewport(bIsPlaying);

        // OBJECTS (below viewport)
        ImGui::SetNextWindowPos(ImVec2(workPos.x, workPos.y + menuBarHeight + viewportHeight + resizeHandleWidth));
        ImGui::SetNextWindowSize(ImVec2(leftPanelWidth, worldHeight));
        DrawObjects();

        // EDITOR TOOLS (top right - buttons + mouse pos, no title bar)
        float toolsHeight = 100.0f;  // Fixed height for buttons + mouse
        ImGui::SetNextWindowPos(ImVec2(workPos.x + leftPanelWidth + resizeHandleWidth, workPos.y + menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(DetailsPanelWidth, toolsHeight));
        DrawEditorTools(bIsPlaying);

        // DETAILS (below tools - actor info)
        float detailsStartY = workPos.y + menuBarHeight + toolsHeight + resizeHandleWidth;
        float detailsHeight = remainingHeight - toolsHeight - resizeHandleWidth;
        ImGui::SetNextWindowPos(ImVec2(workPos.x + leftPanelWidth + resizeHandleWidth, detailsStartY));
        ImGui::SetNextWindowSize(ImVec2(DetailsPanelWidth, detailsHeight));
        DrawDetails(bIsPlaying);
    }

    void Editor::DrawViewport(bool bIsPlaying)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        
        // Dark background for viewport to prevent seeing through
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        
        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar |  // Remove title bar - don't cover render
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        if (ImGui::Begin("Viewport", nullptr, flags))
        {
            bViewportFocused = ImGui::IsWindowFocused();
            bViewportHovered = ImGui::IsWindowHovered();

            // Get render resolution
            vec2u renderRes = Subsystem.Render->GetRenderResolution();
            float renderAspect = (float)renderRes.x / (float)renderRes.y;

            // Get available space
            ImVec2 availSize = ImGui::GetContentRegionAvail();
            float availAspect = availSize.x / availSize.y;

            // Calculate display size maintaining aspect ratio
            ImVec2 displaySize;
            if (availAspect > renderAspect)
            {
                // Window is wider - fit to height
                displaySize.y = availSize.y;
                displaySize.x = displaySize.y * renderAspect;
            }
            else
            {
                // Window is taller - fit to width
                displaySize.x = availSize.x;
                displaySize.y = displaySize.x / renderAspect;
            }

            // Center the image
            ImVec2 cursorPos(
                (availSize.x - displaySize.x) * 0.5f,
                (availSize.y - displaySize.y) * 0.5f
            );
            ImGui::SetCursorPos(cursorPos);

            // Draw the world texture (flip Y to match SFML's coordinate system)
            const texture& worldTex = Subsystem.Render->GetWorldTexture();
            ImTextureID texID = (ImTextureID)(intptr_t)worldTex.getNativeHandle();
            ImGui::Image(texID, displaySize, ImVec2(0, 1), ImVec2(1, 0));

            if (bViewportHovered)
            {
                HandleViewportInput();
                
                // Calculate mouse position in world space
                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 relativeMouse(mousePos.x - windowPos.x - cursorPos.x, mousePos.y - windowPos.y - cursorPos.y);
                
                // Convert to world coordinates
                vec2u renderRes = Subsystem.Render->GetRenderResolution();
                vec2f viewCenter = (vec2f(renderRes) / 2.0f) + Subsystem.Render->GetEditorCameraOffset();
                vec2f viewSize = vec2f(renderRes) / Subsystem.Render->GetEditorZoom();
                
                float normX = relativeMouse.x / displaySize.x;
                float normY = relativeMouse.y / displaySize.y;
                
                MouseWorldPos.x = viewCenter.x - viewSize.x * 0.5f + normX * viewSize.x;
                MouseWorldPos.y = viewCenter.y - viewSize.y * 0.5f + normY * viewSize.y;
            }
        }
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void Editor::DrawObjects()
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        if (ImGui::Begin("Objects", nullptr, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoResize))
        {
            if (Subsystem.World->GetCurrentWorld())
            {
                const auto& actors = Subsystem.World->GetCurrentWorld()->GetActors();
                
                for (const auto& actor : actors)
                {
                    if (!actor) continue;

                    bool isSelected = (SelectedActor == actor.get());
                    std::string label = "Actor##" + std::to_string(reinterpret_cast<uintptr_t>(actor.get()));
                    
                    if (ImGui::Selectable(label.c_str(), isSelected))
                    {
                        SelectedActor = actor.get();
                    }
                }

                if (actors.empty())
                {
                    ImGui::TextDisabled("No objects in world");
                }
            }
            else
            {
                ImGui::TextDisabled("No world loaded");
            }
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void Editor::DrawEditorTools(bool bIsPlaying)
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        
        ImGuiWindowFlags flags = 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoTitleBar |  // No title bar
            ImGuiWindowFlags_NoResize;
        
        if (ImGui::Begin("EditorTools", nullptr, flags))
        {
            // === PLAY/STOP + RECENTER BUTTONS ===
            ImVec2 buttonSize(120, 35);  // Wider for 2x font
            float buttonSpacing = 8.0f;
            
            // Play/Stop button
            if (!bIsPlaying)
            {
                if (ImGui::Button("Play", buttonSize))
                {
                    if (OnRequestPlayStop) OnRequestPlayStop(true);
                }
            }
            else
            {
                if (ImGui::Button("Stop", buttonSize))
                {
                    if (OnRequestPlayStop) OnRequestPlayStop(false);
                }
            }
            
            ImGui::SameLine(0, buttonSpacing);
            
            // Recenter button - only active in Edit mode
            ImGui::BeginDisabled(bIsPlaying);
            if (ImGui::Button("Recenter", buttonSize))
            {
                Subsystem.Render->SetEditorCameraOffset({ 0,0 });
                Subsystem.Render->SetEditorZoom(1.0f);
            }
            ImGui::EndDisabled();
            
            // Mouse Position - always visible below buttons
            ImGui::Separator();
            ImGui::Text("Mouse: %.1f, %.1f", MouseWorldPos.x, MouseWorldPos.y);
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void Editor::DrawDetails(bool bIsPlaying)
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        if (ImGui::Begin("Details", nullptr, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoResize))
        {
            if (SelectedActor)
            {
                ImGui::SeparatorText("Transform");
                
                vec2f pos = SelectedActor->GetPosition();
                float posArr[2] = { pos.x, pos.y };
                // Drag to change, Ctrl+Click or Double-click to type exact value
                if (ImGui::DragFloat2("Position", posArr, 1.0f, -999999.0f, 999999.0f, "%.2f"))
                {
                    SelectedActor->SetPosition({ posArr[0], posArr[1] });
                }

                bool visible = SelectedActor->IsVisible();
                if (ImGui::Checkbox("Visible", &visible))
                {
                    SelectedActor->SetVisible(visible);
                }

                ImGui::SeparatorText("Info");
                ImGui::Text("Type: Actor");
                ImGui::Text("ID: %p", (void*)SelectedActor);
            }
            else
            {
                ImGui::TextDisabled("Select an actor to view details");
            }
        }
        ImGui::End();
        ImGui::PopStyleColor();
    }

    // ============================================================================
    // Input Handling - Visitor Pattern via EditorEventHandler
    // ============================================================================

    void Editor::HandleEvent(const sf::Event& Event)
    {
        // Process event through ImGui-SFML first
        ImGui::SFML::ProcessEvent(*Subsystem.Window, Event);

        // Use visitor pattern for editor-specific event handling
        EditorEventHandler Handler{ *this };
        Event.visit(Handler);
    }

    void Editor::HandleViewportInput()
    {
        ImGuiIO& io = ImGui::GetIO();
        float dt = ImGui::GetIO().DeltaTime;
        
        // Scroll wheel zoom
        float wheel = io.MouseWheel;
        if (wheel != 0.0f)
        {
            float currentZoom = Subsystem.Render->GetEditorZoom();
            float newZoom = currentZoom * (1.0f + wheel * 0.1f);
            // Clamp zoom between 0.1x and 5x
            newZoom = std::max(0.1f, std::min(5.0f, newZoom));
            Subsystem.Render->SetEditorZoom(newZoom);
        }
        
        // WASD pan (only when viewport focused, not typing in text field)
        if (!io.WantTextInput)
        {
            float panSpeed = 500.0f * dt / Subsystem.Render->GetEditorZoom();  // Faster zoom = faster pan
            vec2f currentOffset = Subsystem.Render->GetEditorCameraOffset();
            
            if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow))
                currentOffset.y -= panSpeed;
            if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow))
                currentOffset.y += panSpeed;
            if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow))
                currentOffset.x -= panSpeed;
            if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow))
                currentOffset.x += panSpeed;
            
            Subsystem.Render->SetEditorCameraOffset(currentOffset);
        }
        
        // Middle mouse button pan
        if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
        {
            if (!bIsPanning)
            {
                // Start panning
                bIsPanning = true;
                LastMousePos = vec2f(io.MousePos.x, io.MousePos.y);
            }
            else
            {
                // Continue panning - calculate delta and move camera
                vec2f currentPos(io.MousePos.x, io.MousePos.y);
                vec2f delta = currentPos - LastMousePos;
                
                // Adjust delta by zoom level (dragging same distance should move view same amount regardless of zoom)
                vec2f currentOffset = Subsystem.Render->GetEditorCameraOffset();
                currentOffset.x -= delta.x / Subsystem.Render->GetEditorZoom();
                currentOffset.y -= delta.y / Subsystem.Render->GetEditorZoom();
                Subsystem.Render->SetEditorCameraOffset(currentOffset);
                
                LastMousePos = currentPos;
            }
        }
        else
        {
            bIsPanning = false;
        }
    }

} // namespace we
