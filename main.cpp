#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>            //windows������linuxϵͳ������
#include <conio.h>              //���̲���
#include <ctype.h>              //�ַ�ӳ��
#include <time.h>
#include <graphics.h>           //ʹ��ͼ�ο�
#include <mmsystem.h>           //Windowsý��
#include "password_h.h"         /****����ϵͳ(������д��ͷ�ļ�)****/

#pragma comment(lib,"Winmm.lib")//����Windows Multimedia API

#define NUM_SUBJECT 4           //��Ŀ
#define N_MAX 80                //�����Ϣ�������ɸ��ģ�
#define N2 20                   //����name��ID����ַ�
#define random(n) (rand()%n)    //�������ȡ��ȡ�����

/**���浥λ��ѧ����Ϣ�ṹ�����飨�������л��棩**/
struct stu
{
    char ID[15];
    char name[20];
    int score[NUM_SUBJECT];
    double aver;
    int sum;
} student[N_MAX];

/**��¼ƽ��ֵͳ����Ϣ**/
struct stu_all
{
    double score[NUM_SUBJECT],
           aver,
           sum;
} subject_aver;

int num = 0;           /**����ѧ����Ϣ������Readout��ʼ����**/
struct stu stunull = {'\0','\0',0,0,0,0,0,0};   //��Ϣ�ṹ�建���ָ��
int userRight = 0;     /**ѧ����Ȩ��1����ʦ��Ȩ��2���������������ȷ��**/

/**********************��Ϸȫ�ֱ���********************************/
//��Ϸģ���Ϊ���⣬��Ҫ�������ͬʱ�����޸ı�������ʱȫ�ֱ���������    //
int position_x = 0, position_y = 0;              //�ɻ�λ��        //
int high, width;                                 //����ߴ�        //
bool isFire = false, over = false, stop = false; //״̬���        //
int bullet_x, bullet_y;                          //�ӵ�λ��        //
int enemy_x, enemy_y;                            //����λ��        //
int score = 0, miss = 0;                         //�÷�ʧ��        //
int grade = 1;                                   //�Ѷȵȼ�        //
                                                                   //
/**************************END**************************************/

/************************��Ϸ����9*********************************/
                                                                   //
void IsEnterGames(void);                         //ȷ�Ͻ�����Ϸ    //
int RelaxTime(void);                             //��Ϸ����        //
void startup();                                  //��Ϸ��ʼ��      //
void show();                                     //��Ļ���        //
void updateWithoutInput();                       //��Ϸ���ݸ���    //
void updateWithInput();                          //���̽���        //
void gameEnd();                                  //��Ϸ����        //
void gotoxy(int x, int y);                       //����ƶ�        //
void HideCursor();                               //�������        //
                                                                   //
/***************************END*************************************/


/**********************ѧ����Ϣ����ϵͳ24**************************/

int StartUp(void);                                     //��ʼ��
int ReadOut(struct stu *p);                            //���ļ��ж�ȡ����
int WriteIn(struct stu *p);                            //����д���ļ�
int Input(struct stu *p);                              //����ѧ����Ϣ����ͬʱ���ӵ����򻺴沢�����ļ�
int ChangeStu(struct stu *p);                          //����ѧ����Ϣ��д���ļ�
int DeleteStu(struct stu *p);                          //ɾ��ѧ����Ϣ��д���ļ�
int ShowInformation(struct stu *p);                    //չʾ���г��򻺴�
int RightCheck(void);                                  //Ȩ�޼��
void InformationInitialization(void);                  //�������ѧ����Ϣ�ļ�
void ExitSystem(void);                                 //�˳�ϵͳ
void MainMenu(void);                                   //���˵�
void StuInformationMenu(void);                         //ѧ����Ϣ�����˵�
void SortMenu(void);                                   //����˵�
void PasswordMenu(void);                               //����˵�
void PrintStu(struct stu *p);                          //�������ѧ����Ϣ
void SortSumDown(struct stu *p);                       //���ֽܷ�������
void SortSumUp(struct stu *p);                         //���ܷ���������
void SortIDUp(struct stu *p);                          //��ѧ����������
void SortNameUp(struct stu *p);                        //��������������
void CategoricalStatistics(struct stu *p);             //����ͳ�Ƹ���Ŀ�����������������
struct stu *SearchID_input(struct stu *p);             //����ID׼ȷ����
struct stu *SearchID_part(struct stu *p,char *str);    //��Ƕ����ID���ң�����Ƿ��ظ����룩
struct stu *SearchName(struct stu *p);                 //��������ģ������
struct stu *LookUp(struct stu *p, char *str);          //ģ������

/***************************END*************************************/

int main()
{
    struct stu *p = student;        //ָ��ָ��ṹ������

    StartUp();                      //�����ʼ����������������ϵͳ
    ReadOut(p);                     //��ȡ�ļ����������ݳ�ʼ��
    for(;;) {
        MainMenu();                 //�������˵�
    }

    return 0;
}

