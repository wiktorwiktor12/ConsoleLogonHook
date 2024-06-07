#pragma once

namespace DirectUI
{
	
	
	class UILIB_API XResourceProvider : IXProviderCP
	{
	public:
		XResourceProvider();
		XResourceProvider( const XResourceProvider& );
		XResourceProvider& operator=( const XResourceProvider& );

		//1
		virtual long CreateDUICP(HWNDElement*, HWND, HWND, Element**, class DUIXmlParser**);
		//2
		virtual long CreateParserCP(class DUIXmlParser** pOut);
		//3
		virtual void DestroyCP();

		static long WINAPI Create(XResourceProvider**pOut);
		static long WINAPI Create(HINSTANCE, UCString, UCString, UCString, XResourceProvider**pOut );
		
		long Initialize(HINSTANCE h, UCString s1, UCString s2, UCString s3);
	};
}