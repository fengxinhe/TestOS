

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	struct task* p_task;
	struct proc* p_proc= proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
        u8    privilege;
        u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS+NR_PROCS; i++) {
	        if (i < NR_TASKS) {     /* 任务 */
                        p_task    = task_table + i;
                        privilege = PRIVILEGE_TASK;
                        rpl       = RPL_TASK;
                        eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio      = 15;
                }
                else {                  /* 用户进程 */
                        p_task    = user_proc_table + (i - NR_TASKS);
                        privilege = PRIVILEGE_USER;
                        rpl       = RPL_USER;
                        eflags    = 0x202; /* IF=1, bit 2 is always 1 */
			prio      = 5;
                }

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid = i;			/* pid */

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		/* p_proc->nr_tty		= 0; */

		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p_proc->filp[j] = 0;

		p_proc->ticks = p_proc->priority = prio;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

        /* proc_table[NR_TASKS + 0].nr_tty = 0; */
        /* proc_table[NR_TASKS + 1].nr_tty = 1; */
        /* proc_table[NR_TASKS + 2].nr_tty = 1; */

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int fd;
	int i, n;

	char tty_name[] = "/dev_tty0";

	char rdbuf[128];


	int fd_stdin  = open(tty_name, O_RDWR);
	assert(fd_stdin  == 0);
	int fd_stdout = open(tty_name, O_RDWR);
	assert(fd_stdout == 1);

//	char filename[MAX_FILENAME_LEN+1] = "zsp01";
	const char bufw[80] = {0};
//	const int rd_bytes = 3;
//	char bufr[rd_bytes];
	clear();
	printf("\n\n\n\n\n\n");
	printf("                         HHHHHHHHHHHHHHHH\n");
	printf("                         HH            HH\n");
	printf("                         HH            HH\n");
	printf("                         HHHHHHHHHHHHHHHH\n");
	printf("                         HH      \n");
	printf("                         HH    HHHHHHH   \n");
	printf("                        HH     HH   HH\n");
	printf("                        HH     HHHHHHH\n");
	printf("                        HH        HH      \n");
	printf("                        HH    HHHHHHHHHHH\n");
	printf("                       HH     HH  HH   HH\n");
	printf("                       HH     HH  HH  HHH\n");
	printf("                      HH          HH  \n");
	printf("                     HH           HH   \n");
	
	// milli_delay(3000);


	// clear();
    
 //    	printf("\n\n\n\n\n\n\n\n\n");
	// printf("                        System is starting.....\n");milli_delay(2000);
	// printf("                        Super nuclear docking is ongoing......        \n");milli_delay(2000);
	// printf("                        HH is very handsome......\n");milli_delay(2000);
	// printf("                        HH takes off the clothes.....\n");milli_delay(2000);
	// printf("                        HH takes off the pants......\n");milli_delay(2000);
	// printf("                        Q: what's left of HH?\n");milli_delay(2000);
	// printf("                        Loading......90% \n");milli_delay(2000);
	// printf("                        Loading complete.......\n");milli_delay(2000);
	// printf("                        Jump to the console.......\n");

	// milli_delay(2000);






	clear();
	printf("                        ===================================\n");
	printf("                        =             MyTinix             =\n");
	printf("                        =         Kernel on Orange's      =\n");
	printf("                        =               Main              =\n");
	printf("                        =Designed by FengXinHe & HuangHang=\n");
	printf("                        ===================================\n");
	
	while (1) {
		printl("[root@localhost /] ");
		int r = read(fd_stdin, rdbuf, 70);
		rdbuf[r] = 0;
		//show();
        if (strcmp(rdbuf, "process") == 0)
        {
			ProcessManage();
        }
		else if (strcmp(rdbuf, "caculator") == 0)
		{
			Caculate(fd_stdin);
		}
		else if (strcmp(rdbuf, "help") == 0)
		{
			help();
		}
		else if (strcmp(rdbuf, "file") ==0)
		{
			fileManager(fd_stdin);
		}	
		else if (strcmp(rdbuf, "clear") == 0)
		{
			clear();
			printf("                        ===================================\n");
			printf("                        =             MyTinix             =\n");
			printf("                        =         Kernel on Orange's      =\n");
			printf("                        =               Main              =\n");
			printf("                        =Designed by FengXinHe & HuangHang=\n");
			printf("                        ===================================\n");
	
		}
		else
			printf("Can not find the command, please check!\n");
	}

	
	//assert(rd_bytes <= strlen(bufw));

	///* create */
	//fd = open(filename, O_CREAT | O_RDWR);
	//assert(fd != -1);
	//printl("File created: %s (fd %d)\n", filename, fd);
	//


	///* write */
	//n = write(fd, bufw, strlen(bufw));
	//assert(n == strlen(bufw));

	///* close */
	//close(fd);

	///* open */
	//fd = open(filename, O_RDWR);
	//assert(fd != -1);
	//printl("File opened. fd: %d\n", fd);

	///* read */
	//n = read(fd, bufr, rd_bytes);
	//assert(n == rd_bytes);
	//bufr[n] = 0;
	//printl("%d bytes read: %s\n", n, bufr);

	///* close */
	//close(fd);

	//char * filenames[] = {"/foo", "/bar", "/baz"};

	///* create files */
	//for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
	//	fd = open(filenames[i], O_CREAT | O_RDWR);
	//	assert(fd != -1);
	//	printl("File created: %s (fd %d)\n", filenames[i], fd);
	//	close(fd);
	//}

	//char * rfilenames[] = {"/bar", "/foo", "/baz", "/dev_tty0"};

	///* remove files */
	//for (i = 0; i < sizeof(rfilenames) / sizeof(rfilenames[0]); i++) {
	//	if (unlink(rfilenames[i]) == 0)
	//		printl("File removed: %s\n", rfilenames[i]);
	//	else
	//		printl("Failed to remove file: %s\n", rfilenames[i]);
	//}

	//spin("TestA");
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	while(1){

	}
	
}


