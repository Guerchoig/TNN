//#include <iostream>
#include "BasicElements.h"
#include <curses.h>
using std::this_thread::sleep_for;
using std::chrono::seconds;
void catch_visual_changes(const ViewField& curr_scene, const ViewField& prev_scene) {

}

auto poll_visual_detectors(void) {
		std::cout << "Task started ...\n";
		sleep_for(seconds(10));
		std::cout << " Task done!\n";
	}

auto f2(void) {
	std::cout << "Task started ...\n";
	sleep_for(seconds(30));
	std::cout << " Task done!\n";
}


int main(int argc, char** argv)
{
	Brain* brain = new Brain;
	print_connections(brain);
	
	std::thread t1(poll_visual_detectors);
	std::cout << "Run some other tasks.\n";

	t1.join();

	return 0;
}

