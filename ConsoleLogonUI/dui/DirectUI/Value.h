#pragma once

namespace DirectUI
{

	enum class ValueType : int {
    Unavailable = -2,
    Unset = -1,
    Null = 0,
    Int = 1,
    Bool = 2,
    Element = 3,
    Ellist = 4,
    String = 5,
    Point = 6,
    Size = 7,
    Rect = 8,
    Color = 9,
    Layout = 10,
		Graphic = 11,
    Sheet = 12,
    Expr = 13,
		Atom = 14,
    Cursor = 15,
		Float = 18,
		DblList = 19,
	};

	class UILIB_API Value
	{
	public:
		Value& operator=(const Value &);
		void AddRef();

		static Value* WINAPI CreateAtom(unsigned short);
		static Value* WINAPI CreateAtom(UCString);
		static Value* WINAPI CreateBool(bool);
		static Value* WINAPI CreateColor(unsigned long);
		static Value* WINAPI CreateColor(unsigned long, unsigned long, unsigned char);
		static Value* WINAPI CreateColor(unsigned long, unsigned long, unsigned long, unsigned char);
		static Value* WINAPI CreateCursor(HICON);
		static Value* WINAPI CreateCursor(UCString);
		static Value* WINAPI CreateDFCFill(unsigned int, unsigned int);
		static Value* WINAPI CreateDTBFill(UCString, int, int);
		static Value* WINAPI CreateElementList(DynamicArray<class Element*, 0>*);
		static Value* WINAPI CreateElementRef(class Element*);
		static Value* WINAPI CreateEncodedString(UCString);
		static Value* WINAPI CreateExpression(Expression*);
		static Value* WINAPI CreateFill(const struct Fill &);
		static Value* WINAPI CreateGraphic(HBITMAP, unsigned char, unsigned int, bool, bool, bool);
		static Value* WINAPI CreateGraphic(HENHMETAFILE, HENHMETAFILE);
		static Value* WINAPI CreateGraphic(HICON, bool, bool, bool);
		static Value* WINAPI CreateGraphic(UCString, unsigned char, unsigned int, unsigned short, unsigned short, HINSTANCE, bool, bool);
		static Value* WINAPI CreateGraphic(UCString, unsigned short, unsigned short, HINSTANCE, bool, bool);
		static Value* WINAPI CreateInt(int);
		static Value* WINAPI CreateLayout(class Layout*);
		static Value* WINAPI CreatePoint(int, int);
		static Value* WINAPI CreateRect(int, int, int, int);
		static Value* WINAPI CreateSize(int, int);
		static Value* WINAPI CreateString(UCString, HINSTANCE);
		static Value* WINAPI CreateStyleSheet(StyleSheet*);

		bool GetBool();
		struct Cursor* GetCursor();
		class Element* GetElement();
		DynamicArray<class Element*, 0>* GetElementList();
		class Expression* GetExpression();
		const Fill* GetFill();
		struct Graphic* GetGraphic();
		void* GetImage(bool);
		int GetInt();
		class Layout* GetLayout();
		POINT const * GetPoint();
		RECT const * GetRect();
		int GetRefCount() const;
		SIZE const * GetSize();
		UCString GetString();
		StyleSheet* GetStyleSheet();
		int GetType() const;

		unsigned short GetAtom();
		static Value* WINAPI GetAtomZero();
		static Value* WINAPI GetBoolFalse();
		static Value* WINAPI GetBoolTrue();
		static Value* WINAPI GetColorTrans();
		static Value* WINAPI GetCursorNull();
		static Value* WINAPI GetElListNull();
		static Value* WINAPI GetElementNull();
		static Value* WINAPI GetExprNull();
		static Value* WINAPI GetIntZero();
		static Value* WINAPI GetLayoutNull();
		static Value* WINAPI GetNull();
		static Value* WINAPI GetPointZero();
		static Value* WINAPI GetRectZero();
		static Value* WINAPI GetSheetNull();
		static Value* WINAPI GetSizeZero();
		static Value* WINAPI GetStringNull();
		static Value* WINAPI GetUnavailable();
		static Value* WINAPI GetUnset();

		bool IsEqual(Value*);
		void Release();
		UString ToString(UString, unsigned int) const;
	private:
		void _ZeroRelease();
		static long WINAPI StrDupW(UCString, UString*);
	};

	class UILIB_API ValueProvider 
		: public PatternProvider<ValueProvider, IValueProvider, 12>
		, public IValueProvider
	{
	public:
		ValueProvider(void);
		virtual ~ValueProvider(void);
		virtual unsigned long __stdcall AddRef(void);
		virtual ProviderProxyCall GetProxyCreator(void);
		virtual long __stdcall QueryInterface(GUID const &, void * *);
		virtual unsigned long __stdcall Release(void);
		virtual long __stdcall SetValue(unsigned short const *);
		virtual long __stdcall get_IsReadOnly(int *);
		virtual long __stdcall get_Value(unsigned short * *);
	};

	class UILIB_API ValueProxy : IProxy
	{
	public:
		ValueProxy(ValueProxy const &);
		ValueProxy(void);
		ValueProxy & operator=(ValueProxy const &);
		static ValueProxy * __stdcall Create(Element *);
		static bool __stdcall IsPatternSupported(Element *);

		virtual long DoMethod(int, char *);

	protected:
		virtual void Init(Element *);

	private:
		long GetIsReadOnly(int *);
		long GetValue(unsigned short * *);
		long SetValue(unsigned short const *);
	};
}