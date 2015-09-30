#include "instruction_set.hpp"
#include "vm.hpp"
#include "context.hpp"

#include <iostream>

using namespace bh;

void instruction_set::base(bh::vm& vm)
{
	// push and pop
	for(int i='0'; i <= '9'; ++i)
	{
		vm.register_operation(i, [i](bh::vm& vm) { vm.push(i - '0'); });
	}
	for(int i='A'; i <= 'F'; ++i)
	{
		vm.register_operation(i, [i](bh::vm& vm) { vm.push(i - 'A' + 10); });
	}


	// arithmetic
	vm.register_operation('+', [](bh::vm& vm) { vm.push(vm.pop() + vm.pop()); });
	vm.register_operation('-', [](bh::vm& vm) { vm.push(vm.pop() - vm.pop()); });
	vm.register_operation('*', [](bh::vm& vm) { vm.push(vm.pop() * vm.pop()); });
	vm.register_operation('/', [](bh::vm& vm) { vm.push(vm.pop() / vm.pop()); });
	vm.register_operation('i', [](bh::vm& vm) { vm.push(vm.pop() + 1); });
	vm.register_operation('d', [](bh::vm& vm) { vm.push(vm.pop() - 1); });

	// io
	vm.register_operation('P', [](bh::vm& vm) { std::cout << vm.pop() << std::endl; });
	vm.register_operation('a', [](bh::vm& vm) { std::cout << (char)vm.pop() << std::endl; });

	// jumps
	vm.register_operation('J', [](bh::vm& vm) { vm.jump_relative(vm.pop()); });
	vm.register_operation('j', [](bh::vm& vm) { vm.jump(vm.pop()); });
	vm.register_operation('c', [](bh::vm& vm) {
			pointer to = vm.pop();
			vm.push(vm.get_ip());
			std::cout << "jumping to " << to << std::endl;
			vm.jump(to);
		});
	vm.register_operation('r', [](bh::vm& vm) {
			cell return_value = vm.pop();
			vm.jump(vm.pop());
			vm.push(return_value);
		});
	vm.register_operation('?', [](bh::vm& vm) { // conditional relative
			if (vm.pop())
				vm.jump_relative(vm.pop());
		});
	vm.register_operation('!', [](bh::vm& vm) { // conditional absolute
			if (vm.pop())
				vm.jump(vm.pop());
		});

	// logic
	vm.register_operation('=', [](bh::vm& vm) {
		   vm.push(vm.pop() == vm.pop() ? 1 : 0); });
	vm.register_operation('<', [](bh::vm& vm) {
		   vm.push(vm.pop() < vm.pop() ? 1 : 0); });
	vm.register_operation('>', [](bh::vm& vm) {
		   vm.push(vm.pop() > vm.pop() ? 1 : 0); });
	vm.register_operation('&', [](bh::vm& vm) { vm.push(vm.pop() & vm.pop()); });
	vm.register_operation('|', [](bh::vm& vm) { vm.push(vm.pop() | vm.pop()); });
	vm.register_operation('x', [](bh::vm& vm) { vm.push(vm.pop() ^ vm.pop()); });
	vm.register_operation('n', [](bh::vm& vm) { vm.push(~vm.pop()); });

	// stack manipulation
	vm.register_operation('@', [](bh::vm& vm) {
			int n = vm.pop() - 1;
			cell top = vm.pop();

			stack tmp;
			for (int i=0; i<n; ++i)
				tmp.push(vm.pop());

			vm.push(top);

			for (int i=0; i<n; ++i)
			{
				vm.push(tmp.top());
				tmp.pop();
			}
		});
	vm.register_operation('"', [](bh::vm& vm) {
			int n = vm.pop();
			std::vector<cell> tmp(n);
			for (int i=0; i<n; ++i)
				tmp.push_back(vm.pop());
			
			for (int i=0; i<n; ++i)
				vm.push(tmp[i]);
			for (int i=0; i<n; ++i)
				vm.push(tmp[i]);
		});
	vm.register_operation('p', [](bh::vm& vm) { vm.pop(); });
}

