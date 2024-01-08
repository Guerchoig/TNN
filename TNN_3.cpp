//#include <iostream>
#include "BasicElements.h"
#include <conio.h>
using std::this_thread::sleep_for;
using std::chrono::seconds;
//using namespace cv;


//int testOpenCV(int argc, char** argv) {
//
//	if (argc != 2)
//	{
//		cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
//		return -1;
//	}
//	Mat image;
//	image = imread(argv[1], IMREAD_COLOR); // Read the file
//	if (image.empty()) // Check for invalid input
//	{
//		cout << "Could not open or find the image" << std::endl;
//		return -1;
//	}
//	namedWindow("Display window", WINDOW_NORMAL); // Create a window for display.
//	imshow("Display window", image); // Show our image inside it.
//	waitKey(0); // Wait for a keystroke in the window
//	return 0;
//}
//void testTimer(void) {
//	// Using time point and system_clock
//	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
//
//	start = std::chrono::high_resolution_clock::now();
//	for (int n = 0; n < 1000; ++n);
//	end = std::chrono::high_resolution_clock::now();
//
//	std::chrono::duration<long long, std::nano> elapsed_nanoseconds = end - start;
//
//	std::cout << "elapsed time: " << elapsed_nanoseconds.count() << "s\n";
//	waitKey(0); // Wait for a keystroke in the window
//
//}

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

