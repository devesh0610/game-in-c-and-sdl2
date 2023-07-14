#include <stdio.h>

#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include<string.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include <stdbool.h>


#define GRAVITY 0.32f
#define NUM_STARS 1
#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2
#define STATUS_STATE_WIN 3
#define manW    65
#define manH    50

const int BULLET_SPEED = 10;                                                        ///////
const int MAX_BULLETS = 3;
int played1=0;
//global
bool isRunning = true;
bool isLeftPressed = false;                                                     /////////
bool isRightPressed = false;
bool isShooting = false;

typedef struct {
    int x,y;                                                                       ////////
    bool active;
    int direction;  // 0 for left, 1 for right

} Bullet;

typedef struct
{

    float x, y,score,saved;
    float dx, dy;
    short lives;
    char *name;
    int onLedge;
    int animFrames;
    int facingLeft;
    int slowingDown;
    int isDead;
    int destroyed;
} Man;

typedef struct
{
    int x,y,baseX,baseY,mode;
    float phase;
    int destroyed;
} Star;

typedef struct
{
    int x, y, w, h;
} Ledge;
typedef struct
{
    int x,y,w,h;
}Lives;

typedef struct
{
    int x,y;
}Flag;

typedef struct
{
    int x,y;

}Trophy;

typedef struct
{
    int x,y;
}Vehicle;


typedef struct
{
     float scrollX;
    // Players
    Man man;

    // Stars
    Star stars[NUM_STARS];

    // Bricks
    Ledge bricks[151];

    //lives
    Lives hearts[5];

    Flag flags[50];

    Bullet* bullets;

    Trophy win;

    Vehicle vehicle;


    int time,deathCountdown;
    int statusState;
    // Images
    SDL_Texture *star;
    SDL_Texture *manFrames[3];
    SDL_Texture *brick;
    SDL_Texture *fire;
    SDL_Texture *live;
    SDL_Texture *label;
    SDL_Texture *label2;
    SDL_Texture *logo;
    SDL_Texture *flagg;
    SDL_Texture *bulletTexture;
    SDL_Texture *trophy;
    SDL_Texture *vehicleTexture;
    TTF_Font *font;
    int labelW,labelH,label2W,label2H;

    //sound
    int musicChannel,musicChannel2;
    Mix_Chunk *bgMusic,*jumpSound,*landSound,*dieSound,*startMusic,*levelSound,*bulletSound;

    SDL_Renderer *renderer;
} GameState;



