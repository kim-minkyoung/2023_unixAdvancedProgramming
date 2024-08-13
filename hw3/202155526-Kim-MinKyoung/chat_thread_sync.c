#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_USERS 3 // 최대 사용자 수 정의

typedef struct {
    char userID[20]; // 사용자 ID
    long messageTime; // 메시지 시간
    char message[40]; // 메시지 내용
    int count;
    } CHAT_INFO;

pthread_mutex_t mutex; // 쓰레드 동기화를 위한 뮤텍스
WINDOW *output, *pureOutput, *input, *id, *logo; // ncurses 윈도우들
CHAT_INFO *chatinfo = NULL; // 공유 메모리를 가리키는 포인터
int countUser;
char userlist[MAX_USERS+1][20]; // 사용자 목록
// 채팅 쓰레드 함수
void *chatThread(void *arg) {
    int nowMsgTime, prevMsgTime;

    while (1) {
        pthread_mutex_lock(&mutex); // 잠금 획득

        nowMsgTime = chatinfo->messageTime;
        chatinfo->count = countUser +1;
        // 이전 메시지와 현재 메시지의 시간이 다를 경우에만 처리
        if (prevMsgTime != nowMsgTime) {
            bool isPrevUser = false; //이미 존재하는 user 인지

            // 새로운 사용자인지 확인
            for (int i = 0; i < countUser; i++) {
                if (strcmp(userlist[i], chatinfo->userID) == 0) {
                    isPrevUser = true;
                    break;
                }
            }
            if (!isPrevUser) {
                strcpy(userlist[countUser], chatinfo->userID);
            }
            for (int i = 0; i < countUser + 1; i++) {
                mvwprintw(id, i + 1, 1, userlist[i]);
                wrefresh(id);
            }
            // 새로운 사용자일 경우 사용자 목록에 추가하고 출력, user list 에 이름 넣기+메세지 출력 화면 업데이트
            if (!isPrevUser) {
                countUser++;
            }


            // 메시지 출력
            wprintw(pureOutput, "[%s]%ld: %s \n",
                chatinfo->userID,
                chatinfo->messageTime,
                chatinfo->message
                );
        }
        else {
            pthread_mutex_unlock(&mutex); // 잠금 해제
            continue;
        }

        // 종료 메시지를 받으면 해당 사용자가 나간 것으로 출력
        if (strcmp(chatinfo->message, "/q") == 0) {
            wprintw(pureOutput, "%s is out\n", chatinfo->userID);
            countUser--;
        
        }

        prevMsgTime = chatinfo->messageTime;
    
        // 변경사항 적용
        wrefresh(output);
        wrefresh(pureOutput);
        pthread_mutex_unlock(&mutex); // 잠금 해제
        sleep(2); // 2초 대기
    }

    return NULL;
}

// 메인 함수
int main(int argc, char** argv)
{
    int shmid;
    void *shmaddr = (void *)0;
    char userID[20];

    //userID입력
    //사용자가 userID를 입력하지 않았을 경우 에러 출력
    if (argc < 2) {
        fprintf(stderr, "[Usage]: ./shmwrite UserID \n, %d",countUser);
        exit(0);
    }
    
    // 공유 메모리 생성 또는 연결
    shmid = shmget((key_t)202155532, sizeof(CHAT_INFO), 0666|IPC_CREAT|IPC_EXCL);

    if (shmid < 0) {
        shmid = shmget((key_t)202155532, sizeof(CHAT_INFO), 0666);
        if (shmid < 0) {
            perror("shmget");
            exit(EXIT_FAILURE);
        }
    }

    // 공유 메모리에 연결
    shmaddr = shmat(shmid, (void *)0, 0666);
    if (shmaddr == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // 연결된 공유 메모리를 구조체 포인터에 매핑
    chatinfo = (CHAT_INFO *)shmaddr;
    
    if (chatinfo->count <= 3) {strcpy(userID, argv[1]);}
    else {
        fprintf(stderr, "MAX_USER = 3, and now %d,it's full",chatinfo->count);
        exit(0);
    }

    // ncurses 초기화
    initscr();
    refresh();

    // 서브 윈도우 생성
    output = subwin(stdscr, 13, 60, 0, 0);
    pureOutput = subwin(stdscr, 11, 58, 1, 1);
    scrollok(pureOutput, TRUE);
    input = subwin(stdscr, 3, 60, 13, 0);
    id = subwin(stdscr, MAX_USERS + 2, 11, 0, 60);
    logo = subwin(stdscr, 11, 11, 5, 60);

    //id 출력
//    mvwprintw(id, 1, 1, userID); //입력받은 userId 출력
    // 로고 출력
    mvwprintw(logo, 2, 2, "k    k\n  k   k\n  k  k\n  kk\n  k  k\n  k   k\n  k    k\n");
    // 윈도우 테두리 그리기
    box(output, 0, 0);
    box(input, 0, 0);
    box(id, 0, 0);
    box(logo, 0, 0);

    // 변경사항 적용
    refresh();
    wrefresh(input);
    wrefresh(id);
    wrefresh(output);
    wrefresh(logo);
    wrefresh(pureOutput);

    // 쓰레드 뮤텍스 초기화
    pthread_mutex_init(&mutex, NULL);

    // 채팅 쓰레드 생성
    pthread_t thread;
    if (pthread_create(&thread, NULL, chatThread, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // 채팅 입력 처리 부분
    while (1) {
        char inputstr[40];
        const char *exit = "/q";

        mvwgetstr(input, 1, 1, inputstr);

        pthread_mutex_lock(&mutex); // 잠금 획득

        strcpy(chatinfo->userID, userID);
        chatinfo->messageTime++;
        strcpy(chatinfo->message, inputstr);

        if (strstr(inputstr, exit) != NULL) {
            pthread_mutex_unlock(&mutex); // 잠금 해제
            pthread_cancel(thread); //채팅 스레드 종료
            break;
        }
        

        // pureOutput 윈도우 갱신
        wrefresh(pureOutput);

        for (int i = 0; i < strlen(inputstr); i++) {
            inputstr[i] = 0x20;
        }

        mvwprintw(input, 1, 1, "%s", inputstr);
        wrefresh(input);
        wrefresh(output);

        pthread_mutex_unlock(&mutex); // 잠금 해제
    }


    // 쓰레드 종료 대기
    if (pthread_join(thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    // 쓰레드 뮤텍스 해제
    pthread_mutex_destroy(&mutex);

    // ncurses 종료
    getch();
    delwin(input);
    delwin(id);
    delwin(output);
    delwin(logo);
    delwin(pureOutput);
    endwin();

    return 0;
}
