#include "test_common.h"
#include "../src/Particle.h"

class ParticleTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        particle_ = std::make_unique<Particle>();
    }
    
    std::unique_ptr<Particle> particle_;
};

TEST_F(ParticleTest, DefaultConstruction) {
    EXPECT_THAT(particle_->getPosition(), Vec2Near(Vec2(0, 0)));
    EXPECT_THAT(particle_->getVelocity(), Vec2Near(Vec2(0, 0)));
    EXPECT_DOUBLE_EQ(particle_->getMass(), 1.0);
}

TEST_F(ParticleTest, BasicProperties) {
    Vec2 testPos(10.5, 20.3);
    Vec2 testVel(3.2, -4.7);
    double testMass = 2.5;
    
    particle_->setPosition(testPos);
    particle_->setVelocity(testVel);
    particle_->setMass(testMass);
    
    EXPECT_THAT(particle_->getPosition(), Vec2Near(testPos));
    EXPECT_THAT(particle_->getVelocity(), Vec2Near(testVel));
    EXPECT_DOUBLE_EQ(particle_->getMass(), testMass);
}

TEST_F(ParticleTest, ForceAndImpulse) {
    particle_->setMass(2.0);
    
    Vec2 force(10.0, 5.0);
    particle_->addForce(force);
    
    Vec2 impulse(4.0, 2.0);
    particle_->addImpulse(impulse);
    
    // Test that forces accumulate
    particle_->addForce(Vec2(5.0, 2.5));
    
    // Update physics
    particle_->updateParticle();
    
    // Velocity should be affected by impulse immediately
    Vec2 expectedVelocityChange = impulse / particle_->getMass();
    EXPECT_THAT(particle_->getVelocity(), Vec2Near(expectedVelocityChange));
}

TEST_F(ParticleTest, PhysicsUpdate) {
    Vec2 initialPos(0, 0);
    Vec2 velocity(5.0, 3.0);
    
    particle_->setPosition(initialPos);
    particle_->setVelocity(velocity);
    
    particle_->updateParticle();
    
    // Position should change based on velocity and DeltaTime
    Vec2 newPos = particle_->getPosition();
    Vec2 expectedPos = initialPos + velocity * DeltaTime;
    EXPECT_THAT(newPos, Vec2Near(expectedPos));
}