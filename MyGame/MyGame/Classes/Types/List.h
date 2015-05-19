////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012-2015, Wojciech Figat - www.celelej.com
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "../../Platform/Platform.h"
//#include "../Core.h"
//#include "Array.h"

// port do naszej gry
#define FORCE_INLINE
#define INVALID_INDEX -1
#define ASSERT(x)

// Template for dynamic list with variable capacity
template<typename T, int defaultCapacity = 8>
class List
{
	friend List;

private:

	int _count;
	int _capacity;
	T* _data;

public:

	// Default constructor
	List()
		:_count(0),
		_capacity(defaultCapacity)
	{
		ASSERT(_capacity >= 0);
		if (_capacity > 0)
			_data = new T[_capacity];
		else
			_data = nullptr;
	}

	// Construct list with given capacity
	List(int capacity)
		:_count(0),
		_capacity(capacity)
	{
		ASSERT(_capacity >= 0);
		if (_capacity > 0)
			_data = new T[_capacity];
		else
			_data = nullptr;
	}

	// Copy constructor
	List(const List &a)
	{
		// Check if input list has no data allocated
		if (a._capacity == 0)
		{
			_capacity = 0;
			_count = 0;
			_data = nullptr;
			return;
		}

		ASSERT(a._data != nullptr);
		_capacity = a._capacity;
		_count = a._count;
		_data = new T[_capacity];
		memcpy(_data, a._data, sizeof(T) * _count);
	}

	// Destructor
	~List()
	{
		if (_data)
		{
			delete[] _data;
			_data = nullptr;
		}
		_count = 0;
		_capacity = 0;
	}

	// Get amount of the elements in the list
	FORCE_INLINE int Count() const
	{
		ASSERT(_count >= 0);
		return _count;
	}

	// Get amount of the elements that can be holded by list without resizing
	FORCE_INLINE int Capacity() const
	{
		ASSERT(_count >= 0);
		return _capacity;
	}

	// Returns true if list is empty
	FORCE_INLINE bool IsEmpty() const
	{
		return _count < 1;
	}

	// Clear the list but without changing its capacity
	FORCE_INLINE void Clear()
	{
		_count = 0;
	}

	// Clear the list but without changing its capacity. Delete all not null elements
	void ClearDelete()
	{
		for (int i = 0; i < _count; i++)
		{
			if (_data[i])
			{
				delete (_data[i]);
			}
		}
		_count = 0;
	}

	// Returns pointer to the first element in the list
	FORCE_INLINE T* GetData() const
	{
		return _data;
	}

	// Get/Set element by index
	FORCE_INLINE T& operator[](int index)
	{
		ASSERT(index >= 0 && index < _count);
		return _data[index];
	}

	// Get/Set element by index
	FORCE_INLINE const T& operator[](int index) const
	{
		ASSERT(index >= 0 && index < _count);
		return _data[index];
	}

	List& operator = (const List &a)
	{
		// Check if sb tries assign list to itself 
		if (this == &a)
			return *this;

		// Check if other list is empty
		if (a._count == 0)
		{
			Clear();
			return *this;
		}

		ASSERT(a._data != nullptr);

		// Check if need to resize list
		if (_capacity < a._count)
		{
			// Change capacity
			T newList = new T[a._capacity];
			if (_data)
				delete[] _data;
			_data = newList;
			_capacity = a._capacity;
		}

		// Copy data
		memcpy(_data, a._data, sizeof(T) * a._count);
		_count = a._count;

		return *this;
	}

	// Change capacity of the list
	// @param capacity New capacity
	void setCapacity(const int capacity)
	{
		// Validate input
		ASSERT(capacity >= 0 && capacity >= _count);

		// Check if value won't change
		if (capacity == _capacity)
			return;

		// Resize collection
		T* newList = new T[capacity];
		if (_data)
		{
			if (_count > 0)
				memcpy(newList, _data, _count * sizeof(T));
			delete[] _data;
		}
		_data = newList;
		_capacity = capacity;
	}

