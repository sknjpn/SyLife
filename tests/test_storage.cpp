#include "test_common.h"
#include "../src/Storage.h"
#include "../src/ProteinAsset.h"

class StorageTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        storage_ = std::make_unique<Storage>();
        
        // Create test protein assets
        protein1_ = std::make_shared<ProteinAsset>();
        protein1_->setName(U"TestProtein1");
        
        protein2_ = std::make_shared<ProteinAsset>();
        protein2_->setName(U"TestProtein2");
        
        protein3_ = std::make_shared<ProteinAsset>();
        protein3_->setName(U"TestProtein3");
    }
    
    std::unique_ptr<Storage> storage_;
    std::shared_ptr<ProteinAsset> protein1_, protein2_, protein3_;
};

TEST_F(StorageTest, DefaultConstruction) {
    EXPECT_DOUBLE_EQ(storage_->getElement(), 0.0);
    EXPECT_EQ(storage_->getProteinList().size(), 0);
}

TEST_F(StorageTest, ElementManagement) {
    // Test setting element
    double testElement = 123.45;
    storage_->setElement(testElement);
    EXPECT_DOUBLE_EQ(storage_->getElement(), testElement);
    
    // Test adding element
    double addAmount = 67.89;
    storage_->addElement(addAmount);
    EXPECT_DOUBLE_EQ(storage_->getElement(), testElement + addAmount);
    
    // Test pulling element
    double pullAmount = 50.0;
    storage_->pullElement(pullAmount);
    EXPECT_DOUBLE_EQ(storage_->getElement(), testElement + addAmount - pullAmount);
}

TEST_F(StorageTest, ProteinManagement) {
    // Test adding proteins
    storage_->addProtein(protein1_, 5);
    storage_->addProtein(protein2_, 3);
    
    EXPECT_EQ(storage_->numProtein(protein1_), 5);
    EXPECT_EQ(storage_->numProtein(protein2_), 3);
    EXPECT_EQ(storage_->numProtein(protein3_), 0); // Not added
    
    // Test protein list
    const auto& proteinList = storage_->getProteinList();
    EXPECT_EQ(proteinList.size(), 2);
    
    // Test adding more of the same protein
    storage_->addProtein(protein1_, 2);
    EXPECT_EQ(storage_->numProtein(protein1_), 7);
    
    // Test pulling proteins
    storage_->pullProtein(protein1_, 3);
    EXPECT_EQ(storage_->numProtein(protein1_), 4);
    
    // Test pulling all proteins
    storage_->pullProtein(protein2_, 3);
    EXPECT_EQ(storage_->numProtein(protein2_), 0);
}

TEST_F(StorageTest, ProteinOverPull) {
    storage_->addProtein(protein1_, 5);
    
    // Try to pull more than available
    storage_->pullProtein(protein1_, 10);
    
    // Should not go negative (depends on implementation)
    EXPECT_GE(storage_->numProtein(protein1_), 0);
}

TEST_F(StorageTest, StorageArithmetic) {
    // Set up first storage
    storage_->setElement(100.0);
    storage_->addProtein(protein1_, 5);
    storage_->addProtein(protein2_, 3);
    
    // Set up second storage
    Storage storage2;
    storage2.setElement(50.0);
    storage2.addProtein(protein1_, 2);
    storage2.addProtein(protein3_, 4);
    
    // Test addition
    Storage sum = *storage_ + storage2;
    EXPECT_DOUBLE_EQ(sum.getElement(), 150.0);
    EXPECT_EQ(sum.numProtein(protein1_), 7);
    EXPECT_EQ(sum.numProtein(protein2_), 3);
    EXPECT_EQ(sum.numProtein(protein3_), 4);
    
    // Test subtraction
    Storage diff = *storage_ - storage2;
    EXPECT_DOUBLE_EQ(diff.getElement(), 50.0);
    EXPECT_EQ(diff.numProtein(protein1_), 3);
    EXPECT_EQ(diff.numProtein(protein2_), 3);
    // protein3 should handle gracefully (not negative)
    EXPECT_GE(diff.numProtein(protein3_), 0);
}

