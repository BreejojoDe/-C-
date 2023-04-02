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

#pragma comment(lib,"Winmm.lib")//����Windoes Multimedia API

//==================================================================
//�� �� ����PassCheck
//����������ͼ�λ����棬��������ϵͳ
//         ���������飬��Ϊ����Ȩ��
//         �����ļ�����
//         ������
//���������void
//�� �� ֵ��ѧ���˷���1����ʦ�˷���2��ʧ�ܷ���-1(int)
//���ú������ļ���������
//         ͼ�λ�����EasyX����
//         defense();  --��������
//         PasswordInitialization();  --�����ļ���ʼ��
//==================================================================
int PassCheck()
{
    //������ʼ��
    char key_teacher[N1] = {0}, //��ʦ��������
         key_student[N1] = {0}, //ѧ����������
         password[N1] = {0};    //������������
    int wrong_time = 0;         //�������
    bool again = true;          //�ж��Ƿ���������

    //�������ļ���fgets��ȡ����
    FILE *fppass = fopen(".\\ause\\passwords.txt","r");
    if(fppass==NULL) {
        printf("��ȡʧ��\n");
        //�����ļ���ʼ������������
        PasswordInitialization();
        return -1;  //����״ֵ̬-1
    }
    fgets(key_teacher,N1,fppass);
    fgets(key_student,N1,fppass);
    fclose(fppass);

    /***EasyXͼ�λ�����***/
    //��ʼ�����ֱ���
    int text_x=190, text_y=205; //�ı�����
    int len = 0;                //����
    char star[N1] = {0};        //���������ַ�������ʼ��0

    //��ʼ������
    initgraph(WIDTH,HIGH);      //���ƽ����С
    setfillcolor(WHITE);        //�����ɫ
    setlinecolor(BLACK);        //������ɫ

    //�����ʼ������ʾ
    char s[] = "Please enter password:",
         warning[30] = {0};
    outtextxy(180,175,s);       //��ָ��λ���������

    //�������뼰�������
    while(wrong_time<3){
        setfillcolor(WHITE);    //���ɫ
        settextcolor(BLACK);    //������ɫ-��
        setbkmode(TRANSPARENT); //���ֱ���-͸��
        settextstyle(28,12,0);  //���ָ�ʽ-28,12,Ĭ������
        memset(password,0,sizeof(password));//��ʼ��������������
        memset(star,0,sizeof(star));   //��ʼ��������������

        fillrectangle(180,200,600,230); //ָ��λ�û��ƾ���
        //������ʵʱ�����ַ�������password���س�\r����
        for(int i=0; i<N1&&password[i-1]!='\r'; i++) {
            password[i] = getch();  //�����ַ�
            star[i] = '*';          //������������һ��
            len = strlen(password); //�������볤��

            mciSendString("close tomusic", NULL, 0, NULL); // �ر�������Ƶ
            mciSendString("open .\\music\\touch1.mp3 alias tomusic", NULL, 0, NULL); // ����Ƶ
            mciSendString("play tomusic", NULL, 0, NULL); // play������һ��

            // ɾ���ַ�����
            //\b�ǳ��õ�ɾ��������C���Բ�ʶ������д���뽫����\bת��Ϊɾ��һλ����
            if(password[i]=='\b') {
                if(i>=1) {                  //��λ������
                    password[i] = password[i-1] = '\0'; //������������ɾ��ĩλ
                    star[i] = star[i-1] = '\0';         //������������ɾ��ĩλ
                    i-=2;   //�������λ�û���
                } else if (i==0) {          //һλ
                    password[i] = '\0';     //������������ɾ��ĩλ
                    star[i]='\0';           //������������ɾ��ĩλ
                    i--;    //�������λ�û���
                }
            }

            //�жϽ������룬�س�\r
            if(password[i]=='\r') {
                star[i]='\0';
            }

            //����Ǻż�������******
            fillrectangle(180,200,600,230); //���ƾ���
            outtextxy(text_x,text_y,star);  //�����������
        }

        // \r��\n
        for(int i=0; i<N1; i++) {
            if(password[i]=='\r') {
                password[i] = '\n';
                break;
            }
        }

        //���������ж�
        if(strcmp(password,"reset\n")==0) {
            closegraph();   //�رս���
            PasswordInitialization();   //��������
        }

        //����ƥ����
        settextcolor(WHITE);    //��ɫ����
        settextstyle(18,0,0);   //���ָ�ʽ-18
            if(strcmp(password,"0\n")==0) {
                outtextxy(300,280,"Exit."); //����0�˳�
                Sleep(2000);
                closegraph();               //�رս���
                exit(0);
            } else if(strcmp(password,key_teacher)==0) {
                settextstyle(28,0,0);       //��ʦ��
                outtextxy(180,280,"User: teacher");
                Sleep(2000);
                closegraph();
                return 2;
            } else if(strcmp(password,key_student)==0) {
                settextstyle(28,0,0);       //ѧ����
                outtextxy(180,280,"User: student");
                Sleep(2000);
                closegraph();
                return 1;
            } else {
                wrong_time++;
                settextstyle(28,0,0);       //��δ���
                sprintf(warning,"Wrong. Left %d times.",3-wrong_time);
                outtextxy(180,280,warning); //�������
                Sleep(2000);
            }

        setfillcolor(BLACK);
        fillrectangle(180,230,600,300); //�����ɫ���θ���������Ϣ

    }

    closegraph();   //�رս���
    defense();      //��������
}

