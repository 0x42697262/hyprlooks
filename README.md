# hyprlooks

In-process UI system for Hyprland — Vivaldi-styled bars and widgets rendered
directly in the compositor. ~1-5MB additional memory (vs 50-200MB for
eww/ags/quickshell).

## What

A Hyprland plugin that provides a full widget toolkit rendered in the
compositor's render pipeline, configured via Lua. No separate process, no
GTK/Qt, no Node.js — just the compositor's own OpenGL/GLES3 renderer.

## Features

- **Lua-configured widgets** — `hl.plugin.hyprlooks.bar({ ... })` in your
  `hyprland.lua`
- **In-process rendering** — uses Hyprland's render pass system (`EK_CUSTOM`),
  damage tracking, and occlusion culling
- **Command pattern** — all render and input operations are command objects,
  not direct API calls
- **Safety system** — crash isolation, safe mode state machine
  (NORMAL → DEGRADED → SAFE_MODE → DISABLED), auto-recovery
- **Vivaldi-inspired theme** — glassmorphism, rounded corners, blur, shadows

## Building

Requires Hyprland headers matching the **exact running commit** (plugins are
ABI-coupled; use the same compiler Hyprland was built with).

```sh
# against installed headers (hyprland.pc present):
make

# against a source checkout:
make HYPRLAND_HEADERS=~/Development/Hyprland
```

Load it:

```sh
hyprctl plugin load $(pwd)/hyprlooks.so
hyprctl plugins list
```

Or with hyprpm:

```sh
hyprpm add <repo-url>
hyprpm enable hyprlooks
hyprpm reload
```

## Usage

In your `hyprland.lua`:

```lua
hl.plugin.hyprlooks.bar({
    position = "top",
    height = 44,
    style = {
        bg = { 0.08, 0.08, 0.10, 0.85 },
        blur = true,
        blur_alpha = 0.8,
        border_radius = { 0, 0, 12, 12 },
    },
    left = {
        {
            type = "button",
            icon = "󰖳",
            on_click = function()
                hl.dispatch(hl.dsp.exec_cmd("rofi -show drun"))
            end,
        },
    },
    center = {
        { type = "label", text = "Hyprland" },
    },
    right = {
        { type = "label", text = "󰕻" },
    },
})
```

See `examples/default_bar.lua` for a complete example.

## Safety System

The plugin wraps every Hyprland → plugin entry point in crash isolation guards.
If errors occur, it escalates through safe modes:

| Mode | Rendering | Input | Blur | Animations |
|------|-----------|-------|------|------------|
| NORMAL | Full | Full | Yes | Yes |
| DEGRADED | Full | Full | No | No |
| SAFE_MODE | None | None | No | No |
| DISABLED | None | None | No | No |

Auto-recovery: DEGRADED → NORMAL after 3 consecutive clean frames.
SAFE_MODE and DISABLED require manual recovery.

### Dispatchers

- `hyprctl dispatch hyprlooks_recover` — attempt recovery
- `hyprctl dispatch hyprlooks_status` — print current status
- `hyprctl dispatch hyprlooks_safe_mode` — force safe mode
- `hyprctl dispatch hyprlooks_disable` — disable plugin

### Lua API

- `hl.plugin.hyprlooks.bar({ ... })` — create a bar
- `hl.plugin.hyprlooks.widget({ ... })` — create a standalone widget
- `hl.plugin.hyprlooks.status()` — get safety status string

## Architecture

```
platform/   Hyprland API adapters (Adapter pattern)
core/       Widget model (IWidget, CWidgetTree, CWidgetRegistry)
layout/     Layout strategies (Strategy pattern: CFlexLayout)
state/      Widget visual states (State pattern: hover, pressed, focused)
theme/      Theme system (Vivaldi-inspired defaults)
render/     Render commands (Command pattern: IRenderCommand → pass elements)
input/      Input commands (Command pattern: IInputCommand → event bus hooks)
widgets/    Concrete widget library (CBar, CLabel, CButton, CImage, ...)
lua/        Lua bridge (Facade pattern: hl.plugin.hyprlooks.*)
safety/     Safety, recovery, watchdog (CSafetyManager, CCrashIsolation)
```

Layers depend only downward. No circular dependencies. All Hyprland API
calls go through `platform/` adapters.

See `AGENTS.md` for full architecture and code guidelines.
