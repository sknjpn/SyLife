#pragma once

#include "sylife/core/common.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <regex>
#include <functional>

namespace sylife::network {

/**
 * @brief Security level for different operations
 */
enum class SecurityLevel {
    PUBLIC = 0,     ///< Public access, no restrictions
    RESTRICTED = 1, ///< Restricted access, basic validation
    PRIVATE = 2,    ///< Private access, enhanced validation
    ADMIN = 3       ///< Administrative access, full validation
};

/**
 * @brief Result of security validation
 */
struct SecurityValidationResult {
    bool isValid;
    SecurityLevel requiredLevel;
    std::string reason;
    std::vector<std::string> violations;
    
    SecurityValidationResult() 
        : isValid(true), requiredLevel(SecurityLevel::PUBLIC) {}
    
    SecurityValidationResult(bool valid, const std::string& msg = "")
        : isValid(valid), requiredLevel(SecurityLevel::PUBLIC), reason(msg) {}
        
    void addViolation(const std::string& violation) {
        violations.push_back(violation);
        isValid = false;
    }
};

/**
 * @brief Rate limiting for connections
 */
class RateLimiter {
public:
    /**
     * @brief Constructor
     * @param maxRequests Maximum requests per time window
     * @param timeWindowSeconds Time window in seconds
     */
    RateLimiter(size_t maxRequests, int timeWindowSeconds);

    /**
     * @brief Check if request is allowed
     * @param clientId Client identifier
     * @return true if request is allowed
     */
    bool allowRequest(const std::string& clientId);

    /**
     * @brief Reset rate limit for client
     * @param clientId Client identifier
     */
    void resetClient(const std::string& clientId);

    /**
     * @brief Clear all rate limit data
     */
    void clear();

    /**
     * @brief Get current request count for client
     * @param clientId Client identifier
     * @return Current request count
     */
    size_t getRequestCount(const std::string& clientId) const;

private:
    struct ClientData {
        std::vector<std::chrono::steady_clock::time_point> requests;
        size_t totalRequests;
        
        ClientData() : totalRequests(0) {}
    };

    void cleanupExpiredRequests(ClientData& data);

    size_t maxRequests_;
    std::chrono::seconds timeWindow_;
    mutable std::unordered_map<std::string, ClientData> clients_;
    mutable std::mutex mutex_;
};

/**
 * @brief Input sanitizer and validator
 */
class InputValidator {
public:
    /**
     * @brief Constructor
     */
    InputValidator();

    /**
     * @brief Validate and sanitize JSON input
     * @param input JSON string input
     * @param maxSize Maximum allowed size
     * @return Validation result
     */
    SecurityValidationResult validateJSON(const std::string& input, size_t maxSize = 1024 * 1024);

    /**
     * @brief Validate file path for security
     * @param path File path to validate
     * @param allowedDirectories Allowed base directories
     * @return Validation result
     */
    SecurityValidationResult validateFilePath(
        const std::string& path,
        const std::vector<std::string>& allowedDirectories = {}
    );

    /**
     * @brief Validate asset name
     * @param name Asset name to validate
     * @return Validation result
     */
    SecurityValidationResult validateAssetName(const std::string& name);

    /**
     * @brief Validate world name
     * @param name World name to validate
     * @return Validation result
     */
    SecurityValidationResult validateWorldName(const std::string& name);

    /**
     * @brief Sanitize string input
     * @param input Input string
     * @param maxLength Maximum allowed length
     * @return Sanitized string
     */
    std::string sanitizeString(const std::string& input, size_t maxLength = 1000);

    /**
     * @brief Check for malicious patterns
     * @param input Input to check
     * @return true if malicious patterns detected
     */
    bool containsMaliciousPatterns(const std::string& input);

    /**
     * @brief Add custom validation rule
     * @param name Rule name
     * @param validator Validation function
     */
    void addCustomRule(const std::string& name, 
                      std::function<SecurityValidationResult(const std::string&)> validator);

private:
    std::vector<std::regex> maliciousPatterns_;
    std::unordered_map<std::string, std::function<SecurityValidationResult(const std::string&)>> customRules_;
    
    void initializeMaliciousPatterns();
    bool isValidUTF8(const std::string& input);
    std::string removeControlCharacters(const std::string& input);
};

/**
 * @brief Connection authentication and authorization
 */
class ConnectionSecurity {
public:
    /**
     * @brief Authentication token
     */
    struct AuthToken {
        std::string token;
        std::string clientId;
        SecurityLevel level;
        std::chrono::steady_clock::time_point createdAt;
        std::chrono::steady_clock::time_point expiresAt;
        std::unordered_set<std::string> permissions;
        
        bool isValid() const {
            return std::chrono::steady_clock::now() < expiresAt;
        }
        
        bool hasPermission(const std::string& permission) const {
            return permissions.find(permission) != permissions.end();
        }
    };

    /**
     * @brief Constructor
     */
    ConnectionSecurity();

    /**
     * @brief Generate authentication token
     * @param clientId Client identifier
     * @param level Security level
     * @param durationMinutes Token validity duration in minutes
     * @return Generated token
     */
    std::string generateToken(const std::string& clientId, 
                             SecurityLevel level, 
                             int durationMinutes = 60);

    /**
     * @brief Validate authentication token
     * @param token Token to validate
     * @return Validation result with token info
     */
    std::pair<bool, AuthToken> validateToken(const std::string& token);

    /**
     * @brief Revoke authentication token
     * @param token Token to revoke
     */
    void revokeToken(const std::string& token);

