#pragma once

#include "sylife/core/common.h"
#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <new>

namespace sylife::utils {

/**
 * @brief High-performance object pool for frequent allocations/deallocations
 * @tparam T Object type
 */
template<typename T>
class ObjectPool {
public:
    /**
     * @brief Constructor
     * @param initialSize Initial pool size
     * @param maxSize Maximum pool size (0 = unlimited)
     */
    explicit ObjectPool(size_t initialSize = 16, size_t maxSize = 0)
        : maxSize_(maxSize)
        , totalAllocated_(0)
        , totalReused_(0)
        , peakUsage_(0) {
        
        // Pre-allocate initial objects
        for (size_t i = 0; i < initialSize; ++i) {
            available_.push(createObject());
        }
    }

    /**
     * @brief Destructor
     */
    ~ObjectPool() {
        clear();
    }

    /**
     * @brief Acquire object from pool
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Unique pointer to object
     */
    template<typename... Args>
    std::unique_ptr<T, std::function<void(T*)>> acquire(Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        T* obj = nullptr;
        
        if (!available_.empty()) {
            obj = available_.top();
            available_.pop();
            totalReused_++;
        } else {
            if (maxSize_ == 0 || pool_.size() < maxSize_) {
                obj = createObject();
            } else {
                // Pool is at maximum capacity, allocate on heap
                obj = new T(std::forward<Args>(args)...);
                totalAllocated_++;
                
                return std::unique_ptr<T, std::function<void(T*)>>(
                    obj, [](T* ptr) { delete ptr; }
                );
            }
        }
        
        // Reconstruct object with new arguments
        if constexpr (sizeof...(args) > 0) {
            obj->~T();
            new (obj) T(std::forward<Args>(args)...);
        }
        
        size_t currentUsage = pool_.size() - available_.size();
        peakUsage_ = std::max(peakUsage_, currentUsage);
        
        return std::unique_ptr<T, std::function<void(T*)>>(
            obj, [this](T* ptr) { release(ptr); }
        );
    }

    /**
     * @brief Get pool statistics
     * @return Statistics object
     */
    struct Statistics {
        size_t poolSize;
        size_t availableCount;
        size_t inUseCount;
        size_t totalAllocated;
        size_t totalReused;
        size_t peakUsage;
        double reuseRatio;
    };

    Statistics getStatistics() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        Statistics stats;
        stats.poolSize = pool_.size();
        stats.availableCount = available_.size();
        stats.inUseCount = stats.poolSize - stats.availableCount;
        stats.totalAllocated = totalAllocated_;
        stats.totalReused = totalReused_;
        stats.peakUsage = peakUsage_;
        stats.reuseRatio = (totalAllocated_ + totalReused_ > 0) ?
                          static_cast<double>(totalReused_) / (totalAllocated_ + totalReused_) : 0.0;
        
        return stats;
    }

    /**
     * @brief Clear all objects from pool
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        while (!available_.empty()) {
            available_.pop();
        }
        
        for (auto& obj : pool_) {
            obj->~T();
            std::free(obj);
        }
        
        pool_.clear();
        totalAllocated_ = 0;
        totalReused_ = 0;
        peakUsage_ = 0;
    }

    /**
     * @brief Shrink pool to optimal size
     */
    void shrink() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Keep only half of available objects
        size_t keepCount = available_.size() / 2;
        size_t removeCount = available_.size() - keepCount;
        
        for (size_t i = 0; i < removeCount; ++i) {
            if (!available_.empty()) {
                available_.pop();
            }
        }
    }

private:
    void release(T* obj) {
        if (!obj) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if object belongs to our pool
        bool belongsToPool = false;
        for (const auto& poolObj : pool_) {
            if (poolObj == obj) {
                belongsToPool = true;
                break;
            }
        }
        
        if (belongsToPool) {
            available_.push(obj);
        } else {
            // Object was allocated outside pool, delete it
            delete obj;
        }
    }

    T* createObject() {
        void* memory = std::aligned_alloc(alignof(T), sizeof(T));
        if (!memory) {
            throw std::bad_alloc();
        }
        
        T* obj = new (memory) T();
        pool_.push_back(obj);
        totalAllocated_++;
        
        return obj;
    }

    mutable std::mutex mutex_;
    std::vector<T*> pool_;
    std::stack<T*> available_;
    size_t maxSize_;
    
    // Statistics
    std::atomic<size_t> totalAllocated_;
    std::atomic<size_t> totalReused_;
    std::atomic<size_t> peakUsage_;
};

/**
 * @brief Memory-aligned allocator for specific alignment requirements
 * @tparam T Object type
 * @tparam Alignment Memory alignment in bytes
 */
