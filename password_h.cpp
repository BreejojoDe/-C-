#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <ctype.h>
#include <graphics.h>
#include <windows.h>
#include <Mmsystem.h>
#include "password_h.h"

#pragma comment(lib,"Winmm.lib")//引用Windoes Multimedia API

//==================================================================
//函 数 名：PassCheck
//功能描述：图形化界面，密码启动系统
//         输入密码检查，分为两个权限
//         密码文件重置
//         错误检查
//输入参数：void
//返 回 值：学生端返回1，教师端返回2，失败返回-1(int)
//调用函数：文件操作函数
//         图形化界面EasyX函数
//         defense();  --防御函数
//         PasswordInitialization();  --密码文件初始化
//==================================================================
int PassCheck()
{
    //变量初始化
    char key_teacher[N1] = {0}, //教师密码数组
         key_student[N1] = {0}, //学生密码数组
         password[N1] = {0};    //键入密码数组
    int wrong_time = 0;         //错误次数
    bool again = true;          //判断是否重新输入

    //打开密码文件，fgets读取密码
    FILE *fppass = fopen(".\\ause\\passwords.txt","r");
    if(fppass==NULL) {
        printf("读取失败\n");
        //密码文件初始化，重新生成
        PasswordInitialization();
        return -1;  //返回状态值-1
    }
    fgets(key_teacher,N1,fppass);
    fgets(key_student,N1,fppass);
    fclose(fppass);

    /***EasyX图形化界面***/
    //初始化部分变量
    int text_x=190, text_y=205; //文本坐标
    int len = 0;                //长度
    char star[N1] = {0};        //加密文字字符串，初始化0

    //初始化界面
    initgraph(WIDTH,HIGH);      //绘制界面大小
    setfillcolor(WHITE);        //填充颜色
    setlinecolor(BLACK);        //线条颜色

    //输出初始文字提示
    char s[] = "Please enter password:",
         warning[30] = {0};
    outtextxy(180,175,s);       //在指定位置输出文字

    //密码输入及界面绘制
    while(wrong_time<3){
        setfillcolor(WHITE);    //填充色
        settextcolor(BLACK);    //文字颜色-黑
        setbkmode(TRANSPARENT); //文字背景-透明
        settextstyle(28,12,0);  //文字格式-28,12,默认字体
        memset(password,0,sizeof(password));//初始化键入密码数组
        memset(star,0,sizeof(star));   //初始化加密文字数组

        fillrectangle(180,200,600,230); //指定位置绘制矩形
        //按键入实时读入字符到数组password，回车\r结束
        for(int i=0; i<N1&&password[i-1]!='\r'; i++) {
            password[i] = getch();  //读入字符
            star[i] = '*';          //加密文字增加一个
            len = strlen(password); //键入密码长度

            mciSendString("close tomusic", NULL, 0, NULL); // 关闭上条音频
            mciSendString("open .\\music\\touch1.mp3 alias tomusic", NULL, 0, NULL); // 打开音频
            mciSendString("play tomusic", NULL, 0, NULL); // play仅播放一次

            // 删除字符功能
            //\b是常用的删除键，但C语言不识别，所以写代码将键入\b转换为删除一位操作
            if(password[i]=='\b') {
                if(i>=1) {                  //两位及以上
                    password[i] = password[i-1] = '\0'; //键入密码数字删除末位
                    star[i] = star[i-1] = '\0';         //加密文字数组删除末位
                    i-=2;   //数组操作位置回退
                } else if (i==0) {          //一位
                    password[i] = '\0';     //键入密码数字删除末位
                    star[i]='\0';           //加密文字数组删除末位
                    i--;    //数组操作位置回退
                }
            }

            //判断结束输入，回车\r
            if(password[i]=='\r') {
                star[i]='\0';
            }

            //输出星号加密文字******
            fillrectangle(180,200,600,230); //绘制矩形
            outtextxy(text_x,text_y,star);  //输出加密文字
        }

        // \r换\n
        for(int i=0; i<N1; i++) {
            if(password[i]=='\r') {
                password[i] = '\n';
                break;
            }
        }

        //密码重置判断
        if(strcmp(password,"reset\n")==0) {
            closegraph();   //关闭界面
            PasswordInitialization();   //重置密码
        }

        //密码匹配检测
        settextcolor(WHITE);    //白色文字
        settextstyle(18,0,0);   //文字格式-18
            if(strcmp(password,"0\n")==0) {
                outtextxy(300,280,"Exit."); //输入0退出
                Sleep(2000);
                closegraph();               //关闭界面
                exit(0);
            } else if(strcmp(password,key_teacher)==0) {
                settextstyle(28,0,0);       //教师端
                outtextxy(180,280,"User: teacher");
                Sleep(2000);
                closegraph();
                return 2;
            } else if(strcmp(password,key_student)==0) {
                settextstyle(28,0,0);       //学生端
                outtextxy(180,280,"User: student");
                Sleep(2000);
                closegraph();
                return 1;
            } else {
                wrong_time++;
                settextstyle(28,0,0);       //多次错误
                sprintf(warning,"Wrong. Left %d times.",3-wrong_time);
                outtextxy(180,280,warning); //输出警告
                Sleep(2000);
            }

        setfillcolor(BLACK);
        fillrectangle(180,230,600,300); //输出黑色矩形覆盖文字消息

    }

    closegraph();   //关闭界面
    defense();      //防御程序
}

