#include "test_common.h"
#include "../src/World.h"
#include "../src/CellAsset.h"
#include "../src/CellState.h"
#include "../src/PartAsset.h"
#include "../src/ProteinAsset.h"

class SimulationIntegrationTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        
        // Create test world
        World::Make();
        world_ = World::GetInstance();
        
        // Set up basic world
        world_->setTileSize(Point(50, 50));
        world_->setName(U"IntegrationTestWorld");
    }
    
    void TearDown() override {
        world_ = nullptr;
        SyLifeTestBase::TearDown();
    }
    
    World* world_;
};

TEST_F(SimulationIntegrationTest, BasicWorldSetup) {
    ASSERT_NE(world_, nullptr);
    EXPECT_EQ(world_->getName(), U"IntegrationTestWorld");
    EXPECT_EQ(world_->getTiles().size(), Point(50, 50));
}

TEST_F(SimulationIntegrationTest, CellLifecycle) {
    // Create cell asset with all necessary components
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"IntegrationTestCell");
    cellAsset->setMass(2.0);
    cellAsset->setRadius(1.5);
    cellAsset->setMaxHitPoint(100.0);
    cellAsset->setLifespanTime(300.0);
    cellAsset->setBornTime(10.0);
    cellAsset->setYieldTime(60.0);
    
    // Add cell to world
    Vec2 initialPosition(100.0, 150.0);
    auto cellState = world_->addCellState(cellAsset, initialPosition);
    
    ASSERT_NE(cellState, nullptr);
    EXPECT_EQ(world_->getCellStates().size(), 1);
    
    // Simulate cell lifecycle
    for (int i = 0; i < 100; ++i) {
        world_->updatePhysics();
        world_->updateLogic();
        
        // Check if cell is still alive
        if (cellState->isDead()) {
            break;
        }
    }
    
    // Cell should still be alive after 100 updates
    EXPECT_FALSE(cellState->isDead());
}

TEST_F(SimulationIntegrationTest, MultiCellInteraction) {
    // Create multiple cell assets
    auto cellAsset1 = world_->makeAsset<CellAsset>();
    cellAsset1->setName(U"TestCell1");
    cellAsset1->setMass(1.0);
    cellAsset1->setRadius(1.0);
    cellAsset1->setMaxHitPoint(100.0);
    
    auto cellAsset2 = world_->makeAsset<CellAsset>();
    cellAsset2->setName(U"TestCell2");
    cellAsset2->setMass(1.5);
    cellAsset2->setRadius(1.2);
    cellAsset2->setMaxHitPoint(120.0);
    
    // Add cells close to each other
    auto cell1 = world_->addCellState(cellAsset1, Vec2(100.0, 100.0));
    auto cell2 = world_->addCellState(cellAsset2, Vec2(105.0, 100.0));
    
    EXPECT_EQ(world_->getCellStates().size(), 2);
    
    // Simulate interaction
    for (int i = 0; i < 50; ++i) {
        world_->updatePhysics();
        world_->updateLogic();
    }
    
    // Both cells should still exist
    EXPECT_EQ(world_->getCellStates().size(), 2);
    EXPECT_FALSE(cell1->isDead());
    EXPECT_FALSE(cell2->isDead());
}

TEST_F(SimulationIntegrationTest, EggHatchingProcess) {
    // Create cell asset for egg
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"HatchingTestCell");
    cellAsset->setBornTime(5.0); // Short born time for testing
    cellAsset->setMass(1.0);
    cellAsset->setRadius(1.0);
    cellAsset->setMaxHitPoint(100.0);
    
    // Add egg to world
    Vec2 eggPosition(200.0, 200.0);
    auto eggState = world_->addEggState(cellAsset, eggPosition);
    
    ASSERT_NE(eggState, nullptr);
    EXPECT_EQ(world_->getEggStates().size(), 1);
    EXPECT_EQ(world_->getCellStates().size(), 0);
    
    // Simulate until egg should hatch
    for (int i = 0; i < 20; ++i) {
        world_->updateLogic();
        
        // Check if egg has hatched
        if (world_->getCellStates().size() > 0) {
            break;
        }
    }
    
    // Should have hatched into a cell
    EXPECT_GT(world_->getCellStates().size(), 0);
}

TEST_F(SimulationIntegrationTest, ResourceManagement) {
    // Create cell with resource requirements
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"ResourceTestCell");
    cellAsset->setMass(1.0);
    cellAsset->setRadius(1.0);
    cellAsset->setMaxHitPoint(100.0);
    
    // Create protein asset
    auto proteinAsset = world_->makeAsset<ProteinAsset>();
    proteinAsset->setName(U"TestProtein");
    
    // Add cell to world
    auto cellState = world_->addCellState(cellAsset, Vec2(150.0, 150.0));
    
    // Add resources to cell storage
    auto& storage = cellState->getStorage();
    storage.addElement(100.0);
    storage.addProtein(proteinAsset, 10);
    
    EXPECT_DOUBLE_EQ(storage.getElement(), 100.0);
    EXPECT_EQ(storage.numProtein(proteinAsset), 10);
    
    // Simulate resource consumption
    for (int i = 0; i < 10; ++i) {
        world_->updateLogic();
        
        // Resource levels might change during simulation
        double currentElement = storage.getElement();
        int currentProtein = storage.numProtein(proteinAsset);
        
        EXPECT_GE(currentElement, 0.0);
        EXPECT_GE(currentProtein, 0);
    }
}

