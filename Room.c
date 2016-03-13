#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
//struct rooms_info
//{
//    int rooms_count;
//    ROOM room[100];
//};
//typedef struct rooms_info ROOMS;

struct stu_info
{
    //default表示没有
    char name[20];
    //0表示没有
    int st_no;
    //0表示没有
    int st_sex;
};
typedef struct stu_info STU;

struct room_info
{
    int no;
    //10代表男性，11代表女性
    int sex;
    //已容纳学生数
    int count;
    STU stu[4];
};
typedef struct room_info ROOM;


void WriteToFile( ROOM *proom, int num);
void ReadFromFile( ROOM *proom, int num);
void ShowAllRoom(int num);
void ShowRoom(ROOM *proom);
int CreateRoom();
void InputStu(int num);
void SearchStu(int num);
void SearchRoom(int num);
void ChangeRoom(int num);
void ExchangeRoom(int num);
void Quit(int num);
int Check(int no, int num);

int main()
{
    int cmd;
    int i,num;
    char str[20];
    FILE *fp2;
    printf("欢迎来到宿舍管理系统\n");
    printf("命令如下：\n");
    printf("0：输入宿舍信息 1：全部宿舍 2：输入学生信息并分配宿舍\n");
    printf("3：查询学生信息 4：查询寝室信息 5：调换宿舍\n");
    printf("6：互换宿舍 7：毕业退寝  8：退出系统\n");
    printf("提示信息：男为10，女为11，默认为0\n");
    fp2 = fopen("rooms_num.txt", "rt");
    fgets(str, 20, fp2);
    if(str!=NULL)
    {
        num=atoi(str);
        printf("现在共有%d个宿舍\n",num);
    }
    else
    {
        printf("现在还没有任何宿舍，请先创建宿舍\n");
    }
    fclose(fp2);
    while(1)
    {
        printf("请输入命令:");
        scanf("%d",&cmd);
        switch(cmd)
        {
        case 0:
            num = CreateRoom();
            break;
        case 1:
            ShowAllRoom(num);
            break;
        case 2:
            InputStu(num)   ;
            break;
        case 3:
            SearchStu(num)   ;
            break;
        case 4:
            SearchRoom(num)   ;
            break;
        case 5:
            ChangeRoom(num)  ;
            break;
        case 6:
            ExchangeRoom(num)  ;
            break;
        case 7:
            Quit(num);
            break;
        case 8:
            exit(0);
            break;
        }
    }
    return 0;
}

//返回0表示此学号不存在，返回1表示已存在
int Check(int no, int num)
{
    int i,j;
    int found = 0;
    ROOM *proom;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    ReadFromFile(proom, num);
    for(i=0;i<num;i++)
    {
        for(j=0;j<4;j++)
        {
            if(proom[i].stu[j].st_no == no)
            {
                found = 1;
                break;
            }
        }
        if(found == 1)
            break;
    }
    memset(proom, 0, num*sizeof(ROOM));
    free(proom);
    if(found==1)
        return 1;
    else
        return 0;
}

void Quit(int num)
{
    int i,j,no;
    //用以中断循环的判定符
    int found = 0;
    ROOM *proom;
    //为指针分配内存
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    //从文件中获得对象
    ReadFromFile(proom, num);
    printf("请输入要退寝的学生学号：");
    scanf("%d",&no);
    for(i=0;i<num;i++)
    {
        for(j=0;j<4;j++)
        {
            if(proom[i].stu[j].st_no == no)
            {
                strcpy(proom[i].stu[j].name, "default");
                proom[i].stu[j].st_no = 0;
                proom[i].stu[j].st_sex = 0;
                proom[i].count --;
                found = 1;
                printf("退寝后您寝室的情况为：\n");
                ShowRoom(&proom[i]);
                break;
            }
        }
        if(found == 1)
            break;
    }
    //重新写入文件
    WriteToFile(proom, num);
    //释放所指向的内存块
    memset(proom, 0, num*sizeof(ROOM));
    //释放指针
    free(proom);
    printf("***********************************************************\n");
}

