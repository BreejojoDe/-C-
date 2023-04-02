#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>            //windows函数，linux系统不包含
#include <conio.h>              //键盘操作
#include <ctype.h>              //字符映射
#include <time.h>
#include <graphics.h>           //使用图形库
#include <mmsystem.h>           //Windows媒体
#include "password_h.h"         /****密码系统(自主编写的头文件)****/

#pragma comment(lib,"Winmm.lib")//引用Windows Multimedia API

#define NUM_SUBJECT 4           //科目
#define N_MAX 80                //最大信息条数（可更改）
#define N2 20                   //输入name，ID最大字符
#define random(n) (rand()%n)    //随机函数取余取随机数

/**储存单位：学生信息结构体数组（程序运行缓存）**/
struct stu
{
    char ID[15];
    char name[20];
    int score[NUM_SUBJECT];
    double aver;
    int sum;
} student[N_MAX];

/**记录平均值统计信息**/
struct stu_all
{
    double score[NUM_SUBJECT],
           aver,
           sum;
} subject_aver;

int num = 0;           /**已有学生信息个数（Readout初始化）**/
struct stu stunull = {'\0','\0',0,0,0,0,0,0};   //信息结构体缓存或指向
int userRight = 0;     /**学生端权限1，教师端权限2，根据输入的密码确定**/

/**********************游戏全局变量********************************/
//游戏模块较为特殊，需要多个函数同时访问修改变量，此时全局变量较适宜    //
int position_x = 0, position_y = 0;              //飞机位置        //
int high, width;                                 //画面尺寸        //
bool isFire = false, over = false, stop = false; //状态检查        //
int bullet_x, bullet_y;                          //子弹位置        //
int enemy_x, enemy_y;                            //敌人位置        //
int score = 0, miss = 0;                         //得分失分        //
int grade = 1;                                   //难度等级        //
                                                                   //
/**************************END**************************************/

/************************游戏函数9*********************************/
                                                                   //
void IsEnterGames(void);                         //确认进入游戏    //
int RelaxTime(void);                             //游戏主控        //
void startup();                                  //游戏初始化      //
void show();                                     //屏幕输出        //
void updateWithoutInput();                       //游戏数据更新    //
void updateWithInput();                          //键盘交互        //
void gameEnd();                                  //游戏结束        //
void gotoxy(int x, int y);                       //光标移动        //
void HideCursor();                               //光标隐藏        //
                                                                   //
/***************************END*************************************/


/**********************学生信息管理系统24**************************/

int StartUp(void);                                     //初始化
int ReadOut(struct stu *p);                            //从文件中读取数据
int WriteIn(struct stu *p);                            //数据写入文件
int Input(struct stu *p);                              //新增学生信息，会同时增加到程序缓存并更新文件
int ChangeStu(struct stu *p);                          //更改学生信息，写入文件
int DeleteStu(struct stu *p);                          //删除学生信息，写入文件
int ShowInformation(struct stu *p);                    //展示所有程序缓存
int RightCheck(void);                                  //权限检查
void InformationInitialization(void);                  //随机生成学生信息文件
void ExitSystem(void);                                 //退出系统
void MainMenu(void);                                   //主菜单
void StuInformationMenu(void);                         //学生信息操作菜单
void SortMenu(void);                                   //排序菜单
void PasswordMenu(void);                               //密码菜单
void PrintStu(struct stu *p);                          //输出单个学生信息
void SortSumDown(struct stu *p);                       //按总分降序排序
void SortSumUp(struct stu *p);                         //按总分升序排序
void SortIDUp(struct stu *p);                          //按学号升序排序
void SortNameUp(struct stu *p);                        //按姓名升序排序
void CategoricalStatistics(struct stu *p);             //分类统计各科目各分数段人数并输出
struct stu *SearchID_input(struct stu *p);             //输入ID准确查找
struct stu *SearchID_part(struct stu *p,char *str);    //内嵌传入ID查找（检测是否重复输入）
struct stu *SearchName(struct stu *p);                 //输入姓名模糊查找
struct stu *LookUp(struct stu *p, char *str);          //模糊查找

/***************************END*************************************/

int main()
{
    struct stu *p = student;        //指针指向结构体数组

    StartUp();                      //程序初始化，包括启动密码系统
    ReadOut(p);                     //读取文件，缓存数据初始化
    for(;;) {
        MainMenu();                 //程序主菜单
    }

    return 0;
}

//==================================================================
//函 数 名：StartUp
//功能描述：信息管理系统初始化
//输入参数：void
//返 回 值：int（main函数中未使用）
//调用函数：PassCheck();
//==================================================================
int StartUp(void)
{
    srand((unsigned) time(NULL));   //时间种子
    system("color F0");             //改变控制台颜色

    userRight = PassCheck();        //密码检查，储存到全局变量userRight

    if(userRight ==1) {
        printf("学生端\n");
        return 1;
    } else if (userRight == 2) {
        printf("教师端\n");
        return 2;
    }

    return 0;
}

//==================================================================
//函 数 名：RightCheck
//功能描述：权限检查（根据密码系统输入的密码确定权限）
//输入参数：void
//返 回 值：int（功能类似bool，但为了多于两种的权限的拓展性，使用int返回权限状态）
//调用函数：无; 该函数可重复被其他函数调用
//==================================================================
int RightCheck(void)
{
    if(userRight==1) {
        printf("\n\n\t\t\t___________暂无权限__________\n\n");
        return false;       //学生权限*检测失败
    } else {
        return true;
    }
}