void loadGame(GameState *game)
{
    SDL_Surface *starSurface;
    SDL_Surface *manSurface;
    SDL_Surface *brickSurface;
    SDL_Surface *man2Surface;
    SDL_Surface *fireSurface;
    SDL_Surface *jumpSurface;
    SDL_Surface *livesSurrface;
    SDL_Surface *flagSurface;
    SDL_Surface *bulletSurface;
    SDL_Surface *trophySurface;
    SDL_Surface *vehicleSurface;
      // Initialize man
    game->man.x =30220;//220
    game->man.y = 100;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrames=0;
    game->man.facingLeft=1;
    game->man.slowingDown=0;
    game->scrollX=0;
    game->time=0;
    game->man.isDead=0;
    game->statusState=STATUS_STATE_GAMEOVER;
    game->label=NULL;
    game->label2=NULL;
    game->man.lives=3;
    game->deathCountdown=-1;
    game->man.score=0;
    game->man.saved=0;
    game->man.destroyed=0;

    init_status_lives(game);
    // Load images and create rendering textures from them
    starSurface = SDL_LoadBMP("star.bmp");
    if (starSurface == NULL)
    {
        printf("Can't find star");
        SDL_Quit();
        exit(1);
    }


        manSurface = SDL_LoadBMP("walk2.bmp");
        if (manSurface == NULL)
        {
        printf("Can't find man");
        SDL_Quit();
        exit(1);
        }


        man2Surface = SDL_LoadBMP("walk3.bmp");
        if (man2Surface == NULL)
        {
        printf("Can't find man2");
        SDL_Quit();
        exit(1);
        }


    brickSurface = SDL_LoadBMP("brick.bmp");
    if (brickSurface == NULL)
    {
        printf("Can't find brick");
        SDL_Quit();
        exit(1);
    }

    fireSurface=SDL_LoadBMP("dead.bmp");
    if(fireSurface==NULL)
    {
        printf("Can't find fire");
        SDL_Quit();
        exit(1);
    }
    jumpSurface=SDL_LoadBMP("jumppp.bmp");
    if(jumpSurface==NULL)
    {
        printf("Can't find jump");
        SDL_Quit();
        exit(1);
    }

    livesSurrface=SDL_LoadBMP("heart.bmp");
    if(livesSurrface==NULL)
    {
        printf("Can't find heart");
        SDL_Quit();
        exit(1);
    }
    flagSurface=SDL_LoadBMP("flag.bmp");
    if(flagSurface==NULL)
    {
        printf("Can't find flag");
        SDL_Quit();
        exit(1);
    }

    bulletSurface = SDL_LoadBMP("b.bmp");                                                      /////////////
    if (bulletSurface == NULL) {
        printf("Failed to load bullet image! SDL Error: %s\n", SDL_GetError());

    }
    trophySurface=SDL_LoadBMP("trophy.bmp");
    if (trophySurface == NULL) {
        printf("Failed to load bullet image! SDL Error: %s\n", SDL_GetError());
    }
    vehicleSurface=SDL_LoadBMP("vehicle.bmp");
    if (vehicleSurface == NULL) {
        printf("Failed to load vehicle image! SDL Error: %s\n", SDL_GetError());
    }



    //LOAD SOUNDS
     game->startMusic = Mix_LoadWAV("start.wav");
    if(game->startMusic!=NULL)
    {
        Mix_VolumeChunk(game->startMusic,3);//decrease audio
    }
    game->bgMusic = Mix_LoadWAV("music.wav");
    if(game->bgMusic!=NULL)
    {
        Mix_VolumeChunk(game->bgMusic,5);//decrease audio
    }
    game->dieSound = Mix_LoadWAV("die.wav");
    game->jumpSound= Mix_LoadWAV("jump2.wav");
    game->landSound = Mix_LoadWAV("land.wav");
    game->levelSound = Mix_LoadWAV("level.wav");
    game->bulletSound = Mix_LoadWAV("bullet.wav");
    //////\

    for (int i = 0; i < 101; i++)
    {
        game->bricks[i].w = 256;
        game->bricks[i].h = 64;
        game->bricks[i].x = 350+rand()%38400;
        if(i%2==0)
            game->bricks[i].y=200;
        else
            game->bricks[i].y=350;



        game->bricks[i].x = i*354;

        if (i ==0) {
                 // Place bricks in a line at the bottom
            game->bricks[i].y = 400;
        }
        else {

   // Place random bricks at the top
            game->bricks[i].y=300+100-rand()%200;
        }
    }

    game->bricks[94].x=10200;
    game->bricks[94].y=200;
    game->bricks[95].x=20200;
    game->bricks[95].y=200;
    game->bricks[96].x=30200;
    game->bricks[96].y=200;

    game->bricks[97].x=5200;
    game->bricks[97].y=200;
    game->bricks[98].x = 200;
    game->bricks[98].y = 200;
    game->bricks[99].x = 350;
    game->bricks[99].y = 200;

    game->bricks[100].x=33900;
    game->bricks[100].y=200;
    // Initialize bricks

    /*for (int i = 0; i < 100; i++)
    {
        game->bricks[i].w = 256;
        game->bricks[i].h = 100;
        game->bricks[i].y = 400;

        if (i < 98) {
                 // Place bricks in a line at the bottom
            game->bricks[i].x = i * 257;
        } else {

   // Place random bricks at the top
            game->bricks[i].x = rand() % 1200;  // Random x coordinate within the window width
            game->bricks[i].y = rand() % 201+50;   // Random y coordinate within the top 200 pixels
            printf("\n%d %d ",game->bricks[i].x,game->bricks[i].y);
        }
    }

    game->bricks[99].x = 350;
    game->bricks[99].y = 200;
*/

 // Initialize stars
   /* for (int i = 0; i < NUM_STARS; i++)
    {
        game->stars[i].x = 320+rand() % 38400;
        game->stars[i].y = rand() % 480;
    }*/
    for(int i=0;i<NUM_STARS;i++)
    {


        game->stars[i].baseX=320+rand()%38400+100;
        game->stars[i].baseY=rand()%200;
        game->stars[i].mode=rand()%2;
        game->stars[i].phase=2*3.14*(rand()%360)/360.0f;
        game->stars[i].destroyed = 0;

    }
    for(int i=0;i<game->man.lives;i++)
    {
        game->hearts[i].x=100+(i*41);
        game->hearts[i].y=30;
        game->hearts[i].h=50;
        game->hearts[i].w=50;
    }
    for(int i=1;i<4;i++)
    {
        game->flags[i].x=(i*10000)+220;
        game->flags[i].y=24;
    }

    game->flags[0].x=200;
    game->flags[0].y=24;
    game->flags[4].x=5220;
    game->flags[4].y=24;


    game->win.x=34020;
    game->win.y=120;

    game->vehicle.x=34020;
    game->vehicle.y=120;



    game->brick = SDL_CreateTextureFromSurface(game->renderer, brickSurface);
    SDL_FreeSurface(brickSurface);

    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, manSurface);
    SDL_FreeSurface(manSurface);

    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, man2Surface);
    SDL_FreeSurface(man2Surface);

    game->manFrames[2]=SDL_CreateTextureFromSurface(game->renderer, jumpSurface);
    SDL_FreeSurface(jumpSurface);

    game->star = SDL_CreateTextureFromSurface(game->renderer, starSurface);
    SDL_FreeSurface(starSurface);

    game->fire=SDL_CreateTextureFromSurface(game->renderer, fireSurface);
    SDL_FreeSurface(fireSurface);

    game->live=SDL_CreateTextureFromSurface(game->renderer, livesSurrface);
    SDL_FreeSurface(livesSurrface);

    game->flagg=SDL_CreateTextureFromSurface(game->renderer, flagSurface);
    SDL_FreeSurface(flagSurface);

    game->bulletTexture = SDL_CreateTextureFromSurface(game->renderer, bulletSurface);
    if (game->bulletTexture == NULL) {
        printf("Failed to create bullet texture! SDL Error: %s\n", SDL_GetError());
    }
    game->trophy = SDL_CreateTextureFromSurface(game->renderer, trophySurface);
    if (game->trophy == NULL) {
        printf("Failed to create bullet texture! SDL Error: %s\n", SDL_GetError());
    }
    game->vehicleTexture = SDL_CreateTextureFromSurface(game->renderer, vehicleSurface);
    if (game->vehicleTexture == NULL) {
        printf("Failed to create bullet texture! SDL Error: %s\n", SDL_GetError());
    }

}
int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                isRunning = false;
                done = 1;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                isRunning = false;
                done = 1;
                break;
            case SDLK_KP_ENTER:
                Mix_HaltChannel(game->musicChannel);
                init_status_lives(game);
                game->statusState=STATUS_STATE_LIVES;

                break;


            case SDLK_UP:
                if (game->man.onLedge == 1)                                                                                 //
                {
                    game->man.dy = -8;
                    game->man.onLedge = 0;
                    Mix_PlayChannel(-1,game->jumpSound,0);
                }
                break;
            case SDLK_SPACE:

                    if(!isShooting&&game->man.destroyed<15)
                        isShooting = true;


                        break;

            }
            break;

        case SDL_QUIT:
            isRunning = false;
            done = 1;
            break;


        }


    }



    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_UP])
      {
         game->man.dy-=0.2f;

      }
      if(!state[SDL_SCANCODE_SPACE])
        {
        isShooting = false;
        }
    if (state[SDL_SCANCODE_LEFT])
    {
        game->man.dx -= 7;
        if(game->man.dx < -8)
        {
            game->man.dx = -8;
        }
        game->man.facingLeft=0;
        game->man.slowingDown=0;
        isLeftPressed = true;
    }
   else if (state[SDL_SCANCODE_RIGHT])
{
    game->man.dx += 0.7;
    if (game->man.dx > 8)
    {
        game->man.dx = 8;
    }
    game->man.facingLeft=1;
    game->man.slowingDown=0;
    isRightPressed = true;
}

