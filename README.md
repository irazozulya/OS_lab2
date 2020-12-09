# OS_lab2

 Memory allocation with pages

 ## Common setup

 Clone the repo and install the dependencies.

 ```bash
 git clone https://github.com/irazozulya/OS_lab2
 ```

 ## Installation

 Use Visual Studio to compile and start the project.
 

 ## Usage

 The program is used for working with the memory.
 
 
 ## Theory
 A computer can address more memory than the amount physically installed on the system. This extra memory is actually called virtual memory and it is a section of a hard that's set up to emulate the computer's RAM. Paging technique plays an important role in implementing virtual memory.

 Paging is a memory management technique in which process address space is broken into frames of the same size called pages (size is power of 2, between 512 bytes and 8192 bytes). The size of the process is measured in the number of pages.

 Similarly, main memory is divided into small fixed-sized frames of (physical) memory called frames and the size of a frame is kept the same as that of a page to have optimum utilization of the main memory and to avoid external fragmentation.
 
 When the system allocates a frame to any page, it translates this logical address into a physical address and create entry into the page table to be used throughout execution of the program.

 When a process is to be executed, its corresponding pages are loaded into any available memory frames. Suppose you have a program of 8Kb but your memory can accommodate only 5Kb at a given point in time, then the paging concept will come into picture. When a computer runs out of RAM, the operating system (OS) will move idle or unwanted pages of memory to secondary memory to free up RAM for other processes and brings them back when needed by the program.

 This process continues during the whole execution of the program where the OS keeps removing idle pages from the main memory and write them onto the secondary memory and bring them back when required by the program.
 ![The paging scheme](https://github.com/irazozulya/OS_lab2/blob/main/6.png)
 
 
 ## Examples
 
 Created 8 pages:<br />
 ![The first picture](https://github.com/irazozulya/OS_lab2/blob/main/1.png)
 Allocated frames of sizes 10, 16, 54, 20, 650 and 1800:<br />
 ![The second picture](https://github.com/irazozulya/OS_lab2/blob/main/2.png)
 Reallocated frames of sizes 20 (to 12) and 650 (to 200):<br />
 ![The third picture](https://github.com/irazozulya/OS_lab2/blob/main/3.png)
 Cleared frame of size 1800:<br />
 ![The fourth picture](https://github.com/irazozulya/OS_lab2/blob/main/4.png)
 Allocated frame of size 150:<br />
 ![The fifth picture](https://github.com/irazozulya/OS_lab2/blob/main/5.png)

 ## Functions
 
 mem_alloc(size_t size) is used for memory allocation<br />
 mem_free(void* addr) is used for memory clearing<br />
 mem_realloc(void* addr, size_t size) is used for memory reallocation<br />
 mem_show() is used for memory state output<br />
 create_pages() is used for creation of pages in allocator<br />
 create_header(size_t size, uint8_t* begining) is used for creation of headers for frames<br />
 search_for_page(size_t size) is used to search for a page id by the frame size<br />
 search_for_page(uint8_t* addr) is used to search for a page id by the page address<br />
 calc_size(size_t size) is used for calcutating the proper size of the frame<br />
 get_size(uint8_t* begining) is used for calculating the size of the frame<br />


 ## Version

 OS_lab1 Version 1.0 09/12/2020
 

 ## Author

 - **Iryna Zozulia** - [irazozulya](https://github.com/irazozulya)


 ## Architecture

 main.cpp - file with main() function;

 alloc.h - file with the description of classes for working with memory;
 
 alloc.cpp - file with methods of classes for working with memory;
 

 ## License

 [MIT license](https://choosealicense.com/licenses/mit/)

