#include <chrono>
#include <vector>
#include <iostream>
#include <format>
#include <functional>
#include <numeric>

using TimeCost = size_t;

TimeCost fn_1(size_t array_size, size_t step_size)
{
	constexpr size_t magic = 2;
	size_t* elements = new size_t[array_size];

	// init  
	for (size_t i = 0; i < array_size; i++)
		elements[i] = 0;

	// timing 
	auto start = std::chrono::steady_clock::now();

	for (size_t i = 0; i < array_size; i += step_size)
		elements[i] = i * magic;

	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start).count();

	// release resources 
	delete[] elements;

	return cost;
}

TimeCost fn_2(size_t array_size, size_t step_size, size_t nsteps)
{
	size_t* elements = new size_t[array_size];

	// init  
	for (size_t i = 0; i < array_size; i++)
		elements[i] = 0;

	// timing 
	auto start = std::chrono::steady_clock::now();

	for (size_t i = 0; i < nsteps; i += 1) 
		elements[(i * step_size) % array_size] ++;

	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start).count();

	// release resources 
	delete[] elements;

	return cost;
}

size_t average_cost(size_t count, std::function<TimeCost()> fn)
{
	std::vector<TimeCost> buf;
	for (size_t i = 0; i < count; i++)
	{
		buf.push_back(fn());
	}

	return std::reduce(buf.begin(), buf.end()) / count;
}

void part_1()
{
	constexpr size_t repeat = 8;

	// memory size of array 
	constexpr size_t min_ary_size = 1024 ; // 1 kb
	constexpr size_t max_ary_size = 1024 * 1024 * 256; // 128 mb
	constexpr size_t ary_size_expand_rate = 4;


	constexpr size_t min_step_size = 1;
	constexpr size_t max_step_size = 1024;
	constexpr size_t step_size_expand_rate = 2;

	constexpr const char* fmt_1 = "{:>16} {:>16} {:>8}";
	std::cout << std::format(fmt_1, "Memory Size (bytes)", "Step Size", "Cost (ms)") << std::endl;
	for (size_t ary_mem_size = min_ary_size; ary_mem_size <= max_ary_size; ary_mem_size *= ary_size_expand_rate)
	{
		for (size_t step_size = min_step_size; step_size <= max_step_size; step_size *= step_size_expand_rate)
		{
			std::cout << std::format(fmt_1,
				ary_mem_size,
				step_size,
				average_cost(repeat, [=]() {	return fn_1(ary_mem_size / sizeof(size_t), step_size); })) << std::endl;
		}
	}
}

void part_2()
{
	constexpr size_t repeat = 4;

	// memory size of array 
	constexpr size_t min_ary_size = 1024; // 1 kb
	constexpr size_t max_ary_size = 1024 * 1024 * 16; // 16mb
	constexpr size_t ary_size_expand_rate = 4;


	constexpr size_t min_step_size = 1;
	constexpr size_t max_step_size = 512;
	constexpr size_t step_size_expand_rate = 8;

	constexpr size_t min_nstep = 1024 * 64;
	constexpr size_t max_nstep = 1024 * 1024;
	constexpr size_t nstep_expand_rate = 4;


	constexpr const char* fmt_2 = "{:>16} {:>16} {:>16} {:>16}";
	std::cout << std::format(fmt_2, "Memory Size (bytes)", "Step Size", "nSteps", "Cost (ms)") << std::endl;
	for (size_t ary_mem_size = min_ary_size; ary_mem_size <= max_ary_size; ary_mem_size *= ary_size_expand_rate)
	{
		for (size_t step_size = min_step_size; step_size <= max_step_size; step_size *= step_size_expand_rate)
		{
			for (size_t nstep = min_nstep; nstep <= max_nstep; nstep *= nstep_expand_rate)
			{
				std::cout << std::format(fmt_2,
					ary_mem_size,
					step_size,
					nstep,
					average_cost(repeat, [=]() {	return fn_2(ary_mem_size / sizeof(size_t), step_size, nstep); })) << std::endl;
			}
		}
	}
}

int main()
{
	// 64-bit CPU
	// assert(sizeof(size_t) == 8)

	std::cout << "part 1:" << std::endl;
	part_1();

	std::cout << "part 2:" << std::endl;
	part_2();


	return 0;
}
