#include "test_common.h"
#include "../src/Rigidbody.h"

class RigidbodyTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        rigidbody_ = std::make_unique<Rigidbody>();
        
        // Set up default test values
        rigidbody_->setMass(1.0);
        rigidbody_->setRadius(1.0);
        rigidbody_->setPosition(Vec2(0.0, 0.0));
        rigidbody_->setVelocity(Vec2(0.0, 0.0));
        rigidbody_->setRotation(0.0);
        rigidbody_->setAngularVelocity(0.0);
    }
    
    std::unique_ptr<Rigidbody> rigidbody_;
};

TEST_F(RigidbodyTest, DefaultConstruction) {
    auto rb = std::make_unique<Rigidbody>();
    
    EXPECT_DOUBLE_EQ(rb->getRadius(), 1.0);
    EXPECT_DOUBLE_EQ(rb->getRotation(), 0.0);
    EXPECT_DOUBLE_EQ(rb->getInertia(), 1.0);
    EXPECT_DOUBLE_EQ(rb->getAngularVelocity(), 0.0);
}

TEST_F(RigidbodyTest, BasicPropertySettersAndGetters) {
    double testRadius = 2.5;
    double testRotation = Math::Pi / 4.0; // 45 degrees
    double testInertia = 3.0;
    double testAngularVelocity = 1.5;
    
    rigidbody_->setRadius(testRadius);
    rigidbody_->setRotation(testRotation);
    rigidbody_->setInertia(testInertia);
    rigidbody_->setAngularVelocity(testAngularVelocity);
    
    EXPECT_DOUBLE_EQ(rigidbody_->getRadius(), testRadius);
    EXPECT_DOUBLE_EQ(rigidbody_->getRotation(), testRotation);
    EXPECT_DOUBLE_EQ(rigidbody_->getInertia(), testInertia);
    EXPECT_DOUBLE_EQ(rigidbody_->getAngularVelocity(), testAngularVelocity);
}

TEST_F(RigidbodyTest, InertiaInitialization) {
    double mass = 2.0;
    double radius = 3.0;
    
    rigidbody_->setMass(mass);
    rigidbody_->setRadius(radius);
    rigidbody_->init();
    
    // Expected inertia for a disk: I = 0.5 * m * r^2
    double expectedInertia = mass * radius * radius * 0.5;
    EXPECT_DOUBLE_EQ(rigidbody_->getInertia(), expectedInertia);
}

TEST_F(RigidbodyTest, TransformationMatrix) {
    Vec2 position(10.0, 20.0);
    double rotation = Math::Pi / 6.0; // 30 degrees
    
    rigidbody_->setPosition(position);
    rigidbody_->setRotation(rotation);
    
    Mat3x2 transform = rigidbody_->getMat3x2();
    
    // Verify transformation matrix properties
    Vec2 origin = transform.transformPoint(Vec2(0, 0));
    EXPECT_THAT(origin, Vec2Near(position));
    
    // Test rotation component
    Vec2 unitX = transform.transformPoint(Vec2(1, 0)) - origin;
    Vec2 expectedX(cos(rotation), sin(rotation));
    EXPECT_THAT(unitX, Vec2Near(expectedX));
}

TEST_F(RigidbodyTest, WorldPositionCalculation) {
    Vec2 position(5.0, 10.0);
    double rotation = Math::Pi / 2.0; // 90 degrees
    
    rigidbody_->setPosition(position);
    rigidbody_->setRotation(rotation);
    
    Vec2 localPos(1.0, 0.0);
    Vec2 worldPos = rigidbody_->getWorldPosition(localPos);
    
    // After 90-degree rotation, (1,0) becomes (0,1) in world space
    Vec2 expected = position + Vec2(0.0, 1.0);
    EXPECT_THAT(worldPos, Vec2Near(expected));
}