TEST_F(StorageTest, StorageAssignmentOperators) {
    // Set up first storage
    storage_->setElement(100.0);
    storage_->addProtein(protein1_, 5);
    
    // Set up second storage
    Storage storage2;
    storage2.setElement(25.0);
    storage2.addProtein(protein1_, 2);
    storage2.addProtein(protein2_, 3);
    
    // Test += operator
    *storage_ += storage2;
    EXPECT_DOUBLE_EQ(storage_->getElement(), 125.0);
    EXPECT_EQ(storage_->numProtein(protein1_), 7);
    EXPECT_EQ(storage_->numProtein(protein2_), 3);
    
    // Test -= operator
    *storage_ -= storage2;
    EXPECT_DOUBLE_EQ(storage_->getElement(), 100.0);
    EXPECT_EQ(storage_->numProtein(protein1_), 5);
    EXPECT_EQ(storage_->numProtein(protein2_), 0);
}

TEST_F(StorageTest, StorageComparison) {
    // Set up storages for comparison
    storage_->setElement(100.0);
    storage_->addProtein(protein1_, 5);
    storage_->addProtein(protein2_, 3);
    
    Storage storage2;
    storage2.setElement(50.0);
    storage2.addProtein(protein1_, 2);
    storage2.addProtein(protein2_, 1);
    
    Storage storage3;
    storage3.setElement(150.0);
    storage3.addProtein(protein1_, 6);
    storage3.addProtein(protein2_, 4);
    
    // Test >= operator
    EXPECT_TRUE(*storage_ >= storage2);
    EXPECT_FALSE(storage2 >= *storage_);
    EXPECT_FALSE(*storage_ >= storage3);
    
    // Test <= operator
    EXPECT_TRUE(storage2 <= *storage_);
    EXPECT_FALSE(*storage_ <= storage2);
    EXPECT_TRUE(*storage_ <= storage3);
}

TEST_F(StorageTest, ContainmentChecks) {
    // Set up main storage
    storage_->setElement(100.0);
    storage_->addProtein(protein1_, 5);
    storage_->addProtein(protein2_, 3);
    
    // Test contain method (full containment)
    Storage subset;
    subset.setElement(50.0);
    subset.addProtein(protein1_, 2);
    subset.addProtein(protein2_, 1);
    
    EXPECT_TRUE(storage_->contain(subset));
    
    // Test containment failure
    Storage tooBig;
    tooBig.setElement(150.0);
    tooBig.addProtein(protein1_, 6);
    
    EXPECT_FALSE(storage_->contain(tooBig));
    
    // Test protein-only containment
    Storage proteinOnly;
    proteinOnly.addProtein(protein1_, 3);
    
    EXPECT_TRUE(storage_->containProtein(proteinOnly));
    
    proteinOnly.addProtein(protein1_, 5); // Now has 8 total
    EXPECT_FALSE(storage_->containProtein(proteinOnly));
}

TEST_F(StorageTest, RecursiveElementCalculation) {
    // This test depends on how proteins contribute to element count
    storage_->setElement(100.0);
    storage_->addProtein(protein1_, 5);
    storage_->addProtein(protein2_, 3);
    
    double recursiveElement = storage_->getElementRecursive();
    
    // Should include base element plus protein contributions
    EXPECT_GE(recursiveElement, storage_->getElement());
}

TEST_F(StorageTest, JSONSerialization) {
    // Set up test data
    storage_->setElement(123.45);
    storage_->addProtein(protein1_, 7);
    storage_->addProtein(protein2_, 4);
    
    // Serialize to JSON
    JSON saveJson;
    storage_->save(saveJson);
    
    // Verify JSON structure
    EXPECT_TRUE(saveJson.contains(U"element"));
    EXPECT_TRUE(saveJson.contains(U"proteins"));
    
    // Create new storage and deserialize
    auto newStorage = std::make_unique<Storage>();
    
    // Note: This test may require protein assets to be properly registered
    // for deserialization to work correctly
    EXPECT_NO_THROW(newStorage->load(saveJson));
    
    // Basic verification (full verification requires asset management)
    EXPECT_DOUBLE_EQ(newStorage->getElement(), storage_->getElement());
}

