#ifndef DEBUG_HXX
#define DEBUG_HXX

// Windows/MSVC debug macro.
#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifdef DEBUG
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace debug {
  namespace fg {
    constexpr const char* black{"\x1b[90m"};
    constexpr const char* red{"\x1b[91m"};
    constexpr const char* green{"\x1b[92m"};
    constexpr const char* yellow{"\x1b[93m"};
    constexpr const char* blue{"\x1b[94m"};
    constexpr const char* magenta{"\x1b[95m"};
    constexpr const char* cyan{"\x1b[96m"};
    constexpr const char* white{"\x1b[97m"};
    constexpr const char* def{"\x1b[99m"};
  }
  constexpr const char* reset{"\x1b[0m"};
}

#define DEBUG_LOG(x) do { cout << x; } while (false);
#define DEBUG_LINE(x) do { cout << x << endl; } while (false);
#define DEBUG_NEWLINE() do { cout << endl; } while (false);
#else
#define DEBUG_LOG(x) // no-op
#define DEBUG_LINE(x) // no-op
#define DEBUG_NEWLINE() // no-op
#endif // DEBUG

#endif // DEBUG_HXX
