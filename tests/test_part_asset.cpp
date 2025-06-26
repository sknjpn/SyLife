#include "test_common.h"
#include "../src/PartAsset.h"
#include "../src/PartConfig.h"

class PartAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        partAsset_ = std::make_shared<PartAsset>();
    }
    
    std::shared_ptr<PartAsset> partAsset_;
};

TEST_F(PartAssetTest, DefaultConstruction) {
    EXPECT_EQ(partAsset_->getTypeName(), U"PartAsset");
    EXPECT_TRUE(partAsset_->getName().empty());
}

TEST_F(PartAssetTest, BasicProperties) {
    partAsset_->setName(U"TestPart");
    EXPECT_EQ(partAsset_->getName(), U"TestPart");
    
    // Test part-specific properties
    EXPECT_NO_THROW(partAsset_->getPartConfig());
    EXPECT_NO_THROW(partAsset_->getColor());
}

TEST_F(PartAssetTest, JSONSerialization) {
    partAsset_->setName(U"SerializablePart");
    partAsset_->setNameJP(U"シリアライズ可能パーツ");
    
    JSON saveJson;
    partAsset_->save(saveJson);
    
    auto newPartAsset = std::make_shared<PartAsset>();
    newPartAsset->load(saveJson);
    
    EXPECT_EQ(newPartAsset->getName(), partAsset_->getName());
    EXPECT_EQ(newPartAsset->getNameJP(), partAsset_->getNameJP());
}