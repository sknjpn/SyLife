#include <gtest/gtest.h>
#include "sylife/di/container.h"
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace sylife::di;

// Test interfaces and implementations
class ITestService {
public:
    virtual ~ITestService() = default;
    virtual std::string getName() const = 0;
    virtual int getValue() const = 0;
};

class TestServiceImpl : public ITestService {
public:
    explicit TestServiceImpl(const std::string& name = "TestService", int value = 42)
        : m_name(name), m_value(value) {
    }

    std::string getName() const override { return m_name; }
    int getValue() const override { return m_value; }

private:
    std::string m_name;
    int m_value;
};

class DependentService {
public:
    explicit DependentService(std::shared_ptr<ITestService> service)
        : m_service(std::move(service)) {
    }

    std::string getServiceName() const { return m_service->getName(); }
    int getServiceValue() const { return m_service->getValue(); }

private:
    std::shared_ptr<ITestService> m_service;
};

class ContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        container = std::make_unique<Container>();
    }

    std::unique_ptr<Container> container;
};

TEST_F(ContainerTest, RegisterAndResolveType) {
    container->registerType<ITestService, TestServiceImpl>();
    
    auto service = container->resolve<ITestService>();
    
    EXPECT_NE(service, nullptr);
    EXPECT_EQ(service->getName(), "TestService");
    EXPECT_EQ(service->getValue(), 42);
}

TEST_F(ContainerTest, RegisterAndResolveTypeWithArguments) {
    container->registerType<ITestService, TestServiceImpl>(
        Container::Lifetime::Singleton, 
        "CustomService", 
        100
    );
    
    auto service = container->resolve<ITestService>();
    
    EXPECT_NE(service, nullptr);
    EXPECT_EQ(service->getName(), "CustomService");
    EXPECT_EQ(service->getValue(), 100);
}

TEST_F(ContainerTest, SingletonLifetime) {
    container->registerType<ITestService, TestServiceImpl>(Container::Lifetime::Singleton);
    
    auto service1 = container->resolve<ITestService>();
    auto service2 = container->resolve<ITestService>();
    
    EXPECT_EQ(service1, service2); // Same instance
}

TEST_F(ContainerTest, TransientLifetime) {
    container->registerType<ITestService, TestServiceImpl>(Container::Lifetime::Transient);
    
    auto service1 = container->resolve<ITestService>();
    auto service2 = container->resolve<ITestService>();
    
    EXPECT_NE(service1, service2); // Different instances
    EXPECT_EQ(service1->getName(), service2->getName()); // Same configuration
}

TEST_F(ContainerTest, RegisterFactory) {
    container->registerFactory<ITestService>([]() {
        return std::make_shared<TestServiceImpl>("FactoryService", 999);
    });
    
    auto service = container->resolve<ITestService>();
    
    EXPECT_NE(service, nullptr);
    EXPECT_EQ(service->getName(), "FactoryService");
    EXPECT_EQ(service->getValue(), 999);
}

TEST_F(ContainerTest, RegisterInstance) {
    auto instance = std::make_shared<TestServiceImpl>("InstanceService", 777);
    container->registerInstance<ITestService>(instance);
    
    auto resolved = container->resolve<ITestService>();
    
    EXPECT_EQ(instance, resolved); // Same instance
    EXPECT_EQ(resolved->getName(), "InstanceService");
    EXPECT_EQ(resolved->getValue(), 777);
}

TEST_F(ContainerTest, IsRegistered) {
    EXPECT_FALSE(container->isRegistered<ITestService>());
    
    container->registerType<ITestService, TestServiceImpl>();
    
    EXPECT_TRUE(container->isRegistered<ITestService>());
}

TEST_F(ContainerTest, UnregisteredTypeThrows) {
    EXPECT_THROW(container->resolve<ITestService>(), DIException);
}

TEST_F(ContainerTest, RegisterNullInstanceThrows) {
    EXPECT_THROW(container->registerInstance<ITestService>(nullptr), DIException);
}

TEST_F(ContainerTest, DependencyInjection) {
    // Register the dependency
    container->registerType<ITestService, TestServiceImpl>();
    
    // Register dependent service with factory that resolves dependency
    container->registerFactory<DependentService>([this]() {
        auto service = container->resolve<ITestService>();
        return std::make_shared<DependentService>(service);
    });
    
    // Resolve dependent service
    auto dependent = container->resolve<DependentService>();
    
    EXPECT_NE(dependent, nullptr);
    EXPECT_EQ(dependent->getServiceName(), "TestService");
    EXPECT_EQ(dependent->getServiceValue(), 42);
}

TEST_F(ContainerTest, Clear) {
    container->registerType<ITestService, TestServiceImpl>();
    EXPECT_TRUE(container->isRegistered<ITestService>());
    EXPECT_GT(container->getRegistrationCount(), 0);
    
    container->clear();
    
    EXPECT_FALSE(container->isRegistered<ITestService>());
    EXPECT_EQ(container->getRegistrationCount(), 0);
}

TEST_F(ContainerTest, MultipleRegistrations) {
    container->registerType<ITestService, TestServiceImpl>();
    
    // Register another type
    container->registerType<DependentService, DependentService>(
        Container::Lifetime::Transient,
        container->resolve<ITestService>()
    );
    
    EXPECT_EQ(container->getRegistrationCount(), 2);
    
    auto service = container->resolve<ITestService>();
    auto dependent = container->resolve<DependentService>();
    
    EXPECT_NE(service, nullptr);
    EXPECT_NE(dependent, nullptr);
}

// Thread safety test
TEST_F(ContainerTest, ThreadSafety) {
    container->registerType<ITestService, TestServiceImpl>(Container::Lifetime::Singleton);
    
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<ITestService>> results(10);
    
    // Create multiple threads that resolve simultaneously
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([this, &results, i]() {
            results[i] = container->resolve<ITestService>();
        });
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    // All results should be the same instance (singleton)
    for (int i = 1; i < 10; ++i) {
        EXPECT_EQ(results[0], results[i]);
    }
}