# AGENTS.md

## Review guidelines

- Prioritize crash safety above all else: a plugin crash kills the compositor.
  Flag any uncaught exception path, unchecked null deref, or unbounded loop.
- Flag any direct Hyprland API call outside of `platform/` — all Hyprland
  interaction must go through the adapter layer.
- Flag any widget that calls render/input APIs directly instead of producing
  commands — widgets must use the Command pattern (IRenderCommand / IInputCommand).
- Flag any code path that could throw across a C boundary (PLUGIN_INIT, Lua C
  functions, event bus callbacks) without a CCrashIsolation guard.
- For new widgets, verify they are registered in CWidgetRegistry and have a
  Lua factory path in CLuaWidgetFactory.
- For new config options, remind the author to update examples/ and README.
- Flag silent config breakage: changing an existing option's behavior. This is
  not allowed.
- Flag bad config style that breaks the style guidelines below and suggest fixes.
- Flag violations of the safe mode contract: NORMAL mode features must not be
  assumed available — always check CSafetyManager before using blur,
  animations, or other expensive features.
- For performance-sensitive paths (per-frame render, per-event input), flag
  obvious algorithmic regressions, allocations in hot loops, or missing
  texture caches.
- For tests, flag missing coverage for changed or new behavior, as long as the
  testing framework is capable of testing it.

## Architecture

### Layer dependency rule

Layers may only depend downward:

```
platform   → (Hyprland headers)
core       → platform (types only)
layout     → core
state      → core
theme      → core
render     → core + platform
input      → core + platform
widgets    → core + layout + state + render + theme
lua        → widgets + core + theme
safety     → wraps all layers
```

No circular dependencies. No upward imports. No skipping layers (e.g. widgets
must not import platform/ directly — go through render/ or input/).

### Design patterns used

- **Command**: render/ and input/ use command queues, not direct API calls
- **Strategy**: layout/ provides interchangeable layout algorithms
- **State**: state/ manages widget visual states (hover, pressed, focused)
- **Factory**: core/CWidgetRegistry creates widgets by type name
- **Facade**: lua/CLuaAPI is the single Lua entry point
- **Adapter**: platform/ wraps all Hyprland APIs
- **Observer**: hyprutils Signals for Hyprland event subscriptions

### Safety contract

Every entry point from Hyprland into the plugin must be wrapped in
CCrashIsolation::guard(). Entry points are:

- render stage hook (CWidgetRenderer)
- event bus callbacks (CInputManager, CWidgetRenderer)
- Lua C functions (CLuaAPI)
- PLUGIN_INIT / PLUGIN_EXIT
- dispatcher handlers

### Safe mode state machine

```
NORMAL → DEGRADED → SAFE_MODE → DISABLED
  ↑         │            │
  └─ auto-recover (3 clean frames)
```

- NORMAL: all features (blur, animations, all widgets)
- DEGRADED: no blur, no animations, all widgets render
- SAFE_MODE: no rendering at all, input disabled
- DISABLED: plugin fully inactive, requires reload

Auto-recovery: DEGRADED → NORMAL after 3 consecutive clean frames.
SAFE_MODE and DISABLED require manual `hyprctl dispatch hyprlooks_recover` or
plugin reload.

## Style guidelines

- Code must be clang-formatted according to `.clang-format`.
- single-line if and else statements must come without braces. This rule
  applies only to if / else, not do / while / other.
- Avoid function bodies in headers as much as possible.
- Avoid namespace {} in source files to mark local functions. Prefer `static`.
- Prefer guards in functions and loops: `if (!cond) continue;`
- Prefer forward-declaration in headers to inclusion.
- Leave a stray `,` at the end of brace-enclosed lists to make formatting easier.
- Leave a `;` inside empty function bodies for formatting.
- Naming conventions:
  - class: `CMyClass`
  - struct: `SMyStruct`
  - interface: `IMyInterface`
  - class (not struct) member variables: `m_variable`
- Do not use absolute includes from `src/` in headers: instead of
  `#include "a/b.hpp"` use `#include "../a/b.hpp"`.
- Hyprland headers are included with the `src/` prefix (e.g.
  `#include <src/render/Renderer.hpp>`) — these are the only exception.

## Core code guidelines

- Stick to good code practices:
  - Avoid complex classes / functions, prefer SRP.
  - Use the Command pattern for all render and input operations.
  - Use the Strategy pattern for layout algorithms.
  - Use the State pattern for widget visual states.
  - Watch out for typical bad practices: feature envy, LSP, etc.
  - For obtaining singletons, use a `UP<CClass>& myClass();` pattern inside
    a namespace. This can be implemented in source as making and returning a
    static ptr.
- Do not, under any circumstance:
  - `using namespace std;`
  - leave uninitialized primitives (int, float, etc)
  - call Hyprland APIs directly from widgets/ or core/ — use platform/ adapters
  - throw exceptions across C boundaries — wrap in CCrashIsolation::guard()
- Avoid, unless absolutely necessary:
  - the C standard library. Use the C++ STL.
  - `malloc` / `free` / etc
  - C-style pointers. Use SP<> WP<> and UP<> from hyprutils. These are Shared,
    Weak and Unique pointers respectively. C-style pointers may be used in
    select scenarios (e.g. destroying fns, where it's impossible to make a
    mistake) but everywhere else must not be used unless necessary.
  - C-style casts. Use rc<>, sc<>, or cc<> from hyprutils. These are shorthands
    to equivalent C++ casts.
- Avoid:
  - violating clang-tidy (`.clang-tidy`)
  - manual C-style cleanup
  - allocations in per-frame hot paths — cache textures, reuse buffers
- Texture caching: all text and image textures must be cached and invalidated
  on content/style change, not re-created every frame.

## Testing

- Core/ classes (IWidget, layout strategies, state machine) should be unit-
  testable without a Hyprland instance or GL context.
- Render commands can be inspected without execution — tests should verify
  the correct commands are produced for a given widget tree.
- Input commands can be queued and inspected — tests should verify hit-testing
  and command dispatch logic.
