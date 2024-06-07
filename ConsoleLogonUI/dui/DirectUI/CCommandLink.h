#pragma once

namespace DirectUI
{
	class UILIB_API CCCommandLink :public CCPushButton
	{
	public:
		CCCommandLink(CCCommandLink const &);
		CCCommandLink(unsigned long v1=0xE);
		virtual ~CCCommandLink(void);
		CCCommandLink & operator=(CCCommandLink const &);

		static long __stdcall Create(unsigned int, Element *, unsigned long *, Element * *);
		static long __stdcall Create(Element *, unsigned long *, Element * *);
		static IClassInfo * __stdcall GetClassInfoPtr(void);
		static long __stdcall Register(void);
		static void __stdcall SetClassInfoPtr(IClassInfo *);
		static PropertyInfo const * __stdcall NoteProp(void);
		
		UCString GetNote(Value * *);
		long SetNote(UCString);

		virtual IClassInfo * GetClassInfoW(void);
		virtual SIZE GetContentSize(int, int, Surface *);
		virtual void OnPropertyChanged(PropertyInfo const *, int, Value *, Value *);
		virtual void OnSelectedPropertyChanged(void);
	protected:
		void SyncNoteAndGlyph(HWND);
		virtual void PostCreate(HWND);

	private:
		static IClassInfo * s_pClassInfo;
	};
}