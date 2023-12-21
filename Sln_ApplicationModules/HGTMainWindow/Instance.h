#pragma once
template<typename T>
class Instance<T>:
{
public:
	Instance();
	~Instance();

	static Instance* Ins() {
		static Instance* instance = nullptr;
		if (instance == nullptr)
		{
			instance = new Instance;
			qDebug() << "new Utility";
		}
		return instance;
	}
};