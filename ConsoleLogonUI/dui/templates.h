#pragma once
#include "dui_includes.h"

namespace DirectUI
{
	template<typename Class, typename BaseClass>
	class ClassInfo : ClassInfoBase
	{
	public:
		HRESULT CreateInstance(Element* rootElement, unsigned long* unknown, Element** result) override
		{
			return Class::CreateInstance(rootElement, unknown, result);
		}
		IClassInfo* GetBaseClass() override
		{
			return BaseClass::GetClassInfoPtr();
		}
		HRESULT DoRegister() { return ClassInfoBase::Register(); }

		void Destroy() override
		{
			delete this;
			Class::Class = NULL;
		}

		static HRESULT Create(ClassInfo** result, HINSTANCE hinst)
		{
			ClassInfo* theClass = new ClassInfo();

			memset(theClass->data, 0, sizeof(theClass->data));

			HRESULT hr = theClass->Initialize(hinst, (UCString)Class::DoGetClassName(), false, NULL, 0);
			if (SUCCEEDED(hr))
			{
				*result = theClass;
			}
			return hr;
		}
		static HRESULT Register(HINSTANCE hinst)
		{
			HRESULT hr = S_OK;

			if (BaseClass::GetClassInfoPtr() == NULL)
			{
				hr = BaseClass::Register();
			}
			else
			{
				BaseClass::GetClassInfoPtr()->AddRef();
			}

			if (SUCCEEDED(hr))
			{
				CritSecLock lock(Element::GetFactoryLock());

				IClassInfo* baseclassptr = BaseClass::GetClassInfoPtr();
				IClassInfo* newrclass = Class::GetClassInfoPtr();

				if (DirectUI::ClassInfoBase::ClassExist(&baseclassptr, NULL, 0, newrclass, hinst, (UCString)Class::DoGetClassName(), false))
				{
					Class::Class = baseclassptr;
				}
				else
				{
					ClassInfo* classInfo = NULL;
					if (SUCCEEDED(hr = Create(&classInfo, hinst)))
					{
						if (SUCCEEDED(hr = classInfo->DoRegister()))
						{
							Class::Class = (IClassInfo*)classInfo;
						}
						else
						{
							classInfo->Destroy();
						}
					}
				}
			}
			return hr;
		}
	};

	template<typename Class, typename BaseClass>
	class ClassInfoNoCreator : ClassInfoBase
	{
	public:
		IClassInfo* GetBaseClass() override
		{
			return BaseClass::GetClassInfoPtr();
		}
		HRESULT DoRegister() { return ClassInfoBase::Register(); }

		HRESULT CreateInstance(Element* rootElement, unsigned long* unknown, Element** result) override
		{
			return E_NOTIMPL;
		}

		void Destroy() override
		{
			delete this;
			Class::Class = NULL;
		}

		static HRESULT Create(ClassInfoNoCreator** result, HINSTANCE hinst)
		{
			ClassInfoNoCreator* theClass = new ClassInfoNoCreator();

			memset(theClass->data, 0, sizeof(theClass->data));

			HRESULT hr = theClass->Initialize(hinst, (UCString)Class::DoGetClassName(), false, NULL, 0);
			if (SUCCEEDED(hr))
			{
				*result = theClass;
			}
			return hr;
		}

		static HRESULT Register(HINSTANCE hinst)
		{
			HRESULT hr = S_OK;

			if (BaseClass::GetClassInfoPtr() == NULL)
			{
				hr = BaseClass::Register();
			}
			else
			{
				BaseClass::GetClassInfoPtr()->AddRef();
			}

			if (SUCCEEDED(hr))
			{
				CritSecLock lock(Element::GetFactoryLock());

				IClassInfo* baseclassptr = BaseClass::GetClassInfoPtr();
				IClassInfo* newrclass = Class::GetClassInfoPtr();

				if (DirectUI::ClassInfoBase::ClassExist(&baseclassptr, NULL, 0, newrclass, hinst, (UCString)Class::DoGetClassName(), false))
				{
					Class::Class = baseclassptr;
				}
				else
				{
					ClassInfoNoCreator* classInfo = NULL;
					if (SUCCEEDED(hr = Create(&classInfo, hinst)))
					{
						if (SUCCEEDED(hr = classInfo->DoRegister()))
						{
							Class::Class = (IClassInfo*)classInfo;
						}
						else
						{
							classInfo->Destroy();
						}
					}
				}
			}
			return hr;
		}
	};
}