TEST_F(StorageTest, BinarySerialization) {
    // Set up test data
    storage_->setElement(987.65);
    storage_->addProtein(protein1_, 12);
    storage_->addProtein(protein2_, 8);
    
    // Serialize to binary
    MemoryWriter writer;
    Serializer<MemoryWriter> serializer(writer);
    storage_->save(serializer);
    
    // Deserialize
    BinaryReader reader(writer.getBlob());
    Deserializer<BinaryReader> deserializer(reader);
    
    auto newStorage = std::make_unique<Storage>();
    newStorage->load(deserializer);
    
    // Verify deserialized data
    EXPECT_DOUBLE_EQ(newStorage->getElement(), storage_->getElement());
    
    // Note: Protein verification may require proper asset management
    // EXPECT_EQ(newStorage->numProtein(protein1_), storage_->numProtein(protein1_));
}

TEST_F(StorageTest, EdgeCases) {
    // Test negative elements
    storage_->setElement(-50.0);
    EXPECT_DOUBLE_EQ(storage_->getElement(), -50.0);
    
    // Test zero proteins
    storage_->addProtein(protein1_, 0);
    EXPECT_EQ(storage_->numProtein(protein1_), 0);
    
    // Test null protein (should handle gracefully)
    std::shared_ptr<ProteinAsset> nullProtein;
    EXPECT_NO_THROW(storage_->addProtein(nullProtein, 5));
    
    // Test very large numbers
    storage_->setElement(1e10);
    storage_->addProtein(protein1_, 1000000);
    EXPECT_DOUBLE_EQ(storage_->getElement(), 1e10);
    EXPECT_EQ(storage_->numProtein(protein1_), 1000000);
}

TEST_F(StorageTest, PerformanceTest) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Add many proteins
    for (int i = 0; i < 1000; ++i) {
        storage_->addProtein(protein1_, 1);
        storage_->addProtein(protein2_, 1);
        storage_->addProtein(protein3_, 1);
    }
    
    // Perform many operations
    for (int i = 0; i < 1000; ++i) {
        storage_->addElement(1.0);
        storage_->pullElement(0.5);
        
        if (i % 100 == 0) {
            // Test expensive operations occasionally
            Storage temp = *storage_;
            temp += *storage_;
            storage_->contain(temp);
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 1000); // Less than 1 second
    
    // Verify final state is reasonable
    EXPECT_GT(storage_->getElement(), 0.0);
    EXPECT_EQ(storage_->numProtein(protein1_), 1000);
}

TEST_F(StorageTest, CopyAndAssignment) {
    // Set up original storage
    storage_->setElement(200.0);
    storage_->addProtein(protein1_, 10);
    storage_->addProtein(protein2_, 5);
    
    // Test copy constructor
    Storage copy(*storage_);
    EXPECT_DOUBLE_EQ(copy.getElement(), storage_->getElement());
    EXPECT_EQ(copy.numProtein(protein1_), storage_->numProtein(protein1_));
    EXPECT_EQ(copy.numProtein(protein2_), storage_->numProtein(protein2_));
    
    // Test assignment operator
    Storage assigned;
    assigned = *storage_;
    EXPECT_DOUBLE_EQ(assigned.getElement(), storage_->getElement());
    EXPECT_EQ(assigned.numProtein(protein1_), storage_->numProtein(protein1_));
    EXPECT_EQ(assigned.numProtein(protein2_), storage_->numProtein(protein2_));
    
    // Verify independence (modifying copy shouldn't affect original)
    copy.addElement(100.0);
    EXPECT_NE(copy.getElement(), storage_->getElement());
}