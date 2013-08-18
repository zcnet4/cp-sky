#include "StdAfx.h"
#include "WinHttpWrapper.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
// WinHttpObFactory
WinHttpApi WinHttpObFactory::s_kWinHttpApi;
BOOL	   WinHttpObFactory::s_bWinHttpInited = FALSE;

/*
@ FuncName	: Initialize
@ Brief		: 初始化WinHttpApi对象。
*/
BOOL WinHttpObFactory::Initialize()
{
	if (!s_bWinHttpInited)
		s_bWinHttpInited = s_kWinHttpApi.Initialize();

	return s_bWinHttpInited;
}

/*
@ FuncName	: Uninitialize
@ Brief		: 反初始化WinHttpApi对象。
*/
BOOL WinHttpObFactory::Uninitialize()
{
	s_bWinHttpInited = FALSE;
	return s_kWinHttpApi.Uninitialize();
}

/*
@ FuncName	: GetWinHttpOb
@ Brief		: 获取WinHttpApi对象。
*/
WinHttpApi& WinHttpObFactory::GetWinHttpOb()
{
	return s_kWinHttpApi;
}

//////////////////////////////////////////////////////////////////////////
// WinHttpObject
WinHttpObject::WinHttpObject()
	: hSession_(NULL),
	  hConnect_(NULL),
	  hRequest_(NULL)
{

}

WinHttpObject::~WinHttpObject()
{
	
}

long OpenWinHttpRequest(IN WinHttpApi http,
						IN LPCWSTR lpszUrl, 
						OUT HINTERNET& hSession, 
						OUT HINTERNET& hConnect, 
						OUT HINTERNET& hRequest)
{
	long lRetCode = -8;

	const int BUF_LEN = 256;
	WCHAR host[BUF_LEN] = { 0 };
	const WCHAR * pszRequestUrl = NULL;
	// 最大 URL 长度是在 Internet Explorer 中的 2,083 字符
	// WCHAR path[2083 + 1] = { 0 };
	// WCHAR extra[256] = { 0 };

	URL_COMPONENTS uc = { 0 };

	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = host;
	uc.dwHostNameLength = BUF_LEN;
	//uc.lpszUrlPath = path;
	//uc.dwUrlPathLength = BUF_LEN;
	//uc.lpszExtraInfo = extra;
	//uc.dwExtraInfoLength = BUF_LEN;

	// 注意http://www.zcnet4.com/xx/do?z1=1&z2=2
	// 会拆解为 HostName:www.zcnet4.com、UrlPath:xx/do、szExtraInfo:?z1=1&z2=2
	http.CrackedUrl(lpszUrl, &uc);

	// 不想分配内存来做UrlPath和ExtraInfo的合并。by ZC. 2010-3-13 22:20
	pszRequestUrl = _tcsstr(lpszUrl, uc.lpszHostName);
	if (pszRequestUrl)
	{
		pszRequestUrl += (uc.dwHostNameLength/* + 1*/);
		// 支持端口格式。。。
		if (_T(':') == *pszRequestUrl)
		{
			TCHAR szPort[32] = { 0 };
			_stprintf_s(szPort, _T(":%d"), uc.nPort);
			pszRequestUrl += _tcslen(szPort);
		}
	}
	else
		goto Exit0;

	// Open session.
	hSession = http.OpenSession(uc.lpszHostName);
	if (!hSession)
	{
		lRetCode = -2;
		goto Exit0;
	}

	// Connect.
	hConnect = http.Connect(hSession, uc.lpszHostName, uc.nPort);
	if (!hConnect)
	{
		lRetCode = -3;
		goto Exit0;
	}

	// Open request.
	hRequest = http.OpenRequest(hConnect, L"GET", pszRequestUrl, uc.nScheme);
	if (!hRequest)
	{
		lRetCode = -4;
		goto Exit0;
	}
	else
	{
		// 连接超时10秒，发送超时20秒，接收超时40秒。by ZC. 2010-8-17.
		http.SetTimeouts(hRequest, 0, 10000 , 20000, 40000);
	}

	lRetCode = 0;
Exit0:
	return lRetCode;
}

long WinHttpObject::SetWinHttpObject(LPCWSTR lpszUrl)
{
	WinHttpApi& _kWinHttp = WinHttpObFactory::GetWinHttpOb();

	return OpenWinHttpRequest(_kWinHttp, lpszUrl, hSession_, hConnect_, hRequest_);
}

BOOL WinHttpObject::SetProxy(LPCWSTR pszServerAndPort, LPCWSTR pszUserName, LPCWSTR pszPassword)
{
	WinHttpApi& _kWinHttp = WinHttpObFactory::GetWinHttpOb();

	return _kWinHttp.SetProxy(pszServerAndPort, pszUserName, pszPassword);
}

BOOL WinHttpObject::AddHttpHead(LPCWSTR pszHttpHead)
{
	WinHttpApi& _kWinHttp = WinHttpObFactory::GetWinHttpOb();

	return _kWinHttp.AddRequestHeaders(hRequest_, pszHttpHead);
}

long WinHttpObject::SendHttpRequest(IOutStream * _pIOutStream,
									IStatusCallback * _pIStatusCallback/* = NULL*/)
{
	long lResult = -11;

	WinHttpApi& _kWinHttp = WinHttpObFactory::GetWinHttpOb();

	// Send post data.
	if (!_kWinHttp.SendRequest(hRequest_, NULL, NULL)) 
	{
		lResult = ::GetLastError();
		if (_pIStatusCallback)
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_ErrorCode, lResult);
		goto Exit0;
	}

	// End request
	if (!_kWinHttp.EndRequest(hRequest_))
	{
		lResult = ::GetLastError();
		if (_pIStatusCallback)
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_ErrorCode, lResult);
		goto Exit0;
	}

	// 接收数据：
	const DWORD cdwSize = 1024;
	char szBuf[cdwSize + 2] = { 0 };
	DWORD dwSize = cdwSize;
	// Query header info.
