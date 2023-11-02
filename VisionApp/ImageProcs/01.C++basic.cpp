
#include <iostream>
#include <vector>

class Shape
{
public:
	Shape() 
		:a(0) // member initialize
		,b(10)
		,pA(nullptr)
	{
		// copy operator
		a = 1;
		b = 2;
		std::cout << "Ctor" << std::endl;
	}
	~Shape();

private:
	// ����� ���ÿ� �ʱ�ȭ
	int a = 10;
	int b;
	int* pA;
};


enum EShape
{
	eCircle = 0,
	eRectagle,
	eTriangle,
	ePloy,
	eShapeMax, // �̰� enum�� Ȱ���Ͽ� �迭�� �ڵ鸵 �Ҷ� ������ �迭�� ���� �� �迭�� ũ��� �����Ͽ� �ε��� ������ �迭�� ũ�⸦ �˼� �ְ� �ȴ�(?)
};

int main()
{
	int a = 1;
	int b(2);
	char byChar = 'A';
	int c = a + b;
	std::cout << c << "\t=\t" << a << "\t+\t" << b << std::endl;

	int* pA = nullptr;
	std::cout << "size of pointer pA = " << sizeof(pA) << std::endl;

	
	EShape _eShape = EShape::eCircle;


	for (size_t i = 0; i < 100; i++)
	{
		// resource create
		int* pShape = new int[EShape::eShapeMax]; // �̷��� new - delete ���ִ°� �����Ƽ� ���°� smart pointer�̴�.

		// init and management
		memset((void*)pShape, 0, sizeof(int) * EShape::eShapeMax);
		
		// destroy
		if (pShape != nullptr)
		{
			delete[] pShape;
			pShape = nullptr;
		}
	}
	
	std::vector<int> vShape(EShape::eShapeMax, 0);
	vShape[0] = 1;
	vShape[1] = 2;
	vShape[2] = 3;
	vShape[0] = 4;

	
	// quiz	������

	int dan_start, dan_end, step_start, step_end;

	std::cin >> dan_start;
	std::cin >> dan_end;
	std::cin >> step_start;
	std::cin >> step_end;

	for (size_t dan = dan_start; dan < dan_end + 1; dan++)
	{
		for (size_t step = step_start; step < step_end + 1; step++)
		{
			std::cout << dan << "*" << step << " = " << dan * step << "\t";
		}
		std::cout << std::endl;
	}


	return 1;
}
