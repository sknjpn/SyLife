#include "test_common.h"
#include "../src/World.h"
#include "../src/CellAsset.h"
#include "../src/CellState.h"
#include "../src/EggState.h"

class WorldTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        // Create a test world instance
        World::Make();
        world_ = World::GetInstance();
    }
    
    void TearDown() override {
        // Clean up world instance
        world_ = nullptr;
        SyLifeTestBase::TearDown();
    }
    
    World* world_;
};

TEST_F(WorldTest, SingletonInstance) {
    ASSERT_NE(world_, nullptr);
    EXPECT_EQ(world_, World::GetInstance());
}

TEST_F(WorldTest, InitialState) {
    EXPECT_EQ(world_->getCellStates().size(), 0);
    EXPECT_EQ(world_->getEggStates().size(), 0);
    EXPECT_GT(world_->getAssets().size(), 0); // Should have default assets
}

TEST_F(WorldTest, TileManagement) {
    // Test tile size setting
    Point testSize(10, 10);
    world_->setTileSize(testSize);
    
    // Verify tiles are properly initialized
    EXPECT_EQ(world_->getTiles().size(), testSize);
    
    // Test tile state access
    for (int32 x = 0; x < testSize.x; ++x) {
        for (int32 y = 0; y < testSize.y; ++y) {
            Point pos(x, y);
            EXPECT_NO_THROW(world_->getTile(pos));
        }
    }
}

TEST_F(WorldTest, CellStateManagement) {
    // Create test cell asset
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"TestCell");
    
    // Add cell state
    Vec2 position(100.0, 100.0);
    auto cellState = world_->addCellState(cellAsset, position);
    
    ASSERT_NE(cellState, nullptr);
    EXPECT_EQ(world_->getCellStates().size(), 1);
    EXPECT_EQ(cellState->getPosition(), position);
    EXPECT_EQ(cellState->getCellAsset(), cellAsset);
}

TEST_F(WorldTest, EggStateManagement) {
    // Create test cell asset for egg
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"TestEggCell");
    
    // Add egg state
    Vec2 position(50.0, 75.0);
    auto eggState = world_->addEggState(cellAsset, position);
    
    ASSERT_NE(eggState, nullptr);
    EXPECT_EQ(world_->getEggStates().size(), 1);
    EXPECT_EQ(eggState->getPosition(), position);
}

TEST_F(WorldTest, AssetManagement) {
    // Test asset creation
    auto cellAsset = world_->makeAsset<CellAsset>();
    ASSERT_NE(cellAsset, nullptr);
    
    // Test asset naming and retrieval
    String testName = U"UniqueTestCell";
    cellAsset->setName(testName);
    
    auto retrievedAsset = world_->getAsset<CellAsset>(testName);
    EXPECT_EQ(retrievedAsset, cellAsset);
    EXPECT_EQ(retrievedAsset->getName(), testName);
}

TEST_F(WorldTest, AssetRetrievalError) {
    // Test error handling for non-existent asset
    EXPECT_THROW(world_->getAsset<CellAsset>(U"NonExistentAsset"), Error);
}

TEST_F(WorldTest, SimulationStep) {
    // Add some test entities
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"SimTestCell");
    
    world_->addCellState(cellAsset, Vec2(100.0, 100.0));
    world_->addEggState(cellAsset, Vec2(200.0, 200.0));
    
    // Perform simulation step
    EXPECT_NO_THROW(world_->updatePhysics());
    EXPECT_NO_THROW(world_->updateLogic());
    
    // Verify entities still exist after update
    EXPECT_EQ(world_->getCellStates().size(), 1);
    EXPECT_EQ(world_->getEggStates().size(), 1);
}

TEST_F(WorldTest, KDTreeFunctionality) {
    // Create multiple cell states for spatial queries
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"KDTestCell");
    
    std::vector<Vec2> positions = {
        Vec2(0.0, 0.0),
        Vec2(100.0, 0.0),
        Vec2(0.0, 100.0),
        Vec2(100.0, 100.0),
        Vec2(50.0, 50.0)
    };
    
    for (const auto& pos : positions) {
        world_->addCellState(cellAsset, pos);
    }
    
    // Test spatial query
    Vec2 queryPoint(50.0, 50.0);
    double searchRadius = 75.0;
    
    auto nearbyStates = world_->getCellStatesInRadius(queryPoint, searchRadius);
    
    // Should find at least the center cell and possibly others
    EXPECT_GT(nearbyStates.size(), 0);
    
    // Verify all returned states are within radius
    for (const auto& state : nearbyStates) {
        double distance = queryPoint.distanceFrom(state->getPosition());
        EXPECT_LE(distance, searchRadius);
    }
}

TEST_F(WorldTest, SaveAndLoad) {
    // Create test world state
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"SaveTestCell");
    
    world_->setName(U"TestWorld");
    world_->addCellState(cellAsset, Vec2(123.0, 456.0));
    
    // Save world
    String testPath = U"test_world_save";
    EXPECT_NO_THROW(world_->save(testPath));
    
    // Create new world and load
    World::Make();
    auto newWorld = World::GetInstance();
    
    EXPECT_NO_THROW(World::Load(testPath));
    
    // Verify loaded state
    newWorld = World::GetInstance();
    EXPECT_EQ(newWorld->getName(), U"TestWorld");
    EXPECT_EQ(newWorld->getCellStates().size(), 1);
    
    // Clean up test file
    FileSystem::RemoveContents(testPath);
    FileSystem::Remove(testPath);
}

TEST_F(WorldTest, PerformanceStressTest) {
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"StressTestCell");
    
    // Add many entities to test performance
    const int entityCount = 100;
    for (int i = 0; i < entityCount; ++i) {
        Vec2 randomPos(Random(0.0, 1000.0), Random(0.0, 1000.0));
        world_->addCellState(cellAsset, randomPos);
    }
    
    EXPECT_EQ(world_->getCellStates().size(), entityCount);
    
    // Measure simulation performance
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10; ++i) {
        world_->updatePhysics();
        world_->updateLogic();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time (adjust threshold as needed)
    EXPECT_LT(duration.count(), 1000); // Less than 1 second for 10 steps
}