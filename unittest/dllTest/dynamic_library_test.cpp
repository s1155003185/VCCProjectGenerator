#include <gtest/gtest.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <string>

#define DLL_NAME "libvpg"

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
    std::string dllName = DLL_NAME;
    #ifdef _WIN32
    dllName += ".dll";
    const DllHandle h(dllName.c_str());
    #else
    dllName = "bin/Debug/" + dllName + ".so";
    const DllHandle h(dllName.c_str());
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