else
{
    game->man.animFrames=0;
    game->man.dx *= 0.8f;
    game->man.slowingDown=1;
    if (fabsf(game->man.dx) < 0.1f)
    {
        game->man.dx = 0;
    }
}

   /*  if(state[SDL_SCANCODE_UP])
        {
            game->man.y-=5;
        }
        if(state[SDL_SCANCODE_DOWN])
        {
            game->man.y+=5;
        }
*/






    return done;
}

void doRender(SDL_Renderer *renderer, GameState *game)
{
   // printf("\nstatus:%d",game->statusState);


if(game->statusState==STATUS_STATE_LIVES)
   {



        draw_status_lives(game);

   }
else if(game->statusState==STATUS_STATE_WIN)
{


SDL_Rect vehicleRect = {game->scrollX+game->man.x,game->man.y-30, 120, 70};
SDL_RenderCopy(renderer, game->vehicleTexture, NULL, &vehicleRect);

}
else if(game->statusState==STATUS_STATE_GAME)
{

        SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);




    for (int i = 0; i < 101; i++)
    {
        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brick, NULL, &brickRect);
    }
     for(int i=0;i<5;i++)
    {
    SDL_Rect flagRect={game->scrollX+game->flags[i].x,game->flags[i].y,100,180};                            //location x and y and width height
    SDL_RenderCopy(renderer,game->flagg,NULL,&flagRect);
    }


    if(!game->man.isDead)
    {
    SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, manW, manH};
    SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &rect, 0, NULL,game->man.facingLeft==0);
    }
    else
    {


    SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, 48, 48};
    SDL_RenderCopyEx(renderer,game->fire, NULL, &rect, 0, NULL,(game->time%20<10));

}


    for(int i=0;i<NUM_STARS;i++)
    {
            if(game->stars[i].destroyed!=1)
            {

            SDL_Rect starRect={game->scrollX+game->stars[i].x,game->stars[i].y,64,64};                            //location x and y and width height
            SDL_RenderCopy(renderer,game->star,NULL,&starRect);
            }
            //else
              //  printf("%d ",i);
    }

    for(int i=0;i<game->man.lives;i++)
    {
    SDL_Rect heartRect={game->hearts[i].x,game->hearts[i].y,30,30};                            //location x and y and width height
    SDL_RenderCopy(renderer,game->live,NULL,&heartRect);
    }


    draw_status_game(game);
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {
            SDL_Rect bulletRect = {game->bullets[i].x,game->bullets[i].y, 15, 15};
            SDL_RenderCopy(renderer, game->bulletTexture, NULL, &bulletRect);
        }
    }

     SDL_Rect trophyRect = {game->scrollX+game->win.x,game->win.y, 100, 100};
    SDL_RenderCopy(renderer, game->trophy, NULL, &trophyRect);





}


    SDL_RenderPresent(renderer);
}

