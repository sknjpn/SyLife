#include "common/test_common.h"
#include "sylife/network/security_manager.h"
#include <thread>
#include <chrono>

namespace sylife::testing {

class SecurityManagerTest : public SecurityTestBase {
protected:
    void SetUp() override {
        SecurityTestBase::SetUp();
        securityManager_ = createSecurityManager();
        securityManager_->initialize();
    }
    
    void TearDown() override {
        securityManager_.reset();
        SecurityTestBase::TearDown();
    }
    
    std::unique_ptr<network::SecurityManager> securityManager_;
};

TEST_F(SecurityManagerTest, Initialization) {
    EXPECT_TRUE(securityManager_->isSecurityEnabled());
    
    auto stats = securityManager_->getSecurityStatistics();
    EXPECT_JSON_HAS_FIELD(stats, U"validationCount");
    EXPECT_JSON_HAS_FIELD(stats, U"violationCount");
    EXPECT_JSON_HAS_FIELD(stats, U"securityEnabled");
    
    EXPECT_EQ(stats[U"securityEnabled"].get<bool>(), true);
}

TEST_F(SecurityManagerTest, WebSocketMessageValidation) {
    // Valid JSON message
    std::string validMessage = R"({"type": "connect", "data": {"version": "1.0"}})";
    auto result = securityManager_->validateWebSocketMessage(validMessage);
    EXPECT_TRUE(result.isValid);
    EXPECT_TRUE(result.violations.empty());
    
    // Invalid JSON message
    std::string invalidMessage = R"({"type": "connect", "data": })";
    result = securityManager_->validateWebSocketMessage(invalidMessage);
    EXPECT_FALSE(result.isValid);
    EXPECT_FALSE(result.violations.empty());
    
    // Malicious message
    std::string maliciousMessage = R"({"type": "<script>alert('xss')</script>"})";
    result = securityManager_->validateWebSocketMessage(maliciousMessage);
    EXPECT_FALSE(result.isValid);
    EXPECT_EQ(result.requiredLevel, network::SecurityLevel::ADMIN);
}

TEST_F(SecurityManagerTest, FileOperationValidation) {
    // Valid file operation
    auto result = securityManager_->validateFileOperation("read", "assets/test.json");
    EXPECT_TRUE(result.isValid);
    
    // Path traversal attempt
    result = securityManager_->validateFileOperation("read", "../../../etc/passwd");
    EXPECT_FALSE(result.isValid);
    EXPECT_FALSE(result.violations.empty());
    
    // Null byte injection
    result = securityManager_->validateFileOperation("read", "assets/test\0.json");
    EXPECT_FALSE(result.isValid);
    
    // Absolute path
    result = securityManager_->validateFileOperation("read", "/etc/passwd");
    EXPECT_FALSE(result.isValid);
}

TEST_F(SecurityManagerTest, AssetOperationValidation) {
    // Valid asset name
    auto result = securityManager_->validateAssetOperation("create", "MyTestAsset");
    EXPECT_TRUE(result.isValid);
    
    // Invalid characters
    result = securityManager_->validateAssetOperation("create", "Test Asset!");
    EXPECT_FALSE(result.isValid);
    
    // Reserved name
    result = securityManager_->validateAssetOperation("create", "admin");
    EXPECT_FALSE(result.isValid);
    EXPECT_EQ(result.requiredLevel, network::SecurityLevel::ADMIN);
    
    // Empty name
    result = securityManager_->validateAssetOperation("create", "");
    EXPECT_FALSE(result.isValid);
    
    // Too long name
    std::string longName(101, 'a');
    result = securityManager_->validateAssetOperation("create", longName);
    EXPECT_FALSE(result.isValid);
}

TEST_F(SecurityManagerTest, RateLimiting) {
    std::string clientId = "test_client";
    
    // First requests should be allowed
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(securityManager_->checkRateLimit(clientId))
            << "Request " << i << " should be allowed";
    }
    
    // After rate limit, requests should be blocked
    // Note: This depends on rate limiter configuration
    bool foundBlocked = false;
    for (int i = 0; i < 100; ++i) {
        if (!securityManager_->checkRateLimit(clientId)) {
            foundBlocked = true;
            break;
        }
    }
    EXPECT_TRUE(foundBlocked) << "Rate limiting should eventually block requests";
}

