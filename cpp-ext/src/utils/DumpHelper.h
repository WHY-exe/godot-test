#pragma once
#if defined(__APPLE__)
#include <TargetConditionals.h>
#if defined(TARGET_OS_IPHONE) && defined(TARGET_OS_MACCATALYST)
// define something for Mac's Catalyst
#include <client/ios/exception_handler_no_mach.h>
#elif TARGET_OS_IPHONE
#else
// define something for OSX
#include <client/mac/handler/exception_handler.h>
#endif
#elif defined(WIN32)
#include <client/windows/handler/exception_handler.h>
#elif defined(__ANDROID__) || defined(__linux__) || (__OHOS__)
#include <client/linux/handler/exception_handler.h>
#include <client/linux/handler/minidump_descriptor.h>
#endif
#include <memory>
#include <string>

#ifdef WIN32
#define STR(raw_str) L ##raw_str
#define OPT_ARGS , GBCustomInfos customInfo = {}, const WCHAR* pipeName = nullptr
#else
#define STR(raw_str) raw_str
#define OPT_ARGS
#endif

namespace dmphelper {
#ifdef WIN32
using String = std::wstring;
#else
using String = std::string;
#endif

namespace gb = google_breakpad;

class Client {
    using GBExceptHandler = std::unique_ptr<gb::ExceptionHandler>;
#if defined (WIN32)
    using GBCustomInfos   = std::vector<gb::CustomInfoEntry>;
#elif defined(__ANDROID__) || defined(__OHOS__)
    using GBDescriptor = std::unique_ptr<gb::MinidumpDescriptor>;
#endif
public:
    static void Instancitiate(String strDumpPath OPT_ARGS);

private:
    Client(String strDumpPath OPT_ARGS);
    static bool DumpCallback(
#if defined(WIN32)
        const wchar_t*      dump_path,
        const wchar_t*      minidump_id,
        void*               context,
        EXCEPTION_POINTERS* exinfo,
        MDRawAssertionInfo* assertion,
        bool                succeeded
#elif defined(__ANDROID__) || defined(__OHOS__)
        const gb::MinidumpDescriptor& descriptor,
        void*                         context,
        bool                          succeeded
#elif defined(__APPLE__)
        const char* dump_dir,
        const char* minidump_id,
        void*       context,
        bool        succeeded
#endif
    );

private:
    String               m_strDumpPath;
    GBExceptHandler      m_eh;
#if defined (WIN32)
    GBCustomInfos        m_infos;
    gb::CustomClientInfo m_clientInfo;
#elif defined(__ANDROID__) || defined(__OHOS__)
    GBDescriptor m_descriptor;
#endif
};
}  // namespace dmphelper