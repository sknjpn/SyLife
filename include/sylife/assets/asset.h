#pragma once

#include "sylife/core/common.h"
#include "sylife/core/object.h"

namespace sylife::assets {

/**
 * @brief Base class for all assets in SyLife
 * 
 * Assets are template objects that define the properties and behavior
 * of entities in the simulation. They are immutable templates that
 * can be instantiated as states during runtime.
 */
class Asset : public core::Object {
public:
    /**
     * @brief Constructor
     */
    Asset();

    /**
     * @brief Destructor
     */
    ~Asset() override = default;

    /**
     * @brief Set asset name
     * @param name Asset name
     */
    void setName(const s3d::String& name);

    /**
     * @brief Get asset name
     * @return Asset name
     */
    const s3d::String& getName() const;

    /**
     * @brief Set Japanese name
     * @param nameJP Japanese name
     */
    void setNameJP(const s3d::String& nameJP);

    /**
     * @brief Get Japanese name
     * @return Japanese name
     */
    const s3d::String& getNameJP() const;

    /**
     * @brief Set file path
     * @param filePath File path where asset is stored
     */
    void setFilePath(const s3d::FilePath& filePath);

    /**
     * @brief Get file path
     * @return File path
     */
    const s3d::FilePath& getFilePath() const;

    /**
     * @brief Set whether this is a user-created asset
     * @param isUserAsset true if user-created
     */
    void setIsUserAsset(bool isUserAsset);

    /**
     * @brief Check if this is a user-created asset
     * @return true if user-created
     */
    bool getIsUserAsset() const;

    /**
     * @brief Set asset icon
     * @param icon Icon texture
     */
    void setAssetIcon(const s3d::Texture& icon);

    /**
     * @brief Get asset icon
     * @return Icon texture
     */
    const s3d::Texture& getAssetIcon() const;

    /**
     * @brief Set description
     * @param description Asset description
     */
    void setDescription(const s3d::String& description);

    /**
     * @brief Get description
     * @return Asset description
     */
    const s3d::String& getDescription() const;

    /**
     * @brief Set creation timestamp
     * @param timestamp Creation timestamp
     */
    void setCreationTime(TimeStamp timestamp);

    /**
     * @brief Get creation timestamp
     * @return Creation timestamp
     */
    TimeStamp getCreationTime() const;

    /**
     * @brief Set modification timestamp
     * @param timestamp Modification timestamp
     */
    void setModificationTime(TimeStamp timestamp);

    /**
     * @brief Get modification timestamp
     * @return Modification timestamp
     */
    TimeStamp getModificationTime() const;

    /**
     * @brief Mark asset as modified
     */
    void markAsModified();

    /**
     * @brief Get asset version
     * @return Version number
     */
    int getVersion() const;

    /**
     * @brief Validate asset data
     * @return true if asset is valid
     */
    virtual bool validate() const;

    /**
     * @brief Get validation errors
     * @return Vector of validation error messages
     */
    virtual std::vector<s3d::String> getValidationErrors() const;

    /**
     * @brief Create a runtime state from this asset
     * @return Unique pointer to state object
     */
    virtual std::unique_ptr<core::Object> createState() const = 0;

    /**
     * @brief Get asset dependencies
     * @return Vector of asset IDs this asset depends on
     */
    virtual std::vector<UniqueId> getDependencies() const;

    /**
     * @brief Check if asset can be safely deleted
     * @return true if asset can be deleted
     */
    virtual bool canDelete() const;

    // Object interface
    void save(s3d::JSON& json) const override;
    void load(const s3d::JSON& json) override;
    bool isValid() const override;

protected:
    /**
     * @brief Increment version number
     */
    void incrementVersion();

    /**
     * @brief Validate asset-specific data
     * @return true if asset-specific data is valid
     */
    virtual bool validateAssetData() const { return true; }

    /**
     * @brief Get asset-specific validation errors
     * @return Vector of asset-specific validation error messages
     */
    virtual std::vector<s3d::String> getAssetValidationErrors() const { return {}; }

private:
    // Basic properties
    s3d::String name_;
    s3d::String nameJP_;
    s3d::FilePath filePath_;
    s3d::String description_;
    bool isUserAsset_;
    s3d::Texture assetIcon_;

    // Metadata
    TimeStamp creationTime_;
    TimeStamp modificationTime_;
    int version_;

    // Validation cache
    mutable bool validationCached_;
    mutable bool isValidCached_;
    mutable std::vector<s3d::String> validationErrorsCached_;
};

/**
 * @brief Asset validation result
 */
struct AssetValidationResult {
    bool isValid;
    std::vector<s3d::String> errors;
    std::vector<s3d::String> warnings;
    
    AssetValidationResult() : isValid(true) {}
    
    void addError(const s3d::String& error) {
        errors.push_back(error);
        isValid = false;
    }
    
    void addWarning(const s3d::String& warning) {
        warnings.push_back(warning);
    }
    
    bool hasIssues() const {
        return !errors.empty() || !warnings.empty();
    }
};

/**
 * @brief Asset dependency graph for validation and management
 */
class AssetDependencyGraph {
public:
    /**
     * @brief Add asset to graph
     * @param asset Asset to add
     */
    void addAsset(std::shared_ptr<Asset> asset);

    /**
     * @brief Remove asset from graph
     * @param assetId Asset ID to remove
     */
    void removeAsset(UniqueId assetId);

    /**
     * @brief Check for circular dependencies
     * @return Vector of asset IDs in circular dependency chains
     */
    std::vector<UniqueId> findCircularDependencies() const;

    /**
     * @brief Get assets that depend on given asset
     * @param assetId Asset ID
     * @return Vector of dependent asset IDs
     */
    std::vector<UniqueId> getDependents(UniqueId assetId) const;

    /**
     * @brief Get topological sort of assets
     * @return Vector of asset IDs in dependency order
     */
    std::vector<UniqueId> getTopologicalSort() const;

    /**
     * @brief Validate dependency graph
     * @return true if graph is valid (no circular dependencies)
     */
    bool isValid() const;

private:
    std::unordered_map<UniqueId, std::vector<UniqueId>> dependencies_;
    std::unordered_map<UniqueId, std::vector<UniqueId>> dependents_;
};

} // namespace sylife::assets