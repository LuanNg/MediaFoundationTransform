//----------------------------------------------------------------------------------------------
// MFTAsynchronousAudio_Event.cpp
//----------------------------------------------------------------------------------------------
#include "Stdafx.h"

HRESULT CMFTAsynchronousAudio::GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent){

	TRACE_TRANSFORM((L"CMFTAsynchronousAudio::GetEvent"));

	HRESULT hr;

	IMFMediaEventQueue* pQueue = NULL;

	{
		AutoLock lock(m_CriticSection);

		LOG_HRESULT(hr = CheckShutdown());

		if(SUCCEEDED(hr)){
			pQueue = m_pEventQueue;
			pQueue->AddRef();
		}
	}

	if(SUCCEEDED(hr)){
		LOG_HRESULT(hr = pQueue->GetEvent(dwFlags, ppEvent));
	}

	SAFE_RELEASE(pQueue);

	return hr;
}

HRESULT CMFTAsynchronousAudio::BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState){

	TRACE_TRANSFORM((L"CMFTAsynchronousAudio::BeginGetEvent"));

	HRESULT hr;

	AutoLock lock(m_CriticSection);

	IF_FAILED_RETURN(hr = CheckShutdown());

	LOG_HRESULT(hr = m_pEventQueue->BeginGetEvent(pCallback, punkState));

	return hr;
}

HRESULT CMFTAsynchronousAudio::EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent){

	TRACE_TRANSFORM((L"CMFTAsynchronousAudio::EndGetEvent"));

	HRESULT hr;

	AutoLock lock(m_CriticSection);

	IF_FAILED_RETURN(hr = CheckShutdown());

	LOG_HRESULT(hr = m_pEventQueue->EndGetEvent(pResult, ppEvent));

	return hr;
}

HRESULT CMFTAsynchronousAudio::QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue){

	TRACE_TRANSFORM((L"CMFTAsynchronousAudio::QueueEvent : %s", MFEventString(met)));

	HRESULT hr;

	AutoLock lock(m_CriticSection);

	IF_FAILED_RETURN(hr = CheckShutdown());

	LOG_HRESULT(hr = m_pEventQueue->QueueEventParamVar(met, guidExtendedType, hrStatus, pvValue));

	return hr;
}