void TestC()
{
	spin("TestC");
}


/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

//clear
void clear()
{
	clear_screen(0,console_table[current_console].cursor);
	console_table[current_console].crtc_start = 0;
	console_table[current_console].cursor = 0;
	
}

void help()
{
	printf("=============================================================================\n");
	printf("Command List     :\n");
	printf("1. process       : Show the Multilevel queue feedback scheduling algorithm\n");
	printf("2. caculator      : Make a simple caculate\n");
	printf("3. file          : Enter the file management system\n");
	printf("4. clear         : Clear the screen\n");
	printf("5. help          : Show this help message\n");
	printf("==============================================================================\n");		
}

//caculate
void Caculate(fd_stdin)
{
	clear();
	printf("                        ===================================\n");
	printf("                        =             MyTinix             =\n");
	printf("                        =         Kernel on Orange's      =\n");
	printf("                        =            Caculator            =\n");
	printf("                        =Designed by FengXinHe & HuangHang=\n");
	printf("                        ===================================\n");
	
	int flag = 1;
	while(flag == 1)
	{

		char cResult;
		char buf[80]={0};

		char symbol;
		char n;
		int x,y;
		char szCmd[80]={0};
		char szCmd2[80]={0};

 		printf("Please input the first number: "); 
		//scanf("%d", x);
		n = read(fd_stdin,szCmd,80);
		//szCmd[10] = 0;
		atoi(szCmd,&x);
		printf("Please input the second number: "); 
		//scanf("%d", y);
		n = read(fd_stdin,szCmd2,80);
		//szCmd[10] = 0;
		atoi(szCmd2,&y);
		printf("Please input the operator: "); 
		read(fd_stdin,buf,2);
		symbol = buf[0];
		//atoi(szCmd,&symbol);

		//printf("x = %d , y = %d \n", x,y); 
		if(symbol == '+')
		{
			printf("The answer is %d\n", x + y);
		}
		else if(symbol == '-')
		{
			printf("The answer is %d\n", x - y);
		}
		else if(symbol == '/')
		{
			if(y == 0)
			{
				printf("The second number in division can not be 0 !\n");
				continue;
			}
			printf("The answer is %d\n", x/y);
		}	
		else if(symbol == '*')
		{
			printf("The answer is %d\n", x * y);
		}
		else
		{
			printf("Input Error!\n");
		}
		int a = 1;
		while(a == 1)
		{
			printf("Do you want to do more caculate?(y/n):");		
			read(fd_stdin,buf,2);
			cResult = buf[0];
			if(cResult == 'N' || cResult == 'n')
			{
				clear();
				printf("                        ===================================\n");
				printf("                        =             MyTinix             =\n");
				printf("                        =         Kernel on Orange's      =\n");
				printf("                        =               Main              =\n");
				printf("                        =Designed by FengXinHe & HuangHang=\n");
				printf("                        ===================================\n");
				a = 2;
				flag = 2;
			}
			else if(cResult == 'Y' || cResult == 'y')
			{
				a = 2;
				//break;
			} 
			else
			{
				printf("Can not identify!Choose again!\n");
			}
		}
	}
}

