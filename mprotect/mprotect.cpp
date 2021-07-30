//mmc.cpp 以下两个例子都是可以使用，只是逻辑处理有点差异

#if __linux__

#include "mprotect.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <gtest/gtest.h>

#define handle_error(msg) \
   do { perror(msg); exit(EXIT_FAILURE); } while (0)

static char *buffer;
static void handler(int sig, siginfo_t *si, void *unused)
{
   printf("Got SIGSEGV at address: 0x%lx\n",
           (long) si->si_addr);
   exit(EXIT_FAILURE);
}

void MProtect::examp1()
{
	char *p;
	int pagesize;
	struct sigaction sa;

	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	if (sigaction(SIGSEGV, &sa, NULL) == -1)
	   handle_error("sigaction");

	pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
	   handle_error("sysconf");

	/* Allocate a buffer aligned on a page boundary;
	  initial protection is PROT_READ | PROT_WRITE */

	buffer = (char *)memalign(pagesize, 4 * pagesize);
	if (buffer == NULL)
	   handle_error("memalign");

	printf("Start of region:        0x%lx\n", (long) buffer);

	if (mprotect(buffer + pagesize * 2, pagesize,PROT_READ) == -1)
	    handle_error("mprotect");

	for (p = buffer ; ; )
               *(p++) = 'a';
	
#if 0
	/* 测试发现len需要为页大小倍数,如果不为页大小倍数情况下，系统会匹配最大页大小倍数，
	   比如页大小为4k，len小于4k，修改范围[addr, addr+4k-1],如果大于4k小于8k，修改范围[addr, addr+8k-1],
	   以此类推。
	*/
	if (mprotect(buffer, 4097,PROT_READ) == -1)
		  handle_error("mprotect");
	int i = 0; 
	/* i<0x4000 (4 * pagesize) */
	for (p = buffer ;i<0x4000 ;p++)
	{
		if (i++ < 8192)
			continue;
		*p = 'a';
	}
	printf("End of region:        0x%lx\n", (long) buffer+i);
#endif
	printf("Loop completed\n");     /* Should never happen */
	//exit(EXIT_SUCCESS);

}

TEST(MProtect, example1)
{
	//MProtect::examp1();
}

int *g_ps32Result;
void add(int a, int b)
{
    *g_ps32Result = a + b;
}
void subtract(int a, int b)
{
    *g_ps32Result = a - b;
}

TEST(MProtect, example2)
{
	//MProtect::examp2();
}

void MProtect::examp2()
{
	 int ret;
    int l_s32PageSize;
    
    /* 获取操作系统一个页的大小, 一般是 4KB == 4096 */
    l_s32PageSize = sysconf(_SC_PAGE_SIZE);
    if (l_s32PageSize == -1) {
        perror("sysconf fail");
		return;
    }
    printf("One Page Size is:%d Byte\r\n", l_s32PageSize);

    /* 按页对齐来申请一页内存, g_ps32Result会是一个可以被页(0x1000 == 4096)整除的地址 */
    ret = posix_memalign((void**)&g_ps32Result, l_s32PageSize, l_s32PageSize);
    if (ret != 0) {
        /* posix_memalign 返回失败不会设置系统的errno, 不能用perror输出错误 */
        printf("posix_memalign fail, ret %u\r\n", ret);
		return;
    }
    printf("posix_memalign mem %p\r\n", g_ps32Result);

    add(1, 1); // 结果写入 *g_ps32Result
    printf("the g_ps32Result is %d\n", *g_ps32Result);

    /* 保护g_ps32Result指向的内存, 权限设为只读 mprotect区间开始的地址start
       必须是一个内存页的起始地址，并且区间长度len必须是页大小的整数倍 */
    ret = mprotect(g_ps32Result, l_s32PageSize, PROT_READ);
    if (ret == -1) {
        perror("mprotect");
		return;
    }

    subtract(1, 1); // 结果写入 *g_ps32Result, 但 *g_ps32Result 的内存地址已设为只读, 所以会引发segment fault
    printf("the g_ps32Result is %d\n", *g_ps32Result);

    /* 申请一定记得释放 */
    free(g_ps32Result);
}

#endif
