#include "AssetManager.h"

namespace Core {

    AssetManager::~AssetManager() {
        Clear();
    }

    void AssetManager::LoadTexture(const std::string& name, const std::string& filepath) {
        if (m_Textures.find(name) == m_Textures.end()) {
            Texture2D tex = ::LoadTexture(filepath.c_str());

            if (tex.id == 0) {
                spdlog::error("AssetManager: Failed to load texture at '{}'", filepath);
            }
            else {
                m_Textures[name] = tex;
                spdlog::info("AssetManager: Loaded texture '{}' ({}x{})", name, tex.width, tex.height);
            }
        }
        else {
            spdlog::warn("AssetManager: Texture '{}' is already loaded.", name);
        }
    }

    Texture2D AssetManager::GetTexture(const std::string& name) {
        auto it = m_Textures.find(name);

        if (it != m_Textures.end()) {
            return it->second;
        }

        spdlog::error("AssetManager: Texture '{}' not found!", name);
        return Texture2D{ 0 };
    }

    void AssetManager::Clear() {
        if (m_Textures.empty()) return;

        spdlog::info("AssetManager: Unloading all textures...");

        for (auto& pair : m_Textures) {
            ::UnloadTexture(pair.second);
        }

        m_Textures.clear();
    }
}