    /**
     * @brief Check if client has required security level
     * @param token Client token
     * @param requiredLevel Required security level
     * @return true if authorized
     */
    bool isAuthorized(const std::string& token, SecurityLevel requiredLevel);

    /**
     * @brief Check if client has specific permission
     * @param token Client token
     * @param permission Permission to check
     * @return true if client has permission
     */
    bool hasPermission(const std::string& token, const std::string& permission);

    /**
     * @brief Add permission to token
     * @param token Client token
     * @param permission Permission to add
     */
    void addPermission(const std::string& token, const std::string& permission);

    /**
     * @brief Remove permission from token
     * @param token Client token
     * @param permission Permission to remove
     */
    void removePermission(const std::string& token, const std::string& permission);

    /**
     * @brief Clean up expired tokens
     */
    void cleanupExpiredTokens();

    /**
     * @brief Get active token count
     * @return Number of active tokens
     */
    size_t getActiveTokenCount() const;

private:
    std::string generateRandomToken();
    std::string hashToken(const std::string& token);

    std::unordered_map<std::string, AuthToken> tokens_;
    mutable std::mutex tokenMutex_;
    std::random_device randomDevice_;
    std::mt19937 randomGenerator_;
};

/**
 * @brief Main security manager
 */
class SecurityManager {
public:
    /**
     * @brief Constructor
     */
    SecurityManager();

    /**
     * @brief Destructor
     */
    ~SecurityManager();

    /**
     * @brief Initialize security manager
     * @param config Security configuration
     */
    void initialize(const s3d::JSON& config = {});

    /**
     * @brief Validate WebSocket message
     * @param message Message content
     * @param clientToken Client authentication token
     * @return Validation result
     */
    SecurityValidationResult validateWebSocketMessage(
        const std::string& message,
        const std::string& clientToken = ""
    );

    /**
     * @brief Validate file operation
     * @param operation Operation type (read, write, delete, etc.)
     * @param filePath File path
     * @param clientToken Client authentication token
     * @return Validation result
     */
    SecurityValidationResult validateFileOperation(
        const std::string& operation,
        const std::string& filePath,
        const std::string& clientToken = ""
    );

    /**
     * @brief Validate asset operation
     * @param operation Operation type (create, modify, delete, etc.)
     * @param assetName Asset name
     * @param clientToken Client authentication token
     * @return Validation result
     */
    SecurityValidationResult validateAssetOperation(
        const std::string& operation,
        const std::string& assetName,
        const std::string& clientToken = ""
    );

    /**
     * @brief Check rate limit for client
     * @param clientId Client identifier
     * @return true if request is allowed
     */
    bool checkRateLimit(const std::string& clientId);

    /**
     * @brief Authenticate client connection
     * @param credentials Client credentials
     * @return Authentication token or empty string if failed
     */
    std::string authenticateClient(const std::string& credentials);

    /**
     * @brief Ban client IP address
     * @param ipAddress IP address to ban
     * @param durationMinutes Ban duration in minutes
     * @param reason Ban reason
     */
    void banClient(const std::string& ipAddress, int durationMinutes, const std::string& reason);

    /**
     * @brief Unban client IP address
     * @param ipAddress IP address to unban
     */
    void unbanClient(const std::string& ipAddress);

    /**
     * @brief Check if client is banned
     * @param ipAddress Client IP address
     * @return true if client is banned
     */
    bool isClientBanned(const std::string& ipAddress);

    /**
     * @brief Get security statistics
     * @return JSON object with security statistics
     */
    s3d::JSON getSecurityStatistics() const;

    /**
     * @brief Enable/disable security features
     * @param enabled true to enable security
     */
    void setSecurityEnabled(bool enabled);

    /**
     * @brief Check if security is enabled
     * @return true if security is enabled
     */
    bool isSecurityEnabled() const;

    /**
     * @brief Set security level for operations
     * @param operation Operation name
     * @param level Required security level
     */
    void setOperationSecurityLevel(const std::string& operation, SecurityLevel level);

    /**
     * @brief Get required security level for operation
     * @param operation Operation name
     * @return Required security level
     */
    SecurityLevel getOperationSecurityLevel(const std::string& operation) const;

private:
    struct BanInfo {
        std::chrono::steady_clock::time_point bannedAt;
        std::chrono::steady_clock::time_point expiresAt;
        std::string reason;
        
        bool isActive() const {
            return std::chrono::steady_clock::now() < expiresAt;
        }
    };

    void loadConfiguration(const s3d::JSON& config);
    void initializeDefaultSecurity();
    void cleanupExpiredBans();

    // Security components
    std::unique_ptr<RateLimiter> rateLimiter_;
    std::unique_ptr<InputValidator> inputValidator_;
    std::unique_ptr<ConnectionSecurity> connectionSecurity_;

    // Configuration
    bool securityEnabled_;
    std::unordered_map<std::string, SecurityLevel> operationSecurityLevels_;
    std::vector<std::string> allowedDirectories_;

    // Ban management
    std::unordered_map<std::string, BanInfo> bannedClients_;
    mutable std::mutex banMutex_;

    // Statistics
    mutable std::atomic<size_t> validationCount_;
    mutable std::atomic<size_t> violationCount_;
    mutable std::atomic<size_t> blockedRequests_;
    mutable std::atomic<size_t> successfulAuths_;
    mutable std::atomic<size_t> failedAuths_;

    // Periodic cleanup
    std::thread cleanupThread_;
    std::atomic<bool> shutdownRequested_;
    void cleanupTask();
};

} // namespace sylife::network