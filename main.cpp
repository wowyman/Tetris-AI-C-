#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "gach.h"
using namespace std;
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 620;
int k;
int board_game[31][42];
void init(int &le_trai,int &le_phai)
{
    for(int i=0; i<30; i++)
    {
        for(int j=1; j<41; j++)
        {
            board_game[i][j] = 0;
        }
    }
    for(int i=0; i<31; i++)
    {
        board_game[i][le_trai] = -1;
        board_game[i][le_phai] = -1;
    }
    for(int i=1; i<=40; i++)
    {
        board_game[30][i] = -1;
    }
}
void logSDLError(std::ostream& os,const std::string &msg, bool fatal = false);

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}
const string WINDOW_TITLE = "An Implementation of Code.org Painter";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void waitUntilKeyPressed();

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&
                (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
bool la_o_thanh_phan(gach &box,point E,point &A,point &B,point &C,point &D)//kiem tra o nhan vao co la 1 trong 4 o cua khoi gach khong, neu co tra ve true ,khong tra ve false
{
    bool check = ((E.x == A.x && E.y == A.y) || (E.x == B.x && E.y == B.y )|| (E.x == C.x && E.y == C.y) || (E.x == D.x && E.y == D.y) );

    return check;
}
bool IsMove(gach &box,point &A,point &B,point &C,point &D,int huong)//huong: 0 la xuong,1 la sang trai,2 la sang phai
{
    point a,b,c,d;
    bool check=true,check1=true,check2=true,check3=true,check4=true;
    if(huong == 0)
    {
        a.x = A.x + 1;
        a.y = A.y;
        b.x = B.x + 1;
        b.y = B.y;
        c.x = C.x + 1;
        c.y = C.y;
        d.x = D.x + 1;
        d.y = D.y;
    }
    else if(huong == 1)
    {
        a.x = A.x;
        a.y = A.y - 1;
        b.x = B.x;
        b.y = B.y - 1;
        c.x = C.x;
        c.y = C.y - 1;
        d.x = D.x;
        d.y = D.y - 1;
    }
    else if(huong == 2)
    {
        a.x = A.x;
        a.y = A.y + 1;
        b.x = B.x;
        b.y = B.y + 1;
        c.x = C.x;
        c.y = C.y + 1;
        d.x = D.x;
        d.y = D.y + 1;
    }

    if(la_o_thanh_phan(box,a,A,B,C,D)== true)
    {
        check1=true;
    }
    else
    {
        if(board_game[a.x][a.y]==-1 || board_game[a.x][a.y] == 1)
        {
            check1 = false;
        }
        if(board_game[a.x][a.y]==0)
        {
            check1 = true;
        }
    }
    if(la_o_thanh_phan(box,b,A,B,C,D)== true)
    {

        check2=true;
    }
    else
    {
        if(board_game[b.x][b.y]==-1 || board_game[b.x][b.y] == 1)
        {
            check2 = false;
        }

        if(board_game[b.x][b.y]==0)
        {
            check2 = true;
        }

    }
    if(la_o_thanh_phan(box,c,A,B,C,D)== true)
    {
        check3=true;
    }

    else
    {
        if(board_game[c.x][c.y]==-1 || board_game[c.x][c.y] == 1)
        {
            check3 = false;
        }
        if(board_game[c.x][c.y]==0)
        {
            check3 = true;
        }
    }
    if(la_o_thanh_phan(box,d,A,B,C,D)== true)
    {
        check4=true;
    }
    else
    {
        if(board_game[d.x][d.y]==-1 || board_game[d.x][d.y] == 1)
        {
            check4 = false;
        }
        if(board_game[d.x][d.y]==0)
        {
            check4 = true;
        }
    }
    check = (check1 && check2 && check3 && check4);
    return check;
}
void xoay(SDL_Renderer *renderer,gach &box,int &k,point &A,point &B,point &C,point &D)
{
    if(box.type == 1 || box.type == 2 || box.type == 5 )
    {
        int Xtemp=A.x;
        A.x=-A.y+box.tam.x+box.tam.y;
        A.y=Xtemp-box.tam.x+box.tam.y;

        Xtemp=B.x;
        B.x=-B.y+box.tam.x+box.tam.y;
        B.y=Xtemp-box.tam.x+box.tam.y;

        Xtemp=C.x;
        C.x=-C.y+box.tam.x+box.tam.y;
        C.y=Xtemp-box.tam.x+box.tam.y;

        Xtemp=D.x;
        D.x=-D.y+box.tam.x+box.tam.y;
        D.y=Xtemp-box.tam.x+box.tam.y;
    }
    else if(box.type == 3 || box.type == 4 || box.type == 7)
    {
        if(k % 2 == 1)
        {
            int Xtemp=A.x;
            A.x=-A.y+box.tam.x+box.tam.y;
            A.y=Xtemp-box.tam.x+box.tam.y;

            Xtemp=B.x;
            B.x=-B.y+box.tam.x+box.tam.y;
            B.y=Xtemp-box.tam.x+box.tam.y;

            Xtemp=C.x;
            C.x=-C.y+box.tam.x+box.tam.y;
            C.y=Xtemp-box.tam.x+box.tam.y;

            Xtemp=D.x;
            D.x=-D.y+box.tam.x+box.tam.y;
            D.y=Xtemp-box.tam.x+box.tam.y;
        }
        if(k % 2 == 0 && k > 0)
        {
            int Xtemp=A.x;
            A.x=A.y+box.tam.x-box.tam.y;
            A.y=-Xtemp+box.tam.x+box.tam.y;

            Xtemp=B.x;
            B.x=B.y+box.tam.x-box.tam.y;
            B.y=-Xtemp+box.tam.x+box.tam.y;

            Xtemp=C.x;
            C.x=C.y+box.tam.x-box.tam.y;
            C.y=-Xtemp+box.tam.x+box.tam.y;

            Xtemp=D.x;
            D.x=D.y+box.tam.x-box.tam.y;
            D.y=-Xtemp+box.tam.x+box.tam.y;
        }
    }
    else if(box.type == 6);
}
void getABCD(gach &box,point &A,point &B,point &C,point &D)
{

    if(box.type == 1)
    {
        A.x = box.tam.x - 1;
        A.y = box.tam.y - 1;
        B.x = box.tam.x;
        B.y = box.tam.y - 1;
        C.x = box.tam.x + 1;
        C.y = box.tam.y - 1;
        D.x = box.tam.x + 1;
        D.y = box.tam.y;

    }
    if(box.type == 2)
    {
        A.x = box.tam.x - 1;
        A.y = box.tam.y + 1;
        B.x = box.tam.x;
        B.y = box.tam.y +1;
        C.x = box.tam.x + 1;
        C.y = box.tam.y + 1;
        D.x = box.tam.x + 1;
        D.y = box.tam.y;

    }
    if(box.type == 3)
    {
        A.x = box.tam.x + 1;
        A.y = box.tam.y - 1;
        B.x = box.tam.x + 1;
        B.y = box.tam.y;
        C.x = box.tam.x ;
        C.y = box.tam.y;
        D.x = box.tam.x;
        D.y = box.tam.y+1;

    }
    if(box.type == 4)
    {
        A.x = box.tam.x;
        A.y = box.tam.y - 1;
        B.x = box.tam.x;
        B.y = box.tam.y;
        C.x = box.tam.x + 1;
        C.y = box.tam.y;
        D.x = box.tam.x + 1;
        D.y = box.tam.y + 1;

    }
    if(box.type == 5)
    {
        A.x = box.tam.x - 1;
        A.y = box.tam.y;
        B.x = box.tam.x;
        B.y = box.tam.y - 1;
        C.x = box.tam.x;
        C.y = box.tam.y;
        D.x = box.tam.x;
        D.y = box.tam.y + 1;

    }
    if(box.type == 6)
    {
        A.x = box.tam.x;
        A.y = box.tam.y;
        B.x = box.tam.x;
        B.y = box.tam.y + 1;
        C.x = box.tam.x + 1;
        C.y = box.tam.y;
        D.x = box.tam.x + 1;
        D.y = box.tam.y + 1;

    }
    if(box.type == 7)
    {
        A.x = box.tam.x - 2;
        A.y = box.tam.y;
        B.x = box.tam.x - 1;
        B.y = box.tam.y ;
        C.x = box.tam.x;
        C.y = box.tam.y;
        D.x = box.tam.x + 1;
        D.y = box.tam.y;
    }
}
void turnLeft(gach &box,point &A,point &B,point &C,point &D,int &b)
{
    if(IsMove(box,A,B,C,D,1))
    {
        b --;
        box.tam.y -= 1;
        A.y -= 1;
        B.y -= 1;
        C.y -= 1;
        D.y -= 1;
    }
    else;
}
void turnRight(gach &box,point &A,point &B,point &C,point &D,int &b)
{
    if(IsMove(box,A,B,C,D,2))
    {
        b ++;
        box.tam.y += 1;
        A.y += 1;
        B.y += 1;
        C.y += 1;
        D.y += 1;
    }
    else;
}
void goDown(gach &box,point &A,point &B,point &C,point &D,int &a)
{
    int v=1;
    a+=v;
    box.tam.x += v;
    A.x += v;
    B.x += v;
    C.x += v;
    D.x += v;
}
void ve_le(SDL_Renderer *renderer,gach &box)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = (SCREEN_WIDTH-10*box.size)/2 ;
    rect.h = SCREEN_HEIGHT;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Rect rect2;
    rect2.x = SCREEN_WIDTH-(SCREEN_WIDTH-10*box.size)/2;
    rect2.y = 0;
    rect2.w = (SCREEN_WIDTH-10*box.size)/2;
    rect2.h = SCREEN_HEIGHT;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect2);
}

