#include "test_common.h"
#include "../src/Shape.h"

class ShapeTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
    }
};

TEST_F(ShapeTest, BasicShapeOperations) {
    // Test basic shape functionality
    // This will depend on the actual Shape class implementation
    EXPECT_NO_THROW(Shape::CreateCircle(5.0));
    EXPECT_NO_THROW(Shape::CreateRectangle(10.0, 15.0));
}

TEST_F(ShapeTest, CollisionDetection) {
    auto circle1 = Shape::CreateCircle(5.0);
    auto circle2 = Shape::CreateCircle(3.0);
    
    // Test collision between overlapping circles
    EXPECT_TRUE(Shape::CheckCollision(circle1, Vec2(0, 0), circle2, Vec2(5, 0)));
    
    // Test no collision between distant circles
    EXPECT_FALSE(Shape::CheckCollision(circle1, Vec2(0, 0), circle2, Vec2(20, 0)));
}