template<typename T, size_t Alignment = alignof(T)>
class AlignedAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind {
        using other = AlignedAllocator<U, Alignment>;
    };

    AlignedAllocator() = default;
    
    template<typename U>
    AlignedAllocator(const AlignedAllocator<U, Alignment>&) {}

    pointer allocate(size_type n) {
        size_t size = n * sizeof(T);
        void* ptr = std::aligned_alloc(Alignment, size);
        
        if (!ptr) {
            throw std::bad_alloc();
        }
        
        return static_cast<pointer>(ptr);
    }

    void deallocate(pointer p, size_type) {
        std::free(p);
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        new (p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) {
        p->~U();
    }

    bool operator==(const AlignedAllocator&) const { return true; }
    bool operator!=(const AlignedAllocator&) const { return false; }
};

/**
 * @brief Stack allocator for temporary allocations
 */
class StackAllocator {
public:
    /**
     * @brief Constructor
     * @param size Stack size in bytes
     */
    explicit StackAllocator(size_t size);

    /**
     * @brief Destructor
     */
    ~StackAllocator();

    /**
     * @brief Allocate memory from stack
     * @param size Size in bytes
     * @param alignment Memory alignment
     * @return Pointer to allocated memory
     */
    void* allocate(size_t size, size_t alignment = sizeof(void*));

    /**
     * @brief Get current stack marker
     * @return Stack marker
     */
    size_t getMarker() const;

    /**
     * @brief Reset stack to marker position
     * @param marker Stack marker
     */
    void resetToMarker(size_t marker);

    /**
     * @brief Reset entire stack
     */
    void reset();

    /**
     * @brief Get total stack size
     * @return Total size in bytes
     */
    size_t getTotalSize() const;

    /**
     * @brief Get used stack size
     * @return Used size in bytes
     */
    size_t getUsedSize() const;

    /**
     * @brief Get available stack size
     * @return Available size in bytes
     */
    size_t getAvailableSize() const;

    /**
     * @brief Check if stack is empty
     * @return true if empty
     */
    bool isEmpty() const;

private:
    uint8_t* memory_;
    size_t totalSize_;
    size_t currentOffset_;
};

/**
 * @brief RAII stack allocator scope guard
 */
class StackAllocatorScope {
public:
    explicit StackAllocatorScope(StackAllocator& allocator)
        : allocator_(allocator)
        , marker_(allocator.getMarker()) {
    }

    ~StackAllocatorScope() {
        allocator_.resetToMarker(marker_);
    }

    StackAllocatorScope(const StackAllocatorScope&) = delete;
    StackAllocatorScope& operator=(const StackAllocatorScope&) = delete;

private:
    StackAllocator& allocator_;
    size_t marker_;
};

/**
 * @brief Global memory pool manager
 */
class MemoryPoolManager {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to instance
     */
    static MemoryPoolManager& getInstance();

    /**
     * @brief Get object pool for specific type
     * @tparam T Object type
     * @return Reference to object pool
     */
    template<typename T>
    ObjectPool<T>& getPool() {
        static ObjectPool<T> pool;
        return pool;
    }

    /**
     * @brief Get stack allocator for current thread
     * @return Reference to stack allocator
     */
    StackAllocator& getStackAllocator();

    /**
     * @brief Get global memory statistics
     * @return Memory statistics
     */
    struct GlobalStatistics {
        size_t totalPoolMemory;
        size_t totalStackMemory;
        size_t activeAllocations;
        size_t peakAllocations;
        double memoryEfficiency;
    };

    GlobalStatistics getGlobalStatistics() const;

    /**
     * @brief Initialize memory pools
     * @param config Configuration JSON
     */
    void initialize(const s3d::JSON& config = {});

    /**
     * @brief Shutdown memory pools
     */
    void shutdown();

    /**
     * @brief Force garbage collection
     */
    void collectGarbage();

private:
    MemoryPoolManager() = default;
    ~MemoryPoolManager() = default;

    mutable std::mutex mutex_;
    std::unordered_map<std::thread::id, std::unique_ptr<StackAllocator>> stackAllocators_;
    std::atomic<size_t> totalMemoryUsage_;
    std::atomic<size_t> peakMemoryUsage_;
};

// Convenience macros for memory pool usage
#define SYLIFE_POOL_ACQUIRE(Type, ...) \
    sylife::utils::MemoryPoolManager::getInstance().getPool<Type>().acquire(__VA_ARGS__)

#define SYLIFE_STACK_SCOPE(allocator) \
    sylife::utils::StackAllocatorScope _stack_scope(allocator)

#define SYLIFE_STACK_ALLOC(allocator, Type, count) \
    static_cast<Type*>((allocator).allocate(sizeof(Type) * (count), alignof(Type)))

} // namespace sylife::utils