#ifdef USE_WINHTTP
	int contextLengthId = WINHTTP_QUERY_CONTENT_LENGTH;
	int statusCodeId = WINHTTP_QUERY_STATUS_CODE;
	int statusTextId = WINHTTP_QUERY_STATUS_TEXT;
#else
	int contextLengthId = HTTP_QUERY_CONTENT_LENGTH;
	int statusCodeId = HTTP_QUERY_STATUS_CODE;
	int statusTextId = HTTP_QUERY_STATUS_TEXT;
#endif
	dwSize = cdwSize;
	ZeroMemory(szBuf, sizeof(szBuf));
	if (_kWinHttp.QueryInfo(hRequest_, statusCodeId, szBuf, &dwSize))
	{
		szBuf[dwSize] = 0;
		u_int uStatusCode = (u_int)_wtoi64((WCHAR*)szBuf);

		if (200 == uStatusCode)
			lResult = 0;
		else
			lResult = uStatusCode;

		if (_pIStatusCallback)
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_StatusCode, uStatusCode);
	}

	dwSize = cdwSize;
	ZeroMemory(szBuf, sizeof(szBuf));
	if (_kWinHttp.QueryInfo(hRequest_, contextLengthId, szBuf, &dwSize))
	{
		szBuf[dwSize] = 0;
		if (_pIStatusCallback)
		{
			WCHAR * _pNum = (WCHAR*)szBuf;
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_ContentLength, (int)_wtoi64(_pNum));
		}
	}

#ifdef _DEBUG
	dwSize = cdwSize;
	ZeroMemory(szBuf, sizeof(szBuf));
	if (_kWinHttp.QueryInfo(hRequest_, statusTextId, szBuf, &dwSize))
		szBuf[dwSize] = 0;
#endif

	// read data.
	while (true)
	{
		dwSize = cdwSize;
		if (!_kWinHttp.ReadData(hRequest_, szBuf, dwSize, &dwSize))
		{
			if (_pIStatusCallback)
				_pIStatusCallback->OnStatusCallback(enumHttpStatus_ErrorCode, ::GetLastError());

			break;
		}

		if (dwSize <= 0)
			break;

		szBuf[dwSize] = 0;

		if (_pIOutStream)
			_pIOutStream->Write((c_byte*)szBuf, dwSize, NULL);

		if (_pIStatusCallback)
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_Progress, dwSize);
	}

Exit0:

	{
		_kWinHttp.CloseInternetHandle(hRequest_);
		_kWinHttp.CloseInternetHandle(hConnect_);
		_kWinHttp.CloseInternetHandle(hSession_);
		hRequest_ = NULL;
		hConnect_ = NULL;
		hSession_ = NULL;
	}

	return lResult;
}

//////////////////////////////////////////////////////////////////////////
// WinHttpWrapper

long WinHttpWrapper::SendHttpRequest(LPCWSTR lpszUrl,
									 IOutStream * _pIOutStream,
									 IStatusCallback * _pIStatusCallback/* = NULL*/)
{
	long lResult = -11;
		
	WinHttpObject _WinHttpOb;

	lResult = _WinHttpOb.SetWinHttpObject(lpszUrl);
	if (0 != lResult)
	{
		if (_pIStatusCallback)
			_pIStatusCallback->OnStatusCallback(enumHttpStatus_ErrorCode, lResult);
		goto Exit0;
	}
	
	_WinHttpOb.AddHttpHead(_T("UserAgent:Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0;)"));

	lResult = _WinHttpOb.SendHttpRequest(_pIOutStream, _pIStatusCallback);

Exit0:
	return lResult;
}

//////////////////////////////////////////////////////////////////////////
// WinHttpHandle

WinHttpHandle::WinHttpHandle(IN WinHttpApi& _WinHttp)
	: WinHttp_(_WinHttp),
	  m_handle(NULL)
{

}

WinHttpHandle::~WinHttpHandle()
{
  Close();
}

BOOL WinHttpHandle::Attach(HINTERNET handle)
{
  ASSERT(0 == m_handle);
  m_handle = handle;
  return 0 != m_handle;
}

HINTERNET WinHttpHandle::Detach()
{
  HANDLE handle = m_handle;
  m_handle = 0;
  return handle;
}

void WinHttpHandle::Close()
{
  if (0 != m_handle)
  {   
	  WinHttp_.CloseInternetHandle(m_handle);
	  m_handle = 0;
  }
}

HRESULT WinHttpHandle::SetOption(DWORD option, const void* value, DWORD length)
{
	/*if (!::WinHttpSetOption(m_handle,
	  option,
	  const_cast<void*>(value),
	  length))
  {
	  return HRESULT_FROM_WIN32(::GetLastError());
  }*/

  return S_OK;
}

HRESULT WinHttpHandle::QueryOption(DWORD option, void* value, DWORD& length) const
{
  /*if (!::WinHttpQueryOption(m_handle,
	  option,
	  value,
	  &length))
  {
	  return HRESULT_FROM_WIN32(::GetLastError());
  }*/

  return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// WinHttpSession
WinHttpSession::WinHttpSession(IN WinHttpApi& _WinHttp)
	: WinHttpHandle(_WinHttp)
{

}
