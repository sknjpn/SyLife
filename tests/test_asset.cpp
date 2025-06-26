#include "test_common.h"
#include "../src/Asset.h"
#include "../src/CellAsset.h"
#include "../src/PartAsset.h"
#include "../src/ProteinAsset.h"

class AssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        asset_ = std::make_shared<Asset>();
    }
    
    std::shared_ptr<Asset> asset_;
};

TEST_F(AssetTest, DefaultConstruction) {
    EXPECT_TRUE(asset_->getName().empty());
    EXPECT_TRUE(asset_->getNameJP().empty());
    EXPECT_TRUE(asset_->getFilePath().empty());
    EXPECT_TRUE(asset_->getIsUserAsset()); // Default should be true
    EXPECT_EQ(asset_->getTypeName(), U"Asset");
}

TEST_F(AssetTest, BasicPropertySettersAndGetters) {
    String testName = U"TestAsset";
    String testNameJP = U"テストアセット";
    String testFilePath = U"/test/path/asset.json";
    bool isUserAsset = false;
    
    asset_->setName(testName);
    asset_->setNameJP(testNameJP);
    asset_->setFilePath(testFilePath);
    asset_->setIsUserAsset(isUserAsset);
    
    EXPECT_EQ(asset_->getName(), testName);
    EXPECT_EQ(asset_->getNameJP(), testNameJP);
    EXPECT_EQ(asset_->getFilePath(), testFilePath);
    EXPECT_EQ(asset_->getIsUserAsset(), isUserAsset);
}

TEST_F(AssetTest, JSONSerialization) {
    // Set up test data
    asset_->setName(U"SerializationTest");
    asset_->setNameJP(U"シリアライゼーションテスト");
    asset_->setFilePath(U"/test/serialization.json");
    asset_->setIsUserAsset(true);
    
    // Serialize to JSON
    JSON saveJson;
    asset_->save(saveJson);
    
    // Verify JSON contains expected fields
    EXPECT_TRUE(saveJson.contains(U"name"));
    EXPECT_TRUE(saveJson.contains(U"nameJP"));
    EXPECT_TRUE(saveJson.contains(U"filepath"));
    EXPECT_TRUE(saveJson.contains(U"isUserAsset"));
    
    // Create new asset and deserialize
    auto newAsset = std::make_shared<Asset>();
    newAsset->load(saveJson);
    
    // Verify deserialized data matches original
    EXPECT_EQ(newAsset->getName(), asset_->getName());
    EXPECT_EQ(newAsset->getNameJP(), asset_->getNameJP());
    EXPECT_EQ(newAsset->getFilePath(), asset_->getFilePath());
    EXPECT_EQ(newAsset->getIsUserAsset(), asset_->getIsUserAsset());
}

TEST_F(AssetTest, JSONLoadPartialData) {
    // Test loading JSON with missing fields
    JSON partialJson;
    partialJson[U"name"] = U"PartialAsset";
    partialJson[U"isUserAsset"] = false;
    // Missing nameJP and filepath
    
    EXPECT_NO_THROW(asset_->load(partialJson));
    
    EXPECT_EQ(asset_->getName(), U"PartialAsset");
    EXPECT_EQ(asset_->getIsUserAsset(), false);
    // These should remain default/empty
    EXPECT_TRUE(asset_->getNameJP().empty());
    EXPECT_TRUE(asset_->getFilePath().empty());
}

TEST_F(AssetTest, JSONLoadInvalidData) {
    // Test loading JSON with invalid data types
    JSON invalidJson;
    invalidJson[U"name"] = 12345; // Should be string
    invalidJson[U"isUserAsset"] = U"not_a_boolean"; // Should be boolean
    
    // Should handle gracefully without crashing
    EXPECT_NO_THROW(asset_->load(invalidJson));
}

class CellAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        cellAsset_ = std::make_shared<CellAsset>();
    }
    
    std::shared_ptr<CellAsset> cellAsset_;
};

TEST_F(CellAssetTest, TypeName) {
    EXPECT_EQ(cellAsset_->getTypeName(), U"CellAsset");
}

TEST_F(CellAssetTest, CellSpecificProperties) {
    // Test cell-specific functionality
    cellAsset_->setName(U"TestCell");
    
    // Test part assembly
    // Note: This would require actual part assets to be meaningful
    EXPECT_NO_THROW(cellAsset_->getPartAssets());
    
    // Test initial state
    EXPECT_EQ(cellAsset_->getPartAssets().size(), 0);
}

class PartAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        partAsset_ = std::make_shared<PartAsset>();
    }
    
    std::shared_ptr<PartAsset> partAsset_;
};

TEST_F(PartAssetTest, TypeName) {
    EXPECT_EQ(partAsset_->getTypeName(), U"PartAsset");
}

TEST_F(PartAssetTest, PartSpecificProperties) {
    partAsset_->setName(U"TestPart");
    
    // Test part-specific functionality
    EXPECT_NO_THROW(partAsset_->getPartConfig());
    
    // Test color properties
    EXPECT_NO_THROW(partAsset_->getColor());
}

class ProteinAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        proteinAsset_ = std::make_shared<ProteinAsset>();
    }
    
    std::shared_ptr<ProteinAsset> proteinAsset_;
};

TEST_F(ProteinAssetTest, TypeName) {
    EXPECT_EQ(proteinAsset_->getTypeName(), U"ProteinAsset");
}

TEST_F(ProteinAssetTest, ProteinSpecificProperties) {
    proteinAsset_->setName(U"TestProtein");
    
    // Test protein-specific functionality
    EXPECT_NO_THROW(proteinAsset_->getStorage());
    
    // Test initial state
    EXPECT_EQ(proteinAsset_->getStorage().size(), 0);
}

// Integration test for asset hierarchy
TEST_F(AssetTest, AssetPolymorphism) {
    std::vector<std::shared_ptr<Asset>> assets = {
        std::make_shared<Asset>(),
        std::make_shared<CellAsset>(),
        std::make_shared<PartAsset>(),
        std::make_shared<ProteinAsset>()
    };
    
    // Test that all assets can be treated as base Asset
    for (size_t i = 0; i < assets.size(); ++i) {
        String testName = U"Asset" + ToString(i);
        assets[i]->setName(testName);
        EXPECT_EQ(assets[i]->getName(), testName);
        
        // Test virtual method calls
        EXPECT_FALSE(assets[i]->getTypeName().empty());
        
        // Test JSON serialization
        JSON json;
        EXPECT_NO_THROW(assets[i]->save(json));
        EXPECT_NO_THROW(assets[i]->load(json));
    }
}

// Performance test for asset operations
TEST_F(AssetTest, PerformanceTest) {
    const int numAssets = 1000;
    std::vector<std::shared_ptr<Asset>> assets;
    assets.reserve(numAssets);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Create many assets
    for (int i = 0; i < numAssets; ++i) {
        auto asset = std::make_shared<Asset>();
        asset->setName(U"Asset" + ToString(i));
        asset->setNameJP(U"アセット" + ToString(i));
        assets.push_back(asset);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    // Should be reasonably fast
    EXPECT_LT(duration.count(), 100000); // Less than 100ms
    
    // Test serialization performance
    startTime = std::chrono::high_resolution_clock::now();
    
    for (const auto& asset : assets) {
        JSON json;
        asset->save(json);
    }
    
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    EXPECT_LT(duration.count(), 500000); // Less than 500ms for serialization
}