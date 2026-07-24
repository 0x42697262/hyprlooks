-- hyprlooks: Vivaldi-styled bar example
-- Place in your hyprland.lua or source from it.

hl.plugin.hyprlooks.bar({
    position = "top",
    height = 44,
    style = {
        bg = { 0.08, 0.08, 0.10, 0.85 },
        blur = true,
        blur_alpha = 0.8,
        border_radius = { 0, 0, 12, 12 },
        padding = { 8, 4 },
    },
    left = {
        {
            type = "button",
            icon = "󰖳",
            on_click = function()
                hl.dispatch(hl.dsp.exec_cmd("rofi -show drun"))
            end,
            style = {
                hover_bg = { 0.15, 0.15, 0.20, 1 },
                border_radius = { 6, 6, 6, 6 },
            },
        },
        {
            type = "label",
            text = "Hyprland",
            style = {
                fg = { 0.5, 0.5, 0.55, 1 },
                font_size = 12,
            },
        },
    },
    center = {
        {
            type = "label",
            text = "Welcome to hyprlooks",
            style = {
                fg = { 0.9, 0.9, 0.95, 1 },
                font_size = 14,
            },
        },
    },
    right = {
        {
            type = "label",
            text = "󰕻",
            style = {
                fg = { 0.2, 0.5, 0.9, 1 },
                font_size = 14,
            },
        },
        {
            type = "button",
            text = "Menu",
            on_click = function()
                os.execute("notify-send 'hyprlooks' 'Button clicked!'")
            end,
            style = {
                hover_bg = { 0.15, 0.15, 0.20, 1 },
                border_radius = { 6, 6, 6, 6 },
            },
        },
    },
})
