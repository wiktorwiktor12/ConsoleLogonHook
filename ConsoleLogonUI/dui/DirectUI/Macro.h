#pragma once

namespace DirectUI
{
	class UILIB_API Macro : public Element
	{
	public:
		Macro(Macro const &);
		Macro(void);
		virtual ~Macro(void);
		Macro & operator=(Macro const &);

		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static PropertyInfo const * __stdcall ExpandProp(void);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);

		void SetDataEntry(IDataEntry *, Element *);;
		long SetExpand(unsigned short const *);
		void SetParser(DUIXmlParser *);
		IDataEntry * GetDataEntry(void);
		unsigned short const * GetExpand(Value * *);
		long Initialize(Element *, unsigned long *);


		virtual long Add(Element * *, unsigned int);
		virtual IClassInfo * GetClassInfoW(void);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
	protected:
		//1
		virtual long BuildElement(void);
		void ResolveBindings(void);

	private:
		static IClassInfo * s_pClassInfo;
	};
}