TEST_F(SimulationIntegrationTest, SpatialPartitioning) {
    // Test KD-tree functionality with many cells
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"SpatialTestCell");
    cellAsset->setMass(1.0);
    cellAsset->setRadius(1.0);
    cellAsset->setMaxHitPoint(100.0);
    
    // Add cells in a grid pattern
    const int gridSize = 10;
    for (int x = 0; x < gridSize; ++x) {
        for (int y = 0; y < gridSize; ++y) {
            Vec2 position(x * 20.0, y * 20.0);
            world_->addCellState(cellAsset, position);
        }
    }
    
    EXPECT_EQ(world_->getCellStates().size(), gridSize * gridSize);
    
    // Test spatial queries
    Vec2 queryPoint(50.0, 50.0);
    double searchRadius = 30.0;
    
    auto nearbyStates = world_->getCellStatesInRadius(queryPoint, searchRadius);
    
    // Should find cells within radius
    EXPECT_GT(nearbyStates.size(), 0);
    EXPECT_LT(nearbyStates.size(), gridSize * gridSize); // But not all cells
    
    // Verify all returned cells are actually within radius
    for (const auto& state : nearbyStates) {
        double distance = queryPoint.distanceFrom(state->getPosition());
        EXPECT_LE(distance, searchRadius);
    }
}

TEST_F(SimulationIntegrationTest, WorldSaveAndLoad) {
    // Create complex world state
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"SaveTestCell");
    cellAsset->setMass(2.5);
    cellAsset->setRadius(1.8);
    cellAsset->setMaxHitPoint(150.0);
    
    auto proteinAsset = world_->makeAsset<ProteinAsset>();
    proteinAsset->setName(U"SaveTestProtein");
    
    // Add multiple entities
    for (int i = 0; i < 5; ++i) {
        Vec2 position(i * 50.0, i * 30.0);
        auto cellState = world_->addCellState(cellAsset, position);
        
        // Add some resources
        cellState->getStorage().addElement(i * 10.0);
        cellState->getStorage().addProtein(proteinAsset, i + 1);
    }
    
    // Add some eggs
    for (int i = 0; i < 3; ++i) {
        Vec2 position(200.0 + i * 40.0, 100.0);
        world_->addEggState(cellAsset, position);
    }
    
    // Set tile states
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            auto& tile = world_->getTile(Point(x, y));
            tile.setElement(x + y * 0.5);
            tile.setPoison(x * 0.1);
        }
    }
    
    // Save world
    String testWorldPath = U"test_integration_world";
    EXPECT_NO_THROW(world_->save(testWorldPath));
    
    // Create new world and load
    World::Make();
    auto newWorld = World::GetInstance();
    
    EXPECT_NO_THROW(World::Load(testWorldPath));
    newWorld = World::GetInstance();
    
    // Verify loaded state
    EXPECT_EQ(newWorld->getCellStates().size(), 5);
    EXPECT_EQ(newWorld->getEggStates().size(), 3);
    
    // Verify tile states
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            const auto& tile = newWorld->getTile(Point(x, y));
            EXPECT_DOUBLE_EQ(tile.getElement(), x + y * 0.5);
            EXPECT_DOUBLE_EQ(tile.getPoison(), x * 0.1);
        }
    }
    
    // Clean up test files
    FileSystem::RemoveContents(testWorldPath);
    FileSystem::Remove(testWorldPath);
}

TEST_F(SimulationIntegrationTest, PerformanceStressTest) {
    auto cellAsset = world_->makeAsset<CellAsset>();
    cellAsset->setName(U"StressTestCell");
    cellAsset->setMass(1.0);
    cellAsset->setRadius(1.0);
    cellAsset->setMaxHitPoint(100.0);
    
    // Add many entities for stress testing
    const int entityCount = 500;
    for (int i = 0; i < entityCount; ++i) {
        Vec2 randomPos(Random(0.0, 1000.0), Random(0.0, 1000.0));
        world_->addCellState(cellAsset, randomPos);
    }
    
    EXPECT_EQ(world_->getCellStates().size(), entityCount);
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Run simulation for multiple steps
    for (int step = 0; step < 50; ++step) {
        world_->updatePhysics();
        world_->updateLogic();
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 10000); // Less than 10 seconds
    
    // Verify simulation stability
    EXPECT_EQ(world_->getCellStates().size(), entityCount);
    
    // Check that entities are still in reasonable positions
    for (const auto& cellState : world_->getCellStates()) {
        Vec2 pos = cellState->getPosition();
        EXPECT_GT(pos.x, -2000.0);
        EXPECT_LT(pos.x, 3000.0);
        EXPECT_GT(pos.y, -2000.0);
        EXPECT_LT(pos.y, 3000.0);
    }
}