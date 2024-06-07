#pragma once

namespace DirectUI
{
	class UILIB_API LinkedListNode
	{
	public:
		LinkedListNode& operator=(LinkedListNode const &);
	};

	class UILIB_API LinkedList
	{
	public:
		LinkedList(void);
		~LinkedList(void);
		LinkedList & operator=(LinkedList const &);
		void Add(LinkedListNode *);
		void Remove(LinkedListNode *);
		class LinkedListNode * RemoveTail(void);
	};

}