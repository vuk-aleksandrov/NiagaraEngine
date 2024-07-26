#include "Application.h"

int main()
{
	auto Application = niagara::FApplication::Construct();

	Application->Run();
}