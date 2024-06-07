#pragma once

constexpr iVec2 actionBarSlotDimensions{ 32,32 };


class imguiBind
{
public:
    void Init(GLFWwindow*, const char*);
    void NewFrame();
    virtual void Update();
    void Render();
    void Close();
};