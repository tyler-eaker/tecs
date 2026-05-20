#pragma once

#include <raylib.h>
#include <spdlog/spdlog.h>

#include <string>
#include <unordered_map>

namespace Core {

    class AssetManager {
    public:
        static AssetManager& Get() {
            static AssetManager instance;
            return instance;
        }

        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        void LoadTexture(const std::string& name, const std::string& filepath);
        Texture2D GetTexture(const std::string& name);
        void Clear();

    private:
        AssetManager() = default;
        ~AssetManager();

        std::unordered_map<std::string, Texture2D> m_Textures;
    };
}