int bulletCollide(int a1,int b1,int a2,int b2)
{

    return(!((a1> (a2+64)) || (a2 >(a1+15)) || (b1 >(b2+64)) ||(b2 > (b1+15))));
}


int collide2d(float x1,float y1,float x2,float y2,float wt1,float ht1,float wt2,float ht2)
{
    return(!((x1+12 > (x2+wt2)) || (x2 >(x1+wt1-11)) || (y1+10 >(y2+ht2)) ||(y2+10 > (y1+ht1))));
}

void collisionDetect(GameState *game)
{


    if(game->man.x>=30220&&game->man.x<=30320&&game->man.y>=24&&game->man.y<=180)
    {
        game->man.saved = 30220;
        if(played1==3)
         Mix_PlayChannel(-1,game->levelSound,0);
            played1++;
    }
    else if(game->man.x>=20220&&game->man.x<=20320&&game->man.y>=24&&game->man.y<=180)
    {
        game->man.saved=20220;

        if(played1==2)
         Mix_PlayChannel(-1,game->levelSound,0);
         played1++;
    }
    else if(game->man.x>=10220&&game->man.x<=10320&&game->man.y>=24&&game->man.y<=180)
     {
         game->man.saved=10220;

         if(played1==1)
         Mix_PlayChannel(-1,game->levelSound,0);
        played1++;
     }
    else if(game->man.x>=5200&&game->man.x<=5320&&game->man.y>=24&&game->man.y<=180)
     {
         game->man.saved=5220;
         if(played1==0)
         Mix_PlayChannel(-1,game->levelSound,0);
        played1++;
     }


for(int i=0;i<MAX_BULLETS;i++)
{
        for(int j=0;j<NUM_STARS;j++)
        {
            if(game->stars[i].destroyed!=1&&game->bullets[i].active)
            {

            if(bulletCollide(game->bullets[i].x,game->bullets[i].y,game->scrollX+game->stars[j].x,game->stars[j].y))
            {

                game->stars[j].destroyed=1;
               //printf(" %d",game->stars[j]);

            }
            }

        }
}



for(int i=0;i<NUM_STARS;i++)
{
    if(game->stars[i].destroyed!=1)
    {

        if(game->man.y>600 || collide2d(game->man.x,game->man.y,game->stars[i].x,game->stars[i].y,manW,manH,64,64))
        {
            if(!game->man.isDead)
            {
            game->man.isDead=1;
            game->man.score=(game->man.x-220)/100;
            printf("SCORE:%f\n",game->man.score);
            Mix_HaltChannel(game->musicChannel);
            Mix_PlayChannel(-1,game->dieSound,0);

            }
            break;
        }
    }
}



    for(int i=0;i<101;i++) //why hundred
    {
        float mw = manW, mh = manH;
        float mx = game->man.x, my = game->man.y;

        float bx = game->bricks[i].x, by = game->bricks[i].y+7, bw = game->bricks[i].w, bh = game->bricks[i].h-9;

        if(mx+mw/2>bx && mx+mw/2<bx+bw)
        {
            //are we bumping our head?
            if(my<by+bh && my>by && game->man.dy<0)
            {
                //correct y
                game->man.y=by+bh;
                my=by+bh;
                //bumped,stop any jump velocity
                game->man.dy=0;
                game->man.onLedge=0;
            }
        }
        if(mx+mw>bx && mx<bx+bw)
        {
            //landing?
            if(my+mh>by && my<by && game->man.dy>0)
            {
                //corect y
                game->man.y=by-mh;
                my=by-mh;
                //landed,stop jump velocity
                game->man.dy=0;
                if(!game->man.onLedge)
                {
                    game->man.onLedge=1;
                    Mix_PlayChannel(-1,game->landSound,0);
                }
            }
        }
        if(my+mh>by && my<by+bh)
        {
            //rubbing against right side
            if(mx < bx+bw && mx+mw > bx+bw && game->man.dx<0)
            {
                //correct x
                game->man.x=bx+bw;
                mx=bx+bw;
                game->man.dx=0;
            }
            //rubbing against left side
            else if(mx+mw>bx && mx<bx)
            {
                //correct x
                game->man.x=bx-mw;
                mx=bx-mw;
                game->man.dx=0;
            }
        }
    }
}
void process(GameState *game)
{

    game->time++;

    //printf("%f",game->man.x);




    if (game->statusState == STATUS_STATE_LIVES)
    {

        if (game->time > 490)
        {
            shutdown_status_lives(game);
            game->statusState = STATUS_STATE_GAME;

            Mix_HaltChannel(game->musicChannel2);
           // game->musicChannel = Mix_PlayChannel(-1,game->bgMusic,-1);//channel,music,no.of times -1 means infinite
        }
    }
    else if (game->statusState == STATUS_STATE_GAMEOVER)
    {

        init_game_over(game);
                if(game->time>220)
                {


                game->time=0;                    // Initialize man
                game->man.x = 220;
                game->man.y = 100;
                game->man.dx = 0;
                game->man.dy = 0;
                game->man.onLedge = 0;
                game->man.animFrames = 0;
                game->man.facingLeft = 1;
                game->man.slowingDown = 0;
                game->man.isDead = 0;
                game->man.lives=0;
                game->man.destroyed=0;
                for(int i=0;i<NUM_STARS;i++)
                {
                    game->stars[i].destroyed=0;
                }
                played1=0;
                }
            }
else if (game->statusState == STATUS_STATE_GAME)
{

        Man *man = &game->man;
        if (!man->isDead)
        {
            man->x += man->dx;
            man->y += man->dy;
            man->dy += GRAVITY;

            if (man->dx != 0 && man->onLedge && !man->slowingDown)   //slowing down causes smooth animation when stopping
            {
                if (game->time % 8 == 0)
                {
                    game->man.animFrames = (game->man.animFrames + 1) % 2;
                }
                man->dy += GRAVITY;
            }

    if(man->x > 33995)
        {

        SDL_DestroyTexture(game->manFrames[0]);
        SDL_DestroyTexture(game->manFrames[1]);
        SDL_DestroyTexture(game->manFrames[2]);
        SDL_DestroyTexture(game->trophy);
         SDL_Delay(100);
        }

             if(man->x > 34000)
            {

                init_game_win(game);
               game->statusState = STATUS_STATE_WIN;


            }

//STAR MOVEMENT
for(int i=0;i<NUM_STARS;i++)
{

            if(game->stars[i].destroyed!=1)
            {

                game->stars[i].x=game->stars[i].baseX;
                game->stars[i].y=game->stars[i].baseY;

                if(game->stars[i].mode==0)
                {
                    game->stars[i].x=game->stars[i].baseX+sinf(game->stars[i].phase+game->time*0.06f)*75;
                }
                else
                {
                        game->stars[i].y=game->stars[i].baseY+sinf(game->stars[i].phase+game->time*0.06f)*75;
                }

            }

}



            game->scrollX = -game->man.x + 600;  // **** VERYYYYYYYYYYY IMPORTANTTTTT
            if (game->scrollX > 0)
                game->scrollX = 0;

            if (!man->onLedge)
            {
                game->man.animFrames = 2;
            }
        }

            if (game->man.isDead && game->deathCountdown<0)
            {

                game->deathCountdown=200;
            }

                if (game->man.lives >= 0)
                {
                    game->deathCountdown--;

                }

                      if(game->deathCountdown == 0)
                        {

                        game->man.lives--;
                        if(game->man.lives>=0)
                        {
                        init_status_lives(game);
                        game->statusState = STATUS_STATE_LIVES;

                        game->time = 0;
                        // Reset
                        game->man.isDead = 0;
                        if(game->man.saved==0)
                            game->man.saved=220;
                       game->man.x=game->man.saved;


                        game->man.y = 80;
                        game->man.dx = 0;
                        game->man.dy = 0;
                        game->man.onLedge = 0;
                        }
                        else
                        {
                        init_game_over(game);
                        game->statusState = STATUS_STATE_GAMEOVER;
                        }
                        }

        }



}
void shoot(GameState *game) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!game->bullets[i].active) {
                Mix_PlayChannel(-1,game->bulletSound,0);
            game->bullets[i].active = true;
            game->bullets[i].x = game->scrollX+game->man.x+ (game->man.facingLeft == 0 ? -5 : 40 + 5);
            game->bullets[i].y = game->man.y + 40 / 2;
            game->bullets[i].direction = game->man.facingLeft;
            break;
        }
    }
}



