#include "test_common.h"
#include "../src/EggState.h"
#include "../src/CellAsset.h"

class EggStateTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        
        cellAsset_ = std::make_shared<CellAsset>();
        cellAsset_->setName(U"TestEggCell");
        cellAsset_->setBornTime(10.0);
        
        Vec2 initialPosition(5.0, 15.0);
        eggState_ = std::make_unique<EggState>(cellAsset_, initialPosition);
    }
    
    std::shared_ptr<CellAsset> cellAsset_;
    std::unique_ptr<EggState> eggState_;
};

TEST_F(EggStateTest, Construction) {
    EXPECT_EQ(eggState_->getCellAsset(), cellAsset_);
    EXPECT_THAT(eggState_->getPosition(), Vec2Near(Vec2(5.0, 15.0)));
    EXPECT_FALSE(eggState_->isReadyToHatch());
}

TEST_F(EggStateTest, Development) {
    double bornTime = cellAsset_->getBornTime();
    
    // Should not be ready initially
    EXPECT_FALSE(eggState_->isReadyToHatch());
    
    // Update development
    eggState_->updateDevelopment(bornTime / 2.0);
    EXPECT_FALSE(eggState_->isReadyToHatch());
    
    // Complete development
    eggState_->updateDevelopment(bornTime);
    EXPECT_TRUE(eggState_->isReadyToHatch());
}