//==================================================================
//�� �� ����StartUp
//������������Ϣ����ϵͳ��ʼ��
//���������void
//�� �� ֵ��int��main������δʹ�ã�
//���ú�����PassCheck();
//==================================================================
int StartUp(void)
{
    srand((unsigned) time(NULL));   //ʱ������
    system("color F0");             //�ı����̨��ɫ

    userRight = PassCheck();        //�����飬���浽ȫ�ֱ���userRight

    if(userRight ==1) {
        printf("ѧ����\n");
        return 1;
    } else if (userRight == 2) {
        printf("��ʦ��\n");
        return 2;
    }

    return 0;
}

//==================================================================
//�� �� ����RightCheck
//����������Ȩ�޼�飨��������ϵͳ���������ȷ��Ȩ�ޣ�
//���������void
//�� �� ֵ��int����������bool����Ϊ�˶������ֵ�Ȩ�޵���չ�ԣ�ʹ��int����Ȩ��״̬��
//���ú�������; �ú������ظ���������������
//==================================================================
int RightCheck(void)
{
    if(userRight==1) {
        printf("\n\n\t\t\t___________����Ȩ��__________\n\n");
        return false;       //ѧ��Ȩ��*���ʧ��
    } else {
        return true;
    }
}

//==================================================================
//�� �� ����ReadOut
//�������������ļ��ж�ȡ�ṹ����Ϣ���ļ�������
//         ����Ƿ�����ļ������ޣ��������һ���ļ�
//���������struct stu *p���ṹ��ָ�룩
//�� �� ֵ����ȡ������Ϣ������int��
//���ú�����InformationInitialization()�ļ���ʼ��;
//         �ú�����α�����
//==================================================================
int ReadOut(struct stu *p)
{
    num = 0;
    FILE *fp = fopen(".\\ause\\or.txt","r");

    //��δ��⵽���ʼ���ļ�
    if(fp==NULL) {
        printf("�״ν���,���ڳ�ʼ������\n\n");
        InformationInitialization();
        printf("�ļ�or.txt��ʼ���ɹ�, ����������\n");
        getch();
        exit(0);
    }
    //��ȡ��Ϣ�����ڶ�ȡͬʱ����ƽ���÷����ܷ�
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

    fclose(fp);//�ر��ļ�

    return num;
}

