Xiwei Ma

In original version, for every loop, it moves a single pixel.
To inporve performance, multithreading can be used here. Each thread
processing a pixel at the same time and after that jumps to the pixel 
not yet being processed by any other threads(move (number of threads)pixels).
The for loop has been divided into nthread parts and
pixels have been processed parallel.
 
However, the print seems incorrect if there are  multithreads.
So, it cannot be printed in the order of procession. Instead, the 
output should be put in its original order. To solve this,
I used an array to save the result of each pixel's right position
according to the info of the pixel of the thread,
and print the array in order at the end.

There still existed a bug. When I printed the px out, I found the
value is incorrect. When I looked at the main function, I noticed that 
I directly put the address of the iterator of the for loop as the argument
of the pthread_create. However, since the value may change with the for
loop so the value in the address may be different from what I expected.
To fix this, I created a array and store each id(1st,2nd, 3rd,4th...)of 
the threads into the array. &array[n] will store a particular number that
would not change in the following program. 

Performance:
time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m52.629s
user 0m51.360s
sys  0m0.006s

time ./srt 2-test.ppm >2-test.ppm.tmp

real 0m24.611s
user 0m49.066s
sys  0m0.010s

time ./srt 4-test.ppm >4-test.ppm.tmp

real 0m11.301s
user 0m44.921s
sys  0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp

real 0m5.821s
user 0m44.845s
sys  0m0.005s

From the above result, it is obvious that the time for more threads have
shortened to half. The original code works as 1 thread, so the performance 
can be greatly improved by my revised implementation of multithreading.
