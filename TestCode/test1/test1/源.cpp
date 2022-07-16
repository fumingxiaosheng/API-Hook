#include<stdio.h>
#include<windows.h>
//对堆进行相应的测试
int main(void) {
	HANDLE hHeap;
	int i = 100;
	while (i--) {
		hHeap= HeapCreate(0, i, i);
		if (i % 2 == 0) HeapDestroy(hHeap);
	}
	printf("This is the test of heap suspecting!\n");
	char c = getchar();
	return 0;
}