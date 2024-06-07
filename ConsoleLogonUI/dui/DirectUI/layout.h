#pragma once

namespace DirectUI
{
	class UILIB_API Layout
	{
	public:
		Layout(const Layout &);
		Layout();
		Layout & operator=(const Layout &);

		static HRESULT WINAPI Create(Layout**);
		static void WINAPI UpdateLayoutRect(Element*, int, int, Element*, int, int, int, int);

		void Destroy();
		Element* GetChildFromLayoutIndex(Element*, int, DynamicArray<Element*, 0>*);
		unsigned int GetLayoutChildCount(Element*);
		int GetLayoutIndexFromChild(Element*, Element*);
		void Initialize();
		//0
		virtual void DoLayout(Element*, int, int);

		//1
		virtual SIZE UpdateDesiredSize(Element*, int, int, Surface*);
		//2
		virtual void OnAdd(Element*, Element**, unsigned int);
		//3
		virtual void OnRemove(Element*, Element**, unsigned int);
		//4
		virtual void OnLayoutPosChanged(Element*, Element*, int, int);

		//5
		virtual void Attach(Element*);
		//6
		virtual void Detach(Element*);
		//7
		virtual Element* GetAdjacent(Element*, Element*, int, NavReference const*, unsigned long);
		//8
		virtual ~Layout();
	protected:
		void ClearCacheDirty();
		bool IsCacheDirty();
		void SetCacheDirty();
	};

	class UILIB_API FillLayout : public Layout
	{
	public:
		FillLayout(FillLayout const &);
		FillLayout();
		virtual ~FillLayout();

		FillLayout & operator=(FillLayout const &);

		static HRESULT WINAPI Create(int, int*, Value**);
		static HRESULT WINAPI Create(Layout**);

		void Initialize();

		virtual void DoLayout(Element*, int, int);
		virtual Element* GetAdjacent(Element*, Element*, int, NavReference const*, unsigned long);
		virtual SIZE UpdateDesiredSize(Element*, int, int, Surface*);
	};

	class UILIB_API GridLayout : public Layout
	{
	public:
		GridLayout(GridLayout const &);
		GridLayout(void);
		virtual ~GridLayout(void);
		GridLayout & operator=(GridLayout const &);

		static long __stdcall Create(int, int, Layout * *);
		static long __stdcall Create(int, int *, Value * *);
		void Initialize(int, int);
		
		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);
	
	protected:
		unsigned int GetCurrentCols(int);
		unsigned int GetCurrentCols(Element *);
		unsigned int GetCurrentRows(int);
		unsigned int GetCurrentRows(Element *);
	};

	class UILIB_API FlowLayout : public Layout
	{
	public:
		FlowLayout(FlowLayout const &);
		FlowLayout(void);
		virtual ~FlowLayout(void);
		FlowLayout & operator=(FlowLayout const &);

		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall Create(bool, unsigned int, unsigned int, unsigned int, Layout * *);
		int GetLine(Element *, Element *);
		void Initialize(bool, unsigned int, unsigned int, unsigned int);
		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);

	protected:
		static SIZE __stdcall SizeZero(void);
		SIZE BuildCacheInfo(Element *, int, int, Surface *, bool);
	};

	class UILIB_API RowLayout : public Layout
	{
	public:
		RowLayout(RowLayout const &);
		RowLayout(void);
		virtual ~RowLayout(void);
		RowLayout & operator=(RowLayout const &);

		static long __stdcall Create(int, unsigned int, unsigned int, Layout * *);
		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall Create(unsigned int, unsigned int, Layout * *);
		
		long Initialize(int, unsigned int, unsigned int);
		
		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);

	};

	class UILIB_API NineGridLayout : public Layout
	{
	public:
		NineGridLayout(NineGridLayout const &);
		NineGridLayout(void);
		virtual ~NineGridLayout(void);
		NineGridLayout & operator=(NineGridLayout const &);

		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall Create(Layout * *);

		void Initialize(void);

		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual void OnAdd(Element *, Element * *, unsigned int);
		virtual void OnLayoutPosChanged(Element *, Element *, int, int);
		virtual void OnRemove(Element *, Element * *, unsigned int);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);

	private:
		void _UpdateTileList(int, Element *);

	};

	class UILIB_API BorderLayout :public Layout
	{
	public:
		BorderLayout(const BorderLayout &);
		BorderLayout(void);
		virtual ~BorderLayout(void);
		BorderLayout & operator=(const BorderLayout &);

		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall Create(Layout * *);

		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, const NavReference *, unsigned long);
		void Initialize(void);
		virtual void OnAdd(Element *, Element * *, unsigned int);
		virtual void OnLayoutPosChanged(Element *, Element *, int, int);
		virtual void OnRemove(Element *, Element * *, unsigned int);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);

	private:
		void SetClient(Element *);
	};

	class UILIB_API ShellBorderLayout : public BorderLayout
	{
	public:
		ShellBorderLayout(ShellBorderLayout const &);
		ShellBorderLayout(void);
		virtual ~ShellBorderLayout(void);
		ShellBorderLayout & operator=(ShellBorderLayout const &);

		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall Create(Layout * *);
		
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual void OnAdd(Element *, Element * *, unsigned int);
		virtual void OnLayoutPosChanged(Element *, Element *, int, int);
		virtual void OnRemove(Element *, Element * *, unsigned int);
	private:
		long _CalcTabOrder(Element *);
		void _Reset(void);

	};

	class UILIB_API TableLayout : public Layout
	{
	public:
		TableLayout(TableLayout const &);
		TableLayout(void);
		virtual ~TableLayout(void);
		TableLayout & operator=(TableLayout const &);

		static long __stdcall Create(int, int *, Value * *);
		static long __stdcall InternalCreate(int, int, int, int *, Layout * *);
		CellInfo * GetCellInfo(int);
		void Initialize(int, int, int, int *);

		virtual void DoLayout(Element *, int, int);
		virtual Element * GetAdjacent(Element *, Element *, int, NavReference const *, unsigned long);
		virtual SIZE UpdateDesiredSize(Element *, int, int, Surface *);

	};


	class UILIB_API VerticalFlowLayout : public Layout
	{
	public:	
		enum Align
		{
			LAYOUT_LEFT = 0,
			LAYOUT_RIGHT = 1,
			LAYOUT_CENTER = 2,
			LAYOUT_TOP = 0,
			LAYOUT_BOTTOM = 1,
			LAYOUT_MIDDLE = 2
		};

		VerticalFlowLayout(VerticalFlowLayout const &);
		VerticalFlowLayout();
		virtual ~VerticalFlowLayout();
		VerticalFlowLayout & operator=(VerticalFlowLayout const &);

		static HRESULT WINAPI Create(int, int*, Value**);
		static HRESULT WINAPI Create(bool, unsigned int horAlign, unsigned int, unsigned int vertAlign, Layout**);
		
		int GetLine(Element*, Element*);
		void Initialize(bool, unsigned int, unsigned int, unsigned int);

		virtual void DoLayout(Element*, int, int);
		virtual Element* GetAdjacent(Element*, Element*, int, NavReference const*, unsigned long);
		virtual SIZE UpdateDesiredSize(Element*, int, int, Surface*);

	protected:
		SIZE BuildCacheInfo(Element*, int, int, Surface*,bool);
		static SIZE WINAPI SizeZero(void);
	};
}