#include "core.h"
#include "drivers.h"

void f1(void){
	Usart0_Init();
	unsigned char i;
	for(i = 0;i < 10;i++)
		Usart0_Puts("aaaa");
}

int main(){
	unsigned  char i;
	Usart0_Init();
	task_state_init();
	for(i = 0;i < 10;i++)
	Usart0_Puts("Skip the first part of the data\t\t\t\t\t\t");
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" status:\t");
		Usart0_Puts(task_show_status(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	task_create(f1,1);
	task_create(f1,2);
	task_create(f1,3);
	task_create(f1,4);
	start();
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" status:\t");
		Usart0_Puts(task_show_status(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	task_lock();
	task_suspend(3);
	task_suspend(4);
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" status:\t");
		Usart0_Puts(task_show_status(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	task_unlock();
	task_awake(3);
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" status:\t");
		Usart0_Puts(task_show_status(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	task_delay(100);
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" status:\t");
		Usart0_Puts(task_show_status(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tt");
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" delay:\t");
		Usart0_Putc(task_show_delay(i));
		Usart0_Puts("\t\t\t");
	}
	Usart0_Puts("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	for(i = 0;i < Tasknum;i++){
		Usart0_Puts("Task ");
		Usart0_Putc(i + '0');
		Usart0_Puts(" stack:\t");
		Usart0_Putsb(task_show_stack(i));
		Usart0_Puts("\t\t\t");
	}
	return 0;
}

//100
