#include <iostream>
#include <vector>
#include <string>


// 구구단 프로그램을 클래스로 리팩토링하기

class Gugudan
{
public:
	Gugudan();
	~Gugudan();
	void setParams(size_t dan_start, size_t dan_end, size_t step_start, size_t step_end);
	bool proc();
	void display();

private:
	size_t dan_start, dan_end, step_start, step_end;
	std::vector<std::string> msg_list;
};

Gugudan::Gugudan()
{
	dan_start = 0;
	dan_end = 0;
	step_start = 0;
	step_end = 0;
	std::cout << "Gugudan 생성자 호출" << std::endl;
}

Gugudan::~Gugudan()
{
	std::cout << "Gugudan 소멸자 호출" << std::endl;
}

void Gugudan::setParams(size_t dan_start = 0, size_t dan_end = 0, size_t step_start = 0, size_t step_end = 0)
{
	std::cout << "Gugudan setParams() 호출" << std::endl;
	this->dan_start = dan_start;
	this->dan_end = dan_end;
	this->step_start = step_start;
	this->step_end = step_end;
}

bool Gugudan::proc()
{
	std::cout << "Gugudan proc() 호출" << std::endl;

	msg_list.clear();

	std::string msg;
	
	for (size_t dan = dan_start; dan < dan_end + 1; dan++)
	{
		for (size_t step = step_start; step < step_end + 1; step++)
		{
			size_t result = dan * step;

			msg = "";
			msg = std::to_string(dan);
			msg += "*";
			msg += std::to_string(step);
			msg += "=";
			msg += std::to_string(result);
			msg += "\t";
			
			msg_list.push_back(msg);
		}
	}

	return true;
}

void Gugudan::display()
{
	std::cout << "Gugudan display() 호출" << std::endl;

	for (size_t msg_index = 0; msg_index < msg_list.size(); msg_index++)
	{
		if ((msg_index != 0) && (msg_index % (step_end - step_start + 1) == 0))
		{
			std::cout << std::endl;
		}
		
		std::cout << msg_list[msg_index];
	}

	std::cout << std::endl;
}

int main()
{
	int test_int = 1;
	size_t test_size_t = 3;
	uint64_t test_uint64_t = 4;


	Gugudan myGugudan;
	size_t dan_start, dan_end, step_start, step_end;

	
	dan_start = 2;
	dan_end = 9;
	step_start = 1;
	step_end = 9;

	/*std::cin >> dan_start;
	std::cin >> dan_end;
	std::cin >> step_start;
	std::cin >> step_end;*/


	myGugudan.setParams(dan_start, dan_end, step_start, step_end);
	myGugudan.proc();
	myGugudan.display();

	std::cout << std::endl;

	dan_start = 3;
	dan_end = 5;
	step_start = 4;
	step_end = 9;

	myGugudan.setParams(dan_start, dan_end, step_start, step_end);
	myGugudan.proc();
	myGugudan.display();

	return 0;
}