TEST_F(SecurityManagerTest, ClientAuthentication) {
    // Valid admin credentials
    std::string adminToken = securityManager_->authenticateClient("admin");
    EXPECT_FALSE(adminToken.empty());
    
    // Valid user credentials
    std::string userToken = securityManager_->authenticateClient("user");
    EXPECT_FALSE(userToken.empty());
    
    // Invalid credentials
    std::string invalidToken = securityManager_->authenticateClient("invalid");
    EXPECT_TRUE(invalidToken.empty());
}

TEST_F(SecurityManagerTest, ClientBanning) {
    std::string clientIp = "192.168.1.100";
    
    // Initially not banned
    EXPECT_FALSE(securityManager_->isClientBanned(clientIp));
    
    // Ban client
    securityManager_->banClient(clientIp, 1, "Test ban");
    EXPECT_TRUE(securityManager_->isClientBanned(clientIp));
    
    // Unban client
    securityManager_->unbanClient(clientIp);
    EXPECT_FALSE(securityManager_->isClientBanned(clientIp));
}

TEST_F(SecurityManagerTest, BanExpiration) {
    std::string clientIp = "192.168.1.101";
    
    // Ban for very short duration (for testing)
    securityManager_->banClient(clientIp, 0, "Short test ban"); // 0 minutes = immediate expiry
    
    // Should be initially banned
    EXPECT_TRUE(securityManager_->isClientBanned(clientIp));
    
    // Wait a bit and check again (ban should expire)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(securityManager_->isClientBanned(clientIp));
}

TEST_F(SecurityManagerTest, SecurityLevelConfiguration) {
    // Set custom security level
    securityManager_->setOperationSecurityLevel("custom_operation", network::SecurityLevel::ADMIN);
    
    auto level = securityManager_->getOperationSecurityLevel("custom_operation");
    EXPECT_EQ(level, network::SecurityLevel::ADMIN);
    
    // Default level for unknown operation
    auto defaultLevel = securityManager_->getOperationSecurityLevel("unknown_operation");
    EXPECT_EQ(defaultLevel, network::SecurityLevel::PUBLIC);
}

TEST_F(SecurityManagerTest, SecurityToggle) {
    // Disable security
    securityManager_->setSecurityEnabled(false);
    EXPECT_FALSE(securityManager_->isSecurityEnabled());
    
    // All validations should pass when security is disabled
    auto result = securityManager_->validateWebSocketMessage("invalid json {");
    EXPECT_TRUE(result.isValid);
    
    result = securityManager_->validateFileOperation("read", "../../../etc/passwd");
    EXPECT_TRUE(result.isValid);
    
    // Re-enable security
    securityManager_->setSecurityEnabled(true);
    EXPECT_TRUE(securityManager_->isSecurityEnabled());
    
    // Validations should work again
    result = securityManager_->validateWebSocketMessage("invalid json {");
    EXPECT_FALSE(result.isValid);
}

TEST_F(SecurityManagerTest, StatisticsTracking) {
    auto initialStats = securityManager_->getSecurityStatistics();
    size_t initialValidations = initialStats[U"validationCount"].get<size_t>();
    size_t initialViolations = initialStats[U"violationCount"].get<size_t>();
    
    // Perform some validations
    securityManager_->validateWebSocketMessage(R"({"valid": "json"})");
    securityManager_->validateWebSocketMessage("invalid json");
    securityManager_->validateFileOperation("read", "../invalid/path");
    
    auto finalStats = securityManager_->getSecurityStatistics();
    size_t finalValidations = finalStats[U"validationCount"].get<size_t>();
    size_t finalViolations = finalStats[U"violationCount"].get<size_t>();
    
    EXPECT_GT(finalValidations, initialValidations);
    EXPECT_GT(finalViolations, initialViolations);
}