SDL_Color white = {255, 255, 255, 255};

void init_status_lives(GameState *game)
{

      char str[128] = "";
    sprintf(str, " x %d", (int)game->man.lives);

    TTF_Font *font = TTF_OpenFont("font.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    SDL_Surface *tmp = TTF_RenderText_Solid(font,str, white);
    if (!tmp) {
        printf("Failed to render text: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    game->labelW=tmp->w;
    game->labelH=tmp->h;

    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    if (!game->label) {
        printf("Failed to create texture from surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }

    SDL_FreeSurface(tmp);
    TTF_CloseFont(font);


    SDL_Surface *logoSurface= SDL_LoadBMP("logo2.bmp");
    if (logoSurface==NULL) {
        printf("Failed to load image: %s\n", IMG_GetError());
        // Handle the error accordingly
    }

    game->logo = SDL_CreateTextureFromSurface(game->renderer, logoSurface);
    if (!game->logo) {
        printf("Failed to create texture from image surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }

    SDL_FreeSurface(logoSurface);




}

void draw_status_lives(GameState *game)
{

game->musicChannel2=Mix_PlayChannel(-1,game->startMusic,0);
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render the image
    SDL_Rect logoRect = {480, 200,140,120};
    SDL_RenderCopy(renderer, game->logo, NULL, &logoRect);


    SDL_Rect textRect = {600, 250, game->labelW,game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);


   // SDL_Rect rect = {550, 240, 48, 48};
   // SDL_RenderCopyEx(renderer,game->manFrames[0], NULL, &rect, 0, NULL,game->man.facingLeft==0);


}

void draw_status_game(GameState *game)
{
     char str[128] = "";
     game->man.score = ((game->man.x-220)/100);

     if(game->man.score<0)
     {
         game->man.score=0;
     }
    sprintf(str, "Score:%.2f ", game->man.score );



    TTF_Font *font = TTF_OpenFont("font.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    SDL_Surface *tmp = TTF_RenderText_Solid(font,str, white);
    if (!tmp) {
        printf("Failed to render text: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    game->labelW=tmp->w;
    game->labelH=tmp->h;

    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    if (!game->label) {
        printf("Failed to create texture from surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }

    SDL_FreeSurface(tmp);
    TTF_CloseFont(font);



    //draw
    SDL_Renderer *renderer = game->renderer;
    SDL_Rect textRect = {800, 50, game->labelW,game->labelH};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);




//         TTF_Font* font = NULL;

}

void shutdown_status_lives(GameState *game)
{

    SDL_DestroyTexture(game->label);
    game->label = NULL;

}

void init_game_over(GameState *game)
{

game->musicChannel = Mix_PlayChannel(-1,game->bgMusic,-1);
    char str2[228] = "";
    char str3[228]="";
    sprintf(str2, " START ");
    sprintf(str3,"STOP");

    TTF_Font *font = TTF_OpenFont("font.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }
     TTF_Font *font3 = TTF_OpenFont("font.ttf", 17);

    SDL_Surface *tmp2 = TTF_RenderText_Solid(font,str2, white);

    SDL_Surface *tmp3 = TTF_RenderText_Solid(font3,str3, white);
    if (!tmp3) {
        printf("Failed to render text: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    game->labelW=tmp2->w;
    game->labelH=tmp2->h;
    game->label2W=75;
    game->label2H=30;

    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp2);

     game->label2 = SDL_CreateTextureFromSurface(game->renderer, tmp3);
    if (!game->label2) {
        printf("Failed to create texture from surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }


    SDL_FreeSurface(tmp2);
    SDL_FreeSurface(tmp3);
    TTF_CloseFont(font);
    TTF_CloseFont(font3);

    SDL_Surface *logoSurface= SDL_LoadBMP("logo.bmp");
    if (logoSurface==NULL) {
        printf("Failed to load image: %s\n", IMG_GetError());
        // Handle the error accordingly
    }

    game->logo = SDL_CreateTextureFromSurface(game->renderer, logoSurface);
    if (!game->logo) {
        printf("Failed to create texture from image surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }

    SDL_FreeSurface(logoSurface);

    ///
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render the image
    SDL_Rect logoRect = {400, 200,140,200};
    SDL_RenderCopy(renderer, game->logo, NULL, &logoRect);


    SDL_Rect textRect = {600, 250, game->labelW,game->labelH};
    SDL_Rect textRect2 ={620,300,game->label2W,game->label2H};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_RenderCopy(renderer, game->label2, NULL, &textRect2);




  }






void init_game_win(GameState *game)
{
    char str[128] = "";
    sprintf(str, "GAME OVER. Your Score: %d", game->man.score);

    TTF_Font *font = TTF_OpenFont("font.ttf", 30);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    SDL_Surface *tmp = TTF_RenderText_Solid(font, str, white);
    if (!tmp) {
        printf("Failed to render text: %s\n", TTF_GetError());
        // Handle the error accordingly
    }

    game->labelW = tmp->w;
    game->labelH = tmp->h;

    game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
    if (!game->label) {
        printf("Failed to create texture from surface: %s\n", SDL_GetError());
        // Handle the error accordingly
    }

    SDL_FreeSurface(tmp);
    TTF_CloseFont(font);

    // Initialize the man's position
    game->man.x = (1200 - manW) / 2;
    game->man.y = (600 - manH) / 2;

    // Animation loop
    int manVelocity = 1; // Adjust the velocity as needed
    while (game->statusState != STATUS_STATE_WIN) {

        // Update man's position
        game->man.x += manVelocity;


        // Render the man
        SDL_Rect vehicleRect = { game->scrollX + game->man.x, game->man.y, manW, manH };
        SDL_RenderCopy(game->renderer, game->vehicleTexture, NULL, &vehicleRect);

        // Update the screen
        SDL_RenderPresent(game->renderer);

        // Add a small delay to control the animation speed
        SDL_Delay(16); // Adjust the delay as needed
    }
}

updateposition(GameState *game)
{
        for (int i = 0; i < MAX_BULLETS; i++) {
        if (game->bullets[i].active) {

            if (game->bullets[i].x > 0 && game->bullets[i].x < game->scrollX+game->man.x+600)
            {
                if (game->bullets[i].direction == 0)
                {
                    game->bullets[i].x -= BULLET_SPEED;
                }
                else
                    game->bullets[i].x += BULLET_SPEED;
            }
            else {
                game->bullets[i].active = false;
            }
        }
    }

}


login(FILE *fptr)
{
    char username[100],password[100];
    char filename[1000],filepass[1000];
    printf("enter username:");
    scanf("%s",username);
    printf("enter password:");
    scanf("%s",password);

    fptr=fopen("pass.txt","r");
    while(fscanf(fptr,"%s %s",filename,filepass) != EOF)
    {
        if(strcmp(username,filename)==0&&strcmp(password,filepass)==0)
        {
            printf("login succesfull");
            fclose(fptr);
            return 1;
        }

    }
    printf("invalid name or pass");
}

create(FILE *fptr)
{
    char username[100],password[100];
    printf("enter username:");
    scanf("%s",username);
    printf("enter password:");
    scanf("%s",password);
    fptr=fopen("pass.txt","a");
    fprintf(fptr,"%s %s\n",username,password);

}


int main()
{
    int input;
    FILE *fptr;
    printf("\n\t1.Login\n\t2.Create\n\t");
    scanf("%d",&input);
    switch(input)
    {
    case 1:
        login(&fptr);
        break;
    case 2:
        create(&fptr);
        break;
    default:
        printf("invaid");
    }




    GameState gameState;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    srand(time(NULL));

    window = SDL_CreateWindow("Game Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1200,//640
                              600,//480
                              0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    gameState.renderer = renderer;

    TTF_Init();

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);

    loadGame(&gameState);

    int done = 0;
    gameState.bullets = (Bullet*)malloc(MAX_BULLETS * sizeof(Bullet));
    for (int i = 0; i < MAX_BULLETS; i++) {

        gameState.bullets[i].active = false;

    }


    while (!done)
    {
        done = processEvents(window, &gameState);
        process(&gameState);

        updateposition(&gameState);
        if (isShooting&&gameState.man.destroyed<15)
        {
        shoot(&gameState);
        isShooting = false; // Reset the flag
        gameState.man.destroyed++;
        }
        collisionDetect(&gameState);
        doRender(renderer, &gameState);
       // printf("%d",gameState.statusState);
        SDL_Delay(8);
    }


    SDL_DestroyTexture(gameState.star);
    SDL_DestroyTexture(gameState.manFrames[0]);
    SDL_DestroyTexture(gameState.manFrames[1]);
    SDL_DestroyTexture(gameState.manFrames[2]);
    SDL_DestroyTexture(gameState.brick);
    SDL_DestroyTexture(gameState.fire);
    SDL_DestroyTexture(gameState.flagg);

    if(gameState.label != NULL)
        SDL_DestroyTexture(gameState.label);

    TTF_CloseFont(gameState.font);

    Mix_FreeChunk(gameState.bgMusic);
    Mix_FreeChunk(gameState.dieSound);
    Mix_FreeChunk(gameState.jumpSound);
    Mix_FreeChunk(gameState.landSound);
    Mix_FreeChunk(gameState.startMusic);
    Mix_FreeChunk(gameState.levelSound);
    Mix_FreeChunk(gameState.bulletSound);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    SDL_Quit();

    return 0;
}


