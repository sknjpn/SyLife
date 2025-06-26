#include "test_common.h"
#include "../src/PartState.h"
#include "../src/PartAsset.h"

class PartStateTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        
        partAsset_ = std::make_shared<PartAsset>();
        partAsset_->setName(U"TestPart");
        
        partState_ = std::make_unique<PartState>(partAsset_);
    }
    
    std::shared_ptr<PartAsset> partAsset_;
    std::unique_ptr<PartState> partState_;
};

TEST_F(PartStateTest, Construction) {
    EXPECT_EQ(partState_->getPartAsset(), partAsset_);
    EXPECT_FALSE(partState_->isActive());
}

TEST_F(PartStateTest, ActivationState) {
    EXPECT_FALSE(partState_->isActive());
    
    partState_->setActive(true);
    EXPECT_TRUE(partState_->isActive());
    
    partState_->setActive(false);
    EXPECT_FALSE(partState_->isActive());
}