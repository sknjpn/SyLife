#include "test_common.h"
#include "../src/CellAsset.h"
#include "../src/PartConfig.h"
#include "../src/Storage.h"

class CellAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        cellAsset_ = std::make_shared<CellAsset>();
    }
    
    std::shared_ptr<CellAsset> cellAsset_;
};

TEST_F(CellAssetTest, DefaultConstruction) {
    EXPECT_EQ(cellAsset_->getTypeName(), U"CellAsset");
    EXPECT_EQ(cellAsset_->getMass(), 0.0);
    EXPECT_EQ(cellAsset_->getRadius(), 0.0);
    EXPECT_EQ(cellAsset_->getInertia(), 0.0);
    EXPECT_EQ(cellAsset_->getDrawRadius(), 0.0);
    EXPECT_EQ(cellAsset_->getPartConfigs().size(), 0);
}

TEST_F(CellAssetTest, PhysicsProperties) {
    cellAsset_->setName(U"PhysicsTestCell");
    
    // Test mass setting and updates
    double testMass = 10.0;
    cellAsset_->setMass(testMass);
    EXPECT_DOUBLE_EQ(cellAsset_->getMass(), testMass);
    
    // Test inertia setting
    double testInertia = 5.0;
    cellAsset_->setInertia(testInertia);
    EXPECT_DOUBLE_EQ(cellAsset_->getInertia(), testInertia);
    
    // Test radius calculation from mass and inertia
    cellAsset_->updateRadius();
    double expectedRadius = sqrt(2 * testInertia / testMass);
    EXPECT_DOUBLE_EQ(cellAsset_->getRadius(), expectedRadius);
}

TEST_F(CellAssetTest, TimeProperties) {
    cellAsset_->setName(U"TimeTestCell");
    
    double lifespanTime = 300.0; // 5 minutes
    double bornTime = 10.0;      // 10 seconds to be born
    double yieldTime = 60.0;     // 1 minute to reproduce
    
    cellAsset_->setLifespanTime(lifespanTime);
    cellAsset_->setBornTime(bornTime);
    cellAsset_->setYieldTime(yieldTime);
    
    EXPECT_DOUBLE_EQ(cellAsset_->getLifespanTime(), lifespanTime);
    EXPECT_DOUBLE_EQ(cellAsset_->getBornTime(), bornTime);
    EXPECT_DOUBLE_EQ(cellAsset_->getYieldTime(), yieldTime);
}

TEST_F(CellAssetTest, HealthProperties) {
    cellAsset_->setName(U"HealthTestCell");
    
    double maxHitPoint = 100.0;
    cellAsset_->setMaxHitPoint(maxHitPoint);
    
    EXPECT_DOUBLE_EQ(cellAsset_->getMaxHitPoint(), maxHitPoint);
}

TEST_F(CellAssetTest, StorageManagement) {
    cellAsset_->setName(U"StorageTestCell");
    
    // Test material storage
    auto& material = cellAsset_->getMaterial();
    EXPECT_EQ(material.size(), 0);
    
    // Test max storage
    auto& maxStorage = cellAsset_->getMaxStorage();
    EXPECT_EQ(maxStorage.size(), 0);
    
    // Add test protein to storage
    // Note: This would require actual protein assets to be fully functional
    cellAsset_->updateMaxStorage();
    cellAsset_->updateMaterial();
    
    // Verify storage operations don't crash
    EXPECT_NO_THROW(cellAsset_->getMaterial());
    EXPECT_NO_THROW(cellAsset_->getMaxStorage());
}

TEST_F(CellAssetTest, PartConfigManagement) {
    cellAsset_->setName(U"PartConfigTestCell");
    
    // Test adding part configs
    auto partConfig = std::make_shared<PartConfig>();
    cellAsset_->addPartConfig(partConfig);
    
    EXPECT_EQ(cellAsset_->getPartConfigs().size(), 1);
    EXPECT_EQ(cellAsset_->getPartConfigs()[0], partConfig);
    
    // Test removing part configs
    cellAsset_->removePartConfig(partConfig);
    EXPECT_EQ(cellAsset_->getPartConfigs().size(), 0);
}

TEST_F(CellAssetTest, UpdateMethods) {
    cellAsset_->setName(U"UpdateTestCell");
    
    // Add some test part configs to make updates meaningful
    auto partConfig = std::make_shared<PartConfig>();
    cellAsset_->addPartConfig(partConfig);
    
    // Test all update methods don't crash
    EXPECT_NO_THROW(cellAsset_->updateMass());
    EXPECT_NO_THROW(cellAsset_->updateRadius());
    EXPECT_NO_THROW(cellAsset_->updateDrawRadius());
    EXPECT_NO_THROW(cellAsset_->updateInertia());
    EXPECT_NO_THROW(cellAsset_->updateMaxStorage());
    EXPECT_NO_THROW(cellAsset_->updateMaterial());
    
    // Verify mass is calculated from parts
    double mass = cellAsset_->getMass();
    EXPECT_GE(mass, 0.0);
}