//file
struct file
{
	//int no;		//当前内容数组到的位置
	char* name;		//文件名
	char* content;	//文件内容
}f[2];

int file_count = 0;
char filename_1[20]={0};
char content_1[50]={0};	
char filename_2[20]={0};
char content_2[50]={0};

void fileManager(fd_stdin)
{

	char cResult;
	char buf[80]={0};

	int choose;

	char rdbuf2[128];

	clear();
	printf("                        ===================================\n");
	printf("                        =             MyTinix             =\n");
	printf("                        =         Kernel on Orange's      =\n");
	printf("                        =            File Model           =\n");
	printf("                        =Designed by FengXinHe & HuangHang=\n");
	printf("                        ===================================\n");

	

	if(file_count == 0)
	{
		printf("No file! Please add some files.\n");
		printf("Please enter the file name for the new file : ");
		
		read(fd_stdin,filename_1,20);
		f[0].name = filename_1;
		// strcpy(f[0].name, filename);
		file_count++;

		while (1) 
		{
			printl("[>>] ");
			int r = read(fd_stdin, rdbuf2, 70);
			rdbuf2[r] = 0;
       		if (strcmp(rdbuf2, "show") == 0)
        	{
				//展示函数
				printf("File name : %s\n", f[0].name);
				printf("File content : %s\n", f[0].content);
        	}
			else if (strcmp(rdbuf2, "add") == 0)
			{
				//添加函数
				//char Result;
				printf("Please enter the content you want to add to the file : ");							
				read(fd_stdin,content_1,50);
				//printf("test..........content : %s\n", content);
				f[0].content = content_1;
				//Result = content[0];
				//strcpy(f[0].content, content);
				printf("Edit success!\nContent : %s\n", f[0].content);
			}
			else if (strcmp(rdbuf2, "delete") == 0)
			{
				//删除函数
				// char content[50]={0};
				// // strcpy(f[0].content, content);
				// read(fd_stdin,f[0].content[50],50);
				f[0].content = "\0";
				printf("Delete success!\n");
			}
			else if (strcmp(rdbuf2, "help") == 0)
			{
				file_help();
			}
			else if (strcmp(rdbuf2, "clear") == 0)
			{
				clear();
				printf("                        ===================================\n");
				printf("                        =             MyTinix             =\n");
				printf("                        =         Kernel on Orange's      =\n");
				printf("                        =            File Model           =\n");
				printf("                        =Designed by FengXinHe & HuangHang=\n");
				printf("                        ===================================\n");
			}
			else if (strcmp(rdbuf2, "quit") == 0)
			{
				break;
			}
			else
			{
				printf("Can not find the command, please check!\n");
			}
		}
	}
	else
	{		
		int i;
		for(i = 0; i < file_count; i++)
		{
			printf("%d. %s\n", i + 1 , f[i].name);
		}
		printf("%d. Create a new file\n", file_count + 1);
		printf("Please choose the file or create a new file(enter the number before the options):");
		read(fd_stdin,buf,2);
		//cResult = buf[0];
		atoi(buf, &choose);
		if(choose == 1)
		{

			clear();
			printf("                        ===================================\n");
			printf("                        =             MyTinix             =\n");
			printf("                        =         Kernel on Orange's      =\n");
			printf("                        =            File Model           =\n");
			printf("                        =Designed by FengXinHe & HuangHang=\n");
			printf("                        ===================================\n");
			printf("File open success!\n");

			while (1) 
			{
				printl("[>>] ");
				int r = read(fd_stdin, rdbuf2, 70);
				rdbuf2[r] = 0;
       			if (strcmp(rdbuf2, "show") == 0)
	        	{
					//展示函数
					printf("File name : %s\n", f[0].name);
					printf("File content : %s\n", f[0].content);
        		}
				else if (strcmp(rdbuf2, "add") == 0)
				{
					//添加函数
					//char Result;
					printf("Please enter the content you want to add to the file : ");							
					read(fd_stdin,content_1,50);
					//printf("test..........content : %s\n", content);
					f[0].content = content_1;
					//Result = content[0];
					//strcpy(f[0].content, content);
					printf("Edit success!\nContent : %s\n", f[0].content);
				}
				else if (strcmp(rdbuf2, "delete") == 0)
				{
					//删除函数
					// char content[50]={0};
					// // strcpy(f[0].content, content);
					// read(fd_stdin,f[0].content[50],50);
					f[0].content = "\0";
					printf("Delete success!\n");
				}
				else if (strcmp(rdbuf2, "help") == 0)
				{
					file_help();
				}
				else if (strcmp(rdbuf2, "clear") == 0)
				{
					clear();
					printf("                        ===================================\n");
					printf("                        =             MyTinix             =\n");
					printf("                        =         Kernel on Orange's      =\n");
					printf("                        =            File Model           =\n");
					printf("                        =Designed by FengXinHe & HuangHang=\n");
					printf("                        ===================================\n");
				}
				else if (strcmp(rdbuf2, "quit") == 0)
				{
					break;
				}
				else
				{
					printf("Can not find the command, please check!\n");
				}
			}
		}
		else if(choose == file_count + 1)
		{

			clear();
			printf("                        ===================================\n");
			printf("                        =             MyTinix             =\n");
			printf("                        =         Kernel on Orange's      =\n");
			printf("                        =            File Model           =\n");
			printf("                        =Designed by FengXinHe & HuangHang=\n");
			printf("                        ===================================\n");

			printf("Please enter the file name for the new file : ");
			read(fd_stdin,filename_2,20);
			f[1].name = filename_2;
			while (1) 
			{
				printl("[>>] ");
				int r = read(fd_stdin, rdbuf2, 70);
				rdbuf2[r] = 0;
       			if (strcmp(rdbuf2, "show") == 0)
	        	{
					//展示函数
					printf("File name : %s\n", f[1].name);
					printf("File content : %s\n", f[1].content);
        		}
				else if (strcmp(rdbuf2, "add") == 0)
				{
					//添加函数
					//char Result;
					printf("Please enter the content you want to add to the file : ");							
					read(fd_stdin,content_2,50);
					//printf("test..........content : %s\n", content);
					f[1].content = content_2;
					//Result = content[0];
					//strcpy(f[0].content, content);
					printf("Edit success!\nContent : %s\n", f[1].content);
				}
				else if (strcmp(rdbuf2, "delete") == 0)
				{
					//删除函数
					// char content[50]={0};
					// // strcpy(f[0].content, content);
					// read(fd_stdin,f[0].content[50],50);
					f[1].content = "\0";
					printf("Delete success!\n");
				}
				else if (strcmp(rdbuf2, "help") == 0)
				{
					file_help();
				}
				else if (strcmp(rdbuf2, "clear") == 0)
				{
					clear();
					printf("                        ===================================\n");
					printf("                        =             MyTinix             =\n");
					printf("                        =         Kernel on Orange's      =\n");
					printf("                        =             FILE MODEL          =\n");
					printf("                        =Designed by FengXinHe & HuangHang=\n");
					printf("                        ===================================\n");
				}
				else if (strcmp(rdbuf2, "quit") == 0)
				{
					break;
				}
				else
				{
					printf("Can not find the command, please check!\n");
				}
			}
			file_count++;
		}
		else
		{

		}
	}

	clear();
	printf("                        ===================================\n");
	printf("                        =             MyTinix             =\n");
	printf("                        =         Kernel on Orange's      =\n");
	printf("                        =               Main              =\n");
	printf("                        =Designed by FengXinHe & HuangHang=\n");
	printf("                        ===================================\n");
}


