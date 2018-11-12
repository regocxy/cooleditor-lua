#include "gui_system.h"

#include "cocos2d.h"
#include "../impl/cocos/ViewImpl.h"
#include "../impl/imgui/imgui_impl_glfw_gl2.h"

gui_system &get_gui_system()
{
    static gui_system s_gui_system;
    return s_gui_system;
}

gui_system::gui_system()
{
    auto director = cocos2d::Director::getInstance();
    auto glview = director->getOpenGLView();

    gui::CreateContext();
    ImGui_ImplGlfwGL2_Init(static_cast<ViewImpl *>(glview)->getWindow(), false);
    gui::StyleColorsDark();
}

gui_system::~gui_system()
{
    ImGui_ImplGlfwGL2_Shutdown();
    gui::DestroyContext();
}

void gui_system::draw_begin()
{
    ImGui_ImplGlfwGL2_NewFrame();
    auto &io = gui::GetIO();
    auto &viewsize = cocos2d::Director::getInstance()->getWinSize();
    io.DisplaySize = ImVec2(static_cast<float>(viewsize.width), static_cast<float>(viewsize.height));
    gui::SetNextWindowPos(ImVec2(0, 0));
    gui::SetNextWindowSize(io.DisplaySize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;

    gui::Begin("###workspace", nullptr, flags);
}

void gui_system::draw_end()
{
    gui::End();
    gui::Render();
    glUseProgram(0);
    ImGui_ImplGlfwGL2_RenderDrawData(gui::GetDrawData());
}

gui_style &get_gui_style()
{
    static gui_style s_gui_style;
    return s_gui_style;
}

void gui_style::reset_style()
{
    set_style_colors(hsv_setup());
}

void gui_style::set_style_colors(const hsv_setup &_setup)
{
    setup = _setup;
    ImVec4 col_text = ImColor::HSV(setup.col_text_hue, setup.col_text_sat, setup.col_text_val);
    ImVec4 col_main = ImColor::HSV(setup.col_main_hue, setup.col_main_sat, setup.col_main_val);
    ImVec4 col_back = ImColor::HSV(setup.col_back_hue, setup.col_back_sat, setup.col_back_val);
    ImVec4 col_area = ImColor::HSV(setup.col_area_hue, setup.col_area_sat, setup.col_area_val);
    float rounding = setup.frame_rounding;

    ImGuiStyle &style = gui::GetStyle();
    style.FrameRounding = rounding;
    style.WindowRounding = rounding;
    style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(col_area.x * 0.8f, col_area.y * 0.8f, col_area.z * 0.8f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(col_text.x, col_text.y, col_text.z, 0.80f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.54f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.10f, 0.10f, 0.10f, 0.55f);
}

gui_style::hsv_setup gui_style::get_dark_style()
{
    hsv_setup result;
    result.col_main_hue = 0.0f / 255.0f;
    result.col_main_sat = 0.0f / 255.0f;
    result.col_main_val = 80.0f / 255.0f;

    result.col_area_hue = 0.0f / 255.0f;
    result.col_area_sat = 0.0f / 255.0f;
    result.col_area_val = 50.0f / 255.0f;

    result.col_back_hue = 0.0f / 255.0f;
    result.col_back_sat = 0.0f / 255.0f;
    result.col_back_val = 35.0f / 255.0f;

    result.col_text_hue = 0.0f / 255.0f;
    result.col_text_sat = 0.0f / 255.0f;
    result.col_text_val = 255.0f / 255.0f;
    result.frame_rounding = 0.0f;

    return result;
}

gui_style::hsv_setup gui_style::get_lighter_red()
{
    hsv_setup result;
    result.col_main_hue = 0.0f / 255.0f;
    result.col_main_sat = 200.0f / 255.0f;
    result.col_main_val = 170.0f / 255.0f;

    result.col_area_hue = 0.0f / 255.0f;
    result.col_area_sat = 0.0f / 255.0f;
    result.col_area_val = 80.0f / 255.0f;

    result.col_back_hue = 0.0f / 255.0f;
    result.col_back_sat = 0.0f / 255.0f;
    result.col_back_val = 35.0f / 255.0f;

    result.col_text_hue = 0.0f / 255.0f;
    result.col_text_sat = 0.0f / 255.0f;
    result.col_text_val = 255.0f / 255.0f;
    result.frame_rounding = 0.0f;

    return result;
}