void ExchangeRoom(int num)
{
    int i,j;
    ROOM *proom;
    //两学生的学号，所查到的寝室对应的存储编号，在寝室中位次
    int no1, no2, room1, room2, loc1, loc2;
    int founded = 0;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    ReadFromFile(proom, num);
    printf("请输入交换寝室的两个人的学号：");
    scanf("%d%d", &no1,&no2);
    //查找第一个人所在的寝室和学生位置
    for(i=0;i<num;i++)
    {
        for(j=0;j<4;j++)
        {
            if(proom[i].stu[j].st_no==no1)
            {
                room1 = i;
                loc1 = j;
                founded = 1;
                break;
            }
        }
        if(founded == 1)
            break;
    }

    //查找第二个人所在的寝室和学生位置，查找前重置founded=0
    founded = 0;
    for(i=0;i<num;i++)
    {
        for(j=0;j<4;j++)
        {
            if(proom[i].stu[j].st_no==no2)
            {
                room2 = i;
                loc2 = j;
                founded = 1;
                break;
            }
        }
        if(founded == 1)
            break;
    }
    if(proom[room1].sex != proom[room2].sex)
    {
        printf("交换宿舍失败，两人性别不同！\n");
        printf("***********************************************************\n");
        memset(proom, 0, num*sizeof(ROOM));
        free(proom);
        return;
    }
    //把两宿舍里关于这两个学生信息互换
    char name[20];
    int st_no,st_sex;
    strcpy(name, proom[room1].stu[loc1].name);
    st_no = proom[room1].stu[loc1].st_no;
    st_sex = proom[room1].stu[loc1].st_sex;

    strcpy(proom[room1].stu[loc1].name,proom[room2].stu[loc2].name);
    proom[room1].stu[loc1].st_no = proom[room2].stu[loc2].st_no;
    proom[room1].stu[loc1].st_sex = proom[room2].stu[loc2].st_sex;

    strcpy(proom[room2].stu[loc2].name, name);
    proom[room2].stu[loc2].st_no = st_no;
    proom[room2].stu[loc2].st_sex = st_sex;
    printf("交换宿舍成功！\n");
    printf("学号为%d的学生的宿舍信息为：\n",no1);
    ShowRoom(&proom[room1]);
    printf("学号为%d的学生的宿舍信息为：\n",no2);
    ShowRoom(&proom[room2]);
    WriteToFile(proom, num);
    memset(proom, 0, num*sizeof(ROOM));
    free(proom);
    printf("***********************************************************\n");
}

void SearchStu(int num)
{
    int s_no,i,j,k;
    int found = 0;
    ROOM *proom;
    //为指针分配内存空间
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    //从文件中获得对象数组的首地址赋予指针
    ReadFromFile(proom, num);
    printf("您的学号是:");
    scanf("%d",&s_no);
    for(i=0; i<num; i++)
    {
        for(j=0; j<4; j++)
        {
            if(proom[i].stu[j].st_no == s_no)
            {
                printf("%10s%10s%15s\n","寝室号","男/女寝","已容纳学生");
                printf("%10d%10d%15d\n",proom[i].no,proom[i].sex,proom[i].count);
                printf("舍友信息：\n");
                printf("%10s%15s%10s\n","姓名","学号","性别");
                for(k=0; k<4; k++)
                {
                    if(k!=j)
                    {
                        printf("%10s%15d%10d\n",proom[i].stu[k].name, proom[i].stu[k].st_no, proom[i].stu[k].st_sex);
                    }
                }
                found = 1;
                break;
            }
        }
        //如果已找到就不再查找
        if(found == 1)
            break;
    }
    //没找到，则提示信息
    if(found ==0 )
        printf("未查到您所输入的学号对应的学生信息\n");
    printf("***********************************************************\n");
    free(proom);
}

