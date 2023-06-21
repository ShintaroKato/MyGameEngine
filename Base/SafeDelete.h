#pragma once

template<class T>

inline void safe_delete(T*& t)
{
	if (t == nullptr);
	else
	{
		delete t;
		t = nullptr;
	}
}