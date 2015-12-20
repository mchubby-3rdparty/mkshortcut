/******************************************************************************
Copyright 2010 Daniel Miranda. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include <iostream>
#include <wchar.h>

#define _UNICODE
#define NTDDI_VERSION NTDDI_WIN7
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <shlwapi.h>
#include <shobjidl.h>

#include <propsys.h>
#include <propkey.h>
#include <propvarutil.h>

#include "help_text.h"

enum {
	SHL_OK = 0,
	SHL_ERR_INVALID_FILENAME,
	SHL_ERR_INVALID_TARGET,
	SHL_ERR_SAVE_FAIL,
	SHL_ERR_GENERAL
};

enum {
	SWITCH_OUTPUT,
	SWITCH_TARGET,
	SWITCH_PARAMS,
	SWITCH_WDIR,
	SWITCH_APPID,
	SWITCH_RELCMD,
	SWITCH_RELNAME,

	SWITCH_MAX
};

const wchar_t switches[SWITCH_MAX][32] = {
	L"-output",
	L"-target",
	L"-parameters",
	L"-workingdir",
	L"-appid",
	L"-relaunchcmd",
	L"-relaunchname"
};

HRESULT SetPropValueString(IPropertyStore* pps, PROPERTYKEY key, PROPVARIANT *pv, wchar_t* value) {
	if(!pps || !value) return E_POINTER;

	HRESULT hr = InitPropVariantFromString(value, pv);
	if(SUCCEEDED(hr)) {
		hr = pps->SetValue(key, *pv);
		PropVariantClear(pv);
	}
	
	return hr;
};

int createShortcut(wchar_t* filename,
				   wchar_t* target,
				   wchar_t* arguments   = NULL,
				   wchar_t* workingdir  = NULL,
				   wchar_t* appid       = NULL,
				   wchar_t* relaunchcmd = NULL,
				   wchar_t* relaunchname = NULL
){
	// argument checking
	if(filename == NULL || !wcslen(filename)) {
		#ifdef DEBUG
		std::wcout << "ERROR: Invalid filename." << std::endl;
		#endif
		
		return SHL_ERR_INVALID_FILENAME;
	} else if (target == NULL || !wcslen(target)) {
		#ifdef DEBUG
		std::wcout << "ERROR: Invalid target." << std::endl;
		#endif
		
		return SHL_ERR_INVALID_TARGET;
	}
	//

	// create ShellLink
	IShellLinkW* psl = NULL;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL,
		CLSCTX_INPROC_SERVER, IID_IShellLinkW, 
		reinterpret_cast<void**>(&psl));
	
	if(FAILED(hr)) {
		#ifdef DEBUG
		std::wcout << "ERROR: Failed instantiating IShellLink interface." << std::endl;
		#endif
		
		return SHL_ERR_GENERAL;
	}
	//

	// Edit shortcut's properties.
	if(appid != NULL || relaunchcmd != NULL) {
		IPropertyStore *pps = NULL;
		hr = psl->QueryInterface(IID_IPropertyStore,
			reinterpret_cast<void**>(&pps));
		
		if(SUCCEEDED(hr)) {
			PROPVARIANT pv;
			PropVariantInit(&pv);
			
			hr = SetPropValueString(pps, PKEY_AppUserModel_ID,
				&pv, appid);
			hr = SetPropValueString(pps, PKEY_AppUserModel_RelaunchCommand,
				&pv, relaunchcmd);
			hr = SetPropValueString(pps, PKEY_AppUserModel_RelaunchDisplayNameResource,
				&pv, relaunchname);

			pps->Release();
		}
	}
	//

	// set target/arguments
	psl->SetPath(target);
	if(arguments != NULL)
		psl->SetArguments(arguments);
	if(workingdir != NULL)
		psl->SetWorkingDirectory(workingdir);


	// save to disk
	IPersistFile* ppf = NULL;
	hr = psl->QueryInterface(IID_IPersistFile,
		reinterpret_cast<void**>(&ppf));

	if(SUCCEEDED(hr)) {
		hr = ppf->Save(filename, TRUE);
		ppf->Release();
		
		if(SUCCEEDED(hr)) {
			psl->Release();
			return SHL_OK;
		}
	};

	#ifdef DEBUG
	std::wcout << "ERROR: Failed saving shortcut." << std::endl;
	#endif
		
	psl->Release();
	return SHL_ERR_SAVE_FAIL;
};

int wmain(int argc, wchar_t** argv) {
	if(argc < 2) {
		std::wcout << g_helpText << std::endl;
		return 0;
	}

	int nextSwitch = -1;

	wchar_t *values[SWITCH_MAX] = {NULL};
	wchar_t *current = NULL;

	for(int i=1; i <= argc; i++) {
		current = argv[i];
		if(!current) break;

		if(nextSwitch >= 0) {
			values[nextSwitch] = current;
			nextSwitch = -1;
			continue;
		}
		
		// show help
		if(!_wcsicmp(current,  L"/?")
		||	!_wcsicmp(current, L"-?")
		||	!_wcsicmp(current, L"-help")
		){
			std::wcout << g_helpText << std::endl;
			return 0;
		};

		for(int j=0; j < SWITCH_MAX; j++) {
			if(!_wcsicmp(current, switches[j])) {
				nextSwitch = j;
				break;
			}
		}
		if(nextSwitch >= 0)
			continue;
		
		std::wcout << "ERROR: Invalid argument - '" << *current << "'" << std::endl;
		return 1;
	}
	
	CoInitialize(NULL);

	int ret = createShortcut(values[SWITCH_OUTPUT],
		values[SWITCH_TARGET], values[SWITCH_PARAMS],
		values[SWITCH_WDIR], values[SWITCH_APPID],
		values[SWITCH_RELCMD], values[SWITCH_RELNAME]);
	
	switch(ret) {
		case SHL_OK:
			std::wcout << values[SWITCH_OUTPUT] << " => OK" << std::endl;
		break;
		case SHL_ERR_INVALID_FILENAME:
			std::wcout << "ERROR: Invalid output file name." << std::endl;
		break;
		case SHL_ERR_INVALID_TARGET:
			std::wcout << "ERROR: Invalid shortcut target file name." << std::endl;
		break;
		case SHL_ERR_SAVE_FAIL:
			std::wcout << "ERROR: Failed to save shortcut." << std::endl;
		break;
		case SHL_ERR_GENERAL:
			std::wcout << "ERROR: An unknown error occurred." << std::endl;
		break;
	};

	CoUninitialize();

	return (ret == SHL_OK) ? 0 : 1;
};