void file_help()
{
	printf("===============================================================================\n");
	printf("Command List For File  :\n");
	printf("1. add          : Add some content to the file\n");
	printf("2. show         : Show the file information(include file name and file content)\n");
	printf("3. clear        : Clear the screen\n");
	printf("4. delete       : Delete the file content\n");
	printf("5. help         : Show this help message\n");
	printf("6. quit         : Quit the file model\n");
	printf("================================================================================\n");	
}

void ProcessManage()
{
int i;
	//printf("=============================================================================\n");
	//printf("      myID      |    name       | spriority    | running?\n");
	//进程号，进程名，优先级，是否是系统进程，是否在运行
	printf("-----------------------------------------------------------------------------\n");
	//for ( i = 0 ; i < NR_TASKS + NR_PROCS ; ++i )//逐个遍历
	//{
//		if ( proc_table[i].priority == 0) continue;//系统资源跳过
	//	printf("        %d           %s            %d                yes\n", proc_table[i].pid, proc_table[i].name, proc_table[i].priority);
	//}
printf("test process    |     time\n");
printf("test1()         |     15\n");
printf("test2()         |     40\n");
printf("test3()         |     5\n");
printf("test4()         |     30\n");
printf("test5()         |     1\n");

	printf("=============================================================================\n");
int t1pro=15;int t2pro=40;int t3pro=5;int t4pro=30;int t5pro=1;
//PCB[0].name=1;
//PCB[1].name=2;
//PCB[3].name=4;
//PCB[4].name=5;
//PCB[0].time=15;
//PCB[1].time=20;
//PCB[2].time=5;
//PCB[3].time=30;
//PCB[4].time=1;
//PCB[0].state=1;
//PCB[1].state=1;
//PCB[2].state=1;
//PCB[3].state=1;
//PCB[4].state=1;


int j=0,k=0;//a=0,b=0,c=0,d=0,e=0;
int queue1[5];
int queue2[25];
////////////////////////
printf("que2323ue 1 running\n");
int t1=4;
for(i=0;i<4;i++){
if(t1pro>t1){
  test1(t1);
  t1pro=t1pro-4;
queue1[0]=1;
j=j+1;
printf(t1);
}
else {test1(t1pro);t1pro=0;}

 if(t2pro>t1){
  test2(t1);
  t2pro=t2pro-t1;
queue1[1]=2;
j=j+1;
}
else {test2(t2pro);t2pro=0;}

 if(t3pro>t1){
  test3(t1);
  t3pro=t3pro-t1;
queue1[2]=3;
j=j+1;
}
else {test3(t4pro);t3pro=0;}

 if(t4pro>t1){
  test4(t1);
  t4pro=t4pro-t1;
queue1[3]=4;

}
else {test4(t4pro);t4pro=0;}

 if(t5pro>t1){
  test5(t1);
  t5pro=t5pro-t1;
queue1[4]=5;
j=j+1;
}
else {test5(t5pro);t5pro=0;}
break;

}
printf("\n");
printf("queue 1 over\n");
/////////////////////////////////////////////
int t2=8;
printf("queue2  running\n");
for(i=0;i<4;i++)
{
if(t1pro>t2&&queue1[i]==1){
  test1(t2);
  t1pro=t1pro-t2;
queue2[k]=1;
k=k+1;

}
else if(t1pro<=t2&&queue1[i]==1) {test1(t1pro);t1pro=0;}

if(t2pro>t2&&queue1[i]==2){
  test2(t2);
  t2pro=t2pro-t2;
queue2[k]=2;
k=k+1;

}
else if(t2pro<=t2&&queue1[i]==2) {test2(t2pro);t2pro=0;}

if(t3pro>t2&&queue1[i]==3){
  test3(t2);
  t3pro=t3pro-t2;
queue2[k]=3;
k=k+1;

}
else if(t3pro<=t2&&queue1[i]==3) {test3(t3pro);t3pro=0;}

if(t4pro>t2&&queue1[i]==4){
  test4(t2);
  t4pro=t4pro-t2;
queue2[k]=4;
k=k+1;

}
else if(t4pro<=8&&queue1[i]==4) {test4(t4pro);t4pro=0;}

if(t5pro>t2&&queue1[i]==5){
  test5(t2);
  t5pro=t5pro-t2;
queue2[k]=5;
k=k+1;

}
else if(t5pro<=t2&&queue1[i]==5) {test5(t5pro);t5pro=0;}

}
printf("\n");
printf("queue 2 over\n");
///////////////////////////
int total=t1pro+t2pro+t3pro+t4pro+t5pro;
int t3=12;
int sum=0;
for(i=0;i<100;i++)
{
  if(queue2[i]!=0) sum=sum+1;
else break;
}
printf("queue 3 running\n");
for(i=0;i<total;i++)
{
if(t1pro>t3&&queue2[i]==1){
  test1(t3);
  t1pro=t1pro-t3;
  queue2[sum+i]=1;
}
else if(t1pro<=t3&&queue2[i]==1) 
{test1(t1pro);
t1pro=0;}

if(t2pro>t3&&queue2[i]==2){
  test2(t3);
  t2pro=t2pro-t3;
  queue2[sum+i]=2;
}
else if(t2pro<=t3&&queue2[i]==2) {test2(t2pro);t2pro=0;}

if(t3pro>t3&&queue2[i]==3){
  test3(t3);
  t3pro=t3pro-t3;  
    queue2[sum+i]=3;
}
else if(t2pro<=t3&&queue2[i]==3) {test3(t3pro);t3pro=0;}

if(t4pro>t3&&queue2[i]==4){
  test4(t3);
  t4pro=t4pro-t3;
    queue2[sum+i]=3;
}
else if(t4pro<=t3&&queue2[i]==4) {test4(t4pro);t4pro=0;}

if(t5pro>t3&&queue2[i]==5){
  test5(t3);
  t5pro=t5pro-t3;
    queue2[sum+i]=3;
}
else if(t5pro<=t3&&queue2[i]==5) {test5(t5pro);t5pro=0;}

}
printf("\n");
printf("queue 3 over\n");

}

void test1(int time)
{
while(time)
{
time--;printf("a");
milli_delay(10);}
}
void test2(int time){
while(time)
{
time--;printf("b");
milli_delay(10);}
}
void test3(int time){
while(time)
{
time--;printf("c");
milli_delay(10);}
}
void test4(int time){
while(time)
{time--;printf("d");
milli_delay(10);
}
}
void test5(int time){
while(time)
{time--;printf("e");
milli_delay(10);}
}