//==================================================================
//函 数 名：PassUpdate
//功能描述：更新密码，从文件更新
//输入参数：void
//返 回 值：int（可拓展）
//调用函数：文件操作函数
//==================================================================
int PassUpdate()
{
    //初始化变量
    char key_teacher[N1] = {0},
         key_student[N1] = {0},
         password[N1] = {0},
         order = '\0';  //键盘命令
    int wrong_time = 0; //错误次数
    bool again = true;

    //读取密码文件
    FILE *fppass = fopen(".\\ause\\passwords.txt","r"),  //read方式打开
         *fppassnew = NULL;
    if(fppass==NULL) {
        printf("读取失败");
        return -1;
    }
    fgets(key_teacher,N1,fppass);
    fgets(key_student,N1,fppass);
    fclose(fppass);

    /**管理员密码验证**/
    printf("请输入管理员密码：");
    fgets(password,N1,stdin);   //从键盘输入获取

    while(wrong_time<3-1) {
        if(strcmp(password,"0\n")==0) {
            printf("退出修改。");
            return 0;
        } else if(strcmp(password,"580580\n")!=0) {
            wrong_time++;
            printf("重新输入：");
            fgets(password,N1,stdin);
        } else {
            break;
        }
    }
    //错误次数检查
    if(wrong_time>=3-1) {
        defense();
        return -1;
    }

    //密码更改模块
    printf("请选择要更改的密码，教师T/学生S，无需回车\n");
    //键入命令
    order = getch();
    while(again) {
        again = false;
        switch(order)
        {
        case 'T': case 't':
            printf("\n输入教师端新密码：");
            fgets(password,N1,stdin);   //键盘读取
            fppassnew = fopen(".\\ause\\passwords.txt","w");   //write方式打开文件
            fputs(password,fppassnew);      //写入
            fputs(key_student,fppassnew);   //写入
            break;
        case 'S': case 's':
            printf("\n输入学生端新密码：");
            fgets(password,N1,stdin);   //键盘读取
            fppassnew = fopen(".\\ause\\passwords.txt","w");   //write方式打开文件
            fputs(key_teacher,fppassnew);   //写入
            fputs(password,fppassnew);      //写入
            break;
        case '0':
            printf("\n停止操作");   //0退出操作
            return -1;
        default:
            printf("非法指令.\n");  //非法指令检查
            again = true;
            order = getch();
            break;
        }
    }

    printf("修改成功。");
    fclose(fppassnew);              //关闭修改文件
    return 1;
}

//==================================================================
//函 数 名：defense
//功能描述：防御函数，执行后退出程序
//输入参数：void
//返 回 值：void
//调用函数：exit(0); 该函数被其他函数多次调用
//==================================================================
void defense(void)
{
    printf("错误过多，程序退出。");
    Sleep(3000);
    exit(0);
}

//==================================================================
//函 数 名：PasswordInitialization
//功能描述：会将密码初始化成默认密码，若无密码文件则创造密码文件
//输入参数：void
//返 回 值：void
//调用函数：文件操作函数; 该函数多次被其他函数调用
//==================================================================
void PasswordInitialization()
{
    char key_teacher[N1] = "000000\n",
         key_student[N1] = "111111\n";

    FILE *f_renew = fopen(".\\ause\\passwords.txt","w");

    fputs(key_teacher,f_renew);
    fputs(key_student,f_renew);

    fclose(f_renew);

    printf("重置成功，请退出重启。");
    getch();
    exit(0);
}

//==================================================================
//函 数 名：ShowPassword
//功能描述：展示已有密码文件储存的密码
//输入参数：void
//返 回 值：void
//调用函数：文件操作函数
//==================================================================
void ShowPassword(void)
{
    FILE *fp = fopen(".\\ause\\passwords.txt","r");
    char key_teacher[N1] = {0},
         key_student[N1] = {0};

    fgets(key_teacher,N1,fp);
    fgets(key_student,N1,fp);
    fclose(fp);

    printf("教师端：%s",key_teacher);
    printf("学生端：%s",key_student);
}