void SearchRoom(int num)
{
    int i,j,r_no;
    ROOM *proom;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    ReadFromFile(proom, num);
    printf("您所要查找的宿舍号是:");
    scanf("%d",&r_no);
    for(i=0; i<num; i++)
    {
        if(proom[i].no == r_no)
        {
            ShowRoom(&proom[i]);
            break;
        }
    }
    if(i==num)
    {
        printf("未查到您所输入的寝室号对应的信息\n");
    }
    printf("***********************************************************\n");
    free(proom);
}

void ChangeRoom(int num)
{
    int i_no,i,j;
    //记录此学生所在的寝室号,及他是寝室里的第几个学生
    int rec_no, rec_rank;
    //所要加入的寝室号
    int choosed;
    int existed = 0;
    ROOM *proom;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    ReadFromFile(proom, num);
    printf("您的学号是：");
    scanf("%d",&i_no);
    printf("可换入的寝室如下：\n");
    for(i=0; i<num; i++)
    {
        existed = 0;
        for(j=0; j<4; j++)
        {
            if(proom[i].stu[j].st_no == i_no)
            {
                existed = 1;
                rec_no = i;
                rec_rank = j;
            }
        }
        if(existed == 0 && proom[i].count<4)
        {
            ShowRoom(&proom[i]);
        }
    }
    printf("\n");
    printf("请输入您想要加入的寝室号:");
    scanf("%d",&choosed);
    //
    for(i=0; i<num; i++)
    {
        if(proom[i].no == choosed)
        {
            if(proom[i].sex != proom[rec_no].sex)
            {
                printf("您所要求换入的寝室与您性别不符，换寝失败！\n");
                printf("***********************************************************\n");
                memset(proom, 0, num*sizeof(ROOM));
                free(proom);
                return;
            }
            //寻找可换的空位
            for(j=0; j<4; j++)
            {
                if(proom[i].stu[j].st_no == 0)
                {
                    proom[i].stu[j].st_no = proom[rec_no].stu[rec_rank].st_no;
                    proom[i].stu[j].st_sex = proom[rec_no].stu[rec_rank].st_sex;
                    strcpy(proom[i].stu[j].name, (proom[rec_no].stu[rec_rank].name));
                    proom[i].count++;
                    //把原来的宿舍里的该学生位置置空
                    proom[rec_no].stu[rec_rank].st_no = 0;
                    proom[rec_no].stu[rec_rank].st_sex = 0;
                    strcpy(proom[rec_no].stu[rec_rank].name,"default");
                    proom[rec_no].count--;
                    break;
                }
            }
            if(j==4)
                printf("您选择的寝室已满\n");
            break;
        }
    }
    if(i==num)
        printf("您输入的寝室号有误\n");
    else
        printf("调换宿舍成功\n");
    WriteToFile(proom, num);
    memset(proom, 0, num*sizeof(ROOM));
    free(proom);
    printf("***********************************************************\n");
}

void InputStu(int num)
{
    char iname[20];
    int ino,isex;
    int selected,i,j;
    int full = 0;
    int succeed = 0;
    ROOM *proom;
    //给指针proom分配内存空间
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    //从room.bat中获取ROOM对象数组首地址于指针proom
    ReadFromFile(proom, num);
    printf("您的名字：");
    scanf("%s",iname);
    printf("学号：");
    scanf("%d",&ino);
    if(Check(ino,num)==1)
    {
        printf("\n您所输入的学号已存在于系统中，请检查是否输错!\n");
        return;
    }
    printf("性别（10代表男性，11代表女性）：");
    scanf("%d",&isex);
    printf("\n");
    printf("所有寝室如下，请选择可加入的寝室加入\n");
    //调用此方法展示所有寝室及其学生信息
    ShowAllRoom(num);
    printf("请输入您所选择的寝室号\n");
    scanf("%d",&selected);
    for(i=0; i<num; i++)
    {
        //printf("%d\n",proom[i].no);
        if(proom[i].no == selected)
        {
            if(proom[i].sex != isex)
            {
                printf("您所要求加入的寝室与您性别不符，加入寝室失败！\n");
                printf("***********************************************************\n");
                memset(proom, 0, num*sizeof(ROOM));
                free(proom);
                return;
            }
            for(j=0; j<4; j++)
            {
                if(proom[i].stu[j].st_no == 0)
                {
                    succeed = 1;
                    strcpy(proom[i].stu[j].name, iname);
                    proom[i].stu[j].st_no = ino;
                    proom[i].stu[j].st_sex = isex;
                    proom[i].count++;
                    break;
                }
            }
            if(j==4)
            {
                printf("您选择的寝室已满员!\n");
                break;
            }
            if(succeed == 1)
            {
                printf("加入宿舍成功!\n");
                break;
            }
        }
    }
    if(i==num)
        printf("不存在您所选择的寝室号对应的寝室\n");
    WriteToFile(proom, num);
    ShowAllRoom(num);
    memset(proom, 0, num*sizeof(ROOM));
    free(proom);
    printf("\n***********************************************************\n");
}

