-- hyprlooks: full Vivaldi-style desktop example
--
-- Demonstrates the pieces that make a browser-like shell:
--   * a top bar that RESERVES space (windows tile below it)
--   * a vertical left "app rail" panel (like Vivaldi's side panel), also
--     reserving space, built from nested containers + flex column layout
--   * interactive buttons wired to on_click
--
-- Source this from your hyprland.lua:  source = ~/.config/hypr/hyprlooks.lua
--
-- Colors are { r, g, b, a } in 0..1. Icons below are Nerd Font glyphs.

local accent  = { 0.20, 0.50, 0.90, 1 }
local surface = { 0.12, 0.12, 0.16, 1 }
local text    = { 0.90, 0.90, 0.95, 1 }
local muted   = { 0.55, 0.55, 0.62, 1 }
local hover   = { 0.18, 0.18, 0.24, 1 }

local function launch(cmd)
    return function() hl.dispatch(hl.dsp.exec_cmd(cmd)) end
end

-- helper: a square icon button for the app rail
local function rail_button(icon, cmd, fg)
    return {
        type     = "button",
        icon     = icon,
        on_click = launch(cmd),
        style    = {
            fg            = fg or text,
            font_size     = 20,
            padding       = { 10, 10 },
            hover_bg      = hover,
            border_radius = 10,
        },
    }
end

--------------------------------------------------------------------------------
-- Top bar
--------------------------------------------------------------------------------
hl.plugin.hyprlooks.bar({
    position = "top",
    height   = 40,
    reserve  = true, -- windows tile below the bar (default true)
    style    = {
        bg            = { 0.08, 0.08, 0.10, 0.85 },
        blur          = true,
        blur_alpha    = 0.8,
        border_radius = { 0, 0, 12, 12 },
        padding       = { 10, 4 },
    },
    left = {
        {
            type     = "button",
            icon     = "󰀻", -- app launcher
            on_click = launch("wofi --show drun"),
            style    = { fg = accent, font_size = 16, hover_bg = hover, border_radius = 8, padding = { 8, 4 } },
        },
        {
            type  = "workspaces",
            style = { active_bg = accent, inactive_fg = muted },
        },
    },
    center = {
        { type = "clock", format = "%a %d %b   %H:%M", style = { fg = text, font_size = 13 } },
    },
    right = {
        { type = "button", icon = "󰕾", on_click = launch("pavucontrol"), style = { fg = text, hover_bg = hover, border_radius = 8, padding = { 8, 4 } } },
        { type = "button", icon = "󰤨", on_click = launch("nm-connection-editor"), style = { fg = text, hover_bg = hover, border_radius = 8, padding = { 8, 4 } } },
        { type = "button", icon = "󰐥", on_click = launch("wlogout"), style = { fg = { 0.9, 0.4, 0.4, 1 }, hover_bg = hover, border_radius = 8, padding = { 8, 4 } } },
    },
})

--------------------------------------------------------------------------------
-- Left app rail (Vivaldi-style vertical side panel)
--------------------------------------------------------------------------------
hl.plugin.hyprlooks.panel({
    anchor  = "left",
    width   = 56,
    margin  = { 8, 48 }, -- leave room under the top bar
    reserve = true,      -- tiled windows keep clear of the rail
    style   = {
        bg            = surface,
        blur          = true,
        blur_alpha    = 0.85,
        border_radius = 16,
        padding       = { 6, 12 },
    },
    -- space_between pushes the two groups to the top and bottom of the rail
    layout   = { direction = "column", justify = "space_between", align = "center" },
    children = {
        -- top group: apps
        {
            type     = "container",
            layout   = { direction = "column", gap = 10, align = "center" },
            children = {
                rail_button("󰈹", "firefox", accent),
                rail_button("󰆍", "kitty"),
                rail_button("󰉋", "nautilus"),
                rail_button("󰨞", "code"),
                rail_button("󰭹", "discord"),
            },
        },
        -- bottom group: system
        {
            type     = "container",
            layout   = { direction = "column", gap = 10, align = "center" },
            children = {
                rail_button("󰒓", "kitty -e htop"),
                rail_button("󰍃", "hyprctl dispatch exit", { 0.9, 0.4, 0.4, 1 }),
            },
        },
    },
})
