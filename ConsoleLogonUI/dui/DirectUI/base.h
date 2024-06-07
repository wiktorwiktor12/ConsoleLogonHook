#pragma once

namespace DirectUI
{
	class UILIB_API ClassInfoBase //: IClassInfo
	{
	public:
		ClassInfoBase(ClassInfoBase const &);
		ClassInfoBase();
		ClassInfoBase & operator=(ClassInfoBase const &);

		//0
		virtual void AddRef();
		//1
		virtual int Release();

		//2
		virtual HRESULT WINAPI CreateInstance(Element*, unsigned long*, Element** ) = 0;

		//3
		virtual const PropertyInfo* EnumPropertyInfo(unsigned int);
		//4
		virtual const PropertyInfo* GetByClassIndex(unsigned int);
		//5
		virtual unsigned int GetPICount() const;
		//6
		virtual unsigned int GetGlobalIndex() const;

		//NULL
		virtual IClassInfo* WINAPI GetBaseClass() = 0;

		//6
		virtual UCString GetName() const;
		//7
		virtual bool IsValidProperty(const PropertyInfo*) const;
		//8
		virtual bool IsSubclassOf(IClassInfo*) const;
		//9
		virtual void Destroy();
		//10
		virtual HINSTANCE GetModule() const;
		//11
		virtual bool IsGlobal() const;

		//12
		virtual void AddChild();
		//13
		virtual void RemoveChild();
		//14
		virtual int GetChildren() const;
		//15
		virtual void AssertPIZeroRef() const;

		virtual ~ClassInfoBase() ;

		static bool WINAPI ClassExist(IClassInfo**, const PropertyInfo* const*, unsigned int, IClassInfo*, HINSTANCE, UCString, bool);
		long Initialize(HINSTANCE, UCString name, bool hasPropertyInfo, const PropertyInfo* const* properties, unsigned int properties_count);
		long Register();
	//private:
		char data[0x10];
	};


	class UILIB_API BaseScrollBar
	{
	public:
		BaseScrollBar(BaseScrollBar const &);
		BaseScrollBar();
		BaseScrollBar & operator=(BaseScrollBar const &);
		//0
		virtual Element* GetElement() = 0;
		//1
		virtual int GetPosition() = 0;
		//2
		virtual int GetMaximum() = 0;
		//3
		virtual int GetMinimum() = 0;
		//4
		virtual int GetPage() = 0;
		//5
		virtual int GetLine() = 0;
		//6
		virtual long SetPosition(int) = 0;
		//7
		virtual long SetMaximum(int) = 0;
		//8
		virtual long SetMinimum(int) = 0;
		//9
		virtual long SetPage(int) = 0;
		//10
		virtual long SetLine(int) = 0;
		//11
		virtual bool CanSetFocus() = 0;
		//12
		virtual void LineUp(unsigned int);
		//13
		virtual void LineDown(unsigned int);
		//14
		virtual void PageUp(unsigned int);
		//15
		virtual void PageDown(unsigned int);
		//16
		virtual void End();
		//17
		virtual void Home();

		bool IsPinned();
		bool IsScrollable();
		void OnMaximumChanged(Value*);
		void OnMinimumChanged(Value*);
		void OnPageChanged(Value*);
		bool OnPageChanging(Value*);
		void OnPositionChanged(Value*);
		bool OnPositionChanging(Value*);
		static UID WINAPI Scroll();
		void SetPinned(bool);
	private:
		int GetPageInc();
	};

	class UILIB_API BaseScrollViewer : public Element, public IElementListener
	{
	public:
		BaseScrollViewer(BaseScrollViewer const &);
		BaseScrollViewer();
		//0
		virtual ~BaseScrollViewer();
		BaseScrollViewer & operator=(BaseScrollViewer const &);

		long Initialize(Element*, unsigned long*);

		//1
		virtual bool OnListenedPropertyChanging(Element*, const PropertyInfo*, int, Value*, Value*);

		//2
		virtual void OnPropertyChanged(const PropertyInfo*, int, Value*, Value*);

		//3
		virtual void OnInput(InputEvent*);

		//4
		virtual void OnEvent(Event*);

		//5
		virtual long Add(Element**, unsigned int);

		//6
		virtual IClassInfo* GetClassInfoW();
		int GetPinning();
		int GetXBarVisibility();
		int GetXOffset();
		bool GetXScrollable();
		int GetYBarVisibility();
		int GetYOffset();
		bool GetYScrollable();

		//第二个类虚函数表
		//0
		virtual void OnListenerAttach(Element*);
		//1
		virtual void OnListenerDetach(Element*);
		//2
		virtual bool OnPropertyChanging(const PropertyInfo*, int, Value*, Value*);
		//3
		virtual void OnListenedPropertyChanged(Element*, const PropertyInfo*, int, Value*, Value*);
		//4
		virtual void OnListenedEvent(Element*, Event*);
		//5
		virtual void OnListenedInput(Element*, InputEvent*);


		long SetPinning(int);
		long SetXBarVisibility(int);
		long SetXOffset(int);
		long SetXScrollable(bool);
		long SetYBarVisibility(int);
		long SetYOffset(int);
		long SetYScrollable(bool);

		static IClassInfo* WINAPI GetClassInfoPtr();

		static long WINAPI Register();
		static void WINAPI SetClassInfoPtr(IClassInfo*);

		static const PropertyInfo* WINAPI PinningProp();
		static const PropertyInfo* WINAPI XBarVisibilityProp();
		static const PropertyInfo* WINAPI XOffsetProp();
		static const PropertyInfo* WINAPI XScrollableProp();
		static const PropertyInfo* WINAPI YBarVisibilityProp();
		static const PropertyInfo* WINAPI YOffsetProp();
		static const PropertyInfo* WINAPI YScrollableProp();
	
	protected:
		void FireAnimationChangeEvent(bool);

	private:
		void CheckScroll(BaseScrollBar*, int, int, int);
		static IClassInfo* s_pClassInfo;
	};
}