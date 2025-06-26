#pragma once

#include "sylife/core/common.h"
#include "sylife/core/object.h"
#include "asset.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <type_traits>

namespace sylife::assets {

/**
 * @brief Central manager for all assets in the world
 */
class AssetManager : public core::Object {
public:
    /**
     * @brief Constructor
     */
    AssetManager();

    /**
     * @brief Destructor
     */
    ~AssetManager() override;

    /**
     * @brief Create a new asset of specified type
     * @tparam T Asset type (must inherit from Asset)
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Shared pointer to created asset
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> create(Args&&... args) {
        static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
        
        auto asset = std::make_shared<T>(std::forward<Args>(args)...);
        registerAsset(asset);
        return asset;
    }

    /**
     * @brief Create asset from type name
     * @param typeName Type name of asset to create
     * @return Shared pointer to created asset
     */
    std::shared_ptr<Asset> createByTypeName(const s3d::String& typeName);

    /**
     * @brief Register an existing asset
     * @param asset Asset to register
     */
    void registerAsset(std::shared_ptr<Asset> asset);

    /**
     * @brief Unregister an asset
     * @param asset Asset to unregister
     */
    void unregisterAsset(std::shared_ptr<Asset> asset);

    /**
     * @brief Find asset by name
     * @param name Asset name
     * @return Shared pointer to asset, or nullptr if not found
     */
    std::shared_ptr<Asset> findByName(const s3d::String& name) const;

    /**
     * @brief Find asset by name with type checking
     * @tparam T Expected asset type
     * @param name Asset name
     * @return Shared pointer to asset of type T, or nullptr if not found
     */
    template<typename T>
    std::shared_ptr<T> findByName(const s3d::String& name) const {
        static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
        
        auto asset = findByName(name);
        return std::dynamic_pointer_cast<T>(asset);
    }

    /**
     * @brief Find asset by ID
     * @param id Asset ID
     * @return Shared pointer to asset, or nullptr if not found
     */
    std::shared_ptr<Asset> findById(UniqueId id) const;

    /**
     * @brief Find asset by ID with type checking
     * @tparam T Expected asset type
     * @param id Asset ID
     * @return Shared pointer to asset of type T, or nullptr if not found
     */
    template<typename T>
    std::shared_ptr<T> findById(UniqueId id) const {
        static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
        
        auto asset = findById(id);
        return std::dynamic_pointer_cast<T>(asset);
    }

    /**
     * @brief Get all assets of specified type
     * @tparam T Asset type
     * @return Vector of assets of type T
     */
    template<typename T>
    std::vector<std::shared_ptr<T>> getAllOfType() const {
        static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
        
        std::vector<std::shared_ptr<T>> result;
        
        for (const auto& [id, asset] : assets_) {
            if (auto typedAsset = std::dynamic_pointer_cast<T>(asset)) {
                result.push_back(typedAsset);
            }
        }
        
        return result;
    }

    /**
     * @brief Get all assets
     * @return Vector of all assets
     */
    std::vector<std::shared_ptr<Asset>> getAllAssets() const;

    /**
     * @brief Get number of assets
     * @return Number of registered assets
     */
    size_t getAssetCount() const;

    /**
     * @brief Clear all assets
     */
    void clear();

    /**
     * @brief Load assets from directory
     * @param directory Directory path
     * @return Number of assets loaded
     */
    size_t loadFromDirectory(const s3d::FilePath& directory);

    /**
     * @brief Save assets to directory
     * @param directory Directory path
     * @return Number of assets saved
     */
    size_t saveToDirectory(const s3d::FilePath& directory) const;

    /**
     * @brief Validate all assets
     * @return true if all assets are valid
     */
    bool validateAllAssets() const;

    /**
     * @brief Get assets with validation errors
     * @return Vector of assets with validation errors
     */
    std::vector<std::shared_ptr<Asset>> getInvalidAssets() const;

    // Object interface
    void save(s3d::JSON& json) const override;
    void load(const s3d::JSON& json) override;
    s3d::String getTypeName() const override;
    std::unique_ptr<core::Object> clone() const override;
    bool isValid() const override;

private:
    /**
     * @brief Generate unique name for asset
     * @param baseName Base name
     * @return Unique name
     */
    s3d::String generateUniqueName(const s3d::String& baseName) const;

    /**
     * @brief Register asset type factory
     * @tparam T Asset type
     */
    template<typename T>
    void registerAssetType() {
        static_assert(std::is_base_of_v<Asset, T>, "T must inherit from Asset");
        
        auto typeName = T().getTypeName();
        assetFactories_[typeName] = []() -> std::shared_ptr<Asset> {
            return std::make_shared<T>();
        };
    }

    /**
     * @brief Initialize built-in asset types
     */
    void initializeAssetTypes();

    // Asset storage
    std::unordered_map<UniqueId, std::shared_ptr<Asset>> assets_;
    std::unordered_map<s3d::String, std::shared_ptr<Asset>> assetsByName_;

    // Asset factories for dynamic creation
    std::unordered_map<s3d::String, std::function<std::shared_ptr<Asset>()>> assetFactories_;

    // Performance metrics
    mutable size_t accessCount_;
    mutable size_t cacheHits_;
    mutable size_t cacheMisses_;
};

/**
 * @brief Asset registration helper for automatic type registration
 */
template<typename T>
class AssetRegistrar {
public:
    AssetRegistrar() {
        // This will be called during static initialization
        // to register the asset type
    }
    
    static bool registerType(AssetManager& manager) {
        manager.registerAssetType<T>();
        return true;
    }
};

/**
 * @brief Macro for easy asset type registration
 */
#define SYLIFE_REGISTER_ASSET(AssetType) \
    namespace { \
        static const sylife::assets::AssetRegistrar<AssetType> \
            _asset_registrar_##AssetType; \
    }

} // namespace sylife::assets