//==================================================================
//函 数 名：ReadOut
//功能描述：从文件中读取结构体信息（文件操作）
//         检查是否存在文件，若无，随机生成一个文件
//输入参数：struct stu *p（结构体指针）
//返 回 值：读取到的信息个数（int）
//调用函数：InformationInitialization()文件初始化;
//         该函数多次被调用
//==================================================================
int ReadOut(struct stu *p)
{
    num = 0;
    FILE *fp = fopen(".\\ause\\or.txt","r");

    //若未检测到则初始化文件
    if(fp==NULL) {
        printf("首次进入,正在初始化数据\n\n");
        InformationInitialization();
        printf("文件or.txt初始化成功, 请重新启动\n");
        getch();
        exit(0);
    }
    //读取信息，并在读取同时计算平均得分与总分
    while (!feof(fp)) {
        fscanf(fp,"%s",p->ID);
        fscanf(fp," %s",p->name);
        for(int i=0; i<NUM_SUBJECT; i++) {
            fscanf(fp," %d",&p->score[i]);
            subject_aver.score[i] += p->score[i];
        }
        fscanf(fp," %lf",&p->aver);
        subject_aver.aver += p->aver;
        fscanf(fp," %d",&p->sum);
        subject_aver.sum += p->sum;
        p++;
        num++;
    }
    for(int i=0; i<NUM_SUBJECT; i++) {
        subject_aver.score[i] /= num;
    }
    subject_aver.sum /= num;
    subject_aver.aver /= num;

    fclose(fp);//关闭文件

    return num;
}

//==================================================================
//函 数 名：WriteIn
//功能描述：把结构体缓存的信息写入文件（覆盖原文件）
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：文件调用状态，0成功，-1失败（可改为writein_times写入次数）
//调用函数：C标准函数sprintf，将其他数据写入一个字符串
//         该函数多次被其他函数调用
//==================================================================
int WriteIn(struct stu *p)
{
    int writein_times=0;
    char information[100] = {0};
    FILE *fp = fopen(".\\ause\\or.txt","w");
    if(fp==NULL) {
        printf("读取失败");
        return -1;
    }

    //fputs将字符写入文件
    for(; p<student+num; p++) {
        sprintf(information,"%s %s %d %d %d %d %.2f %d",p->ID,p->name,p->score[0],p->score[1],p->score[2],p->score[3],p->aver,p->sum);
        if(writein_times!=0) {
            fputs("\n",fp);
        }
        fputs(information,fp);
        writein_times++;
    }

    fclose(fp);

    return 0;
}

//==================================================================
//函 数 名：Input
//功能描述：输入单条信息，包括重复错误检查
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：这次调用输入的信息数（input_times）
//调用函数：C标准sprintf函数
//==================================================================
int Input(struct stu *p)
{
    struct stu temp,
               *q = &temp;      //temp和q缓存输入信息
    char order = '\0',          //键盘输入的命令
         information[100],      //录入信息
         ID_check[15] = {0};    //学号检查缓存
    int input_times=0;

    temp.sum=0;

    //打开文件，p指向结构体数组末尾
    FILE *fp = fopen(".\\ause\\or.txt","a");
    if(fp==NULL) {
        printf("读取失败");
        return -1;
    }
    p = p+num;

    //输入信息
    for(;;) {
        printf("请输入学生信息（六位ID 姓名 四科成绩,用空格隔开，例如：100001 aa 99 99 99 99）\n");
        scanf(" %s",ID_check);
        //重复录入的错误检查
        if( (SearchID_part(student,ID_check))!=NULL ){
            printf("该学号已存在\n");
            while(getchar()!='\n')
                ;
            goto ChooseIsContinue;
        } else {
            printf("ID: %s允许录入\n\n",ID_check);
        }
        //存入程序数组缓存
        strcpy(q->ID,ID_check);
        scanf(" %s",q->name);
        for(int i=0; i<NUM_SUBJECT; i++) {
            scanf(" %d",&q->score[i]);
            q->sum += q->score[i];
        }
        q->aver = (double)(q->sum)/NUM_SUBJECT;

        *p++ = *q;
        num++;

        //写入文件
        sprintf(information,"%s %s %d %d %d %d %.2f %d",q->ID,q->name,q->score[0],q->score[1],q->score[2],q->score[3],q->aver,q->sum);
        fputs("\n",fp);
        fputs(information,fp);
        input_times++;

    //循环判断：选择是否继续录入
    ChooseIsContinue:
        printf("是否继续录入？（Y/N）");
        order = getch();
        if(!(order=='y'||order=='Y')) {
            break;
        }
        memset(ID_check,0,sizeof(ID_check));//清空ID_check
    }

    fclose(fp);
    return input_times;
}