TEST_F(CellAssetTest, PreRenderMethod) {
    cellAsset_->setName(U"RenderTestCell");
    
    // Test pre-render doesn't crash
    EXPECT_NO_THROW(cellAsset_->preRender());
    
    // Test texture access
    EXPECT_NO_THROW(cellAsset_->getCellAssetTexture());
    EXPECT_NO_THROW(cellAsset_->getCellStateTexture());
}

TEST_F(CellAssetTest, ViewerFlag) {
    cellAsset_->setName(U"ViewerFlagTestCell");
    
    // Test viewer flag
    EXPECT_FALSE(cellAsset_->m_isInViewer);
    
    cellAsset_->m_isInViewer = true;
    EXPECT_TRUE(cellAsset_->m_isInViewer);
}

TEST_F(CellAssetTest, JSONSerialization) {
    // Set up comprehensive test data
    cellAsset_->setName(U"SerializationCell");
    cellAsset_->setNameJP(U"シリアライゼーションセル");
    cellAsset_->setMass(15.0);
    cellAsset_->setInertia(7.5);
    cellAsset_->setLifespanTime(400.0);
    cellAsset_->setBornTime(12.0);
    cellAsset_->setYieldTime(80.0);
    cellAsset_->setMaxHitPoint(120.0);
    
    // Serialize to JSON
    JSON saveJson;
    cellAsset_->save(saveJson);
    
    // Verify JSON contains CellAsset-specific fields
    EXPECT_TRUE(saveJson.contains(U"mass"));
    EXPECT_TRUE(saveJson.contains(U"inertia"));
    EXPECT_TRUE(saveJson.contains(U"lifespanTime"));
    EXPECT_TRUE(saveJson.contains(U"bornTime"));
    EXPECT_TRUE(saveJson.contains(U"yieldTime"));
    EXPECT_TRUE(saveJson.contains(U"maxHitPoint"));
    
    // Create new cell asset and deserialize
    auto newCellAsset = std::make_shared<CellAsset>();
    newCellAsset->load(saveJson);
    
    // Verify deserialized data matches original
    EXPECT_EQ(newCellAsset->getName(), cellAsset_->getName());
    EXPECT_EQ(newCellAsset->getNameJP(), cellAsset_->getNameJP());
    EXPECT_DOUBLE_EQ(newCellAsset->getMass(), cellAsset_->getMass());
    EXPECT_DOUBLE_EQ(newCellAsset->getInertia(), cellAsset_->getInertia());
    EXPECT_DOUBLE_EQ(newCellAsset->getLifespanTime(), cellAsset_->getLifespanTime());
    EXPECT_DOUBLE_EQ(newCellAsset->getBornTime(), cellAsset_->getBornTime());
    EXPECT_DOUBLE_EQ(newCellAsset->getYieldTime(), cellAsset_->getYieldTime());
    EXPECT_DOUBLE_EQ(newCellAsset->getMaxHitPoint(), cellAsset_->getMaxHitPoint());
}

TEST_F(CellAssetTest, PhysicsConsistency) {
    cellAsset_->setName(U"PhysicsConsistencyCell");
    
    // Set initial values
    cellAsset_->setMass(20.0);
    cellAsset_->setInertia(10.0);
    
    // Test radius calculation consistency
    cellAsset_->updateRadius();
    double radius1 = cellAsset_->getRadius();
    
    cellAsset_->updateRadius();
    double radius2 = cellAsset_->getRadius();
    
    EXPECT_DOUBLE_EQ(radius1, radius2); // Should be consistent
    
    // Test relationship between mass, inertia, and radius
    double mass = cellAsset_->getMass();
    double inertia = cellAsset_->getInertia();
    double radius = cellAsset_->getRadius();
    
    // For a disk: I = 0.5 * m * r^2, so r = sqrt(2*I/m)
    double expectedRadius = sqrt(2 * inertia / mass);
    EXPECT_NEAR(radius, expectedRadius, 1e-10);
}

TEST_F(CellAssetTest, EdgeCases) {
    cellAsset_->setName(U"EdgeCaseCell");
    
    // Test zero mass (should handle gracefully)
    cellAsset_->setMass(0.0);
    cellAsset_->setInertia(1.0);
    
    // This should not crash but might produce infinite/NaN values
    EXPECT_NO_THROW(cellAsset_->updateRadius());
    
    // Test negative values (should handle gracefully)
    cellAsset_->setMass(-1.0);
    EXPECT_NO_THROW(cellAsset_->updateRadius());
    
    // Test very small values
    cellAsset_->setMass(1e-10);
    cellAsset_->setInertia(1e-10);
    EXPECT_NO_THROW(cellAsset_->updateRadius());
}

TEST_F(CellAssetTest, PerformanceTest) {
    cellAsset_->setName(U"PerformanceCell");
    
    // Add many part configs for performance testing
    const int numParts = 100;
    for (int i = 0; i < numParts; ++i) {
        auto partConfig = std::make_shared<PartConfig>();
        cellAsset_->addPartConfig(partConfig);
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Perform multiple update cycles
    for (int i = 0; i < 1000; ++i) {
        cellAsset_->updateMass();
        cellAsset_->updateInertia();
        cellAsset_->updateRadius();
        cellAsset_->updateDrawRadius();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 100000); // Less than 100ms
}