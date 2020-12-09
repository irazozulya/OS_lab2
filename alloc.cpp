#include <iostream>
#include <list>
#include <math.h>
#include "alloc.h"

using namespace std;

Memory_Allocator::Memory_Allocator() {
    beginingPtr = (uint8_t*)malloc(PAGE_SIZE*PAGE_QUANTITY);
    create_pages();
}


void* Memory_Allocator::mem_alloc(size_t size) {
    if (size <= PAGE_SIZE / 2) {
        size_t expectableSize = calc_size(size);

        for (int i = 0; i < PAGE_QUANTITY; i++){
            if (!pageDescriptions[i].access) {
                uint8_t* beginingFrame = beginingPtr + (i * PAGE_SIZE);
                size_t frameSize = calc_size(get_size(beginingFrame + ACCESSIBILITY_INDENT));

                if (frameSize == expectableSize && pageDescriptions[i].freeFramesQuan > 0) {
                    uint8_t* frameCurrPtr = pageDescriptions[i].firstAcFramePtr;

                    *frameCurrPtr = false;
                    create_header(size, frameCurrPtr + ACCESSIBILITY_INDENT);

                    pageDescriptions[i].firstAcFramePtr += frameSize;
                    pageDescriptions[i].freeFramesQuan--;

                    return (void*)frameCurrPtr;
                }
            }
        }
        
        for (int i = 0; i < PAGE_QUANTITY; i++) {
            if (pageDescriptions[i].access) {
                uint8_t* frameCurrPtr = pageDescriptions[i].firstAcFramePtr;

                *frameCurrPtr = false;
                create_header(size, frameCurrPtr + ACCESSIBILITY_INDENT);
                
                pageDescriptions[i].firstAcFramePtr += expectableSize;
                pageDescriptions[i].access = false;
                pageDescriptions[i].freeFramesQuan = (PAGE_SIZE / expectableSize) - 1;

                return (void*)frameCurrPtr;
            }
        }
    }
    else {
        if (size + HEADER_SIZE < PAGE_SIZE) {
            for (int i = 0; i < PAGE_QUANTITY; i++) {
                if (pageDescriptions[i].access) {
                    uint8_t* frameCurrPtr = pageDescriptions[i].firstAcFramePtr;
                    uint8_t* result = frameCurrPtr;
                    *frameCurrPtr = false;
                    create_header(size, frameCurrPtr + ACCESSIBILITY_INDENT);
                        

                    frameCurrPtr += (PAGE_SIZE - 1);
                    pageDescriptions[i].firstAcFramePtr = frameCurrPtr + HEADER_SIZE + size;
                    pageDescriptions[i].access = false;
                    pageDescriptions[i].freeFramesQuan = 0;
                    return (void*)result;
                }
            }
        }
        else {
            int beginingPage = search_for_page(size);
            int numbOfPages = ceil(size / PAGE_SIZE);
            int currSize = size;
            uint8_t* resultAddr = pageDescriptions[beginingPage].firstAcFramePtr;
            for (int i = beginingPage; i < beginingPage + numbOfPages + 1; i++) {
                pageDescriptions[i].access = false;
                pageDescriptions[i].freeFramesQuan = 0;
                uint8_t* currPtr = pageDescriptions[i].firstAcFramePtr;

                *currPtr = false;
                if (currSize >= PAGE_SIZE) {
                    create_header(PAGE_SIZE, currPtr + ACCESSIBILITY_INDENT);
                    pageDescriptions[i].firstAcFramePtr += PAGE_SIZE - 1;
                    currSize -= PAGE_SIZE;
                }
                else {
                    create_header(currSize, currPtr + ACCESSIBILITY_INDENT);
                    pageDescriptions[i].firstAcFramePtr += (currSize + HEADER_SIZE);
                    return (void*)resultAddr;
                }
            }
        }
    }
}


void Memory_Allocator::mem_free(void* addr) {
    uint8_t* foundAddr = (uint8_t*)addr;
    bool accessible = *foundAddr;
    size_t size = get_size(foundAddr + ACCESSIBILITY_INDENT);
    int numbOfPage = search_for_page(foundAddr);

    if (pageDescriptions[numbOfPage].access || numbOfPage == -1)
        return;

    if (size < PAGE_SIZE / 2) {
        *foundAddr = true;
        pageDescriptions[numbOfPage].freeFramesQuan += 1;

        if (pageDescriptions[numbOfPage].freeFramesQuan == PAGE_SIZE / calc_size(size))
            pageDescriptions[numbOfPage].access = true;
    }
    else if (size < PAGE_SIZE) {
        *foundAddr = true;
        create_header(0, foundAddr + ACCESSIBILITY_INDENT);
        pageDescriptions[numbOfPage].access = true;
        pageDescriptions[numbOfPage].firstAcFramePtr = foundAddr;        
    }
    else {
        int pageCurrNum = numbOfPage;
        uint8_t* currPtr = foundAddr;

        while (size == PAGE_SIZE) {
            *currPtr = true;
            pageDescriptions[pageCurrNum].access = true;
            pageDescriptions[pageCurrNum].firstAcFramePtr = currPtr;

            pageCurrNum++;
            currPtr += PAGE_SIZE; 
            size = get_size(currPtr + ACCESSIBILITY_INDENT);
        }
        
        if (!pageDescriptions[pageCurrNum].access) {
            *currPtr = true;
            pageDescriptions[pageCurrNum].access = true;
            pageDescriptions[pageCurrNum].firstAcFramePtr = currPtr;
        }
    }
}


