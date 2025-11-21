# macros

Small preprocessor utilities used internally by nil/xalt.

Includes: `#include <nil/xalt/MACROS.h>`

## Utilities

- `NIL_XALT_EXPAND(x)` — Macro expansion helper.
- `NIL_XALT_IDENTITY(x)` — Identity macro.
- `NIL_XALT_NARG(...)` — Counts the number of arguments (0–10 supported).
- `NIL_XALT_CONCAT(a, b)` — Token concatenation helper.
- `NIL_XALT_APPLY_N(MACRO, ...)` — Apply a 1-ary macro to each argument and comma-join the results.
  - Implemented for N in 1..10: `NIL_XALT_APPLY_1` … `NIL_XALT_APPLY_10`.
- `NIL_XALT_REPEAT_N(x)` — Repeat the same token `x` N times, comma-separated.
  - Implemented for N in 1..10: `NIL_XALT_REPEAT_1` … `NIL_XALT_REPEAT_10`.

Example:

```c
#include <nil/xalt/MACROS.h>

#define WRAP(x) [x]

// Expands to: [a], [b], [c]
NIL_XALT_APPLY_3(WRAP, a, b, c)

// Expands to: x, x, x, x
NIL_XALT_REPEAT_4(x)
```

Notes:
- These are low-level helpers; prefer normal C++ facilities where possible.
- The supported arities are limited by the provided sequence (1..10).