int main(int argc, char* argv[])
{


    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit = false;
    initSDL(window, renderer);

    point A,B,C,D;

    gach box;
    int a,b;//toa do tam

    a=1;
    b=20;

    box.get_toa_do_Tam(a,b);
    int le_trai = ((SCREEN_WIDTH-10*box.size)/2)/box.size-1;
    int le_phai = le_trai+11;

    init(le_trai,le_phai);

    SDL_Event e;
    int i=0,sig = 0;
    while(true)
    {
        k=0;
        srand(time(0));

        int random = rand() % 7 + 1;
        box.getType(7);
        box.get_toa_do_Tam(a,b);
        getABCD(box,A,B,C,D);
        while(!quit)
        {

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            goDown(box,A,B,C,D,a);
            ve_le(renderer,box);
            box.render(renderer,A,B,C,D);
            int time_delay = 400;
            SDL_RenderPresent(renderer);

            SDL_Delay(time_delay);

            while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    SDL_Delay(time_delay/6);
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        break;
                    case SDLK_a:
                        turnLeft(box,A,B,C,D,b);
                        break;
                    case SDLK_d:
                        turnRight(box,A,B,C,D,b);
                        break;
                    //case SDLK_s: box.turnDown(); break;
                    case SDLK_w:
                        k++;
                        if(IsMove(box,A,B,C,D,0))
                        {
                            xoay(renderer,box,k,A,B,C,D);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                            SDL_RenderClear(renderer);
                            ve_le(renderer,box);
                            box.render(renderer,A,B,C,D);
                            SDL_RenderPresent(renderer);
                            SDL_Delay(time_delay/5);

                        }
                        break;
                    }
                }
            }
//            cout<<A.x<<":"<<A.y<<endl;
            if(i>=1)
            {
                if(!IsMove(box,A,B,C,D,0))
                {
                    a=0;
                    b=20;
                    //cout<<board_game[A.x+1][A.y]<<","<<board_game[B.x+1][B.y]<<", "<<board_game[C.x+1][C.y]<<","<<board_game[D.x+1][D.y]<<endl;
                    break;
                }
            }
            else
            {
                if(max(A.x,B.x,C.x,D.x)>=29)
                {
                    a=0;
                    b=20;
                    break;
                }
            }

        }

        i++;
        if(i>100)
            i=2;
    }
    quitSDL(window, renderer);
    return 0;
}