void ShowRoom(ROOM *proom)
{
    int j;
    printf("%10s%10s%15s\n","寝室号","男/女寝","已容纳学生");
    printf("%10d%10d%15d\n",(*proom).no, (*proom).sex, (*proom).count);
    printf("%10s%15s%10s\n","姓名","学号","性别");
    for(j=0; j<4; j++)
    {
        printf("%10s%15d%10d\n", (*proom).stu[j].name, (*proom).stu[j].st_no, (*proom).stu[j].st_sex);
    }

}


void ShowAllRoom(int num)
{
    int i,j;
    ROOM *proom;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    printf("\n");
    ReadFromFile(proom, num);
    //输出宿舍信息
    for(i=0; i<num; i++)
    {
        printf("%10s%10s%15s\n","寝室号","男/女寝","已容纳学生");
        printf("%10d%10d%15d\n",proom[i].no,proom[i].sex,proom[i].count);
        printf("%10s%15s%10s\n","姓名","学号","性别");
        for(j=0; j<4; j++)
        {
            printf("%10s%15d%10d\n",proom[i].stu[j].name, proom[i].stu[j].st_no, proom[i].stu[j].st_sex);
        }
    }
    printf("***********************************************************\n");
    free(proom);
}

int CreateRoom()
{
    FILE *fp2;
    int i,j,no,sex,num;
    char out[20];
    printf("\n请输入宿舍个数：");
    scanf("%d",&num);

    fp2 = fopen("rooms_num.txt", "w");
    itoa(num, out, 10);
    fputs(out, fp2);
    fclose(fp2);

    ROOM *proom;
    proom = (ROOM *)malloc(num * sizeof(ROOM));
    if(proom == NULL)
    {
        printf("\n内存空间不够");
        return;
    }
    else
    {
        for(i=0; i<num; i++)
        {
            printf("\n第%d个:",i+1);
            printf("\n请输入宿舍号：");
            scanf("%d",&proom[i].no);
            printf("请输入宿舍性别：（10代表男性，11代表女性）");
            scanf("%d",&proom[i].sex);
            proom[i].count=0;
            for(j=0; j<4; j++)
            {
                strcpy(proom[i].stu[j].name,"default");
                proom[i].stu[j].st_no = 0;
                proom[i].stu[j].st_sex = 0;
            }
        }
        WriteToFile(proom, num);
        memset(proom, 0, num*sizeof(ROOM));
        free(proom);
        printf("\n***********************************************************\n");
        return num;
    }


}

//将proom所指向的宿舍信息写到文件room.dat中
void WriteToFile(ROOM *proom, int num)
{
    FILE *fp;

    fp = fopen("room.dat", "wb");
    if(fp==NULL)
    {
        printf("无法创建room.dat\n");
        free(proom);
        exit(0);
    }
    fwrite(proom, sizeof(ROOM), num, fp);
    fclose(fp);
}

//从文件room.dat中读取宿舍信息到proom所指向的内存块中
void ReadFromFile(ROOM *proom, int num)
{
    FILE *fp;

    fp = fopen("room.dat", "rb");
    if(fp==NULL)
    {
        printf("无法打开room.dat\n");
        free(proom);
        exit(0);
    }
    fread(proom, sizeof(ROOM), num, fp);
    fclose(fp);
}