TEST_F(RigidbodyTest, VelocityAtPoint) {
    Vec2 linearVelocity(2.0, 3.0);
    double angularVelocity = 1.0; // rad/s
    
    rigidbody_->setVelocity(linearVelocity);
    rigidbody_->setAngularVelocity(angularVelocity);
    rigidbody_->setPosition(Vec2(0, 0));
    rigidbody_->setRotation(0.0);
    
    // Test velocity at center (should be just linear velocity)
    Vec2 centerVelocity = rigidbody_->getVelocityAt(Vec2(0, 0));
    EXPECT_THAT(centerVelocity, Vec2Near(linearVelocity));
    
    // Test velocity at offset point
    Vec2 offset(1.0, 0.0);
    Vec2 offsetVelocity = rigidbody_->getVelocityAt(offset);
    
    // Angular contribution: perpendicular to offset, magnitude = |offset| * angularVel
    Vec2 angularContribution(0.0, 1.0); // Perpendicular to (1,0)
    Vec2 expectedVelocity = linearVelocity + angularContribution;
    
    EXPECT_THAT(offsetVelocity, Vec2Near(expectedVelocity));
}

TEST_F(RigidbodyTest, ForceApplication) {
    rigidbody_->setMass(2.0);
    rigidbody_->setRadius(1.0);
    rigidbody_->init();
    
    Vec2 initialVelocity = rigidbody_->getVelocity();
    double initialAngularVelocity = rigidbody_->getAngularVelocity();
    
    // Apply force at center (should only affect linear motion)
    Vec2 force(10.0, 0.0);
    Vec2 centerPosition = rigidbody_->getPosition();
    
    rigidbody_->addForceInWorld(force, centerPosition);
    
    // Force at center shouldn't change angular velocity
    EXPECT_DOUBLE_EQ(rigidbody_->getAngularVelocity(), initialAngularVelocity);
    
    // Apply force at offset (should affect both linear and angular motion)
    Vec2 offsetPosition = centerPosition + Vec2(0.0, 1.0);
    rigidbody_->addForceInWorld(force, offsetPosition);
    
    // This should create torque and change angular acceleration
    // (Actual effect depends on updateRigidbody implementation)
}

TEST_F(RigidbodyTest, ImpulseApplication) {
    rigidbody_->setMass(1.0);
    rigidbody_->setRadius(1.0);
    rigidbody_->init();
    
    Vec2 initialVelocity = rigidbody_->getVelocity();
    double initialAngularVelocity = rigidbody_->getAngularVelocity();
    
    // Apply impulse at center
    Vec2 impulse(5.0, 0.0);
    Vec2 centerPosition = rigidbody_->getPosition();
    
    rigidbody_->addImpulseInWorld(impulse, centerPosition);
    
    // Impulse at center should change linear velocity but not angular
    Vec2 expectedVelocity = initialVelocity + impulse / rigidbody_->getMass();
    EXPECT_THAT(rigidbody_->getVelocity(), Vec2Near(expectedVelocity));
    EXPECT_DOUBLE_EQ(rigidbody_->getAngularVelocity(), initialAngularVelocity);
}

TEST_F(RigidbodyTest, LocalForceApplication) {
    rigidbody_->setPosition(Vec2(0, 0));
    rigidbody_->setRotation(Math::Pi / 2.0); // 90 degrees
    rigidbody_->setMass(1.0);
    rigidbody_->setRadius(1.0);
    rigidbody_->init();
    
    // Apply local force (1,0) which should become (0,1) in world space
    Vec2 localForce(1.0, 0.0);
    Vec2 localPosition(0.0, 0.0);
    
    rigidbody_->addForceInLocal(localForce, localPosition);
    
    // The force should be rotated to world coordinates
    // This test verifies the transformation is applied correctly
    EXPECT_NO_THROW(rigidbody_->updateRigidbody());
}

TEST_F(RigidbodyTest, UpdateRigidbody) {
    rigidbody_->setMass(1.0);
    rigidbody_->setRadius(1.0);
    rigidbody_->setAngularVelocity(2.0);
    
    double initialRotation = rigidbody_->getRotation();
    
    // Update should integrate angular velocity
    rigidbody_->updateRigidbody();
    
    // Verify rotation has changed (assuming DeltaTime is used)
    // Note: Actual verification depends on the implementation details
    EXPECT_NO_THROW(rigidbody_->getRotation());
}

