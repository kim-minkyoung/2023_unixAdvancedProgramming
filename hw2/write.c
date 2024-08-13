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
    int  shmid; // shared memory id
    char userID[20]; // user id
    void *shmaddr = (void *)0; //shared memory address

    CHAT_INFO *chatinfo= NULL;

    /**  shared memory 연결 start **/
    if (argc < 2) {
        fprintf(stderr, "[Usage]: ./shmwrite UserID \n");
        exit(0);
    }
    //사용자가 userID를 입력하지 않았을 경우 에러 출력

    strcpy(userID, argv[1]);
    //argv[1]을 userID에 복사
    
    shmid = shmget((key_t)202155532, sizeof(CHAT_INFO), 0666|IPC_CREAT|IPC_EXCL);
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
    }// 

    shmaddr = shmat(shmid, (void *)0, 0666);
    chatinfo = (CHAT_INFO *)shmaddr;
    //연결된 메모리 영역을 "CHAT_INFO"구조체에 포인터로 변환
    printf("shmid: %d\n", shmid);
    /**  shared memory 연결 end**/


    /** window subwindow 설정 start **/
    WINDOW *output, *input, *id, *pureOutput, *logo;
    int i;

    initscr(); //ncurses 라이브러리를 사용하여 초기화
    refresh(); //현재 화면 업데이트 후 변경사항을 터미널에 반영

    output = subwin(stdscr, 13, 60, 0, 0); //ouput 서브윈도우 설정
    pureOutput = subwin(stdscr, 11, 58, 1, 1);
    scrollok(pureOutput, TRUE);//output 서브윈도우에 스크롤 설정
    input = subwin(stdscr, 3, 60, 13, 0);
    id = subwin(stdscr, 5, 11, 0, 60);
    logo = subwin(stdscr,11,11,5, 60 );
    //input, id창 서브윈도우 설정

    mvwprintw(id, 1, 1, userID); //입력받은 userId 출력
    mvwprintw(logo,2,2, "k    k\n  k   k\n  k  k\n  kk\n  k  k\n  k   k\n  k    k\n");

    box(output,0,0);
    box(input,0,0);
    box(id,0,0);
    box(logo,0,0);


    refresh();
    wrefresh(input);
    wrefresh(id);
    wrefresh(output);
    wrefresh(pureOutput);
    wrefresh(logo);
    //화면 변경사항 저장

    /** window subwindow 설정 end **/


    /** start **/
   while(1)
   {
        char inputstr[40]; //input 문자열이 저장될 배열
        const char *exit = "/q"; //exit 키워드

        mvwgetstr(input, 1, 1, inputstr); //input값을 받아서 inputstr에 저장
        wprintw(pureOutput, "%s\n", inputstr); //입력값을 output창에서 출력
        
        strcpy(chatinfo->userID, userID);
        chatinfo->messageTime++;
        strcpy(chatinfo->message, inputstr);
        //chatinfo 구조체에 채팅 정보 저장

        if (strstr(inputstr, exit) != NULL) {
           break;
        }//exit 키워드 들어오면 종료
        
        for(i = 0 ; i < strlen(inputstr) ; i++) {
            inputstr[i] = 0x20;         
        }//input창 초기화

        mvwprintw(input, 1, 1, "%s", inputstr);
        wrefresh(input);
        wrefresh(output);
        wrefresh(pureOutput);
        //화면 변경사항 저장 및 출력
   }
    /** end **/


  getch();
  delwin(input);
  delwin(id);
  delwin(output);
  delwin(logo);
  delwin(pureOutput);

  endwin();
  //curses 모드 종료
  return 0;
}

