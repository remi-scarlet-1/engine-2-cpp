#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "CommonAssets.h"

namespace engine {
	class AssetLoader {
	public:
		template <typename T>
		static std::shared_ptr<T> loadFromFile(const std::filesystem::path& path) {
			throw std::runtime_error("Asset type not supported for loading.");
		}

		// Declare specializations
		template<> static std::shared_ptr<assets::Program> loadFromFile(const std::filesystem::path& path);
		template<> static std::shared_ptr<assets::Mesh> loadFromFile(const std::filesystem::path& path);
	};

	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		template <typename T> 
		std::shared_ptr<T> loadAsset(const std::filesystem::path& path) {
			// Check if asset is already loaded
			if (loadedAssets.find(path) != loadedAssets.end())
				return std::static_pointer_cast<T>(loadedAssets[path]);

			// Load, cache & return asset
			std::shared_ptr<T> asset = engine::AssetLoader::loadFromFile<T>(path);
			loadedAssets[path] = asset;
			return asset;
		}

	private:
		std::unordered_map<std::filesystem::path, std::shared_ptr<void>> loadedAssets;
	};
}