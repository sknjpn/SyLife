#include "test_common.h"
#include "../src/TileState.h"

class TileStateTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        tileState_ = std::make_unique<TileState>();
    }
    
    std::unique_ptr<TileState> tileState_;
};

TEST_F(TileStateTest, DefaultConstruction) {
    EXPECT_DOUBLE_EQ(tileState_->getElement(), 0.0);
    EXPECT_DOUBLE_EQ(tileState_->getPoison(), 0.0);
}

TEST_F(TileStateTest, ElementManagement) {
    double testElement = 42.5;
    tileState_->setElement(testElement);
    EXPECT_DOUBLE_EQ(tileState_->getElement(), testElement);
    
    tileState_->addElement(17.3);
    EXPECT_DOUBLE_EQ(tileState_->getElement(), testElement + 17.3);
}

TEST_F(TileStateTest, PoisonManagement) {
    double testPoison = 15.7;
    tileState_->setPoison(testPoison);
    EXPECT_DOUBLE_EQ(tileState_->getPoison(), testPoison);
    
    tileState_->addPoison(8.2);
    EXPECT_DOUBLE_EQ(tileState_->getPoison(), testPoison + 8.2);
}