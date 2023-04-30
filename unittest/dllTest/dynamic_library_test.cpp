#include <gtest/gtest.h>
#ifdef _WIN32
#include <windows.h> 
#else
#include <dlfcn.h>
#endif

struct DllHandle
{
    #ifdef _WIN32
    DllHandle(const char * const filename) : h(LoadLibrary(filename)) {}
    ~DllHandle() { if (h) FreeLibrary(h); }
    HINSTANCE Get() const { return h; }

    private:
    HINSTANCE h;

    public:
    static FARPROC GetProc(HINSTANCE h, const char* procName) { return GetProcAddress(h, procName); };
    #else
    DllHandle(const char * const filename) : h(dlopen(filename, RTLD_LAZY)) {}
    ~DllHandle() { if (h) dlclose(h); }
    void* Get() const { return h; }

    private:
    void* h;

    public:
    static void *GetProc(void *h, const char* procName) { return dlsym(h, procName); };
    #endif
};

TEST(DllTest, LoadDll) {
    #ifdef _WIN32
    const DllHandle h("libVCCProjGenerator.dll");
    #else
    const DllHandle h("bin/Debug/libVCCProjGenerator.so");
    if (h.Get() == nullptr) {
        fprintf(stderr, "%s\n", dlerror());
    }
    #endif
    EXPECT_TRUE(h.Get());
    
    typedef long long int (*GetVersionFunction)();
    const GetVersionFunction GetVersion = reinterpret_cast<GetVersionFunction>(DllHandle::GetProc(h.Get(), "GetVersion"));
    EXPECT_TRUE(GetVersion != nullptr);
    EXPECT_EQ(GetVersion(), 1);
}
