#pragma once
template <typename Type>

class IForeach
{
	public:
		virtual ~IForeach() {};
		virtual void foreach(void(*func) (Type *, int, int)) = 0;
};
