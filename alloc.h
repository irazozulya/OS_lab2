#include <list>

const int PAGE_SIZE = 1024;
const int PAGE_QUANTITY = 8;
const int FRAME_MIN_SIZE = 16;
const int FRAME_MAX_SIZE = 1024;
const int CELL_MAX_SIZE = FRAME_MAX_SIZE / 4;
const int ACCESSIBILITY_INDENT = 1;
const int INCREASE_NUMBER = 2;
const int SIZE_INDENT = 4;
const int HEADER_SIZE = ACCESSIBILITY_INDENT + SIZE_INDENT;

struct PageDescription { // page details
    uint8_t* firstAcFramePtr;
    int freeFramesQuan;
    bool access = true;
};


class Memory_Allocator { // memory allocator class
public:
    uint8_t* beginingPtr; // pointer at the begining of the allocator
    PageDescription* pageDescriptions =  new PageDescription[PAGE_QUANTITY]; // page description array

    Memory_Allocator();
    void* mem_alloc(size_t size); // memory allocation
    void mem_free(void* addr); // memory clearing
    void* mem_realloc(void* addr, size_t size); // memory reallocation
    void mem_show(); // memory state output

protected: 
    void create_pages(); // creation of pages in allocator
    void create_header(size_t size, uint8_t* begining); // creation of headers for frames
    int search_for_page(size_t size); // search for a page id by the frame size
    int search_for_page(uint8_t* addr); // search for a page id by the page address
    size_t calc_size(size_t size); // calcutating the proper size of the frame
    size_t get_size(uint8_t* begining); // calculating the size of the frame
};