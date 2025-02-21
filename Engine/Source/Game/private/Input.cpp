#include <utility>

#include <windows.h>
#include <WindowsX.h>
#include <WinUser.h>
#include <wrl.h>

#include <ini.h>
#include <Input.h>

namespace GameEngine {

struct IniKey {
    std::string section, key;

    bool operator==(const IniKey&) const = default;
};
}

template<>
struct std::hash<GameEngine::IniKey> {
    std::size_t operator() (const GameEngine::IniKey& s) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(s.section);
        std::size_t h2 = std::hash<std::string>{}(s.key);
        return h1 ^ (h2 << 1);
    }
};

namespace GameEngine {

static int iniHandler (void* user, const char* section, const char* name, const char* value) {
    auto config = reinterpret_cast<std::unordered_map<IniKey, std::string>*>(user);
    IniKey key = {section, name};
    (*config)[key] = value;

    return 1;
}

template <typename T> 
static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void Input::Init (const char* configPath) {
    std::unordered_map<IniKey, std::string> config;

    if (ini_parse(configPath, iniHandler, &config) < 0) {
        std::string error = std::string("Can't find ") + configPath;
        MessageBox(0, std::wstring(error.begin(), error.end()).c_str(), 0, 0);
        return;
    }

    for (auto& [iniKey, keyName] : config) {
        if (iniKey.section == "") continue;

        auto it = VIRTUAL_KEYCODES.find(keyName);

        if (it == VIRTUAL_KEYCODES.end()) {
            std::string error = std::string("Undefined keyname \"") + keyName + "\"";
            MessageBox(0, std::wstring(error.begin(), error.end()).c_str(), 0, 0);
            continue;
        }

        if (iniKey.key == "min") {
            aces[iniKey.section].minKey = it->second;
        } else 
        if (iniKey.key == "max") {
            aces[iniKey.section].maxKey = it->second;
        }
    }

    for (const auto& [axis, descriptor] : aces) {
        if (descriptor.minKey == -1) {
            std::string error = std::string("Undefined (min) at axis \"") + axis + "\"";
            MessageBox(0, std::wstring(error.begin(), error.end()).c_str(), 0, 0);
        }

        if (descriptor.maxKey == -1) {
            std::string error = std::string("Undefined (max) at axis \"") + axis + "\"";
            MessageBox(0, std::wstring(error.begin(), error.end()).c_str(), 0, 0);
        }
    }
}

void Input::Update (float dt) {
    for (auto& axis : aces) {
        int target = !!(GetAsyncKeyState(axis.second.maxKey) & 0x8000) 
                   - !!(GetAsyncKeyState(axis.second.minKey) & 0x8000);

        axis.second.val += sgn(target - axis.second.val) * axis.second.sencetivity * dt;

        axis.second.val /= std::max<float>(1, std::abs(axis.second.val));
    }
}

float Input::GetAxis (const std::string& iniKey) const {
    return aces.find(iniKey)->second.val;
}
}