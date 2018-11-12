#pragma once

struct gui_system
{
    gui_system();
    ~gui_system();
    static void draw_begin();
    static void draw_end();
};

gui_system &get_gui_system();

struct gui_style
{
    struct hsv_setup
    {
        float col_main_hue = 0.0f / 255.0f;
        float col_main_sat = 0.0f / 255.0f; //200.0f / 255.0f;
        float col_main_val = 80.0f / 255.0f; //170.0f / 255.0f;

        float col_area_hue = 0.0f / 255.0f;
        float col_area_sat = 0.0f / 255.0f;
        float col_area_val = 60.0f / 255.0f;

        float col_back_hue = 0.0f / 255.0f;
        float col_back_sat = 0.0f / 255.0f;
        float col_back_val = 35.0f / 255.0f;

        float col_text_hue = 0.0f / 255.0f;
        float col_text_sat = 0.0f / 255.0f;
        float col_text_val = 255.0f / 255.0f;
        float frame_rounding = 0.0f;
    };

    void reset_style();
    void set_style_colors(const hsv_setup& _setup);
    // void load_style();
    // void save_style();
    static hsv_setup get_dark_style();
    static hsv_setup get_lighter_red();
    hsv_setup setup;
};

gui_style& get_gui_style();