//==================================================================
//�� �� ����PassUpdate
//�����������������룬���ļ�����
//���������void
//�� �� ֵ��int������չ��
//���ú������ļ���������
//==================================================================
int PassUpdate()
{
    //��ʼ������
    char key_teacher[N1] = {0},
         key_student[N1] = {0},
         password[N1] = {0},
         order = '\0';  //��������
    int wrong_time = 0; //�������
    bool again = true;

    //��ȡ�����ļ�
    FILE *fppass = fopen(".\\ause\\passwords.txt","r"),  //read��ʽ��
         *fppassnew = NULL;
    if(fppass==NULL) {
        printf("��ȡʧ��");
        return -1;
    }
    fgets(key_teacher,N1,fppass);
    fgets(key_student,N1,fppass);
    fclose(fppass);

    /**����Ա������֤**/
    printf("���������Ա���룺");
    fgets(password,N1,stdin);   //�Ӽ��������ȡ

    while(wrong_time<3-1) {
        if(strcmp(password,"0\n")==0) {
            printf("�˳��޸ġ�");
            return 0;
        } else if(strcmp(password,"580580\n")!=0) {
            wrong_time++;
            printf("�������룺");
            fgets(password,N1,stdin);
        } else {
            break;
        }
    }
    //����������
    if(wrong_time>=3-1) {
        defense();
        return -1;
    }

    //�������ģ��
    printf("��ѡ��Ҫ���ĵ����룬��ʦT/ѧ��S������س�\n");
    //��������
    order = getch();
    while(again) {
        again = false;
        switch(order)
        {
        case 'T': case 't':
            printf("\n�����ʦ�������룺");
            fgets(password,N1,stdin);   //���̶�ȡ
            fppassnew = fopen(".\\ause\\passwords.txt","w");   //write��ʽ���ļ�
            fputs(password,fppassnew);      //д��
            fputs(key_student,fppassnew);   //д��
            break;
        case 'S': case 's':
            printf("\n����ѧ���������룺");
            fgets(password,N1,stdin);   //���̶�ȡ
            fppassnew = fopen(".\\ause\\passwords.txt","w");   //write��ʽ���ļ�
            fputs(key_teacher,fppassnew);   //д��
            fputs(password,fppassnew);      //д��
            break;
        case '0':
            printf("\nֹͣ����");   //0�˳�����
            return -1;
        default:
            printf("�Ƿ�ָ��.\n");  //�Ƿ�ָ����
            again = true;
            order = getch();
            break;
        }
    }

    printf("�޸ĳɹ���");
    fclose(fppassnew);              //�ر��޸��ļ�
    return 1;
}

//==================================================================
//�� �� ����defense
//��������������������ִ�к��˳�����
//���������void
//�� �� ֵ��void
//���ú�����exit(0); �ú���������������ε���
//==================================================================
void defense(void)
{
    printf("������࣬�����˳���");
    Sleep(3000);
    exit(0);
}

//==================================================================
//�� �� ����PasswordInitialization
//�����������Ὣ�����ʼ����Ĭ�����룬���������ļ����������ļ�
//���������void
//�� �� ֵ��void
//���ú������ļ���������; �ú�����α�������������
//==================================================================
void PasswordInitialization()
{
    char key_teacher[N1] = "000000\n",
         key_student[N1] = "111111\n";

    FILE *f_renew = fopen(".\\ause\\passwords.txt","w");

    fputs(key_teacher,f_renew);
    fputs(key_student,f_renew);

    fclose(f_renew);

    printf("���óɹ������˳�������");
    getch();
    exit(0);
}

//==================================================================
//�� �� ����ShowPassword
//����������չʾ���������ļ����������
//���������void
//�� �� ֵ��void
//���ú������ļ���������
//==================================================================
void ShowPassword(void)
{
    FILE *fp = fopen(".\\ause\\passwords.txt","r");
    char key_teacher[N1] = {0},
         key_student[N1] = {0};

    fgets(key_teacher,N1,fp);
    fgets(key_student,N1,fp);
    fclose(fp);

    printf("��ʦ�ˣ�%s",key_teacher);
    printf("ѧ���ˣ�%s",key_student);
}