TEST_F(SecurityManagerTest, ConcurrentAccess) {
    const int numThreads = 10;
    const int requestsPerThread = 100;
    std::vector<std::thread> threads;
    std::atomic<int> successCount{0};
    std::atomic<int> failureCount{0};
    
    // Launch concurrent threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i]() {
            std::string clientId = "client_" + std::to_string(i);
            
            for (int j = 0; j < requestsPerThread; ++j) {
                // Mix of valid and invalid operations
                if (j % 2 == 0) {
                    auto result = securityManager_->validateWebSocketMessage(R"({"valid": "json"})");
                    if (result.isValid) {
                        successCount++;
                    } else {
                        failureCount++;
                    }
                } else {
                    auto result = securityManager_->validateWebSocketMessage("invalid json");
                    if (result.isValid) {
                        successCount++;
                    } else {
                        failureCount++;
                    }
                }
                
                // Test rate limiting
                securityManager_->checkRateLimit(clientId);
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Verify results
    EXPECT_GT(successCount.load(), 0);
    EXPECT_GT(failureCount.load(), 0);
    EXPECT_EQ(successCount.load() + failureCount.load(), numThreads * requestsPerThread);
}

// Test specific security components

class RateLimiterTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        rateLimiter_ = std::make_unique<network::RateLimiter>(5, 1); // 5 requests per second
    }
    
    std::unique_ptr<network::RateLimiter> rateLimiter_;
};

TEST_F(RateLimiterTest, BasicRateLimiting) {
    std::string clientId = "test_client";
    
    // First 5 requests should be allowed
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(rateLimiter_->allowRequest(clientId));
    }
    
    // 6th request should be blocked
    EXPECT_FALSE(rateLimiter_->allowRequest(clientId));
    
    // After time window, requests should be allowed again
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_TRUE(rateLimiter_->allowRequest(clientId));
}

TEST_F(RateLimiterTest, MultipleClients) {
    std::string client1 = "client1";
    std::string client2 = "client2";
    
    // Each client should have independent rate limits
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(rateLimiter_->allowRequest(client1));
        EXPECT_TRUE(rateLimiter_->allowRequest(client2));
    }
    
    // Both clients should be rate limited
    EXPECT_FALSE(rateLimiter_->allowRequest(client1));
    EXPECT_FALSE(rateLimiter_->allowRequest(client2));
}

TEST_F(RateLimiterTest, RequestCounting) {
    std::string clientId = "counting_client";
    
    EXPECT_EQ(rateLimiter_->getRequestCount(clientId), 0);
    
    rateLimiter_->allowRequest(clientId);
    EXPECT_EQ(rateLimiter_->getRequestCount(clientId), 1);
    
    rateLimiter_->allowRequest(clientId);
    rateLimiter_->allowRequest(clientId);
    EXPECT_EQ(rateLimiter_->getRequestCount(clientId), 3);
}

TEST_F(RateLimiterTest, ClientReset) {
    std::string clientId = "reset_client";
    
    // Fill up rate limit
    for (int i = 0; i < 5; ++i) {
        rateLimiter_->allowRequest(clientId);
    }
    EXPECT_FALSE(rateLimiter_->allowRequest(clientId));
    
    // Reset client
    rateLimiter_->resetClient(clientId);
    EXPECT_TRUE(rateLimiter_->allowRequest(clientId));
}

class InputValidatorTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        validator_ = std::make_unique<network::InputValidator>();
    }
    
    std::unique_ptr<network::InputValidator> validator_;
};

TEST_F(InputValidatorTest, JSONValidation) {
    // Valid JSON
    auto result = validator_->validateJSON(R"({"valid": "json", "number": 42})");
    EXPECT_TRUE(result.isValid);
    
    // Invalid JSON syntax
    result = validator_->validateJSON(R"({"invalid": json})");
    EXPECT_FALSE(result.isValid);
    
    // Empty JSON
    result = validator_->validateJSON("{}");
    EXPECT_TRUE(result.isValid);
    
    // Too large JSON
    std::string largeJson(2 * 1024 * 1024, 'a'); // 2MB
    result = validator_->validateJSON(largeJson);
    EXPECT_FALSE(result.isValid);
}

