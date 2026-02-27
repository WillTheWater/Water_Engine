// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/Editor.h"
#include "Framework/EngineSubsystem.h"
#include "Framework/World/World.h"
#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
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
    // Mode Transitions
    // ============================================================================

    void Editor::OnPlay()
    {
        if (CurrentMode == EditorMode::Play) return;
        if (!Subsystem.Worlds) return;

        SaveWorldState();
        
        if (auto world = Subsystem.Worlds->GetCurrentWorld())
        {
            world->BeginPlayGlobal();
        }

        CurrentMode = EditorMode::Play;
        LOG("Editor: Entered Play mode");
    }

    void Editor::OnPause()
    {
        if (CurrentMode == EditorMode::Play)
        {
            CurrentMode = EditorMode::Simulate;
            LOG("Editor: Paused (Simulate mode)");
        }
    }

    void Editor::OnStop()
    {
        if (CurrentMode == EditorMode::Edit) return;

        RestoreWorldState();
        CurrentMode = EditorMode::Edit;
        LOG("Editor: Stopped (Edit mode)");
    }

    // ============================================================================
    // World State Snapshot
    // ============================================================================

    void Editor::SaveWorldState()
    {
        if (!Subsystem.Worlds) return;
        
        WorldSnapshot.clear();
        if (auto world = Subsystem.Worlds->GetCurrentWorld())
        {
            // world->Serialize(WorldSnapshot);
            // TODO: Implement World::Serialize
        }
    }

    void Editor::RestoreWorldState()
    {
        if (!Subsystem.Worlds) return;
        
        if (auto world = Subsystem.Worlds->GetCurrentWorld())
        {
            // world->Deserialize(WorldSnapshot);
            // TODO: Implement World::Deserialize
        }
    }

    // ============================================================================
    // Main UI
    // ============================================================================

    void Editor::DrawUI()
    {
        DrawMainMenuBar();
        DrawSceneViewport();
        DrawHierarchy();
        DrawInspector();
    }

    void Editor::DrawMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Level")) { /* TODO */ }
                if (ImGui::MenuItem("Open Level")) { /* TODO */ }
                if (ImGui::MenuItem("Save Level")) { /* TODO */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) { /* TODO: Quit */ }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* TODO */ }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* TODO */ }
                ImGui::Separator();
                if (ImGui::MenuItem("Settings")) { /* TODO */ }
                ImGui::EndMenu();
            }

            // Play controls
            ImGui::Separator();
            
            if (CurrentMode == EditorMode::Edit || CurrentMode == EditorMode::Simulate)
            {
                if (ImGui::Button("Play")) OnPlay();
            }
            else
            {
                if (ImGui::Button("Stop")) OnStop();
                ImGui::SameLine();
                if (ImGui::Button("Pause")) OnPause();
            }

            // Mode indicator
            ImGui::SameLine();
            ImGui::Separator();
            ImGui::SameLine();
            
            const char* modeStr = (CurrentMode == EditorMode::Edit) ? "EDIT" :
                                  (CurrentMode == EditorMode::Simulate) ? "SIMULATE" : "PLAY";
            ImGui::Text("Mode: %s", modeStr);

            ImGui::EndMainMenuBar();
        }
    }

    void Editor::DrawSceneViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Scene", nullptr, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoScrollbar | 
            ImGuiWindowFlags_NoScrollWithMouse);

        bViewportFocused = ImGui::IsWindowFocused();
        bViewportHovered = ImGui::IsWindowHovered();

        // Get available space
        ImVec2 availSize = ImGui::GetContentRegionAvail();

        // Get world render texture from RenderSubsystem
        const texture& worldTex = Subsystem.Render->GetWorldTexture();
        ImGui::Image(worldTex, availSize);

        if (bViewportHovered)
        {
            HandleViewportInput();
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void Editor::DrawHierarchy()
    {
        ImGui::Begin("Hierarchy");

        if (!Subsystem.Worlds)
        {
            ImGui::Text("WorldSubsystem not initialized");
        }
        else if (auto world = Subsystem.Worlds->GetCurrentWorld())
        {
            // TODO: List all actors
            ImGui::Text("Actors: (TODO)");
        }
        else
        {
            ImGui::Text("No world loaded");
        }

        ImGui::End();
    }

    void Editor::DrawInspector()
    {
        ImGui::Begin("Inspector");

        if (SelectedActor)
        {
            // TODO: Show selected actor properties
            ImGui::Text("Actor: (TODO)");
        }
        else
        {
            ImGui::Text("No actor selected");
        }

        ImGui::End();
    }

    // ============================================================================
    // Input Handling
    // ============================================================================

    bool Editor::ProcessEvent(const sf::Event& e)
    {
        // Let ImGui process first
        ImGui::SFML::ProcessEvent(*Subsystem.Window, e);

        // If ImGui wants input, consume it
        if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
        {
            return true;
        }

        // Viewport shortcuts (only when viewport focused)
        if (bViewportFocused)
        {
            // TODO: Camera controls (middle mouse pan, scroll zoom)
        }

        return false;
    }

    void Editor::HandleViewportInput()
    {
        // TODO: Camera pan/zoom
        // TODO: Actor selection (click to select)
    }

} // namespace we
