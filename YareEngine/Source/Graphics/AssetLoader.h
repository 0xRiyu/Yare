#ifndef YARE_ASSET_LOADER
#define YARE_ASSET_LOADER

#include "Components/Component.h"
#include <memory>
#include <string>

#include <unordered_map>

enum class AssetType {
    Model,
    Texture
};

class AssetLoader {
public:
    AssetLoader();
    std::shared_ptr<Component> loadAsset(AssetType type, const std::string& name);
    std::unordered_map<std::string, std::shared_ptr<Component>> assets;
};


#endif // YARE_ASSET_LOADER