TEST_F(InputValidatorTest, MaliciousPatternDetection) {
    // SQL injection patterns
    EXPECT_TRUE(validator_->containsMaliciousPatterns("SELECT * FROM users"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("'; DROP TABLE users; --"));
    
    // Script injection patterns
    EXPECT_TRUE(validator_->containsMaliciousPatterns("<script>alert('xss')</script>"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("javascript:alert('xss')"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("onload=alert('xss')"));
    
    // Path traversal patterns
    EXPECT_TRUE(validator_->containsMaliciousPatterns("../../../etc/passwd"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("..\\..\\windows\\system32"));
    
    // Command injection patterns
    EXPECT_TRUE(validator_->containsMaliciousPatterns("cmd.exe"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("bash -c 'rm -rf /'"));
    EXPECT_TRUE(validator_->containsMaliciousPatterns("eval('malicious code')"));
    
    // Safe content should not trigger
    EXPECT_FALSE(validator_->containsMaliciousPatterns("normal text content"));
    EXPECT_FALSE(validator_->containsMaliciousPatterns("{\"type\": \"message\", \"data\": \"hello\"}"));
}

TEST_F(InputValidatorTest, FilePathValidation) {
    std::vector<std::string> allowedDirs = {"assets/", "worlds/", "temp/"};
    
    // Valid paths
    auto result = validator_->validateFilePath("assets/cell.json", allowedDirs);
    EXPECT_TRUE(result.isValid);
    
    result = validator_->validateFilePath("worlds/test_world.dat", allowedDirs);
    EXPECT_TRUE(result.isValid);
    
    // Path traversal attempts
    result = validator_->validateFilePath("../../../etc/passwd", allowedDirs);
    EXPECT_FALSE(result.isValid);
    
    result = validator_->validateFilePath("assets/../../../secret.txt", allowedDirs);
    EXPECT_FALSE(result.isValid);
    
    // Absolute paths
    result = validator_->validateFilePath("/etc/passwd", allowedDirs);
    EXPECT_FALSE(result.isValid);
    
    result = validator_->validateFilePath("C:\\Windows\\System32\\config\\SAM", allowedDirs);
    EXPECT_FALSE(result.isValid);
    
    // Null byte injection
    result = validator_->validateFilePath("assets/file\0.txt", allowedDirs);
    EXPECT_FALSE(result.isValid);
    
    // Disallowed directory
    result = validator_->validateFilePath("config/secret.conf", allowedDirs);
    EXPECT_FALSE(result.isValid);
}

TEST_F(InputValidatorTest, AssetNameValidation) {
    // Valid names
    auto result = validator_->validateAssetName("MyAsset");
    EXPECT_TRUE(result.isValid);
    
    result = validator_->validateAssetName("test_asset_123");
    EXPECT_TRUE(result.isValid);
    
    result = validator_->validateAssetName("Asset-Name");
    EXPECT_TRUE(result.isValid);
    
    // Invalid names
    result = validator_->validateAssetName("");
    EXPECT_FALSE(result.isValid);
    
    result = validator_->validateAssetName("Asset Name"); // Space
    EXPECT_FALSE(result.isValid);
    
    result = validator_->validateAssetName("Asset.Name"); // Dot
    EXPECT_FALSE(result.isValid);
    
    result = validator_->validateAssetName("Asset/Name"); // Slash
    EXPECT_FALSE(result.isValid);
    
    // Too long
    std::string longName(101, 'a');
    result = validator_->validateAssetName(longName);
    EXPECT_FALSE(result.isValid);
    
    // Reserved names
    result = validator_->validateAssetName("admin");
    EXPECT_FALSE(result.isValid);
    EXPECT_EQ(result.requiredLevel, network::SecurityLevel::ADMIN);
    
    result = validator_->validateAssetName("SYSTEM");
    EXPECT_FALSE(result.isValid);
}

TEST_F(InputValidatorTest, StringSanitization) {
    // Remove control characters
    std::string input = "Hello\x01\x02World\x7F";
    std::string sanitized = validator_->sanitizeString(input);
    EXPECT_EQ(sanitized, "HelloWorld");
    
    // Preserve allowed whitespace
    input = "Hello\tWorld\nTest\r";
    sanitized = validator_->sanitizeString(input);
    EXPECT_EQ(sanitized, "Hello\tWorld\nTest\r");
    
    // Truncate long strings
    std::string longInput(2000, 'a');
    sanitized = validator_->sanitizeString(longInput, 100);
    EXPECT_EQ(sanitized.length(), 100);
}

class ConnectionSecurityTest : public SyLifeTestBase {
protected:
    void SetUp() override {
        SyLifeTestBase::SetUp();
        connectionSecurity_ = std::make_unique<network::ConnectionSecurity>();
    }
    
    std::unique_ptr<network::ConnectionSecurity> connectionSecurity_;
};

TEST_F(ConnectionSecurityTest, TokenGeneration) {
    std::string token = connectionSecurity_->generateToken("test_client", network::SecurityLevel::PUBLIC);
    EXPECT_FALSE(token.empty());
    EXPECT_EQ(token.length(), 64); // SHA256 hex string length
}

TEST_F(ConnectionSecurityTest, TokenValidation) {
    std::string token = connectionSecurity_->generateToken("test_client", network::SecurityLevel::PUBLIC);
    
    auto [valid, authToken] = connectionSecurity_->validateToken(token);
    EXPECT_TRUE(valid);
    EXPECT_EQ(authToken.clientId, "test_client");
    EXPECT_EQ(authToken.level, network::SecurityLevel::PUBLIC);
    EXPECT_TRUE(authToken.isValid());
}

TEST_F(ConnectionSecurityTest, InvalidTokenValidation) {
    auto [valid, authToken] = connectionSecurity_->validateToken("invalid_token");
    EXPECT_FALSE(valid);
}

TEST_F(ConnectionSecurityTest, TokenRevocation) {
    std::string token = connectionSecurity_->generateToken("test_client", network::SecurityLevel::PUBLIC);
    
    // Token should be valid initially
    auto [valid1, authToken1] = connectionSecurity_->validateToken(token);
    EXPECT_TRUE(valid1);
    
    // Revoke token
    connectionSecurity_->revokeToken(token);
    
    // Token should be invalid after revocation
    auto [valid2, authToken2] = connectionSecurity_->validateToken(token);
    EXPECT_FALSE(valid2);
}

TEST_F(ConnectionSecurityTest, AuthorizationLevels) {
    std::string publicToken = connectionSecurity_->generateToken("public_client", network::SecurityLevel::PUBLIC);
    std::string adminToken = connectionSecurity_->generateToken("admin_client", network::SecurityLevel::ADMIN);
    
    // Public token should have public access only
    EXPECT_TRUE(connectionSecurity_->isAuthorized(publicToken, network::SecurityLevel::PUBLIC));
    EXPECT_FALSE(connectionSecurity_->isAuthorized(publicToken, network::SecurityLevel::RESTRICTED));
    EXPECT_FALSE(connectionSecurity_->isAuthorized(publicToken, network::SecurityLevel::PRIVATE));
    EXPECT_FALSE(connectionSecurity_->isAuthorized(publicToken, network::SecurityLevel::ADMIN));
    
    // Admin token should have all access levels
    EXPECT_TRUE(connectionSecurity_->isAuthorized(adminToken, network::SecurityLevel::PUBLIC));
    EXPECT_TRUE(connectionSecurity_->isAuthorized(adminToken, network::SecurityLevel::RESTRICTED));
    EXPECT_TRUE(connectionSecurity_->isAuthorized(adminToken, network::SecurityLevel::PRIVATE));
    EXPECT_TRUE(connectionSecurity_->isAuthorized(adminToken, network::SecurityLevel::ADMIN));
}

TEST_F(ConnectionSecurityTest, PermissionManagement) {
    std::string token = connectionSecurity_->generateToken("test_client", network::SecurityLevel::PUBLIC);
    
    // Should have basic permissions
    EXPECT_TRUE(connectionSecurity_->hasPermission(token, "connect"));
    EXPECT_FALSE(connectionSecurity_->hasPermission(token, "admin"));
    
    // Add custom permission
    connectionSecurity_->addPermission(token, "custom_permission");
    EXPECT_TRUE(connectionSecurity_->hasPermission(token, "custom_permission"));
    
    // Remove permission
    connectionSecurity_->removePermission(token, "custom_permission");
    EXPECT_FALSE(connectionSecurity_->hasPermission(token, "custom_permission"));
}

TEST_F(ConnectionSecurityTest, TokenExpiration) {
    // Generate token with very short duration
    std::string token = connectionSecurity_->generateToken("test_client", network::SecurityLevel::PUBLIC, 0);
    
    // Should be invalid immediately due to 0 duration
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto [valid, authToken] = connectionSecurity_->validateToken(token);
    EXPECT_FALSE(valid);
}

TEST_F(ConnectionSecurityTest, TokenCleanup) {
    size_t initialCount = connectionSecurity_->getActiveTokenCount();
    
    // Generate some tokens with short expiration
    for (int i = 0; i < 5; ++i) {
        connectionSecurity_->generateToken("client_" + std::to_string(i), network::SecurityLevel::PUBLIC, 0);
    }
    
    EXPECT_EQ(connectionSecurity_->getActiveTokenCount(), initialCount + 5);
    
    // Cleanup expired tokens
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    connectionSecurity_->cleanupExpiredTokens();
    
    // Count should be back to initial
    EXPECT_EQ(connectionSecurity_->getActiveTokenCount(), initialCount);
}

} // namespace sylife::testing