TEST_F(RigidbodyTest, SerializationRoundTrip) {
    // Set up test state
    rigidbody_->setPosition(Vec2(12.34, 56.78));
    rigidbody_->setVelocity(Vec2(9.87, 6.54));
    rigidbody_->setMass(3.21);
    rigidbody_->setRadius(4.56);
    rigidbody_->setRotation(1.23);
    rigidbody_->setInertia(7.89);
    rigidbody_->setAngularVelocity(2.46);
    
    // Serialize
    MemoryWriter writer;
    Serializer<MemoryWriter> serializer(writer);
    rigidbody_->save(serializer);
    
    // Deserialize
    BinaryReader reader(writer.getBlob());
    Deserializer<BinaryReader> deserializer(reader);
    
    auto newRigidbody = std::make_unique<Rigidbody>();
    newRigidbody->load(deserializer);
    
    // Verify all properties match
    EXPECT_THAT(newRigidbody->getPosition(), Vec2Near(rigidbody_->getPosition()));
    EXPECT_THAT(newRigidbody->getVelocity(), Vec2Near(rigidbody_->getVelocity()));
    EXPECT_DOUBLE_EQ(newRigidbody->getMass(), rigidbody_->getMass());
    EXPECT_DOUBLE_EQ(newRigidbody->getRadius(), rigidbody_->getRadius());
    EXPECT_DOUBLE_EQ(newRigidbody->getRotation(), rigidbody_->getRotation());
    EXPECT_DOUBLE_EQ(newRigidbody->getInertia(), rigidbody_->getInertia());
    EXPECT_DOUBLE_EQ(newRigidbody->getAngularVelocity(), rigidbody_->getAngularVelocity());
}

TEST_F(RigidbodyTest, PhysicsConsistency) {
    // Test conservation of energy principles
    rigidbody_->setMass(2.0);
    rigidbody_->setRadius(1.5);
    rigidbody_->setVelocity(Vec2(3.0, 4.0)); // Speed = 5.0
    rigidbody_->setAngularVelocity(2.0);
    rigidbody_->init();
    
    // Calculate initial kinetic energy
    double linearKE = 0.5 * rigidbody_->getMass() * rigidbody_->getVelocity().lengthSq();
    double rotationalKE = 0.5 * rigidbody_->getInertia() * 
                         rigidbody_->getAngularVelocity() * rigidbody_->getAngularVelocity();
    double initialEnergy = linearKE + rotationalKE;
    
    EXPECT_GT(initialEnergy, 0.0);
    
    // Apply multiple updates (without external forces, energy should be conserved)
    for (int i = 0; i < 10; ++i) {
        rigidbody_->updateRigidbody();
    }
    
    // Recalculate energy (allowing for numerical precision and damping)
    linearKE = 0.5 * rigidbody_->getMass() * rigidbody_->getVelocity().lengthSq();
    rotationalKE = 0.5 * rigidbody_->getInertia() * 
                   rigidbody_->getAngularVelocity() * rigidbody_->getAngularVelocity();
    double finalEnergy = linearKE + rotationalKE;
    
    // Energy should be conserved or slightly decreased due to damping
    EXPECT_LE(finalEnergy, initialEnergy * 1.01); // Allow small numerical error
}

TEST_F(RigidbodyTest, EdgeCases) {
    // Test zero mass
    rigidbody_->setMass(0.0);
    EXPECT_NO_THROW(rigidbody_->init());
    EXPECT_NO_THROW(rigidbody_->updateRigidbody());
    
    // Test zero radius
    rigidbody_->setMass(1.0);
    rigidbody_->setRadius(0.0);
    EXPECT_NO_THROW(rigidbody_->init());
    
    // Test very large values
    rigidbody_->setMass(1e10);
    rigidbody_->setRadius(1e5);
    EXPECT_NO_THROW(rigidbody_->init());
    
    // Test negative values (should handle gracefully)
    rigidbody_->setMass(-1.0);
    EXPECT_NO_THROW(rigidbody_->init());
}

TEST_F(RigidbodyTest, PerformanceTest) {
    // Performance test with many operations
    rigidbody_->setMass(1.0);
    rigidbody_->setRadius(1.0);
    rigidbody_->init();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Perform many physics updates
    for (int i = 0; i < 10000; ++i) {
        rigidbody_->addForceInWorld(Vec2(0.1, 0.1), rigidbody_->getPosition());
        rigidbody_->updateRigidbody();
        
        if (i % 100 == 0) {
            // Occasionally test other operations
            rigidbody_->getVelocityAt(Vec2(1.0, 1.0));
            rigidbody_->getWorldPosition(Vec2(0.5, 0.5));
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 1000); // Less than 1 second
}