//==================================================================
//函 数 名：ChangeStu
//功能描述：修改已有的学生信息，较复杂的错误及学号检查
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：修改状态（int）
//调用函数：SearchID_input()查找输入的ID是否存在及位置
//==================================================================
int ChangeStu(struct stu *p)
{
    struct stu *temp = NULL,
               *pos_changed = NULL; //定位要修改信息在数组中的位置
    char ID_check[15] = {0};
    bool is_wrongID = true,//第一次进入循环
         changethesameone = false;

    while(is_wrongID) {
        is_wrongID = false;         //初始化（类似清零）
        changethesameone = false;

        //检查要修改的学号是否存在，若否，跳过循环过程
        printf("修改目标学号：\n");
        if( (pos_changed = SearchID_input(p))==NULL ) {
            printf("该学号不存在\n");
            is_wrongID = true;
            continue;
        }

        //输入修改后的信息
        printf("\n请输入修改后的信息:\n");
        //第二次ID检查，只允许修改同学号或者不存在的其他学号
        //避免学号ID冲突
        scanf(" %s",ID_check);
        if( strcmp(ID_check,pos_changed->ID)==0 ) {
            changethesameone = true;//判断是否修改同一学号
        } else if ( strcmp(ID_check,"0")==0 ) {
            return 0;
        }
        //该检查的逻辑判断
        if( !( ((SearchID_part(student,ID_check))==NULL) || changethesameone ) ){
            printf("该学号已存在\n");
            while(getchar()!='\n')
                ;
            is_wrongID = true;
            continue;
        }
        //写入数组
        strcpy(pos_changed->ID,ID_check);
        scanf(" %s",pos_changed->name);
        pos_changed->sum = pos_changed->aver = 0;
        for(int i=0; i<NUM_SUBJECT; i++) {
            scanf(" %d",&pos_changed->score[i]);
            pos_changed->sum += pos_changed->score[i];
        }
        pos_changed->aver = (double)(pos_changed->sum)/NUM_SUBJECT;
    }

    //将修改后的数组写入文件
    printf("\n修改后对象：");
    PrintStu(pos_changed);
    WriteIn(student);

    return 0;
}

//==================================================================
//函 数 名：DeleteStu
//功能描述：删除一个已存在的信息
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：删除状态，0成功
//调用函数：SearchID_input()查找输入的ID是否存在及位置
//         WriteIn();写入文件
//         C标准函数getch()，获得键盘输入无需回车
//==================================================================
int DeleteStu(struct stu *p)
{
    struct stu *pos_deleted=NULL,
               *q = NULL;
    bool is_wrongID = true,
         is_keybroad = false;
    char order='\0';

    //检查输入ID是否存在
    while(is_wrongID) {
        is_wrongID = false;
        printf("\n删除对象ID：");
        pos_deleted = SearchID_input(student);
        if( pos_deleted ==NULL ) {
            printf("该学号不存在\n");
            is_wrongID = true;
            continue;
        }
    }

    //确认是否删除并执行删除操作
    printf("\n是否删除？(Y确认，N取消，不用按回车)");
    while(!is_keybroad) {
        order = getch();
        switch(order) {
            case 'y': case 'Y':
                stunull = *pos_deleted;//将删除信息存入缓存stunull
                //将删除位置后信息提前一位
                for(q=pos_deleted; q<student+num-1; q++) {
                    *q = *(q+1);
                }
                //被删除信息存到最后一位
                //信息数-1，使最后一位对所有函数不可见
                //该方法可以保留最后删除的信息，便于恢复信息
                *q = stunull;
                num--;
                is_keybroad = true;
                break;
            case 'n': case 'N':
                printf("取消操作。");
                return 0;
                break;
            default: break;
        }
    }

    //写入文件
    WriteIn(student);

    return 0;
}

//==================================================================
//函 数 名：ShowInformation
//功能描述：输入程序缓存数组的所有信息
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：可拓展为输出数量（int拓展）
//调用函数：无; 该函数多次被其他函数调用
//==================================================================
int ShowInformation(struct stu *p)
{
    printf("\t 学号     姓名     科目      科目      科目      科目      平均分       总分\n");
    for(p=student; p<student+num; p++) {
        printf("\t%s     %s      %3d       %3d       %3d       %3d       %6.2f        %3d\n",p->ID,p->name,p->score[0],p->score[1],p->score[2],p->score[3],p->aver,p->sum);
    }
    printf("\n\t共计%d人  平均分:   %.2lf     %.2lf     %.2lf     %.2lf     %.2lf\t %.2lf",num,subject_aver.score[0],subject_aver.score[1],subject_aver.score[2],subject_aver.score[3],subject_aver.aver,subject_aver.sum);
}

