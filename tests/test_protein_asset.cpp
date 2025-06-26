#include "test_common.h"
#include "../src/ProteinAsset.h"

class ProteinAssetTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        proteinAsset_ = std::make_shared<ProteinAsset>();
    }
    
    std::shared_ptr<ProteinAsset> proteinAsset_;
};

TEST_F(ProteinAssetTest, DefaultConstruction) {
    EXPECT_EQ(proteinAsset_->getTypeName(), U"ProteinAsset");
    EXPECT_EQ(proteinAsset_->getStorage().size(), 0);
}

TEST_F(ProteinAssetTest, BasicProperties) {
    proteinAsset_->setName(U"TestProtein");
    EXPECT_EQ(proteinAsset_->getName(), U"TestProtein");
    
    EXPECT_NO_THROW(proteinAsset_->getStorage());
}

TEST_F(ProteinAssetTest, JSONSerialization) {
    proteinAsset_->setName(U"SerializableProtein");
    proteinAsset_->setNameJP(U"シリアライズ可能プロテイン");
    
    JSON saveJson;
    proteinAsset_->save(saveJson);
    
    auto newProteinAsset = std::make_shared<ProteinAsset>();
    newProteinAsset->load(saveJson);
    
    EXPECT_EQ(newProteinAsset->getName(), proteinAsset_->getName());
    EXPECT_EQ(newProteinAsset->getNameJP(), proteinAsset_->getNameJP());
}