#pragma once

namespace RmlUiX
{
    namespace internal
    {
        template <typename T, typename Releaser>
        class RmlUiXElementSharedPointer
        {
        public:
            RmlUiXElementSharedPointer(T* ptr) : ptr(ptr) { if (ptr != nullptr) incrementRefCount(); }
            RmlUiXElementSharedPointer() {}
            RmlUiXElementSharedPointer(const RmlUiXElementSharedPointer& other)
            {
                this->ptr = other.ptr;
                if (this->ptr != nullptr)
                    this->incrementRefCount();
            }
            RmlUiXElementSharedPointer(RmlUiXElementSharedPointer&& other) noexcept { this->operator=(std::move(other)); }
            RmlUiXElementSharedPointer& operator=(const RmlUiXElementSharedPointer& other)
            {
                if (this->ptr == other.ptr)
                    return *this;
                this->reset();
                this->ptr = other.ptr;
                if (this->ptr != nullptr)
                    this->incrementRefCount();
                return *this;
            }
            RmlUiXElementSharedPointer& operator=(RmlUiXElementSharedPointer&& other)
            {
                if (this->ptr == other.ptr)
                    return *this;
                this->reset(); this->ptr = other.ptr; other.ptr = nullptr; return *this;
            }
            void reset()
            {
                if (ptr != nullptr)
                {
                    decrementRefCount();
                    ptr = nullptr;
                }
            }
            explicit operator bool() const { return ptr != nullptr;}
            T* get() const { return ptr;}
            T* operator->() const { return ptr; }
            T& operator*() const { return *ptr; }
        private:
            void decrementRefCount() const
            {
                --ptr->refCount;
                if (ptr->refCount == 0)
                    Releaser{}(ptr);
            }
            void incrementRefCount()
            {
                ++ptr->refCount;
            }
        public:
            unsigned int refCount() const
            {
                return ptr->refCount;
            }
        public:
            ~RmlUiXElementSharedPointer()
            {
                reset();
            }
        private:
            T* ptr = nullptr;
        };
    }
}