void* Memory_Allocator::mem_realloc(void* addr, size_t size) {
    uint8_t* foundAddr = (uint8_t*)addr;
    bool accessible = *foundAddr;
    size_t oldSize = get_size(foundAddr + ACCESSIBILITY_INDENT);
    int numbOfPage = search_for_page(foundAddr);

    if (pageDescriptions[numbOfPage].access || size == oldSize)
        return nullptr;
    if (calc_size(oldSize) == calc_size(size)) {
        create_header(size, foundAddr + ACCESSIBILITY_INDENT);
        return (void*)foundAddr;
    }
    else {
        mem_free(addr);
        void* result = mem_alloc(size);
        return result;
    }
}


void Memory_Allocator::mem_show() {
    for (int i = 0; i < PAGE_QUANTITY; i++) { 
        cout << "Page " << i + 1;
        if (pageDescriptions[i].access) {
            cout << "\t|\t Address " << (void*)pageDescriptions[i].firstAcFramePtr;
            cout << "\t|\t Is accessible" << endl;
        }
        else {
            uint8_t* currPagePtr = beginingPtr + (i * PAGE_SIZE);
            cout << "\t|\t Address " << (void*)currPagePtr;
            cout << "\t|\t Isn't accessible" << endl;

            size_t frameSize = get_size(currPagePtr + ACCESSIBILITY_INDENT);
            size_t currFrameSize = calc_size(frameSize);
            
            int frame = 1;
            if (currFrameSize == PAGE_SIZE) {
                cout << "\t Frame " << frame;
                cout << "\t|\t address " << (void*)currPagePtr;
                if((bool)*currPagePtr) {
                    cout << "\t|\t is accessible ";
                }
                else {
                    cout << "\t|\t isn't accessible ";
                }
                cout << "\t|\t size " << frameSize;
                cout << endl;
                continue;
            }

            while (currPagePtr != pageDescriptions[i].firstAcFramePtr) {
                cout << "\t Frame " << frame;
                cout << "\t|\t address " << (void*)currPagePtr;
                if((bool)*currPagePtr) {
                    cout << "\t|\t is accessible ";
                }
                else {
                    cout << "\t|\t isn't accessible ";
                }
                cout << "\t|\t size " << get_size(currPagePtr + ACCESSIBILITY_INDENT);
                cout << endl;

                frame++;
                if (get_size(currPagePtr + ACCESSIBILITY_INDENT + currFrameSize) == 0) {
                    break;
                } else {
                    currPagePtr += currFrameSize;
                }
            }
        }
        cout << "__________________________________________________________________________________________________________" << endl << endl;
    }
}


void Memory_Allocator::create_pages() {
    uint8_t* currPtr = beginingPtr;
    for (int i = 0; i < PAGE_QUANTITY; i++) {
        PageDescription description = PageDescription();

        description.firstAcFramePtr = currPtr;
        *(currPtr) = true;
        *(currPtr + HEADER_SIZE) = PAGE_SIZE - ACCESSIBILITY_INDENT;

        pageDescriptions[i] = description;
        currPtr += PAGE_SIZE;
    }
}


void Memory_Allocator::create_header(size_t size, uint8_t* begining) {
    int indent = 0;
    int maxP = floor(size / CELL_MAX_SIZE);
    if (maxP == 4) {
        while (maxP > 0) {
            *(begining + indent) = CELL_MAX_SIZE - 1;
            maxP--;
            indent++;
        }
        return;
    }

    int rest = size - (maxP * CELL_MAX_SIZE);
    while (maxP > 0 || indent != 3) {
        if (maxP == 0) 
            *(begining + indent) = 0;
        else {
            *(begining + indent) = CELL_MAX_SIZE - 1;
            maxP--;
        }        
        indent++;
    }
    *(begining + indent) = rest;
}


int Memory_Allocator::search_for_page(size_t size) {
    int numbOfPages = ceil(size / PAGE_SIZE);
    int beginingPage;

    for (int i = 0; i < PAGE_QUANTITY; i++) {
        if (pageDescriptions[i].access) {
            beginingPage = i;
            for (int j = i; j < PAGE_QUANTITY; j++) {
                if (j == PAGE_QUANTITY - 1)
                    return -1;
                if (pageDescriptions[j].access && pageDescriptions[j+1].access) {
                    numbOfPages--;
                    if (numbOfPages == 0)
                        return beginingPage;
                }
            }
        }
    }
}


int Memory_Allocator::search_for_page(uint8_t* addr) {
    uint8_t* currPtr = beginingPtr;

    for (int i = 0; i < PAGE_QUANTITY; i++) {
        if (addr >= currPtr && addr < currPtr + PAGE_SIZE) {
            return i;
        }
        currPtr += PAGE_SIZE;
    }
    return -1;
}


size_t Memory_Allocator::calc_size(size_t size) {
    size_t expSize = size + HEADER_SIZE;
    size_t currSize = FRAME_MIN_SIZE;

    while (currSize != FRAME_MAX_SIZE) {
        if (currSize > expSize)
            return currSize;
        else {
            currSize *= INCREASE_NUMBER;
        }
    }
}


size_t Memory_Allocator::get_size(uint8_t* begining) {
    int indent = 0;
    size_t size = 0;

    while (indent < SIZE_INDENT) {
        if ((size_t) *(begining + indent) == CELL_MAX_SIZE - 1) {
            size += CELL_MAX_SIZE;
        }
        else {
            size += (size_t) *(begining + indent);
        }
        indent++;
    }
    return size;
}