#include "DumpClient.h"
#include <filesystem>

#ifdef WIN32
#define OPT_ARGS_IMPL , GBCustomInfos customInfo, const WCHAR *pipeName
#else
#define OPT_ARGS_IMPL
#endif

namespace dmphelper {
Client::Client(String strDumpPath OPT_ARGS_IMPL)
    : m_strDumpPath(std::move(strDumpPath))
#if defined(WIN32)
    , m_infos(std::move(customInfo))
#endif
{
    m_strDumpPath += STR("/gd_ext_dmp");
    if (!std::filesystem::exists(m_strDumpPath)) {
        std::error_code ec;
        std::filesystem::create_directories(m_strDumpPath);
    }
#if defined(WIN32)
    memset(&m_clientInfo, 0, sizeof(m_clientInfo));
    if (!m_infos.empty() && pipeName != nullptr) {
        m_clientInfo.entries = &m_infos[0];
        m_clientInfo.count   = m_infos.size();
    }
#endif
#if defined(__ANDROID__) || defined(__OHOS__) || (__linux__)
    m_descriptor = std::make_unique<gb::MinidumpDescriptor>(m_strDumpPath);
#endif
    m_eh = std::unique_ptr<gb::ExceptionHandler>(new gb::ExceptionHandler(
#if defined(WIN32)
        m_strDumpPath,
        nullptr,
        DumpCallback,
        nullptr,
        gb::ExceptionHandler::HANDLER_ALL,
        MiniDumpNormal,
        pipeName,
        m_clientInfo.count == 0 ? nullptr : &m_clientInfo
#elif defined(__ANDROID__) || defined(__OHOS__)
        *m_descriptor,
        nullptr,
        DumpCallback,
        nullptr,
        true,
        -1
#elif defined(__APPLE__)
        m_strDumpPath,
        nullptr,  // filter
        DumpCallback,
        nullptr,  // context
        true,     // install handler
        nullptr   // custom info
#endif
        ));
}

void Client::Instancitiate(String strDumpPath OPT_ARGS_IMPL) {
    static Client instance(std::move(strDumpPath)
#if defined(WIN32)
                           , std::move(customInfo), pipeName
#endif
    );
}

bool Client::DumpCallback(
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
) {
    return succeeded;
}
}  // namespace dmphelper
