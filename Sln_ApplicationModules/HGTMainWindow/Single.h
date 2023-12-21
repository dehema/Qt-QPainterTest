/**
* @file      template single design
* @datetime  2022-06-03
* @version   v0.0.1
*/
#include <iostream>
using namespace std;

/**
* @class     template_single
* @datetime  2022-06-03
* @version   v0.0.1
*/
#include <iostream>
using namespace std;

template <typename T>
class Single {
public:
	//�ⲿ��ȡ�����Ľӿ�
	static T& ins()
	{
		static T instance;   //������̬��������ʵ����ʱ���졣
		return instance;
	}

	//��ֹ����
	Single(const Single&) = delete;
	Single& operator=(const Single&) = delete;

protected:
	//ֻ��������ܻ�ȡ���ơ�
	struct Token {};

	//�������������˽�л�
	Single() = default;
	virtual ~Single() = default;
};
