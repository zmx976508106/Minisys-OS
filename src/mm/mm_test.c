#include <mm/mm_test.h>

pid_t pid = 0;
int alloc_size[TEST_COUNT] = {33, 64, 128, 256, 512, 1024, 2048, 4096, 6144, 4096};
// 0x80022000
int index_ = 0;

int test_alloc_memory(){
    int *addr_alloc[TEST_COUNT] = {NULL};
    int addr_true_int[TEST_COUNT] = {0x80022ec0, 0x80022e80, 0x80022c80, 0x80022800, 0x80022400, 0x80022000, 0x80023000, 0x80024000, 0x80025000, 0x80027000};
    int *addr_true[TEST_COUNT];
    
    int i;
    for(i = 0; i < TEST_COUNT; ++i)
        addr_true[i] = (int *)(addr_true_int[i] - 0x80000000);

    for (i = 0; i < TEST_COUNT; ++i){
        // writeValTo7SegsDec(i);
        addr_alloc[i] = (int *)kmalloc(pid, alloc_size[i]);
        if(get_page_status((uint32_t)addr_alloc[i]) != 1){
            // 错误类型一
            // 页面在分配之后未修改标志位
            writeValTo7SegsHex(0xffffffff);
            return 1;
        }

        if(addr_alloc[i] != addr_true[i]){
            // 错误类型二
            // 页面分配得到的地址不正确
            // pte_t *pte = get_pte_by_page_addr(0x00022000);
            // uint32_t high = (uint32_t)((*pte) >> 32);
            // writeValTo7SegsHex(high);

            writeValTo7SegsHex(0xfffffff0);
            writeValTo7SegsDec(i);
            writeValTo7SegsHex(addr_alloc[i]);
            return 2;
        }
            
    }
    writeValTo7SegsHex(0x00);
    return 3;
}

int test_rw_memory(){
    // 用虚拟地址进行读写会触发TLB重填的异常
  // writeValTo7SegsDec(1);
    // writeValTo7SegsHex(0x11111111);
    int *addr1 = (int *)kmalloc(0, 32);
    // writeValTo7SegsHex(0x11111111);
    int *addr2 = (int *)kmalloc(0, 32);
    // writeValTo7SegsHex(0x11111111);
    int *addr3 = (int *)kmalloc(0, 32);
    // writeValTo7SegsHex(0x11111111);
    int *addr4 = (int *)kmalloc(0, 32);
    // writeValTo7SegsHex(0x11111111);
    writeValTo7SegsHex(((uint32_t)addr1));
    // writeValTo7SegsHex(0x11111111);
    writeValTo7SegsHex(((uint32_t)addr2));
    // writeValTo7SegsHex(0x11111111);
    writeValTo7SegsHex(((uint32_t)addr3));
    // writeValTo7SegsHex(0x11111111);
    writeValTo7SegsHex(((uint32_t)addr4));

    // *((int *)addr1) = 123;
    // *((int *)addr2) = 456;
    // *((int *)addr3) = 987;
    // *((int *)addr4) = 555;
    // writeValTo7SegsDec(*((int *)addr1));
    
    // writeValTo7SegsDec(*((int *)addr2));
    
    // writeValTo7SegsDec(*((int *)addr3));
    // writeValTo7SegsDec(*((int *)addr4));
    
    return 1;
}

int test_free_memory(){
    return 1;
}


/**
 * 测试kmalloc与kfree函数
 * 首先申请两块内存，可以看到这两块内存的地址是不相同的
 * 然后释放第二块内存，再次申请，会得到相同的地址
 */
bool test_alloc_free(){
    int *addr = (int *)kmalloc(0, 32);
    writeValTo7SegsHex(((uint32_t)addr));
    addr = (int *)kmalloc(0, 32);
    writeValTo7SegsHex(((uint32_t)addr));
    kfree(addr);
    addr = (int *)kmalloc(0, 32);
    writeValTo7SegsHex(((uint32_t)0));
    writeValTo7SegsHex(((uint32_t)addr));
}

/**
 * 大小为1024的block位于页面的最低端，申请该大小的内存块返回的会是页的地址
 */
bool test_alloc_page_block(){
    int *addr = (int *)kmalloc(0, 4096);
    writeValTo7SegsHex(((uint32_t)addr));
    addr = (int *)kmalloc(0, 1024);
    writeValTo7SegsHex(((uint32_t)addr));
}

/**
 * 不同进程之间可以申请到的block相互隔离
 */
bool test_alloc_block(){
    int *addr = (int *)kmalloc(0, 32);
    writeValTo7SegsHex(((uint32_t)addr));
    addr = (int *)kmalloc(1, 32);
    writeValTo7SegsHex(((uint32_t)addr));
}

/**
 * 测试读写内存
 */
bool test_rw_block(){
    int *addr = (int *)kmalloc(0, 32);
    *addr = 123;
    writeValTo7SegsHex(*addr);
}