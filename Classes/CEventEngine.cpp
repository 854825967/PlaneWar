#include "CEventEngine.h"

CEventEngine * g_pEventEngine = CEventEngine::getInterface();

CEventEngine * CEventEngine::s_pSelf = NULL;

CEventEngine * CEventEngine::getInterface() {
    if (NULL ==  s_pSelf) {
        s_pSelf = NEW CEventEngine;
    }
    
    return s_pSelf;
}

CEventEngine::~CEventEngine() {
    s_pSelf = NULL;
}

CEventEngine::CEventEngine() {
    
}

void CEventEngine::RegisterStaticEvent(const s32 nEventID, const system_call call, const char * debug) {
    m_EventPool.RegisterCall(nEventID, call, debug);
}

void CEventEngine::UnRegisterStaticEvent(const s32 nEventID, const system_call call) {
    m_EventPool.UnRgsCall(nEventID, call);
}

void CEventEngine::EventCall(const s32 nEventID, void * pContext, const s32 size) {
    m_EventPool.Call(nEventID, pContext, size);
}

//===============华丽的分割线=============
void CEventEngine::Bind(const s32 nEventID, void * pBundler, member_address address, const char * debug) {
    m_BundlerPool.bind(nEventID, pBundler, address, debug);
}

void CEventEngine::UnBind(const s32 nEventID, void * pBundler, member_address address) {
    m_BundlerPool.unbind(nEventID, pBundler, address);
}

void CEventEngine::UnBind(void * pBundler) {
    m_BundlerPool.unbind(pBundler);
}

void CEventEngine::BundlerCall(const s32 nEventID, void * pContext, const s32 size) {
    m_BundlerPool.call_bundler(nEventID, pContext, size);
}
