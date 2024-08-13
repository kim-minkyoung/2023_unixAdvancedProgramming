#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h> 
#include "chatHeader.h"

int main(int argc, char** argv)
{
    int shmid, countUser=0; // shared memory id, userCount
    void *shmaddr = (void *)0; //shared memory address
    char userlist[3][20];
    
    CHAT_INFO *chatinfo= NULL;

     /**  shared memory 연결 start **/
    shmid = shmget((key_t)202155532, sizeof(CHAT_INFO),0666|IPC_CREAT|IPC_EXCL);
    //성공적으로 새로운 공유 메모리 생성에 성공한 경우 해당 메모리 식별자가 반환됩니다.
    //공유 메모리가 이미 존재하는 경우 "-1"이 반환됩니다.

    if (shmid < 0) //이미 공유메모리가 존재하는 경우
    {   
        shmid = shmget((key_t)202155532, sizeof(CHAT_INFO),0666); 
        //이미 존재하는 공유 메모리의 식별자 얻기
        shmaddr = shmat(shmid, (void *)0, 0666);
        //공유 메모리를 현재 프로세스의 메모리 공간에 연결
        if (shmaddr < 0) // 메모리 연결 실패시 에러 발생
        {   
            perror("shmat attach is failed : ");
            exit(0);
        }
    }

    shmaddr = shmat(shmid, (void *)0, 0666);
    chatinfo = (CHAT_INFO *)shmaddr;
    //연결된 메모리 영역을 "CHAT_INFO"구조체에 포인터로 변환
    printf("shmid: %d\n", shmid);
    /**  shared memory 연결 end**/


    /** window subwindow 설정 start **/
    WINDOW *output,*pureOutput, *input, *id,*logo;
    int i;

    initscr(); //ncurses 라이브러리를 사용하여 초기화
    refresh();  //현재 화면 업데이트 후 변경사항을 터미널에 반영

    output = subwin(stdscr, 13, 60, 0, 0); //ouput 서브윈도우 설정
    pureOutput = subwin(stdscr, 11, 58, 1, 1);
    scrollok(pureOutput, TRUE); //output 서브윈도우에 스크롤 설정
    input = subwin(stdscr, 3, 60, 13, 0);
    id = subwin(stdscr, 5, 11, 0, 60 );
    logo = subwin(stdscr,11,11,5, 60 );
    //input, id창 서브윈도우 설정

    mvwprintw(logo,2,2, "k    k\n  k   k\n  k  k\n  kk\n  k  k\n  k   k\n  k    k\n");
    box(output,0,0);
    box(input,0,0);
    box(id,0,0);
    box(logo,0,0);

    refresh();
    wrefresh(input);
    wrefresh(id);
    wrefresh(output);
    wrefresh(logo);
    wrefresh(pureOutput);
    //화면 변경사항 저장
    /** window subwindow 설정 end **/

    //입력값 한번만 채팅 올리기-시간설정 변수
    int nowMsgTime = chatinfo->messageTime;
    int prevMsgTime = chatinfo->messageTime;

    /** start **/
    while(1)
   {
        nowMsgTime = chatinfo->messageTime;

        if(prevMsgTime != nowMsgTime) { 
        //user list update
            bool isPrevUser = false; // 이미 존재하는 user인지
            for(int i = 0; i < countUser; i++) {
                if(strcmp(userlist[i], chatinfo->userID) == 0) {
                    isPrevUser = true;
                    break;
                }
            }
            if(!isPrevUser) { //새로 들어온 user일 경우
                //user list에 이름 넣기 + 메시지 출력 화면 업데이트
                strcpy(userlist[countUser], chatinfo->userID);
                mvwprintw(id,countUser+1 , 1, chatinfo->userID); 
                wrefresh(id);
                countUser++;
            }
            wprintw(pureOutput, "[%s]%ld: %s\n",
                chatinfo->userID,
                chatinfo->messageTime,
                chatinfo->message);    
        }
        else {
            continue;
        }
        if (strcmp(chatinfo->message, "/q")==0) {
            wprintw(pureOutput,"%s is out\n", chatinfo->userID);
        } //"/q" 입력받으면 종료
        prevMsgTime = chatinfo->messageTime;  
    
        wrefresh(output);
        wrefresh(pureOutput);
        sleep(2);
   }

//입력값 받기 끝
  getch();
  delwin(input);
  delwin(id);
  delwin(output);
  delwin(pureOutput);
  endwin();
  //curses 모드 종료

  return 0;
}


