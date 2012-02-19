#ifndef TESTCPP_DECLARATIONS_H__
#define TESTCPP_DECLARATIONS_H__

#define TESTCPP_DISABLE_COPY(Cls) \
    Cls(const Cls&); \
    Cls& operator=(const Cls&);

// interface has virtual destructor and no public constructors
#define TESTCPP_INTERFACE(Cls) \
    public: \
        virtual ~Cls() {} \
    protected: \
        Cls() {} \
    private: \
        Cls(const Cls&); \
        Cls& operator=(const Cls&);

// singleton has no public constructors and provides access to instance via
// the static instance() method
#define TESTCPP_SINGLETON(Cls) \
    public: \
        static Cls& instance(); \
    private: \
        Cls(); \
        ~Cls() {} \
        Cls(const Cls&); \
        Cls& operator=(const Cls&);

#endif /* TESTCPP_DECLARATIONS_H__ */
