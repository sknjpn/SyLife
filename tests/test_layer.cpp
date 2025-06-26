#include "test_common.h"
#include "../src/Layer.h"

class LayerTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        layer_ = std::make_unique<Layer>();
    }
    
    std::unique_ptr<Layer> layer_;
};

TEST_F(LayerTest, DefaultConstruction) {
    EXPECT_FALSE(layer_->isVisible());
    EXPECT_DOUBLE_EQ(layer_->getOpacity(), 1.0);
}

TEST_F(LayerTest, VisibilityControl) {
    EXPECT_FALSE(layer_->isVisible());
    
    layer_->setVisible(true);
    EXPECT_TRUE(layer_->isVisible());
    
    layer_->setVisible(false);
    EXPECT_FALSE(layer_->isVisible());
}

TEST_F(LayerTest, OpacityControl) {
    double testOpacity = 0.75;
    layer_->setOpacity(testOpacity);
    EXPECT_DOUBLE_EQ(layer_->getOpacity(), testOpacity);
    
    // Test opacity clamping
    layer_->setOpacity(1.5);
    EXPECT_LE(layer_->getOpacity(), 1.0);
    
    layer_->setOpacity(-0.5);
    EXPECT_GE(layer_->getOpacity(), 0.0);
}