// =============================================================================
// DemoGame - Test Screen for GUI System
// =============================================================================

#pragma once

#include "UI/UIScreen.h"
#include <imgui.h>

class TestScreen : public we::UIScreen
{
public:
    void Draw() override
    {
        ImGui::SetNextWindowPos(ImVec2(100, 100));
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        
        ImGui::Begin("Test GUI");
        ImGui::Text("Hello from GUI Subsystem!");
        
        if (ImGui::Button("Click Me", ImVec2(120, 40)))
        {
            ClickCount++;
        }
        
        ImGui::Text("Clicks: %d", ClickCount);
        ImGui::End();
    }
    
private:
    int ClickCount = 0;
};