//==================================================================
//函 数 名：MainMenu
//功能描述：主函数菜单，打印可视化界面，便捷的菜单交互（跳转至其他函数）
//输入参数：void
//返 回 值：void
//调用函数：gotoxy();HideCursor();
//         SearchID_,SearchName,--搜索函数
//         StuInformationMenu();--信息操作子菜单
//         Readout();WriteIn(); --文件操作写入
//         SortMenu();          --排序统计菜单
//         PasswordMenu();      --密码系统菜单
//         StuInformationMenu(); ExitSystem();
//==================================================================
void MainMenu(void)
{
    int order_position = 1,     //程序识别命令位置
        order_change=1;         //输入命令缓存（要改变的位置，需要进一步判断）
    bool is_sure = false;       //回车确认判断
    char order = '\0';          //键盘输入检测到的命令
    /*字符串指针数组*/
    char *main_menu[] = {"\t\t\t\t1.查询学号",
                        "\n\n\t\t\t\t2.查询姓名",
                        "\n\n\t\t\t\t3.录入成绩",
                        "\n\n\t\t\t\t4.导出文件",
                        "\n\n\t\t\t\t5.导入文件",
                        "\n\n\t\t\t\t6.查看成绩单",
                        "\n\n\t\t\t\t7.排序",
                        "\n\n\t\t\t\t8.密码系统",
                        "\n\n\t\t\t\t9.退出"};

    HideCursor();

    //输出界面
    for(;;) {
        system("cls"); //清屏函数
        gotoxy(0,0); //光标移动到开始
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               学生信息管理系统                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n");
        //输出 "*" 的位置
        for(int i=0; i<9; i++) {
            printf(main_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //getch()获取键盘输入
        order = getch();
        mciSendString("close tomusic", NULL, 0, NULL); // 关闭上条音频
        mciSendString("open .\\music\\touch2.mp3 alias tomusic", NULL, 0, NULL); // 打开音频
        mciSendString("play tomusic", NULL, 0, NULL); // play仅播放一次
        //将输入转换为order_change（字符-'0'）
        order_change = order_position;
        switch(order) {
            case 'w': case 'W':
                order_change--;
                break;
            case 's': case 'S':
                order_change++;
                break;
            case '\r':         //windows系统回车\n\r
                is_sure = true;//判断输入回车
                break;
            case '1': order_change = order-'0'; break;
            case '2': order_change = order-'0'; break;
            case '3': order_change = order-'0'; break;
            case '4': order_change = order-'0'; break;
            case '5': order_change = order-'0'; break;
            case '6': order_change = order-'0'; break;
            case '7': order_change = order-'0'; break;
            case '8': order_change = order-'0'; break;
            case '9': order_change = order-'0'; break;
            default: break;
        }
        //越界检查，只在1到9（检查order_change）
        if(order_change>=1&&order_change<=9) {
            order_position = order_change;//order_positon为输出位置
        }
        //回车检查
        if(is_sure) {
            break;
        }
    }
    //清屏，打印下一个界面
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               学生信息管理系统                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //命令order_position实现
    switch(order_position){
        case 1: SearchID_input(student); IsEnterGames(); break;//搜ID
        case 2: SearchName(student); IsEnterGames(); break; //搜name
        case 3: StuInformationMenu(); break;       //子菜单1：修改信息
        case 4: ReadOut(student); break;           //文件读取
        case 5: WriteIn(student); break;           //文件写入
        case 6: ShowInformation(student); break;   //展示成绩
        case 7: SortMenu(); break;                 //子菜单2：排序统计
        case 8: PasswordMenu(); break;             //子菜单3：密码系统
        case 9: ExitSystem(); break;               //退出程序
        default: printf("未知错误"); break;         //错误检查，不进入下级
    }

    printf("\n");
    system("pause");
}

//==================================================================
//函 数 名：StuInformationMenu
//功能描述：子菜单1：添加信息，删除信息，修改信息
//输入参数：void
//返 回 值：void（可拓展）
//调用函数：gotoxy();HideCursor();
//         RightCheck();       --权限检查
//         Input(student);     --输入信息
//         ChangeStu(student); --修改信息
//         DeleteStu(student); --删除信息
//==================================================================
void StuInformationMenu(void)
{
    int order_position = 1,
        order_change=1;
    bool is_sure = false;
    char order = '\0';
    char *stuinformation_menu[] = {"\t\t\t\t1.录入信息",
                                   "\n\n\t\t\t\t2.修改信息",
                                   "\n\n\t\t\t\t3.删除信息",
                                   "\n\n\t\t\t\t4.返回上级"};

    HideCursor();

    //显示界面
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               学生信息管理系统                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n\n\n");

        for(int i=0; i<4; i++) {
            printf(stuinformation_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //获得键盘输入并判断
        order = getch();
        order_change = order_position;
        switch(order) {
            case 'w': case 'W':
                order_change--;
                break;
            case 's': case 'S':
                order_change++;
                break;
            case '\r':
                is_sure = true;
                break;
            case '1': order_change = order-'0'; break;
            case '2': order_change = order-'0'; break;
            case '3': order_change = order-'0'; break;
            case '4': order_change = order-'0'; break;
            default: break;
        }
        //边界检查
        if(order_change>=1&&order_change<=4) {
            order_position = order_change;
        }
        //回车确认检查
        if(is_sure) {
            break;
        }
    }
    //清屏，输出下级
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               学生信息管理系统                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //调用RightCheck检查权限userRight,仅教师可操作
    if(order_position!=4 && RightCheck()==false) {
        return;
    }
    //跳转菜单或返回上层
    switch(order_position){
        case 1: Input(student); break;
        case 2: ChangeStu(student); break;
        case 3: DeleteStu(student); break;
        case 4: return; break;
        default: printf("未知错误"); break;
    }

    printf("\n");
}

//==================================================================
//函 数 名：SortMenu
//功能描述：子菜单2：4种排序，分类统计
//输入参数：void
//返 回 值：void(可拓展)
//调用函数：gotoxy();HideCursor();
//         SortSumDown(student); --按总分sum从高到低排序
//         SortSumUp(student);   --按总分sum从低到高排序
//         SortIDUp(student);    --按学号ID从小到大排序
//         SortNameUp(student);  --按姓名name字典顺序排序
//         CategoricalStatistics(student); --分类统计各学科各分数段人数与百分比
//==================================================================
void SortMenu(void)
{
    int order_position = 1,
        order_change=1;
    bool is_sure = false;
    char order = '\0'; //键盘命令
    char *sort_menu[] = {"\t\t\t\t1.按总分降序",
                        "\n\n\t\t\t\t2.按总分升序",
                        "\n\n\t\t\t\t3.按学号升序",
                        "\n\n\t\t\t\t4.按姓名升序",
                        "\n\n\t\t\t\t5.优秀率汇总",
                        "\n\n\t\t\t\t6.返回主菜单"};
    HideCursor();
    //输出界面
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               学生信息管理系统                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n");

        for(int i=0; i<6; i++) {
            printf(sort_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //键盘命令获取
        order = getch();
        order_change = order_position;
        //判断命令
        switch(order) {
            case 'w': case 'W':
                order_change--;
                break;
            case 's': case 'S':
                order_change++;
                break;
            case '\r':
                is_sure = true;
                break;
            case '1': order_position = order-'0'; break;
            case '2': order_position = order-'0'; break;
            case '3': order_position = order-'0'; break;
            case '4': order_position = order-'0'; break;
            case '5': order_position = order-'0'; break;
            case '6': order_position = order-'0'; break;
            default: break;
        }
        //边界检查
        if(order_change>=1&&order_change<=6) {
            order_position = order_change;
        }
        //回车检查
        if(is_sure) {
            break;
        }
    }
    //清屏，进入下级
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               学生信息管理系统                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //命令对应的函数调用
    switch(order_position){
        case 1: SortSumDown(student); break;//总分降序
        case 2: SortSumUp(student); break;  //总分升序
        case 3: SortIDUp(student); break;   //学号升序
        case 4: SortNameUp(student); break; //姓名升序
        case 5: CategoricalStatistics(student); break;//分类统计
        case 6: return ; break;             //返回上层
        default: printf("未知错误"); break; //错误检查
    }
    //输出排序结果(仅调用Sort类函数时)
    if(order_position!=5) {
        ShowInformation(student);
    }
}

//==================================================================
//函 数 名：PasswordMenu
//功能描述：子菜单3：密码系统(查看密码，修改密码，重置密码，游戏接口)
//输入参数：void
//返 回 值：void（可拓展）
//调用函数：gotoxy();HideCursor();
//         ShowPassword();  --查看密码
//         PassUpdate();    --更新密码
//         PasswordInitialization(); --重置密码
//         IsEnterGames();  --游戏接口
//         defense();       --防御函数，退出程序
//==================================================================
void PasswordMenu(void)
{
    int order_position = 1,
        order_change=1,
        wrong_time=0;   //错误次数检查，到三次自动退出程序
    bool is_sure = false;
    char order = '\0';
    char password[N1] = {0},
         *pass_menu[] = {"\t\t\t\t1.密码查看",
                        "\n\n\t\t\t\t2.密码更改",
                        "\n\n\t\t\t\t3.重置密码",
                        "\n\n\t\t\t\t4.轻松一下"};
    HideCursor();

    /**管理员密码验证模块**/
    printf("请输入管理员密码：");
    fgets(password,N1,stdin);
    //判断是否正确，输入0退出
    while(wrong_time<3-1) {
        if(strcmp(password,"0\n")==0) {
            printf("退出修改。");
            return;
        } else if(strcmp(password,"580580\n")!=0) {
            wrong_time++;
            printf("重新输入：");
            fgets(password,N1,stdin);
        } else {
            break;
        }
    }
    //错误次数到3次启动防御函数
    if(wrong_time>=3-1) {
        defense();
        return;
    }
    //清屏，输出界面
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               学生信息管理系统                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n\n\n");

        for(int i=0; i<4; i++) {
            printf(pass_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //键盘输入
        order = getch();
        order_change = order_position;
        switch(order) {
            case 'w': case 'W':
                order_change--;
                break;
            case 's': case 'S':
                order_change++;
                break;
            case '\r':
                is_sure = true;
                break;
            case '1': order_position = order-'0'; break;
            case '2': order_position = order-'0'; break;
            case '3': order_position = order-'0'; break;
            case '4': order_position = order-'0'; break;
            default: break;
        }
        //边界检查
        if(order_change>=1&&order_change<=4) {
            order_position = order_change;
        }
        //回车检查
        if(is_sure) {
            break;
        }
    }
    //进入下级
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               学生信息管理系统                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //函数调用
    switch(order_position){
        case 1: ShowPassword(); break;  //查看密码
        case 2: PassUpdate(); break;    //更新密码
        case 3: PasswordInitialization(); break; //密码初始化
        case 4: IsEnterGames(); break;  //游戏接口
        default: printf("未知错误"); break; //错误检查
    }

}

//==================================================================
//函 数 名：ExitSystem
//功能描述：退出程序
//输入参数：void
//返 回 值：void
//调用函数：无
//==================================================================
void ExitSystem()
{
    printf("退出程序");
    exit(0);
}

//==================================================================
//函 数 名：PrintStu
//功能描述：输出单条信息
//输入参数：结构体数组中某一地址（struct stu *p）
//返 回 值：void（可拓展）
//调用函数：无，该函数会被其他函数多次调用
//==================================================================
void PrintStu(struct stu *p)
{
    if(p==NULL) {
        printf("地址错误。");
    } else {
        printf("%s  %s  %d  %d  %d  %d  %.2f  %d\n",p->ID,p->name,p->score[0],p->score[1],p->score[2],p->score[3],p->aver,p->sum);
    }
}

//==================================================================
//函 数 名：SortSumDown
//功能描述：按总分降序排序并输出排序后的成绩，**排序算法**
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：void（可拓展）
//调用函数：无.排序功能函数
//==================================================================
void SortSumDown(struct stu *p)
{
    struct stu temp,
               *q = NULL;
    for(; p<student+num-1; p++) {
        for(q=p+1; q<student+num; q++) {
            if((p->sum)<(q->sum)) {
                temp = *p;
                *p = *q;
                *q = temp;
            }
        }
    }
}

//==================================================================
//函 数 名：SortSumUp
//功能描述：按总分升序排序并输出成绩，**排序算法**
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：void（可拓展）
//调用函数：无; 排序功能函数
//==================================================================
void SortSumUp(struct stu *p)
{
    struct stu temp,
               *q = NULL;
    for(; p<student+num-1; p++) {
        for(q=p+1; q<student+num; q++) {
            if((p->sum)>(q->sum)) {
                temp = *p;
                *p = *q;
                *q = temp;
            }
        }
    }
}

//==================================================================
//函 数 名：SortIDUp
//功能描述：按学号升序排序并输出成绩，**排序算法**
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：void（可拓展）
//调用函数：无; 排序功能函数
//==================================================================
void SortIDUp(struct stu *p)
{
    struct stu temp,
               *q = NULL;
    for(; p<student+num-1; p++) {
        for(q=p+1; q<student+num; q++) {
            if(strcmp(p->ID,q->ID)>0) {
                temp = *p;
                *p = *q;
                *q = temp;
            }
        }
    }
}

//==================================================================
//函 数 名：SortNameUp
//功能描述：按姓名升序排序并输出成绩，**排序算法**
//输入参数：结构体数组地址（struct stu *p）
//返 回 值：void（可拓展）
//调用函数：无; 排序功能函数
//==================================================================
void SortNameUp(struct stu *p)
{
    struct stu temp,
               *q = NULL;
    for(; p<student+num-1; p++) {
        for(q=p+1; q<student+num; q++) {
            if(strcmp(p->name,q->name)>0) {
                temp = *p;
                *p = *q;
                *q = temp;
            }
        }
    }
}

//==================================================================
//函 数 名：SearchID_input
//功能描述：输入ID，在结构体数组p中调用SearchID_part准确查找ID
//输入参数：结构体数组首地址（struct stu *p）
//返 回 值：查找到的信息在结构体数组的*地址，没有返回NULL
//调用函数：SearchID_part(p,str);
//==================================================================
struct stu *SearchID_input(struct stu *p)
{
	char str[N2];   //查找的ID

	printf("\n请输入待查找的ID：");
	scanf("%s",str);

    //调用SearchID_part在p中查找ID为str的信息，返回其地址
	return SearchID_part(p,str);
}

//==================================================================
//函 数 名：SearchID_part
//功能描述：在结构体数组中查找  **递归算法**
//输入参数：结构体数组地址(struct stu *p),待查找ID(str)
//返 回 值：查找到的信息地址*，没有返回NULL
//调用函数：无，该函数被其他函数多次调用，递归算法检查ID
//==================================================================
struct stu *SearchID_part(struct stu *p,char *str)
{
    if (p == student+num) {
        printf("\n未检测到匹配项\n");
        return NULL; //检测到数组末尾
	} else if ( p<student+num && strcmp(str,p->ID)==0 ) {
	    printf("\n查找到该学生\n");
        PrintStu(p);
        return p;
	} else {
        return SearchID_part(p+1,str); //递归部分，再次调用函数本身
	}
}

//==================================================================
//函 数 名：SearchName
//功能描述：在结构体数组中模糊查找姓名  **模糊查找**
//输入参数：结构体数组地址(struct stu *p)
//返 回 值：状态值，没有返回NULL
//调用函数：LookUp(p,str); 模糊查找
//==================================================================
struct stu *SearchName(struct stu *p)
{
    char str[N2];
    struct stu *temp = NULL;

    //输入查找的部分姓名
	printf("\n请输入待查找的部分姓名：");
	scanf("%s",str);

    //遍历数组，对每一个信息姓名都作为主串查找一次，确保没有遗漏
	for(p=student; p<student+num; p++) {
		if( (temp = LookUp(p,str))!=NULL ) {
            Sleep(250);
            PrintStu(p);
		}
	}
}

//==================================================================
//函 数 名：LookUp
//功能描述：在一个结构体的name变量字符串模糊查找str
//输入参数：查找主串的结构体地址(struct stu *p),模式串(char *str)
//返 回 值：状态值，若无，返回NULL
//调用函数：无
//==================================================================
struct stu *LookUp(struct stu *p, char *str)
{
    //初始化
    int len = strlen(str),
        match_times = 0;
    char *name = p->name,
         *i=NULL, *j=NULL;

    //模糊查找
    while(*name!='\0') {
        i = name,
        j = str;
        match_times=0;  //字符匹配次数

        //主串与模式串匹配
        if(*name==*str) {
            while( (*i!='\0')&&(*j!='\0') ) {
                if( (*i)==(*j) ) {
                    match_times++;
                }
                if( (*i++)!=(*j++) ) {
                    break;
                }
            }

            //如果匹配次数等于模式串长度，查找成功，返回地址
            if(match_times==len) {
                return p;
            }
        }

        name++;
    }

    return NULL;
}

//==================================================================
//函 数 名：CategoricalStatistics
//功能描述：分类统计各分数段人数和百分比  -二维数组
//输入参数：结构体数组地址(struct stu *p)
//返 回 值：void（可拓展）
//调用函数：无
//==================================================================
void CategoricalStatistics(struct stu *p)
{
    //定义两个二维数组储存科目-分数段信息
    int gradenum[NUM_SUBJECT][5]={0};
    double gradepercent[NUM_SUBJECT][5]={0};

    //将信息分类处理，存入二维数组
    for(p=student; p<student+num; p++) {
        for(int i=0; i<NUM_SUBJECT; i++) {
            if(p->score[i]>=90&&p->score[i]<=100) {
                gradenum[i][0]++;
            } else if (p->score[i]>=80&&p->score[i]<90) {
                gradenum[i][1]++;
            } else if (p->score[i]>=70&&p->score[i]<80) {
                gradenum[i][2]++;
            } else if (p->score[i]>=60&&p->score[i]<70) {
                gradenum[i][3]++;
            } else {
                gradenum[i][4]++;
            }
        }
    }

    //计算百分比
    for(int i=0; i<NUM_SUBJECT; i++){
        for(int j=0; j<5; j++) {
            gradepercent[i][j] = ((double)(gradenum[i][j]))/num *100;
        }
    }

    //输出结果
    printf("     \t优秀\t\t良好\t\t中等\t\t及格\t\t不及格\n");
    for(int i=0; i<NUM_SUBJECT; i++){
        printf("科目%d   ",i+1);
        for(int j=0; j<5; j++) {
            printf("%2d--%5.2lf%%     ",gradenum[i][j],gradepercent[i][j]);
        }
        printf("\n");
    }
}

//==================================================================
//函 数 名：InformationInitialization
//功能描述：初始化生成一个随机ID姓名成绩的文件，用于建立文件
//输入参数：void
//返 回 值：void
//调用函数：无，用于建立文件
//==================================================================
void InformationInitialization(void)
{
    //定义每次随机生成用到的变量
    int a=0, b=0, c=0, d=0,
        i=0;
    char ID[15],
         name[15];
    char str[30] = {0};

    //在指定文件夹下新建文件or.txt
    FILE *fp = fopen(".\\ause\\or.txt","w");

    //随机生成20个数据条
    for(i=0; i<20; i++) {
        //生成6位学号ID
        ID[0] = random(3)+1+'0';
        ID[1]= '0';
        ID[2] = random(1) + '0';
        ID[3] = random(10) + '0';
        ID[4] = random(10) + '0';
        ID[5] = random(10) + '0';
        ID[6] = '\0';   //补加空字符，构成字符串
        //生成两个小写字母的姓名
        name[0] = random(26) + 'a';
        name[1] = random(26) + 'a';
        name[2] = '\0'; //加空字符形成字符串
        //生成四科成绩，最低50，最高99
        a = random(50)+50;
        b = random(50)+50;
        c = random(50)+50;
        d = random(50)+50;
        //将数据写入字符串str
        sprintf(str,"%s %s %d %d %d %d %.2f %d",ID,name,a,b,c,d,(float)(a+b+c+d)/4,a+b+c+d);
        //添加换行符，fputs遇到\n会写入并停止
        if(i!=19) {
            strcat(str,"\n");
        }
        //写入文件fp
        fputs(str,fp);
    }
    fclose(fp);

    return;
}

//==================================================================
//函 数 名：IsEnterGames
//功能描述：是否进入游戏判断，从成绩查询页面触发
//输入参数：void
//返 回 值：void
//调用函数：RelaxTime(); --游戏主控函数
//         该函数多次被其他函数调用
//==================================================================
void IsEnterGames(void)
{
    char order = '\0';  //键盘命令

    printf("\n成绩有些不尽如人意啊，不如开摆？\n按F键进入开摆");
    //获取键盘命令
    order = getch();
    printf("\n");
    //F进入游戏
    if(order=='F'||order=='f') {
        RelaxTime();
    }
}

/******************************************************************************************************************************************/
/*******************************游戏函数定义**********************************************************************************************/
/******************************************************************************************************************************************/

//==================================================================
//函 数 名：RelaxTime
//功能描述：游戏主控函数
//输入参数：void
//返 回 值：int（可拓展）
//调用函数：startup();--游戏初始化
//          show();              --界面显示
//          updateWithoutUpdate();--游戏数据更新
//          updateWithUpdate();   --键盘输入处理
//==================================================================
int RelaxTime(void)
{
	startup();

    system("pause");
	for(;!over;) {
		show();
		updateWithoutInput();
		updateWithInput();
	}

	gameEnd();
	return 0;
}

//==================================================================
//函 数 名：startup
//功能描述：游戏环境初始化，展示游戏说明
//输入参数：void
//返 回 值：void
//调用函数：HideCursor(); system("cls");--清屏
//==================================================================
void startup()
{
    //变量初始化
	high = 25;              //画面高度
	width = 60;             //画面宽度
	position_x = high/2;    //操作位置x
	position_y = width/2;   //操作位置y
	bullet_x = 0;           //子弹位置x
	bullet_y = position_y;  //子弹位置y
	enemy_x = 0;            //敌人位置x
	enemy_y = position_y;   //敌人位置y
	score = 0,miss = 0;     //分数
	grade = 1;              //等级

    //环境初始化
	srand((unsigned) time(NULL)); //随机种子
	HideCursor();           //隐藏光标
	system("cls");          //清屏函数

	//音频载入
	mciSendString("open .\\music\\back.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // repeat循环播放

	printf("\n\n"
           "\t    飞机游戏说明：\n\n"
           "\t\t控制我方飞机*消灭敌方飞机@\n\n"
           "\t\t每10分升级，最高等级10，100积分通关\n\n\n"
           "\t    操作说明：\n\n"
           "\t\tW,S,A,D控制飞机移动\n\n"
           "\t\t空格space发射子弹\n\n"
           "\t\tESC暂停, 数字0结束游戏\n\n");
    getch();
}

//==================================================================
//函 数 名：show
//功能描述：输出图形界面，游戏可视化处理
//输入参数：void
//返 回 值：void
//调用函数：gotoxy();
//==================================================================
void show()
{
    //游戏暂停(画面停止更新)判断，ESC暂停，再按恢复
	if(stop) {
		while(getch()!=27)  //ESC的ASCII编码27
            ;
		stop = false;
	}

    //移动光标到左上角
	gotoxy(0,0);
	int i=0, j=0;

    //边界检查，防止自己出界
	if(position_x>high-1) {
		position_x = high - 1;
	}
	if(position_x<0) {
		position_x = 0;
	}
	if(position_y<2) {
		position_y = 2;
	}
	if(position_y>width-2) {
		position_y = width-2;
	}

    //画面输出，先战机再敌机再子弹
	for(i=0; i<high; i++) {
		for(j=0; j<width; j++) {
			if( (i==position_x) && (j==position_y) ) {
				printf("*");
			} else if( (i==position_x+1) && (j==position_y-2) ) {
				printf("*****");
				j += 4;
			} else if( (i==position_x+2) && (j==position_y-2) ) {
				printf(" * * ");
				j += 4;
			} else if ( (i==enemy_x) && (j==enemy_y) ) {
				printf("@");
			} else if ( (i==bullet_x) && (j==bullet_y) ) {
				printf("|");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
    //数据记录
	printf("得分: %3d	等级: %2d	miss: %2d",score,grade,miss);
}

//==================================================================
//函 数 名：updateWithoutInput
//功能描述：敌人位置，子弹位置，判断击中，得分和难度更新
//输入参数：void
//返 回 值：void
//调用函数：
//==================================================================
void updateWithoutInput()
{
    //子弹前进
	if(bullet_x>-1) {
		bullet_x--;
	}

    //判断子弹位置是否与敌机位置重合
	if( (bullet_x==enemy_x) && (bullet_y==enemy_y) ) {
		score += 2;     //得分增加
		enemy_x = -1;   //敌机消失
		enemy_y = rand()%(width-4) + 2; //随机位置生成新敌机
		bullet_x = -1;  //重设敌机位置

		mciSendString("close gemusic", NULL, 0, NULL);//关闭上条音频
		mciSendString("open .\\music\\gotEnemy.mp3 alias gemusic", NULL, 0, NULL); //打开音频
        mciSendString("play gemusic", NULL, 0, NULL); //play仅播放一次
	}
    //判断敌机到达底部
	if( enemy_x>high ) {
		miss++;         //miss增加
		enemy_x = -1;   //重设敌机
		enemy_y = rand()%width; //随机敌机位置

		mciSendString("close lomusic", NULL, 0, NULL);//关闭上条音频
		mciSendString("open .\\music\\popmusic.wav alias lomusic", NULL, 0, NULL); //打开音频
        mciSendString("play lomusic", NULL, 0, NULL); //play仅播放一次
	}
    //判断游戏结束，失误到达20或者等级满级11
	if(miss == 20 || grade == 11) {
		over = true;
	}

    //每10分游戏难度+1
	if(score>=grade*10) {
		grade++;
	}

    //难度显示：敌机速度，通多多次循环实现速度控制
	static int speed = 0;   //静态变量，全游戏可见速度
    //控制speed，难度grade增加时，speed到达11-grade次数减少
	if (speed<11-grade) {
		speed++;
	}
    //speed到达11-grade，敌机移动一次
	if (speed == 11-grade) {
		enemy_x++;  //敌机前进
		speed = 0;  //重置speed
	}
}

//==================================================================
//函 数 名：updateWithInput
//功能描述：处理键盘操作，不断刷新(kbhit实现)
//输入参数：void
//返 回 值：void
//调用函数：kbhit(); 判断由键盘输入返回true，否则返回false
//         getch(); 获取键盘输入
//==================================================================
void updateWithInput()
{
	char input = '1';   //键盘输入命令

    //判断输入，改变飞机位置(边界检查在函数show()里完成)
	if(kbhit()) {
		input = getch();
		switch(input) {
			case 'a': case 'A':
				position_y--;
				break;
			case 'd': case 'D':
				position_y++;
				break;
			case 'w': case 'W':
				position_x--;
				break;
			case 's': case 'S':
				position_x++;
				break;
			case ' ':   //空格发射子弹
				bullet_x = position_x - 1;
				bullet_y = position_y;
				mciSendString("close fgmusic", NULL, 0, NULL); // 关闭上条音频
				mciSendString("open .\\music\\Jump.mp3 alias fgmusic", NULL, 0, NULL); // 打开音频
				mciSendString("play fgmusic", NULL, 0, NULL); // play仅播放一次
				break;
			case '0':   //0结束游戏
				over = true;
				break;
			case 27:
				stop = true;
				break;
			default: break;
		}
	}
}

void gameEnd()
{
    printf("\n游戏结束");
    getch();
    mciSendString("close bkmusic", NULL, 0, NULL);
}
//==================================================================
//函 数 名：gotoxy
//功能描述：将控制台光标移动到x，y处，依托于windows.h
//输入参数：坐标x，y(int x, int y)
//返 回 值：void
//调用函数：HANDLE句柄，COORD结构体
//         GetStdHandle(STD_OUTPUT_HANDLE);
//         SetConsoleCursorPosition(handle,pos);
//==================================================================
void gotoxy(int x,int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//定义并获取的句柄
    COORD pos;  //windows.h的坐标结构体，声明结构体变量pos
    pos.X = x;  //x坐标
    pos.Y = y;  //y坐标
    SetConsoleCursorPosition(handle,pos);   //移动命令行中光标的位置
}

//==================================================================
//函 数 名：HideCursor
//功能描述：隐藏光标
//输入参数：void
//返 回 值：void
//调用函数：CONSOLE_CURSOR_INFO结构体
//==================================================================
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};   //控制光标高度1，显示为false
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//标准输出控制台执行操作
}

/******************************************************************************************************************************************/
/*******END*END*END*END*END*END*END*END*END*END*END*END*END*END*END*END********************************************************************/
/******************************************************************************************************************************************/