	// Add single item to the collection
	// @param item Item to add
	void Add(const T& item)
	{
		// Ensure to have enough memory for next item
		EnsureCapacity(_count + 1);

		// Insert element at the end of the list
		_data[_count++] = item;
	}

	// Add array of items to the collection
	// @param items Items array to add
	// @param count Amount of items to add
	void Add(const T* items, int count)
	{
		// Ensure to have enough memory for items
		EnsureCapacity(_count + count);

		// Insert elements at the end of the list
		for (int i = 0; i < count; i++)
		{
			_data[_count + i] = items[i];
		}
		_count += count;
	}

	// Add space for new items
	// @param count Amount of items to allocate space for them
	void AddSpace(int count)
	{
		// Ensure to have enough memory for items
		EnsureCapacity(_count + count);
		_count += count;
	}

	// Checks if given element is in the list
	// @param item Element to check
	// @returns True if item has been found in the list, ohwersie false
	bool Contains(const T& item)
	{
		for (int i = 0; i < _count; i++)
		{
			if (_data[i] == item)
			{
				return true;
			}
		}
		return false;
	}

	// Searches for the specified object and returns the zero-based index of the first occurrence within the entire list
	// @param item Element to find
	// @returns The zero-based index of the first occurrence of itm within the entire list, if found; otherwise, INVALID_INDEX
	int IndexOf(const T& item)
	{
		for (int i = 0; i < _count; i++)
		{
			if (_data[i] == item)
			{
				return i;
			}
		}
		return INVALID_INDEX;
	}

	// Removes the first occurrence of a specific object from the list and keeps order of the elements in the list
	// @param item Element to remove
	// @returns True if cannot remove item from the list because cannot find it, otherwise false
	bool RemoveKeepOrder(const T& item)
	{
		int index = IndexOf(item);
		if (index == INVALID_INDEX)
			return true;
		RemoveAtKeepOrder(index);
		return false;
	}

	// Removes the element at the specified index of the list and keeps order of the elements in the list
	// @param index The zero-based index of the element to remove
	void RemoveAtKeepOrder(const int index)
	{
		ASSERT(index < _count && index >= 0);

		_count--;
		if (index < _count)
		{
			memcpy(_data + index * sizeof(T), _data + (index + 1) * sizeof(T), _count - index)
		}
	}

	// Removes the first occurrence of a specific object from the list
	// @param item Element to remove
	// @returns True if cannot remove item from the list because cannot find it, otherwise false
	bool Remove(const T& item)
	{
		int index = IndexOf(item);
		if (index == INVALID_INDEX)
			return true;
		RemoveAt(index);
		return false;
	}

	// Removes the element at the specified index of the list
	// @param index The zero-based index of the element to remove
	void RemoveAt(const int index)
	{
		ASSERT(index < _count && index >= 0);

		// Check if there is more than 1 element in the collection
		if (_count > 1)
		{
			// Swap element to remove with the last one
			T last = _data[_count - 1];
			_data[index] = last;
		}
		_count--;
	}

	// Create instance of Array class from the list
	/*Array<T> ToArray()
	{
		return new Array<T>(_data, _count);
	}*/

	// Sets the capacity to the actual number of elements in the list, if that number is less than a threshold value
	void TrimExcess()
	{
		if (_count < (int)(_capacity * 0.8))
		{
			setCapacity(_count);
		}
	}

	// Ensure that collection has given capacity
	// @param minCapacity Minimum capacity
	void EnsureCapacity(int minCapacity)
	{
		if (_capacity >= minCapacity)
			return;
		int num = _capacity == 0 ? defaultCapacity : _capacity * 2;
		if (num > 2146435071)
		{
			num = 2146435071;
		}
		if (num < minCapacity)
		{
			num = minCapacity;
		}
		setCapacity(num);
	}
};
