#include "../olcPGE.h"

class SimpleExample : public olc::PixelGameEngine
{
public:
	SimpleExample()
	{
		sAppName = "SimpleExample";
	}

public:
	bool OnUserCreate() override
	{
		std::string example;

		olc::ScriptEngine script;

		/*
		std::string helloWorld = 
			"void helloWorld() {"
			"	print('Hello World!');"
			"}";

		std::string helloPeople = 
			"string helloString(string name) {		"
			"	return 'Hello ' + name + '!';		"
			"}										"
			"										"
			"void helloPeople(string name) {		"
			"	print(helloString(name));			"
			"}										";
		*/

		example = "5---2";
		script.LoadScript(example);
		std::cout << "Expected: 3" << std::endl;
		std::cout << std::endl;

		example = "5 + 5 - 2";
		script.LoadScript(example);
		std::cout << "Expected: 8" << std::endl;
		std::cout << std::endl;

		example = "2 * 5 + 5";
		script.LoadScript(example);
		std::cout << "Expected: 15" << std::endl;
		std::cout << std::endl;

		example = "2 * (5 + 5)";
		script.LoadScript(example);
		std::cout << "Expected: 20" << std::endl;
		std::cout << std::endl;

		example = "10 / (5 + 5) * 20";
		script.LoadScript(example);
		std::cout << "Expected: 20" << std::endl;
		std::cout << std::endl;

		example = "10 / 5 + 5 * 20";
		script.LoadScript(example);
		std::cout << "Expected: 102" << std::endl;
		std::cout << std::endl;

		example = "10a + 4";
		script.LoadScript(example);
		std::cout << "Expected: IllegalCharError" << std::endl;
		std::cout << std::endl;

		example = "0123 + 123";
		script.LoadScript(example);
		std::cout << "Expected: IllegalCharError" << std::endl;
		std::cout << std::endl;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


int main()
{
	SimpleExample demo;
	if (demo.Construct(256, 240, 2, 2))
		demo.Start();

	return 0;
}