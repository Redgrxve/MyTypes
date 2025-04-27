#pragma once
#include <stdexcept>

namespace rg
{
template<typename T>
class list
{
public:
	list() {}
	list(const list& other)
	{

	}

	~list()
	{
		while (head)
		{
			node* tmp = head;
			head = head->next;
			delete tmp;
		}
	}

public:
	T& operator[](size_t index) const
	{
		if (index >= size || index < 0) throw std::out_of_range("Index out of bounds");

		node* tmp = head;
		for (; index > 0; --index)
		{
			tmp = tmp->next;
		}

		return tmp->item;
	}

public:
	void push_front(const T& item)
	{
		node* add = new node(item, head);
		head = add;
	}

	void push_back(const T& item)
	{
		node* add = new node(item, nullptr);
		++size;

		if (!head)
		{
			head = add;
		}
		else
		{
			auto last = get_last();
			last->next = add;
		}
	}

	void push_to(const T& item, size_t index)
	{
		if (index >= size && index != 0 || index < 0) throw std::out_of_range("Index out of bounds");

		node* add = new node(item, nullptr);
		++size;

		if (index == 0)
		{
			add->next = head;
			head = add;
		}
		else
		{
			node* tmp = get_node(index);
			add->next = tmp->next;
			tmp->next = add;
		}
	}

	/*bool pop_back(T& out)
	{
		if (size == 0) return false;

		auto last = get_last();
		out = last->item;

		delete last;
	}*/

	const size_t length() const { return size; }

private:
	struct node
	{
		node(const T& item_, node* next_)
			: item(item_), next(next_) {}

		T item;
		node* next;
	};

	node* head = nullptr;
	size_t size = 0;

private:
	node* get_last() const
	{
		node* tmp = head;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		return tmp;
	}

	node* get_node(size_t index) const
	{
		node* tmp = head;
		for (; index > 1; --index)
		{
			tmp = tmp->next;
		}
		return tmp;
	}
};
}


