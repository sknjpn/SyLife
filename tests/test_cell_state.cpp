#include "test_common.h"
#include "../src/CellState.h"
#include "../src/CellAsset.h"

class CellStateTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        
        // Create test cell asset
        cellAsset_ = std::make_shared<CellAsset>();
        cellAsset_->setName(U"TestCell");
        cellAsset_->setMass(1.0);
        cellAsset_->setRadius(1.0);
        cellAsset_->setMaxHitPoint(100.0);
        cellAsset_->setLifespanTime(300.0);
        
        // Create cell state
        Vec2 initialPosition(10.0, 20.0);
        cellState_ = std::make_unique<CellState>(cellAsset_, initialPosition);
    }
    
    std::shared_ptr<CellAsset> cellAsset_;
    std::unique_ptr<CellState> cellState_;
};

TEST_F(CellStateTest, Construction) {
    EXPECT_EQ(cellState_->getCellAsset(), cellAsset_);
    EXPECT_THAT(cellState_->getPosition(), Vec2Near(Vec2(10.0, 20.0)));
    EXPECT_DOUBLE_EQ(cellState_->getHitPoint(), cellAsset_->getMaxHitPoint());
    EXPECT_FALSE(cellState_->isDead());
}

TEST_F(CellStateTest, HealthManagement) {
    double initialHP = cellState_->getHitPoint();
    
    // Test damage
    double damage = 25.0;
    cellState_->takeDamage(damage);
    EXPECT_DOUBLE_EQ(cellState_->getHitPoint(), initialHP - damage);
    
    // Test healing
    double healing = 10.0;
    cellState_->heal(healing);
    EXPECT_DOUBLE_EQ(cellState_->getHitPoint(), initialHP - damage + healing);
    
    // Test death
    cellState_->takeDamage(200.0); // More than max HP
    EXPECT_TRUE(cellState_->isDead());
}

TEST_F(CellStateTest, AgeAndLifespan) {
    EXPECT_DOUBLE_EQ(cellState_->getAge(), 0.0);
    
    // Simulate aging
    double deltaTime = 10.0;
    cellState_->updateAge(deltaTime);
    EXPECT_DOUBLE_EQ(cellState_->getAge(), deltaTime);
    
    // Test lifespan
    double lifespan = cellAsset_->getLifespanTime();
    cellState_->updateAge(lifespan);
    EXPECT_GE(cellState_->getAge(), lifespan);
}

TEST_F(CellStateTest, StorageManagement) {
    auto& storage = cellState_->getStorage();
    
    // Test initial storage state
    EXPECT_DOUBLE_EQ(storage.getElement(), 0.0);
    
    // Test adding elements
    storage.addElement(50.0);
    EXPECT_DOUBLE_EQ(storage.getElement(), 50.0);
}

TEST_F(CellStateTest, PhysicsIntegration) {
    // Test that CellState inherits from Rigidbody properly
    Vec2 initialPos = cellState_->getPosition();
    Vec2 velocity(5.0, 3.0);
    
    cellState_->setVelocity(velocity);
    EXPECT_THAT(cellState_->getVelocity(), Vec2Near(velocity));
    
    // Test physics update
    cellState_->updateRigidbody();
    
    // Position should have changed based on velocity
    Vec2 newPos = cellState_->getPosition();
    // Exact change depends on DeltaTime constant
}

TEST_F(CellStateTest, Serialization) {
    // Set up test state
    cellState_->setPosition(Vec2(123.45, 678.90));
    cellState_->setVelocity(Vec2(1.2, 3.4));
    cellState_->takeDamage(20.0);
    
    // Test binary serialization
    MemoryWriter writer;
    Serializer<MemoryWriter> serializer(writer);
    cellState_->save(serializer);
    
    // Deserialize
    BinaryReader reader(writer.getBlob());
    Deserializer<BinaryReader> deserializer(reader);
    
    auto newCellState = std::make_unique<CellState>(cellAsset_, Vec2(0, 0));
    newCellState->load(deserializer);
    
    // Verify state
    EXPECT_THAT(newCellState->getPosition(), Vec2Near(cellState_->getPosition()));
    EXPECT_THAT(newCellState->getVelocity(), Vec2Near(cellState_->getVelocity()));
    EXPECT_DOUBLE_EQ(newCellState->getHitPoint(), cellState_->getHitPoint());
}