//==================================================================
//�� �� ����WriteIn
//�����������ѽṹ�建�����Ϣд���ļ�������ԭ�ļ���
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ���ļ�����״̬��0�ɹ���-1ʧ�ܣ��ɸ�Ϊwritein_timesд�������
//���ú�����C��׼����sprintf������������д��һ���ַ���
//         �ú�����α�������������
//==================================================================
int WriteIn(struct stu *p)
{
    int writein_times=0;
    char information[100] = {0};
    FILE *fp = fopen(".\\ause\\or.txt","w");
    if(fp==NULL) {
        printf("��ȡʧ��");
        return -1;
    }

    //fputs���ַ�д���ļ�
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
//�� �� ����Input
//�������������뵥����Ϣ�������ظ�������
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ����ε����������Ϣ����input_times��
//���ú�����C��׼sprintf����
//==================================================================
int Input(struct stu *p)
{
    struct stu temp,
               *q = &temp;      //temp��q����������Ϣ
    char order = '\0',          //�������������
         information[100],      //¼����Ϣ
         ID_check[15] = {0};    //ѧ�ż�黺��
    int input_times=0;

    temp.sum=0;

    //���ļ���pָ��ṹ������ĩβ
    FILE *fp = fopen(".\\ause\\or.txt","a");
    if(fp==NULL) {
        printf("��ȡʧ��");
        return -1;
    }
    p = p+num;

    //������Ϣ
    for(;;) {
        printf("������ѧ����Ϣ����λID ���� �ĿƳɼ�,�ÿո���������磺100001 aa 99 99 99 99��\n");
        scanf(" %s",ID_check);
        //�ظ�¼��Ĵ�����
        if( (SearchID_part(student,ID_check))!=NULL ){
            printf("��ѧ���Ѵ���\n");
            while(getchar()!='\n')
                ;
            goto ChooseIsContinue;
        } else {
            printf("ID: %s����¼��\n\n",ID_check);
        }
        //����������黺��
        strcpy(q->ID,ID_check);
        scanf(" %s",q->name);
        for(int i=0; i<NUM_SUBJECT; i++) {
            scanf(" %d",&q->score[i]);
            q->sum += q->score[i];
        }
        q->aver = (double)(q->sum)/NUM_SUBJECT;

        *p++ = *q;
        num++;

        //д���ļ�
        sprintf(information,"%s %s %d %d %d %d %.2f %d",q->ID,q->name,q->score[0],q->score[1],q->score[2],q->score[3],q->aver,q->sum);
        fputs("\n",fp);
        fputs(information,fp);
        input_times++;

    //ѭ���жϣ�ѡ���Ƿ����¼��
    ChooseIsContinue:
        printf("�Ƿ����¼�룿��Y/N��");
        order = getch();
        if(!(order=='y'||order=='Y')) {
            break;
        }
        memset(ID_check,0,sizeof(ID_check));//���ID_check
    }

    fclose(fp);
    return input_times;
}

//==================================================================
//�� �� ����ChangeStu
//�����������޸����е�ѧ����Ϣ���ϸ��ӵĴ���ѧ�ż��
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ���޸�״̬��int��
//���ú�����SearchID_input()���������ID�Ƿ���ڼ�λ��
//==================================================================
int ChangeStu(struct stu *p)
{
    struct stu *temp = NULL,
               *pos_changed = NULL; //��λҪ�޸���Ϣ�������е�λ��
    char ID_check[15] = {0};
    bool is_wrongID = true,//��һ�ν���ѭ��
         changethesameone = false;

    while(is_wrongID) {
        is_wrongID = false;         //��ʼ�����������㣩
        changethesameone = false;

        //���Ҫ�޸ĵ�ѧ���Ƿ���ڣ���������ѭ������
        printf("�޸�Ŀ��ѧ�ţ�\n");
        if( (pos_changed = SearchID_input(p))==NULL ) {
            printf("��ѧ�Ų�����\n");
            is_wrongID = true;
            continue;
        }

        //�����޸ĺ����Ϣ
        printf("\n�������޸ĺ����Ϣ:\n");
        //�ڶ���ID��飬ֻ�����޸�ͬѧ�Ż��߲����ڵ�����ѧ��
        //����ѧ��ID��ͻ
        scanf(" %s",ID_check);
        if( strcmp(ID_check,pos_changed->ID)==0 ) {
            changethesameone = true;//�ж��Ƿ��޸�ͬһѧ��
        } else if ( strcmp(ID_check,"0")==0 ) {
            return 0;
        }
        //�ü����߼��ж�
        if( !( ((SearchID_part(student,ID_check))==NULL) || changethesameone ) ){
            printf("��ѧ���Ѵ���\n");
            while(getchar()!='\n')
                ;
            is_wrongID = true;
            continue;
        }
        //д������
        strcpy(pos_changed->ID,ID_check);
        scanf(" %s",pos_changed->name);
        pos_changed->sum = pos_changed->aver = 0;
        for(int i=0; i<NUM_SUBJECT; i++) {
            scanf(" %d",&pos_changed->score[i]);
            pos_changed->sum += pos_changed->score[i];
        }
        pos_changed->aver = (double)(pos_changed->sum)/NUM_SUBJECT;
    }

    //���޸ĺ������д���ļ�
    printf("\n�޸ĺ����");
    PrintStu(pos_changed);
    WriteIn(student);

    return 0;
}

//==================================================================
//�� �� ����DeleteStu
//����������ɾ��һ���Ѵ��ڵ���Ϣ
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ��ɾ��״̬��0�ɹ�
//���ú�����SearchID_input()���������ID�Ƿ���ڼ�λ��
//         WriteIn();д���ļ�
//         C��׼����getch()����ü�����������س�
//==================================================================
int DeleteStu(struct stu *p)
{
    struct stu *pos_deleted=NULL,
               *q = NULL;
    bool is_wrongID = true,
         is_keybroad = false;
    char order='\0';

    //�������ID�Ƿ����
    while(is_wrongID) {
        is_wrongID = false;
        printf("\nɾ������ID��");
        pos_deleted = SearchID_input(student);
        if( pos_deleted ==NULL ) {
            printf("��ѧ�Ų�����\n");
            is_wrongID = true;
            continue;
        }
    }

    //ȷ���Ƿ�ɾ����ִ��ɾ������
    printf("\n�Ƿ�ɾ����(Yȷ�ϣ�Nȡ�������ð��س�)");
    while(!is_keybroad) {
        order = getch();
        switch(order) {
            case 'y': case 'Y':
                stunull = *pos_deleted;//��ɾ����Ϣ���뻺��stunull
                //��ɾ��λ�ú���Ϣ��ǰһλ
                for(q=pos_deleted; q<student+num-1; q++) {
                    *q = *(q+1);
                }
                //��ɾ����Ϣ�浽���һλ
                //��Ϣ��-1��ʹ���һλ�����к������ɼ�
                //�÷������Ա������ɾ������Ϣ�����ڻָ���Ϣ
                *q = stunull;
                num--;
                is_keybroad = true;
                break;
            case 'n': case 'N':
                printf("ȡ��������");
                return 0;
                break;
            default: break;
        }
    }

    //д���ļ�
    WriteIn(student);

    return 0;
}

//==================================================================
//�� �� ����ShowInformation
//����������������򻺴������������Ϣ
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ������չΪ���������int��չ��
//���ú�������; �ú�����α�������������
//==================================================================
int ShowInformation(struct stu *p)
{
    printf("\t ѧ��     ����     ��Ŀ      ��Ŀ      ��Ŀ      ��Ŀ      ƽ����       �ܷ�\n");
    for(p=student; p<student+num; p++) {
        printf("\t%s     %s      %3d       %3d       %3d       %3d       %6.2f        %3d\n",p->ID,p->name,p->score[0],p->score[1],p->score[2],p->score[3],p->aver,p->sum);
    }
    printf("\n\t����%d��  ƽ����:   %.2lf     %.2lf     %.2lf     %.2lf     %.2lf\t %.2lf",num,subject_aver.score[0],subject_aver.score[1],subject_aver.score[2],subject_aver.score[3],subject_aver.aver,subject_aver.sum);
}

//==================================================================
//�� �� ����MainMenu
//�����������������˵�����ӡ���ӻ����棬��ݵĲ˵���������ת������������
//���������void
//�� �� ֵ��void
//���ú�����gotoxy();HideCursor();
//         SearchID_,SearchName,--��������
//         StuInformationMenu();--��Ϣ�����Ӳ˵�
//         Readout();WriteIn(); --�ļ�����д��
//         SortMenu();          --����ͳ�Ʋ˵�
//         PasswordMenu();      --����ϵͳ�˵�
//         StuInformationMenu(); ExitSystem();
//==================================================================
void MainMenu(void)
{
    int order_position = 1,     //����ʶ������λ��
        order_change=1;         //��������棨Ҫ�ı��λ�ã���Ҫ��һ���жϣ�
    bool is_sure = false;       //�س�ȷ���ж�
    char order = '\0';          //���������⵽������
    /*�ַ���ָ������*/
    char *main_menu[] = {"\t\t\t\t1.��ѯѧ��",
                        "\n\n\t\t\t\t2.��ѯ����",
                        "\n\n\t\t\t\t3.¼��ɼ�",
                        "\n\n\t\t\t\t4.�����ļ�",
                        "\n\n\t\t\t\t5.�����ļ�",
                        "\n\n\t\t\t\t6.�鿴�ɼ���",
                        "\n\n\t\t\t\t7.����",
                        "\n\n\t\t\t\t8.����ϵͳ",
                        "\n\n\t\t\t\t9.�˳�"};

    HideCursor();

    //�������
    for(;;) {
        system("cls"); //��������
        gotoxy(0,0); //����ƶ�����ʼ
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               ѧ����Ϣ����ϵͳ                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n");
        //��� "*" ��λ��
        for(int i=0; i<9; i++) {
            printf(main_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //getch()��ȡ��������
        order = getch();
        mciSendString("close tomusic", NULL, 0, NULL); // �ر�������Ƶ
        mciSendString("open .\\music\\touch2.mp3 alias tomusic", NULL, 0, NULL); // ����Ƶ
        mciSendString("play tomusic", NULL, 0, NULL); // play������һ��
        //������ת��Ϊorder_change���ַ�-'0'��
        order_change = order_position;
        switch(order) {
            case 'w': case 'W':
                order_change--;
                break;
            case 's': case 'S':
                order_change++;
                break;
            case '\r':         //windowsϵͳ�س�\n\r
                is_sure = true;//�ж�����س�
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
        //Խ���飬ֻ��1��9�����order_change��
        if(order_change>=1&&order_change<=9) {
            order_position = order_change;//order_positonΪ���λ��
        }
        //�س����
        if(is_sure) {
            break;
        }
    }
    //��������ӡ��һ������
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               ѧ����Ϣ����ϵͳ                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //����order_positionʵ��
    switch(order_position){
        case 1: SearchID_input(student); IsEnterGames(); break;//��ID
        case 2: SearchName(student); IsEnterGames(); break; //��name
        case 3: StuInformationMenu(); break;       //�Ӳ˵�1���޸���Ϣ
        case 4: ReadOut(student); break;           //�ļ���ȡ
        case 5: WriteIn(student); break;           //�ļ�д��
        case 6: ShowInformation(student); break;   //չʾ�ɼ�
        case 7: SortMenu(); break;                 //�Ӳ˵�2������ͳ��
        case 8: PasswordMenu(); break;             //�Ӳ˵�3������ϵͳ
        case 9: ExitSystem(); break;               //�˳�����
        default: printf("δ֪����"); break;         //�����飬�������¼�
    }

    printf("\n");
    system("pause");
}

//==================================================================
//�� �� ����StuInformationMenu
//�����������Ӳ˵�1�������Ϣ��ɾ����Ϣ���޸���Ϣ
//���������void
//�� �� ֵ��void������չ��
//���ú�����gotoxy();HideCursor();
//         RightCheck();       --Ȩ�޼��
//         Input(student);     --������Ϣ
//         ChangeStu(student); --�޸���Ϣ
//         DeleteStu(student); --ɾ����Ϣ
//==================================================================
void StuInformationMenu(void)
{
    int order_position = 1,
        order_change=1;
    bool is_sure = false;
    char order = '\0';
    char *stuinformation_menu[] = {"\t\t\t\t1.¼����Ϣ",
                                   "\n\n\t\t\t\t2.�޸���Ϣ",
                                   "\n\n\t\t\t\t3.ɾ����Ϣ",
                                   "\n\n\t\t\t\t4.�����ϼ�"};

    HideCursor();

    //��ʾ����
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               ѧ����Ϣ����ϵͳ                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n\n\n");

        for(int i=0; i<4; i++) {
            printf(stuinformation_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //��ü������벢�ж�
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
        //�߽���
        if(order_change>=1&&order_change<=4) {
            order_position = order_change;
        }
        //�س�ȷ�ϼ��
        if(is_sure) {
            break;
        }
    }
    //����������¼�
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               ѧ����Ϣ����ϵͳ                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //����RightCheck���Ȩ��userRight,����ʦ�ɲ���
    if(order_position!=4 && RightCheck()==false) {
        return;
    }
    //��ת�˵��򷵻��ϲ�
    switch(order_position){
        case 1: Input(student); break;
        case 2: ChangeStu(student); break;
        case 3: DeleteStu(student); break;
        case 4: return; break;
        default: printf("δ֪����"); break;
    }

    printf("\n");
}

//==================================================================
//�� �� ����SortMenu
//�����������Ӳ˵�2��4�����򣬷���ͳ��
//���������void
//�� �� ֵ��void(����չ)
//���ú�����gotoxy();HideCursor();
//         SortSumDown(student); --���ܷ�sum�Ӹߵ�������
//         SortSumUp(student);   --���ܷ�sum�ӵ͵�������
//         SortIDUp(student);    --��ѧ��ID��С��������
//         SortNameUp(student);  --������name�ֵ�˳������
//         CategoricalStatistics(student); --����ͳ�Ƹ�ѧ�Ƹ�������������ٷֱ�
//==================================================================
void SortMenu(void)
{
    int order_position = 1,
        order_change=1;
    bool is_sure = false;
    char order = '\0'; //��������
    char *sort_menu[] = {"\t\t\t\t1.���ֽܷ���",
                        "\n\n\t\t\t\t2.���ܷ�����",
                        "\n\n\t\t\t\t3.��ѧ������",
                        "\n\n\t\t\t\t4.����������",
                        "\n\n\t\t\t\t5.�����ʻ���",
                        "\n\n\t\t\t\t6.�������˵�"};
    HideCursor();
    //�������
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               ѧ����Ϣ����ϵͳ                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n");

        for(int i=0; i<6; i++) {
            printf(sort_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //���������ȡ
        order = getch();
        order_change = order_position;
        //�ж�����
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
        //�߽���
        if(order_change>=1&&order_change<=6) {
            order_position = order_change;
        }
        //�س����
        if(is_sure) {
            break;
        }
    }
    //�����������¼�
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               ѧ����Ϣ����ϵͳ                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //�����Ӧ�ĺ�������
    switch(order_position){
        case 1: SortSumDown(student); break;//�ֽܷ���
        case 2: SortSumUp(student); break;  //�ܷ�����
        case 3: SortIDUp(student); break;   //ѧ������
        case 4: SortNameUp(student); break; //��������
        case 5: CategoricalStatistics(student); break;//����ͳ��
        case 6: return ; break;             //�����ϲ�
        default: printf("δ֪����"); break; //������
    }
    //���������(������Sort�ຯ��ʱ)
    if(order_position!=5) {
        ShowInformation(student);
    }
}

//==================================================================
//�� �� ����PasswordMenu
//�����������Ӳ˵�3������ϵͳ(�鿴���룬�޸����룬�������룬��Ϸ�ӿ�)
//���������void
//�� �� ֵ��void������չ��
//���ú�����gotoxy();HideCursor();
//         ShowPassword();  --�鿴����
//         PassUpdate();    --��������
//         PasswordInitialization(); --��������
//         IsEnterGames();  --��Ϸ�ӿ�
//         defense();       --�����������˳�����
//==================================================================
void PasswordMenu(void)
{
    int order_position = 1,
        order_change=1,
        wrong_time=0;   //���������飬�������Զ��˳�����
    bool is_sure = false;
    char order = '\0';
    char password[N1] = {0},
         *pass_menu[] = {"\t\t\t\t1.����鿴",
                        "\n\n\t\t\t\t2.�������",
                        "\n\n\t\t\t\t3.��������",
                        "\n\n\t\t\t\t4.����һ��"};
    HideCursor();

    /**����Ա������֤ģ��**/
    printf("���������Ա���룺");
    fgets(password,N1,stdin);
    //�ж��Ƿ���ȷ������0�˳�
    while(wrong_time<3-1) {
        if(strcmp(password,"0\n")==0) {
            printf("�˳��޸ġ�");
            return;
        } else if(strcmp(password,"580580\n")!=0) {
            wrong_time++;
            printf("�������룺");
            fgets(password,N1,stdin);
        } else {
            break;
        }
    }
    //���������3��������������
    if(wrong_time>=3-1) {
        defense();
        return;
    }
    //�������������
    for(;;) {
        system("cls");
        gotoxy(0,0);
        printf("\n"
               "\t\t**************************************************\n"
               "\t\t                                                  \n"
               "\t\t               ѧ����Ϣ����ϵͳ                   \n"
               "\t\t                                                  \n"
               "\t\t**************************************************\n"
               "\n\n\n");

        for(int i=0; i<4; i++) {
            printf(pass_menu[i]);
            if(order_position==i+1) {
                printf("  *");
            }
        }
        //��������
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
        //�߽���
        if(order_change>=1&&order_change<=4) {
            order_position = order_change;
        }
        //�س����
        if(is_sure) {
            break;
        }
    }
    //�����¼�
    system("cls");
    gotoxy(0,0);
    printf("\n"
           "\t\t**************************************************\n"
           "\t\t                                                  \n"
           "\t\t               ѧ����Ϣ����ϵͳ                   \n"
           "\t\t                                                  \n"
           "\t\t**************************************************\n"
           "\n");
    //��������
    switch(order_position){
        case 1: ShowPassword(); break;  //�鿴����
        case 2: PassUpdate(); break;    //��������
        case 3: PasswordInitialization(); break; //�����ʼ��
        case 4: IsEnterGames(); break;  //��Ϸ�ӿ�
        default: printf("δ֪����"); break; //������
    }

}

//==================================================================
//�� �� ����ExitSystem
//�����������˳�����
//���������void
//�� �� ֵ��void
//���ú�������
//==================================================================
void ExitSystem()
{
    printf("�˳�����");
    exit(0);
}

//==================================================================
//�� �� ����PrintStu
//�������������������Ϣ
//����������ṹ��������ĳһ��ַ��struct stu *p��
//�� �� ֵ��void������չ��
//���ú������ޣ��ú����ᱻ����������ε���
//==================================================================
void PrintStu(struct stu *p)
{
    if(p==NULL) {
        printf("��ַ����");
    } else {
        printf("%s  %s  %d  %d  %d  %d  %.2f  %d\n",p->ID,p->name,p->score[0],p->score[1],p->score[2],p->score[3],p->aver,p->sum);
    }
}

//==================================================================
//�� �� ����SortSumDown
//�������������ֽܷ���������������ĳɼ���**�����㷨**
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ��void������չ��
//���ú�������.�����ܺ���
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
//�� �� ����SortSumUp
//�������������ܷ�������������ɼ���**�����㷨**
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ��void������չ��
//���ú�������; �����ܺ���
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
//�� �� ����SortIDUp
//������������ѧ��������������ɼ���**�����㷨**
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ��void������չ��
//���ú�������; �����ܺ���
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
//�� �� ����SortNameUp
//����������������������������ɼ���**�����㷨**
//����������ṹ�������ַ��struct stu *p��
//�� �� ֵ��void������չ��
//���ú�������; �����ܺ���
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
//�� �� ����SearchID_input
//��������������ID���ڽṹ������p�е���SearchID_part׼ȷ����ID
//����������ṹ�������׵�ַ��struct stu *p��
//�� �� ֵ�����ҵ�����Ϣ�ڽṹ�������*��ַ��û�з���NULL
//���ú�����SearchID_part(p,str);
//==================================================================
struct stu *SearchID_input(struct stu *p)
{
	char str[N2];   //���ҵ�ID

	printf("\n����������ҵ�ID��");
	scanf("%s",str);

    //����SearchID_part��p�в���IDΪstr����Ϣ���������ַ
	return SearchID_part(p,str);
}

//==================================================================
//�� �� ����SearchID_part
//�����������ڽṹ�������в���  **�ݹ��㷨**
//����������ṹ�������ַ(struct stu *p),������ID(str)
//�� �� ֵ�����ҵ�����Ϣ��ַ*��û�з���NULL
//���ú������ޣ��ú���������������ε��ã��ݹ��㷨���ID
//==================================================================
struct stu *SearchID_part(struct stu *p,char *str)
{
    if (p == student+num) {
        printf("\nδ��⵽ƥ����\n");
        return NULL; //��⵽����ĩβ
	} else if ( p<student+num && strcmp(str,p->ID)==0 ) {
	    printf("\n���ҵ���ѧ��\n");
        PrintStu(p);
        return p;
	} else {
        return SearchID_part(p+1,str); //�ݹ鲿�֣��ٴε��ú�������
	}
}

//==================================================================
//�� �� ����SearchName
//�����������ڽṹ��������ģ����������  **ģ������**
//����������ṹ�������ַ(struct stu *p)
//�� �� ֵ��״ֵ̬��û�з���NULL
//���ú�����LookUp(p,str); ģ������
//==================================================================
struct stu *SearchName(struct stu *p)
{
    char str[N2];
    struct stu *temp = NULL;

    //������ҵĲ�������
	printf("\n����������ҵĲ���������");
	scanf("%s",str);

    //�������飬��ÿһ����Ϣ��������Ϊ��������һ�Σ�ȷ��û����©
	for(p=student; p<student+num; p++) {
		if( (temp = LookUp(p,str))!=NULL ) {
            Sleep(250);
            PrintStu(p);
		}
	}
}

//==================================================================
//�� �� ����LookUp
//������������һ���ṹ���name�����ַ���ģ������str
//������������������Ľṹ���ַ(struct stu *p),ģʽ��(char *str)
//�� �� ֵ��״ֵ̬�����ޣ�����NULL
//���ú�������
//==================================================================
struct stu *LookUp(struct stu *p, char *str)
{
    //��ʼ��
    int len = strlen(str),
        match_times = 0;
    char *name = p->name,
         *i=NULL, *j=NULL;

    //ģ������
    while(*name!='\0') {
        i = name,
        j = str;
        match_times=0;  //�ַ�ƥ�����

        //������ģʽ��ƥ��
        if(*name==*str) {
            while( (*i!='\0')&&(*j!='\0') ) {
                if( (*i)==(*j) ) {
                    match_times++;
                }
                if( (*i++)!=(*j++) ) {
                    break;
                }
            }

            //���ƥ���������ģʽ�����ȣ����ҳɹ������ص�ַ
            if(match_times==len) {
                return p;
            }
        }

        name++;
    }

    return NULL;
}

//==================================================================
//�� �� ����CategoricalStatistics
//��������������ͳ�Ƹ������������Ͱٷֱ�  -��ά����
//����������ṹ�������ַ(struct stu *p)
//�� �� ֵ��void������չ��
//���ú�������
//==================================================================
void CategoricalStatistics(struct stu *p)
{
    //����������ά���鴢���Ŀ-��������Ϣ
    int gradenum[NUM_SUBJECT][5]={0};
    double gradepercent[NUM_SUBJECT][5]={0};

    //����Ϣ���ദ�������ά����
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

    //����ٷֱ�
    for(int i=0; i<NUM_SUBJECT; i++){
        for(int j=0; j<5; j++) {
            gradepercent[i][j] = ((double)(gradenum[i][j]))/num *100;
        }
    }

    //������
    printf("     \t����\t\t����\t\t�е�\t\t����\t\t������\n");
    for(int i=0; i<NUM_SUBJECT; i++){
        printf("��Ŀ%d   ",i+1);
        for(int j=0; j<5; j++) {
            printf("%2d--%5.2lf%%     ",gradenum[i][j],gradepercent[i][j]);
        }
        printf("\n");
    }
}

//==================================================================
//�� �� ����InformationInitialization
//������������ʼ������һ�����ID�����ɼ����ļ������ڽ����ļ�
//���������void
//�� �� ֵ��void
//���ú������ޣ����ڽ����ļ�
//==================================================================
void InformationInitialization(void)
{
    //����ÿ����������õ��ı���
    int a=0, b=0, c=0, d=0,
        i=0;
    char ID[15],
         name[15];
    char str[30] = {0};

    //��ָ���ļ������½��ļ�or.txt
    FILE *fp = fopen(".\\ause\\or.txt","w");

    //�������20��������
    for(i=0; i<20; i++) {
        //����6λѧ��ID
        ID[0] = random(3)+1+'0';
        ID[1]= '0';
        ID[2] = random(1) + '0';
        ID[3] = random(10) + '0';
        ID[4] = random(10) + '0';
        ID[5] = random(10) + '0';
        ID[6] = '\0';   //���ӿ��ַ��������ַ���
        //��������Сд��ĸ������
        name[0] = random(26) + 'a';
        name[1] = random(26) + 'a';
        name[2] = '\0'; //�ӿ��ַ��γ��ַ���
        //�����ĿƳɼ������50�����99
        a = random(50)+50;
        b = random(50)+50;
        c = random(50)+50;
        d = random(50)+50;
        //������д���ַ���str
        sprintf(str,"%s %s %d %d %d %d %.2f %d",ID,name,a,b,c,d,(float)(a+b+c+d)/4,a+b+c+d);
        //��ӻ��з���fputs����\n��д�벢ֹͣ
        if(i!=19) {
            strcat(str,"\n");
        }
        //д���ļ�fp
        fputs(str,fp);
    }
    fclose(fp);

    return;
}

//==================================================================
//�� �� ����IsEnterGames
//�����������Ƿ������Ϸ�жϣ��ӳɼ���ѯҳ�津��
//���������void
//�� �� ֵ��void
//���ú�����RelaxTime(); --��Ϸ���غ���
//         �ú�����α�������������
//==================================================================
void IsEnterGames(void)
{
    char order = '\0';  //��������

    printf("\n�ɼ���Щ���������Ⱑ�����翪�ڣ�\n��F�����뿪��");
    //��ȡ��������
    order = getch();
    printf("\n");
    //F������Ϸ
    if(order=='F'||order=='f') {
        RelaxTime();
    }
}

/******************************************************************************************************************************************/
/*******************************��Ϸ��������**********************************************************************************************/
/******************************************************************************************************************************************/

//==================================================================
//�� �� ����RelaxTime
//������������Ϸ���غ���
//���������void
//�� �� ֵ��int������չ��
//���ú�����startup();--��Ϸ��ʼ��
//          show();              --������ʾ
//          updateWithoutUpdate();--��Ϸ���ݸ���
//          updateWithUpdate();   --�������봦��
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
//�� �� ����startup
//������������Ϸ������ʼ����չʾ��Ϸ˵��
//���������void
//�� �� ֵ��void
//���ú�����HideCursor(); system("cls");--����
//==================================================================
void startup()
{
    //������ʼ��
	high = 25;              //����߶�
	width = 60;             //������
	position_x = high/2;    //����λ��x
	position_y = width/2;   //����λ��y
	bullet_x = 0;           //�ӵ�λ��x
	bullet_y = position_y;  //�ӵ�λ��y
	enemy_x = 0;            //����λ��x
	enemy_y = position_y;   //����λ��y
	score = 0,miss = 0;     //����
	grade = 1;              //�ȼ�

    //������ʼ��
	srand((unsigned) time(NULL)); //�������
	HideCursor();           //���ع��
	system("cls");          //��������

	//��Ƶ����
	mciSendString("open .\\music\\back.mp3 alias bkmusic", NULL, 0, NULL);//�򿪱�������
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // repeatѭ������

	printf("\n\n"
           "\t    �ɻ���Ϸ˵����\n\n"
           "\t\t�����ҷ��ɻ�*����з��ɻ�@\n\n"
           "\t\tÿ10����������ߵȼ�10��100����ͨ��\n\n\n"
           "\t    ����˵����\n\n"
           "\t\tW,S,A,D���Ʒɻ��ƶ�\n\n"
           "\t\t�ո�space�����ӵ�\n\n"
           "\t\tESC��ͣ, ����0������Ϸ\n\n");
    getch();
}

//==================================================================
//�� �� ����show
//�������������ͼ�ν��棬��Ϸ���ӻ�����
//���������void
//�� �� ֵ��void
//���ú�����gotoxy();
//==================================================================
void show()
{
    //��Ϸ��ͣ(����ֹͣ����)�жϣ�ESC��ͣ���ٰ��ָ�
	if(stop) {
		while(getch()!=27)  //ESC��ASCII����27
            ;
		stop = false;
	}

    //�ƶ���굽���Ͻ�
	gotoxy(0,0);
	int i=0, j=0;

    //�߽��飬��ֹ�Լ�����
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

    //�����������ս���ٵл����ӵ�
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
    //���ݼ�¼
	printf("�÷�: %3d	�ȼ�: %2d	miss: %2d",score,grade,miss);
}

//==================================================================
//�� �� ����updateWithoutInput
//��������������λ�ã��ӵ�λ�ã��жϻ��У��÷ֺ��Ѷȸ���
//���������void
//�� �� ֵ��void
//���ú�����
//==================================================================
void updateWithoutInput()
{
    //�ӵ�ǰ��
	if(bullet_x>-1) {
		bullet_x--;
	}

    //�ж��ӵ�λ���Ƿ���л�λ���غ�
	if( (bullet_x==enemy_x) && (bullet_y==enemy_y) ) {
		score += 2;     //�÷�����
		enemy_x = -1;   //�л���ʧ
		enemy_y = rand()%(width-4) + 2; //���λ�������µл�
		bullet_x = -1;  //����л�λ��

		mciSendString("close gemusic", NULL, 0, NULL);//�ر�������Ƶ
		mciSendString("open .\\music\\gotEnemy.mp3 alias gemusic", NULL, 0, NULL); //����Ƶ
        mciSendString("play gemusic", NULL, 0, NULL); //play������һ��
	}
    //�жϵл�����ײ�
	if( enemy_x>high ) {
		miss++;         //miss����
		enemy_x = -1;   //����л�
		enemy_y = rand()%width; //����л�λ��

		mciSendString("close lomusic", NULL, 0, NULL);//�ر�������Ƶ
		mciSendString("open .\\music\\popmusic.wav alias lomusic", NULL, 0, NULL); //����Ƶ
        mciSendString("play lomusic", NULL, 0, NULL); //play������һ��
	}
    //�ж���Ϸ������ʧ�󵽴�20���ߵȼ�����11
	if(miss == 20 || grade == 11) {
		over = true;
	}

    //ÿ10����Ϸ�Ѷ�+1
	if(score>=grade*10) {
		grade++;
	}

    //�Ѷ���ʾ���л��ٶȣ�ͨ����ѭ��ʵ���ٶȿ���
	static int speed = 0;   //��̬������ȫ��Ϸ�ɼ��ٶ�
    //����speed���Ѷ�grade����ʱ��speed����11-grade��������
	if (speed<11-grade) {
		speed++;
	}
    //speed����11-grade���л��ƶ�һ��
	if (speed == 11-grade) {
		enemy_x++;  //�л�ǰ��
		speed = 0;  //����speed
	}
}

//==================================================================
//�� �� ����updateWithInput
//����������������̲���������ˢ��(kbhitʵ��)
//���������void
//�� �� ֵ��void
//���ú�����kbhit(); �ж��ɼ������뷵��true�����򷵻�false
//         getch(); ��ȡ��������
//==================================================================
void updateWithInput()
{
	char input = '1';   //������������

    //�ж����룬�ı�ɻ�λ��(�߽����ں���show()�����)
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
			case ' ':   //�ո����ӵ�
				bullet_x = position_x - 1;
				bullet_y = position_y;
				mciSendString("close fgmusic", NULL, 0, NULL); // �ر�������Ƶ
				mciSendString("open .\\music\\Jump.mp3 alias fgmusic", NULL, 0, NULL); // ����Ƶ
				mciSendString("play fgmusic", NULL, 0, NULL); // play������һ��
				break;
			case '0':   //0������Ϸ
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
    printf("\n��Ϸ����");
    getch();
    mciSendString("close bkmusic", NULL, 0, NULL);
}
//==================================================================
//�� �� ����gotoxy
//����������������̨����ƶ���x��y����������windows.h
//�������������x��y(int x, int y)
//�� �� ֵ��void
//���ú�����HANDLE�����COORD�ṹ��
//         GetStdHandle(STD_OUTPUT_HANDLE);
//         SetConsoleCursorPosition(handle,pos);
//==================================================================
void gotoxy(int x,int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//���岢��ȡ�ľ��
    COORD pos;  //windows.h������ṹ�壬�����ṹ�����pos
    pos.X = x;  //x����
    pos.Y = y;  //y����
    SetConsoleCursorPosition(handle,pos);   //�ƶ��������й���λ��
}

//==================================================================
//�� �� ����HideCursor
//�������������ع��
//���������void
//�� �� ֵ��void
//���ú�����CONSOLE_CURSOR_INFO�ṹ��
//==================================================================
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};   //���ƹ��߶�1����ʾΪfalse
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//��׼�������ִ̨�в���
}

/******************************************************************************************************************************************/
/*******END*END*END*END*END*END*END*END*END*END*END*END*END*END*END*END********************************************************************/
/******************************************************************************************************************************************/
