#pragma once
#include "common.h"
#include "WinHttpApi.h"
#include "iseqstream.h"

enum enumKHttpStatus
{
	enumHttpStatus_Progress			= 1,
	enumHttpStatus_ErrorCode		= 2,
	enumHttpStatus_StatusCode		= 3,
	enumHttpStatus_ContentLength	= 4,
};

interface IStatusCallback
{
	virtual BOOL OnStatusCallback(enumKHttpStatus uStatus, int nValue) = 0;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpObFactory
class WinHttpObFactory
{
public:
	/*
	@ FuncName	: Initialize
	@ Brief		: 初始化WinHttpApi对象。
	*/
	static BOOL Initialize();
	/*
	@ FuncName	: Uninitialize
	@ Brief		: 反初始化WinHttpApi对象。
	*/
	static BOOL Uninitialize();
public:
	/*
	@ FuncName	: GetWinHttpOb
	@ Brief		: 获取WinHttpApi对象。
	*/
	static WinHttpApi& GetWinHttpOb();
protected:
	static WinHttpApi s_kWinHttpApi;
	static BOOL		  s_bWinHttpInited;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpObject
class WinHttpObject
{
public:
	WinHttpObject();
	~WinHttpObject();
public:
	long SetWinHttpObject(LPCWSTR lpszUrl);

	BOOL SetProxy(LPCWSTR pszServerAndPort, LPCWSTR pszUserName, LPCWSTR pszPassword);

	BOOL AddHttpHead(LPCWSTR pszHttpHead);

	long SendHttpRequest(IOutStream * _pIOutStream,
						 IStatusCallback * _pIStatusCallback = NULL);
protected:
	HINTERNET	hSession_; 
	HINTERNET	hConnect_; 
	HINTERNET	hRequest_;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpWrapper
class WinHttpWrapper
{
public:
	static long SendHttpRequest(LPCWSTR lpszUrl,
								IOutStream * _pIOutStream,
								IStatusCallback * _pIStatusCallback = NULL);
};

//////////////////////////////////////////////////////////////////////////
// WinHttpHandle
class WinHttpHandle
{
public:
	WinHttpHandle(IN WinHttpApi& _WinHttp);
	~WinHttpHandle();
public:
	BOOL Attach(HINTERNET handle);
	
	HINTERNET Detach();
	
	void Close();

	HRESULT SetOption(DWORD option, const void* value, DWORD length);

	HRESULT QueryOption(DWORD option, void* value, DWORD& length) const;
protected:
	HINTERNET m_handle;
protected:
	WinHttpApi& WinHttp_;
};

//////////////////////////////////////////////////////////////////////////
// WinHttpSession
class WinHttpSession 
	: public WinHttpHandle
{
public:
	WinHttpSession(IN WinHttpApi& _WinHttp);
public:
	HRESULT Initialize();
};
