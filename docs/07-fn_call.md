# fn_call

Attempts to invoke a callable with the best-matching subset of the provided arguments, without changing the call sites.

Includes: `#include <nil/xalt/fn_call.hpp>`

## nil::xalt::fn_call

Tries different argument combinations (dropping trailing arguments as needed) until a valid call is found. Uses lightweight casting probes to guide overload resolution, but forwards the original arguments as-is when calling.

Constraints:
- Supports up to 8 call arguments (callable + args must be fewer than 10 template parameters internally).
- If no valid call is found, it returns an internal sentinel type (compile-time detectable via substitution failure in generic contexts).

Example:

```cpp
#include <nil/xalt/fn_call.hpp>
#include <string>
#include <string_view>

using nil::xalt::fn_call;

void f(int, double);
void g(int, std::string_view);

int main() {
    // Drops the trailing argument "x" because f(int,double) matches earlier
    fn_call(f, 42, 3.14, "x");

    std::string s = "hello";
    // Accepts std::string for std::string_view (checked via probing, then forwards original argument)
    fn_call(g, 7, s);
}
```

### How it works (brief)
- Computes candidate subsets of the provided arguments using a bitmask scan.
- Checks invocability using `implicit_cast<T>` during compile-time probing.
- Calls the target with the original arguments in their natural order once a match is found.

## Dependencies

- [fn_sign](./06-fn_sign.md)
- [cast](./12-cast.md)
- [tlist](./03-tlist.md)
