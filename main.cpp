#include <iostream>
#include "alloc.h"

using namespace std;

int main() {
    Memory_Allocator memAllocator = Memory_Allocator();
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 10" << endl << endl;
    memAllocator.mem_alloc(10);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 16" << endl << endl;
    memAllocator.mem_alloc(16);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 54" << endl << endl;
    memAllocator.mem_alloc(54);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 20" << endl << endl;
    void* addr1 = memAllocator.mem_alloc(20);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 650" << endl << endl;
    void* addr2 = memAllocator.mem_alloc(650);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Allocate 1800" << endl << endl;
    void* addr3 = memAllocator.mem_alloc(1800);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Reallocate 20 to 12" << endl << endl;
    void* addr4 = memAllocator.mem_realloc(addr1, 12);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Reallocate 650 to 200" << endl << endl;
    void* addr5 = memAllocator.mem_realloc(addr2, 200);
    memAllocator.mem_show();

    cout << endl << "> > > > > > > > > > > > > > > > Free 1800" << endl << endl;
    memAllocator.mem_free(addr3);
    memAllocator.mem_show();


    cout << endl << "> > > > > > > > > > > > > > > > Allocate 150" << endl << endl;
    memAllocator.mem_alloc(150);
    memAllocator.mem_show();
}