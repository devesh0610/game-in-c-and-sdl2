#include <stdio.h>

#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include <stdbool.h>


#define GRAVITY 0.32f
#define NUM_STARS 100
#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2
#define STATUS_STATE_WIN 3
int manW=65;
#define manH    50
#define BACKGROUND_SCROLL_SPEED 11
#define MAX_SCROLLS 8

const int BULLET_SPEED = 10;                                                        ///////
const int MAX_BULLETS = 3;
int played1=0,played2=0,played3=0,played4=0;
int smoke=0;
int backgroundY = 0;
int scrollCount = 0;
//global
bool isRunning = true;
bool isLeftPressed = false;                                                     /////////
bool isRightPressed = false;
bool isShooting = false;
int shooot=0;
int on=0;
int once=1;

typedef struct {
    int x,y;                                                                       ////////
    bool active;
    int direction;  // 0 for left, 1 for right

} Bullet;
typedef struct {
    int x,y;
}Back;
typedef struct
{

    float x, y,score,saved,past;
    float dx, dy;
    short lives;
    char *name;
    int onLedge;
    int down;
    int animFrames;
    int shootFrames;
    int facingLeft;
    int slowingDown;
    int isDead;
    int destroyed;
    int spawn;
} Man;
typedef struct
{
    int x,y,exist,blasted;
}Blast;
typedef struct
{
    int x,y,baseX,baseY,mode;
    float phase;
    int destroyed;
    Blast blast;
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
    int x,y;
}BG;

typedef struct {
    char username[50], password[50];
    int loginned;
} LOGIN;


typedef struct
{
     float scrollX,past;
    // Players
    Man man;

    // Stars
    Star stars[NUM_STARS];

    // Bricks
    Ledge bricks[151];

    //lives
    Lives hearts[5];

    Flag flags[20];

    Bullet* bullets;

    Trophy win;

    Vehicle vehicle;

    Back back[50];


   // BG bg;

    int time,deathCountdown,blastCountdown;
    int statusState;
    // Images
    SDL_Texture *star;
    SDL_Texture *manFrames[17];

    SDL_Texture *fire;
    SDL_Texture *live;
    SDL_Texture *label;
    SDL_Texture *label2;
    SDL_Texture *logo;
    SDL_Texture *flagg;
    SDL_Texture *bulletTexture;
    SDL_Texture *trophy;
    SDL_Texture *vehicleTexture;
    SDL_Texture *smokeTexture[2];
    SDL_Texture *bgTexture[9];
    SDL_Texture *brickTexture[5];
    SDL_Texture *blastTexture[7];
    SDL_Texture *shootTexture[10];
    SDL_Texture* background ;


    TTF_Font *font;
    int labelW,labelH,label2W,label2H;

    //sound
    int musicChannel,musicChannel2;
    Mix_Chunk *bgMusic,*jumpSound,*landSound,*dieSound,*startMusic,*levelSound,*bulletSound,*destroySound,*walkSound;

    SDL_Renderer *renderer;
} GameState;



void loadGame(GameState *game,FILE *fptr,LOGIN *loginInfo)
{
    SDL_Surface *starSurface;
    SDL_Surface *manSurface;
    SDL_Surface *man2Surface;
    SDL_Surface *man3Surface;
    SDL_Surface *man4Surface;
    SDL_Surface *man5Surface;


    SDL_Surface *man6Surface;
    SDL_Surface *man7Surface;
    SDL_Surface *man8Surface;
    SDL_Surface *man9Surface;
    SDL_Surface *man10Surface;

    SDL_Surface *man11Surface;
    SDL_Surface *man12Surface;
    SDL_Surface *man13Surface;
    SDL_Surface *man14Surface;
    SDL_Surface *man15Surface;


    SDL_Surface *fireSurface;
    SDL_Surface *jumpSurface;
    SDL_Surface *livesSurrface;
    SDL_Surface *flagSurface;
    SDL_Surface *bulletSurface;
    SDL_Surface *trophySurface;
    SDL_Surface *vehicleSurface;
    SDL_Surface *smokeSurface;
    SDL_Surface *smoke2Surface;
    SDL_Surface *brickSurface[5];
    SDL_Surface *bgSurface[9];
    SDL_Surface *blastSurface[7];
    SDL_Surface *shootSurface[10];

      // Initialize man

//file
char name[50],pass[50];
int lives;
int manX;


 if(loginInfo->loginned==1)
{
    game->statusState=STATUS_STATE_GAMEOVER;
    fptr=fopen("pass.txt","r");
    while(fscanf(fptr,"%s %s %d %d",name,pass,&lives,&manX) != EOF)
    {
        if(strcmp(loginInfo->username,name)==0&&strcmp(loginInfo->password,pass)==0&&lives>=0)
        {
            if(manX==999999)
            {
                system("cls");
               // system("color 1F");

            printf("\n\n\n\n\n\n\n\n");
                for(int i=0;i<30;i++)
                {
                     printf("\t\t");
                    for(int j=0;j<50;j++)
                    {

                        if(i==0||i==29||j==0||j==49)
                        printf("* ");
                        else if(i==14&&j==24)
                        printf("                         %s ALREADY WON THE GAME !!! CONGRATULATIONS !!!                      ",loginInfo->username);
                        else if(i!=14)
                        printf("  ");


                    }

                printf("\n");
                }





                 exit(1);
            }
            else
            {
            game->man.x=manX;
            game->man.lives=lives;
            break;
            }
        }
        else if(lives==-1)
        {
        game->man.x=220;
        game->man.lives=3;
        }
    }
    fclose(fptr);
}
else if(loginInfo->loginned==1&&lives>=0)
{
    game->statusState=STATUS_STATE_GAMEOVER;
    game->man.x=220;
    game->man.lives=3;
}
else if(loginInfo->loginned==0)
{


    game->man.x=220;
    game->man.lives=3;
    game->man.y = 100;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrames=0;
    game->man.shootFrames=0;
    game->man.facingLeft=1;
    game->man.slowingDown=0;
    game->scrollX=0;
    game->time=0;
    game->man.isDead=0;
    game->statusState=STATUS_STATE_GAMEOVER;
    game->label=NULL;
    game->label2=NULL;

    game->deathCountdown=-1;
    game->blastCountdown=-1;
    game->man.score=0;
    game->man.saved=0;
    game->man.destroyed=100;
    game->past=400;
    game->past=0;

}


    game->man.y = -5;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrames=0;
    game->man.shootFrames=0;
    game->man.facingLeft=1;
    game->man.slowingDown=0;
    game->scrollX=0;
    game->time=0;
    game->man.isDead=0;
    game->statusState=STATUS_STATE_GAMEOVER;
    game->label=NULL;
    game->label2=NULL;
    game->deathCountdown=-1;
    game->blastCountdown=-1;
    game->man.score=0;
    game->man.saved=0;
    game->man.destroyed=100;
    game->past=400;
    game->past=0;
    game->man.spawn=1;








    for(int i=0;i<NUM_STARS;i++)
    {
    game->stars[i].blast.exist=0;
    game->stars[i].blast.blasted=0;
    }

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
       //manSurface = SDL_LoadBMP("sheet2.bmp");

        if (manSurface == NULL)
        {
        printf("Can't find man");
        SDL_Quit();
        exit(1);
        }

        man2Surface = SDL_LoadBMP("walk3.bmp");
       //man2Surface = SDL_LoadBMP("sheet3.bmp");

        if (man2Surface == NULL)
        {
        printf("Can't find man2");
        SDL_Quit();
        exit(1);
        }


        man3Surface = SDL_LoadBMP("walk.bmp");
        //man3Surface = SDL_LoadBMP("sheet.bmp");

        if (man3Surface == NULL)
        {
        printf("Can't find man3");
        SDL_Quit();
        exit(1);
        }


        man4Surface = SDL_LoadBMP("walkk.bmp");
        //man3Surface = SDL_LoadBMP("sheet.bmp");

        if (man4Surface == NULL)
        {
        printf("Can't find man3");
        SDL_Quit();
        exit(1);
        }

        man5Surface = SDL_LoadBMP("sit.bmp");
        //man3Surface = SDL_LoadBMP("sheet.bmp");

        if (man5Surface == NULL)
        {
        printf("Can't find man3");
        SDL_Quit();
        exit(1);
        }


        man6Surface = SDL_LoadBMP("s1.bmp");
        man7Surface = SDL_LoadBMP("s2.bmp");
        man8Surface = SDL_LoadBMP("s3.bmp");
        man9Surface = SDL_LoadBMP("s4.bmp");
        man10Surface = SDL_LoadBMP("s5.bmp");

        man11Surface = SDL_LoadBMP("s6.bmp");
        man12Surface = SDL_LoadBMP("s7.bmp");
        man13Surface = SDL_LoadBMP("s8.bmp");
        man14Surface = SDL_LoadBMP("s9.bmp");
        man15Surface = SDL_LoadBMP("s10.bmp");





    jumpSurface=SDL_LoadBMP("jump.bmp");
   //jumpSurface=SDL_LoadBMP("sheet4.bmp");

    if(jumpSurface==NULL)
    {
        printf("Can't find jump");
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
    smokeSurface=SDL_LoadBMP("smoke.bmp");
    if (smokeSurface == NULL) {
        printf("Failed to load blast image! SDL Error: %s\n", SDL_GetError());
    }
    smoke2Surface=SDL_LoadBMP("smokee.bmp");
    if (smoke2Surface == NULL) {
        printf("Failed to load blast image! SDL Error: %s\n", SDL_GetError());
    }


    blastSurface[0]=SDL_LoadBMP("exp1.bmp");
    blastSurface[1]=SDL_LoadBMP("exp2.bmp");
    blastSurface[2]=SDL_LoadBMP("exp3.bmp");
    blastSurface[3]=SDL_LoadBMP("exp4.bmp");
    blastSurface[4]=SDL_LoadBMP("exp5.bmp");
    blastSurface[5]=SDL_LoadBMP("exp6.bmp");
    blastSurface[6]=SDL_LoadBMP("exp7.bmp");

    shootSurface[0]=SDL_LoadBMP("mwalk2.bmp");
    shootSurface[1]=SDL_LoadBMP("mwalk.bmp");
    shootSurface[2]=SDL_LoadBMP("mjump.bmp");
    shootSurface[3]=SDL_LoadBMP("m1.bmp");

    shootSurface[4]=SDL_LoadBMP("mm1.bmp");
    shootSurface[5]=SDL_LoadBMP("mm2.bmp");
    shootSurface[6]=SDL_LoadBMP("mm3.bmp");
    shootSurface[7]=SDL_LoadBMP("mm4.bmp");

    //skin
    bgSurface[0]=SDL_LoadBMP("jungle8.bmp");
    //bgSurface=SDL_LoadBMP("volcano.bmp");

    //skin
    bgSurface[1]=SDL_LoadBMP("skyy3.bmp");
    //bgSurface=SDL_LoadBMP("volcano.bmp");

     bgSurface[2]=SDL_LoadBMP("volcano.bmp");
     bgSurface[3]=SDL_LoadBMP("lavaa.bmp");
    bgSurface[4]=SDL_LoadBMP("metalbg.bmp");
    bgSurface[5]=SDL_LoadBMP("jungle5.bmp");
    bgSurface[6]=SDL_LoadBMP("2jungle5.bmp");


    //skin
    brickSurface[0] = SDL_LoadBMP("five.bmp");
    brickSurface[1] = SDL_LoadBMP("brickk2.bmp");
    brickSurface[2] = SDL_LoadBMP("volcanotile.bmp");
    brickSurface[3] = SDL_LoadBMP("metallic.bmp");
    brickSurface[4] = SDL_LoadBMP("black3.bmp");
    // brickSurface = SDL_LoadBMP("volcanotile.bmp");

 // Load background image
    SDL_Surface* backgroundSurface = SDL_LoadBMP("background.bmp");
    game->background = SDL_CreateTextureFromSurface(game->renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);


    //LOAD SOUNDS
     game->startMusic = Mix_LoadWAV("start.wav");
    if(game->startMusic!=NULL)
    {
        Mix_VolumeChunk(game->startMusic,10);//decrease audio
    }
    game->bgMusic = Mix_LoadWAV("music.wav");
    if(game->bgMusic!=NULL)
    {
        Mix_VolumeChunk(game->bgMusic,10);//decrease audio
    }

    game->dieSound = Mix_LoadWAV("die.wav");
    game->jumpSound= Mix_LoadWAV("jump2.wav");
     if(game->jumpSound!=NULL)
    {
        Mix_VolumeChunk(game->jumpSound,10);//decrease audio
    }

    game->landSound = Mix_LoadWAV("land.wav");
    if(game->landSound!=NULL)
    {
     //   Mix_VolumeChunk(game->landSound,10);//decrease audio
    }
    game->levelSound = Mix_LoadWAV("level.wav");
    game->bulletSound = Mix_LoadWAV("bullet.wav");
    if(game->bulletSound!=NULL)
    {
   //     Mix_VolumeChunk(game->bulletSound,10);//decrease audio
    }
    game->destroySound = Mix_LoadWAV("flame.wav");
    if(game->destroySound!=NULL)
    {
        Mix_VolumeChunk(game->destroySound,103);//decrease audio
    }

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
    game->bricks[100].y=400;
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
    game->win.y=320;

    game->vehicle.x=34020;
    game->vehicle.y=120;





    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, manSurface);
    SDL_FreeSurface(manSurface);

    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, man2Surface);
    SDL_FreeSurface(man2Surface);

    game->manFrames[2]=SDL_CreateTextureFromSurface(game->renderer, jumpSurface);
    SDL_FreeSurface(jumpSurface);

    game->manFrames[3] = SDL_CreateTextureFromSurface(game->renderer, man3Surface);
    SDL_FreeSurface(man3Surface);

    game->manFrames[4] = SDL_CreateTextureFromSurface(game->renderer, man4Surface);
    SDL_FreeSurface(man4Surface);

    game->manFrames[5] = SDL_CreateTextureFromSurface(game->renderer, man5Surface);
    SDL_FreeSurface(man5Surface);



    game->manFrames[6] = SDL_CreateTextureFromSurface(game->renderer, man6Surface);
    SDL_FreeSurface(man6Surface);

    game->manFrames[7] = SDL_CreateTextureFromSurface(game->renderer, man7Surface);
    SDL_FreeSurface(man7Surface);

    game->manFrames[8]=SDL_CreateTextureFromSurface(game->renderer, man8Surface);
    SDL_FreeSurface(man8Surface);

    game->manFrames[9] = SDL_CreateTextureFromSurface(game->renderer, man9Surface);
    SDL_FreeSurface(man9Surface);

    game->manFrames[10] = SDL_CreateTextureFromSurface(game->renderer, man10Surface);
    SDL_FreeSurface(man10Surface);



    game->manFrames[11] = SDL_CreateTextureFromSurface(game->renderer, man11Surface);
    SDL_FreeSurface(man11Surface);

    game->manFrames[12]=SDL_CreateTextureFromSurface(game->renderer, man12Surface);
    SDL_FreeSurface(man12Surface);

    game->manFrames[13] = SDL_CreateTextureFromSurface(game->renderer, man13Surface);
    SDL_FreeSurface(man13Surface);

    game->manFrames[14] = SDL_CreateTextureFromSurface(game->renderer, man14Surface);
    SDL_FreeSurface(man14Surface);

    game->manFrames[15] = SDL_CreateTextureFromSurface(game->renderer, man15Surface);
    SDL_FreeSurface(man15Surface);





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
    game->smokeTexture[0]=SDL_CreateTextureFromSurface(game->renderer, smokeSurface);
    SDL_FreeSurface(smokeSurface);

    game->smokeTexture[1]=SDL_CreateTextureFromSurface(game->renderer, smoke2Surface);
    SDL_FreeSurface(smoke2Surface);


    game->blastTexture[0]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[0]);
    SDL_FreeSurface(blastSurface[0]);

    game->blastTexture[1]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[1]);
    SDL_FreeSurface(blastSurface[1]);

    game->blastTexture[2]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[2]);
    SDL_FreeSurface(blastSurface[2]);

    game->blastTexture[3]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[3]);
    SDL_FreeSurface(blastSurface[3]);

    game->blastTexture[4]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[4]);
    SDL_FreeSurface(blastSurface[4]);

    game->blastTexture[5]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[5]);
    SDL_FreeSurface(blastSurface[5]);

    game->blastTexture[6]=SDL_CreateTextureFromSurface(game->renderer,blastSurface[6]);
    SDL_FreeSurface(blastSurface[6]);


    game->shootTexture[0]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[0]);
    SDL_FreeSurface(shootSurface[0]);
    game->shootTexture[1]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[1]);
    SDL_FreeSurface(shootSurface[1]);
    game->shootTexture[2]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[2]);
    SDL_FreeSurface(shootSurface[2]);
    game->shootTexture[3]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[3]);
    SDL_FreeSurface(shootSurface[3]);

    game->shootTexture[4]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[4]);
    SDL_FreeSurface(shootSurface[4]);
    game->shootTexture[5]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[5]);
    SDL_FreeSurface(shootSurface[5]);
    game->shootTexture[6]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[6]);
    SDL_FreeSurface(shootSurface[6]);
    game->shootTexture[7]=SDL_CreateTextureFromSurface(game->renderer,shootSurface[7]);
    SDL_FreeSurface(shootSurface[7]);





    game->bgTexture[0]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[0]);
    SDL_FreeSurface(bgSurface[0]);

    game->bgTexture[1]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[1]);
    SDL_FreeSurface(bgSurface[1]);

     game->bgTexture[2]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[2]);
    SDL_FreeSurface(bgSurface[2]);

    game->bgTexture[3]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[3]);
    SDL_FreeSurface(bgSurface[3]);

     game->bgTexture[4]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[4]);
    SDL_FreeSurface(bgSurface[4]);

    game->bgTexture[5]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[5]);
    SDL_FreeSurface(bgSurface[5]);

     game->bgTexture[6]=SDL_CreateTextureFromSurface(game->renderer, bgSurface[6]);
    SDL_FreeSurface(bgSurface[6]);




    game->brickTexture[0] = SDL_CreateTextureFromSurface(game->renderer, brickSurface[0]);
    SDL_FreeSurface(brickSurface[0]);

    game->brickTexture[1] = SDL_CreateTextureFromSurface(game->renderer, brickSurface[1]);
    SDL_FreeSurface(brickSurface[1]);

    game->brickTexture[2] = SDL_CreateTextureFromSurface(game->renderer, brickSurface[2]);
    SDL_FreeSurface(brickSurface[2]);

    game->brickTexture[3] = SDL_CreateTextureFromSurface(game->renderer, brickSurface[3]);
    SDL_FreeSurface(brickSurface[3]);

    game->brickTexture[4] = SDL_CreateTextureFromSurface(game->renderer, brickSurface[4]);
    SDL_FreeSurface(brickSurface[4]);
}
int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;
bool keyPressed = false;
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
            keyPressed = true;
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
                if(game->man.spawn==0)
                {

                if (game->man.onLedge == 1)                                                                                 //
                {
                    game->man.dy = -8;
                    game->man.onLedge = 0;
                    Mix_PlayChannel(-1,game->jumpSound,0);
                    game->man.down=0;
                }

                }
                break;

            case SDLK_SPACE:
if(game->man.spawn==0)
                {
                    if(!isShooting&&game->man.destroyed>0)
                    {

                        isShooting = true;

                    }
                    game->man.down = 0;
                        break;

            }
            }
            break;



        case SDL_QUIT:
            isRunning = false;
            done = 1;
            break;

        }

    }
    if (!keyPressed)
    {
    game->man.down = 0; // Set game->man.down to 0 if no key was pressed
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

if(game->man.spawn==0)
{

    if(state[SDL_SCANCODE_UP])
      {

         game->man.dy-=0.2f;

      }
    if(state[SDL_SCANCODE_DOWN])
    {
                 if (game->man.onLedge != 1)
                 {

                 if(game->man.dy<4)
                 {

                     game->man.dy += GRAVITY+1;

                 }
                 }
                 else
                 {

                 game->man.down = 1;

                 }
    }
      if(!state[SDL_SCANCODE_SPACE])
        {
        isShooting = false;
        }
        if(state[SDL_SCANCODE_KP_ENTER])
        {


                Mix_HaltChannel(game->musicChannel);
                init_status_lives(game);
                game->statusState=STATUS_STATE_LIVES;

        }
    if (state[SDL_SCANCODE_LEFT])
    {
        game->man.down = 0;
        game->man.dx -= 7;
        if(game->man.dx < -9)
        {
            game->man.dx = -9;
        }
        game->man.facingLeft=0;
        game->man.slowingDown=0;
        isLeftPressed = true;
        Mix_PlayChannel(-1,game->walkSound,0);
    }

   else if (state[SDL_SCANCODE_RIGHT])
    {
    Mix_PlayChannel(-1,game->walkSound,0);
    game->man.down = 0;
    game->man.dx += 7;
    if (game->man.dx > 9)
    {
        game->man.dx = 9;
    }
    game->man.facingLeft=1;
    game->man.slowingDown=0;
    isRightPressed = true;
    }


else
{
    game->man.shootFrames=0;
    game->man.dx *= 0.8f;
    game->man.slowingDown=1;
    if (fabsf(game->man.dx) < 0.1f)
    {
        game->man.dx = 0;

        if(game->time%30==0)
        {

                    if(game->man.animFrames==4)
                    {
                        game->man.animFrames=3;
                    }
                    else
                    {
                        game->man.animFrames=4;
                    }

        }
        game->man.shootFrames=3;
    }
}
    if(state[SDL_SCANCODE_A])
        game->man.facingLeft=0;
    else if(state[SDL_SCANCODE_D])
        game->man.facingLeft=1;




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

if(game->man.x < 0)
    game->man.x=0;
if(game->man.x < game->man.past)
{
    game->man.x=game->man.past;
}








for(int i=0;i<30;i++)
{

    game->back[i].x=i*1200;
    game->back[i].y=0;

    if(i<6)
    {
        if(i%2==0)
        {
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopy(game->renderer,game->bgTexture[0],NULL,&bgRect);
        }
        else
        {
        int a=1;
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopyEx(game->renderer,game->bgTexture[0],NULL,&bgRect,0,NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

        }
    }
    else if(i<12)
    {
        if(i%2==0)
        {


        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopy(game->renderer,game->bgTexture[1],NULL,&bgRect);

        }
        else
        {
        int a=1;
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopyEx(game->renderer, game->bgTexture[1], NULL, &bgRect, 0, NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        }
    }
    else if(i<18)
    {

        if(i%2==0)
        {

        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopy(game->renderer,game->bgTexture[2],NULL,&bgRect);

        SDL_Rect bgRect2={game->scrollX + game->back[i].x,550,1200,50};
        SDL_RenderCopy(game->renderer,game->bgTexture[3],NULL,&bgRect2);
        }
        else
        {
        int a=1;
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopyEx(game->renderer, game->bgTexture[2], NULL, &bgRect, 0, NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

        SDL_Rect bgRect2={game->scrollX + game->back[i].x,550,1200,50};
        SDL_RenderCopyEx(game->renderer, game->bgTexture[3], NULL, &bgRect2, 0, NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);


        }


    }
    else if(i<24)
    {
        if(i%2==0)
        {
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopy(game->renderer,game->bgTexture[4],NULL,&bgRect);
        }
        else
        {
        int a=1;
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopyEx(game->renderer, game->bgTexture[4], NULL, &bgRect, 0, NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);


        }
    }
    else if(i<30)
    {
        if(i%2==0)
        {

        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopy(game->renderer,game->bgTexture[6],NULL,&bgRect);

        }
        else
        {
        int a=1;
        SDL_Rect bgRect={game->scrollX + game->back[i].x,game->back[i].y,1200,600};
        SDL_RenderCopyEx(game->renderer, game->bgTexture[6], NULL, &bgRect, 0, NULL, (a == 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

        }
    }

}




 SDL_Rect srcRect, destRect;
    // Set initial source and destination rectangles
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 1200;
    srcRect.h = 600;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = 1200;
    destRect.h = 600;

     // Scroll the background
        if(scrollCount < MAX_SCROLLS)
        backgroundY -= BACKGROUND_SCROLL_SPEED;

        // If the background has scrolled past the screen, reset its position
        if (backgroundY <= -600) {
            backgroundY = 0;
            scrollCount++;
        }
               // Render the background
        destRect.y = backgroundY;



        SDL_RenderCopy(game->renderer, game->background, &srcRect, &destRect);



        // Render the background again if necessary to fill the screen
        if (backgroundY < 0&&scrollCount<MAX_SCROLLS) {
            destRect.y = backgroundY + 600;
            SDL_RenderCopy(game->renderer, game->background, &srcRect, &destRect);
        }

    if(scrollCount==MAX_SCROLLS)
        SDL_DestroyTexture(game->background);



if(game->time>950||once==0||game->man.isDead==1)
{
    for (int i = 0; i < 101; i++)
    {
    if(game->bricks[i].x<7200)
    {

        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brickTexture[0], NULL, &brickRect);

    }
    else if(game->bricks[i].x<14400)
    {
        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brickTexture[1], NULL, &brickRect);

    }
    else if(game->bricks[i].x<21600)
    {
        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brickTexture[2], NULL, &brickRect);

    }
    else if(game->bricks[i].x<28800)
    {
        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brickTexture[3], NULL, &brickRect);

    }
    else
    {
        SDL_Rect brickRect = {game->scrollX+game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};
        SDL_RenderCopy(renderer, game->brickTexture[4], NULL, &brickRect);

    }

    }


     for(int i=0;i<5;i++)
    {
    SDL_Rect flagRect={game->scrollX+game->flags[i].x,game->flags[i].y,100,180};                            //location x and y and width height
    SDL_RenderCopy(renderer,game->flagg,NULL,&flagRect);
    }

}


    if(!game->man.isDead)
    {

if(shooot==0&&on==0)
        {


         if(game->man.animFrames==4||game->man.animFrames==3)
        {
            manW=55;
        }

        else
             manW=65;

        if(game->man.down==1)
            game->man.animFrames=5;







        SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, manW, manH};
        SDL_Rect rect2 = {game->scrollX+game->man.x, game->man.y, manW-22, manH-15};
        SDL_Rect sitrect = {game->scrollX+game->man.x, game->man.y+15, manW-22, manH-15};

        SDL_Rect rect1 = {220, 150, manW, manH};

        SDL_Rect rect3 = {220, 150, manW-15, manH};
        SDL_Rect rect4 = {220, 150, manW-10, manH+5};

if(once==1)
{

if(game->time<725)
{
    game->man.animFrames=6;
}
else if(game->time<940)
    game->man.animFrames=7;
else if(game->time<950)
    game->man.animFrames=8;
else if(game->time<960)
    game->man.animFrames=9;
else if(game->time<970)
    game->man.animFrames=10;


else if(game->time<975)
    game->man.animFrames=11;
else if(game->time<980)
    game->man.animFrames=12;
else if(game->time<985)
    game->man.animFrames=13;
else if(game->time<990)
    game->man.animFrames=14;
else if(game->time<995)
    game->man.animFrames=15;
else if(game->time<1000)
    game->man.animFrames=14;
else if(game->time<1005)
    game->man.animFrames=13;
else if(game->time<1010)
    game->man.animFrames=12;
else if(game->time<1100)
    game->man.animFrames=11;
else
{
    once=0;
    game->man.spawn=0;
}

}


        if(game->man.animFrames==5)
            SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &sitrect, 0, NULL,game->man.facingLeft==0);

        else if(game->man.animFrames==6||game->man.animFrames==7)
            SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &rect1, 0, NULL,game->man.facingLeft==0);

        else if(game->man.animFrames==8||game->man.animFrames==9||game->man.animFrames==10)
            SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &rect3, 0, NULL,game->man.facingLeft==0);

        else if(game->man.animFrames==11||game->man.animFrames==12||game->man.animFrames==13||game->man.animFrames==14||game->man.animFrames==15)
        SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &rect4, 0, NULL,game->man.facingLeft==0);

        else
            SDL_RenderCopyEx(renderer,game->manFrames[game->man.animFrames], NULL, &rect, 0, NULL,game->man.facingLeft==0);
        }
else if(shooot==1)
{
    if(game->man.shootFrames==0)
    {
//mm2

            if(on%7==0)
            {
            game->man.shootFrames=5;
            }
            if(on%17==0)
            {
            game->man.shootFrames=1;
            }
            else
                 game->man.shootFrames=0;



            SDL_Rect shootRect = {game->scrollX+game->man.x+7, game->man.y-5, manW-15, manH+8};
            SDL_RenderCopyEx(renderer,game->shootTexture[game->man.shootFrames], NULL, &shootRect, 0, NULL,game->man.facingLeft==0);
            shooot=1;


        }
            else if(game->man.shootFrames==1)
            {
//mm3
            if(on%7==0)
            {
            game->man.shootFrames=6;
            }
            if(on%17==0)
            {
            game->man.shootFrames=0;
            }
            else
                 game->man.shootFrames=1;

            SDL_Rect shootRect = {game->scrollX+game->man.x+7, game->man.y-5, manW-15, manH+8};
           SDL_RenderCopyEx(renderer,game->shootTexture[game->man.shootFrames], NULL, &shootRect, 0, NULL,game->man.facingLeft==0);
            shooot=1;
            }
            else if(game->man.shootFrames==2)
            {
//mm4
            if(on%7==0)
            {
            game->man.shootFrames=7;
            }

            SDL_Rect shootRect = {game->scrollX+game->man.x+7, game->man.y-5, manW-15, manH+8};
            SDL_RenderCopyEx(renderer,game->shootTexture[game->man.shootFrames], NULL, &shootRect, 0, NULL,game->man.facingLeft==0);
            shooot=1;
            }
            else if(game->man.shootFrames==3)
            {
//mm1
            if(on%7==0)
            {
            game->man.shootFrames=4;
            }


            SDL_Rect shootRect = {game->scrollX+game->man.x+5, game->man.y-6, manW-3, manH+5};
            SDL_RenderCopyEx(renderer,game->shootTexture[game->man.shootFrames], NULL, &shootRect, 0, NULL,game->man.facingLeft==0);
            shooot=1;
            }
}


    }
    else
    {
    SDL_Rect rect = {game->scrollX+game->man.x, game->man.y, 55, 55};
    SDL_RenderCopyEx(renderer,game->fire, NULL, &rect, 0, NULL,(game->time%20<10));
    }

if(game->time>950||once==0||game->man.isDead==1)
{


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
            SDL_Rect bulletRect = {game->bullets[i].x,game->bullets[i].y, 14, 14};
            SDL_RenderCopy(renderer, game->bulletTexture, NULL, &bulletRect);

            shooot=1;

        }
    }

     SDL_Rect trophyRect = {game->scrollX+game->win.x,game->win.y, 100, 100};
    SDL_RenderCopy(renderer, game->trophy, NULL, &trophyRect);

for(int i=0;i<NUM_STARS;i++)
{

    if(game->stars[i].blast.exist==1)
    {
        game->stars[i].blast.blasted++;

/////////////////////////////////////////


        if(game->stars[i].blast.blasted<=6&& game->stars[i].blast.blasted>0)
        {
        SDL_Rect blastRect1={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[0],NULL,&blastRect1);
        }

        else if(game->stars[i].blast.blasted<=12&& game->stars[i].blast.blasted>6)
        {
        SDL_Rect blastRect2={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[1],NULL,&blastRect2);
        }

        else if(game->stars[i].blast.blasted<=18&& game->stars[i].blast.blasted>12)
        {
        SDL_Rect blastRect3={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[2],NULL,&blastRect3);
        }
        else if(game->stars[i].blast.blasted<=24&& game->stars[i].blast.blasted>18)
        {
        SDL_Rect blastRect4={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[3],NULL,&blastRect4);
        }
        else if(game->stars[i].blast.blasted<=30&& game->stars[i].blast.blasted>24)
        {
        SDL_Rect blastRect5={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[4],NULL,&blastRect5);
        }
        else if(game->stars[i].blast.blasted<=36&& game->stars[i].blast.blasted>30)
        {
        SDL_Rect blastRect6={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[5],NULL,&blastRect6);
        }
        else if(game->stars[i].blast.blasted<=42&& game->stars[i].blast.blasted>36)
        {
        SDL_Rect blastRect7={game->scrollX + game->stars[i].blast.x,game->stars[i].blast.y,60,60};
        SDL_RenderCopy(game->renderer,game->blastTexture[6],NULL,&blastRect7);
        }


/////////////////////////////////////////


        if(game->stars[i].blast.blasted>70)
        {
            game->stars[i].blast.exist=0;
            game->stars[i].blast.blasted=0;
        }
    }
}



if(shooot==1)
{
    on++;
   // printf("%d",on);
    if(on==25)
    {

        shooot=0;
    on=0;
    }
}

}




}


    SDL_RenderPresent(renderer);
}

int bulletCollide(int a1,int b1,int a2,int b2)
{

    return(!((a1> (a2+64)) || (a2 >(a1+15)) || (b1 >(b2+64)) ||(b2 > (b1+15))));
}


int collide2d(float x1,float y1,float x2,float y2,float wt1,float ht1,float wt2,float ht2)
{
    return(!((x1+12 > (x2+wt2)) || (x2 >(x1+wt1-21)) || (y1+10 >(y2+ht2)) ||(y2+10 > (y1+ht1))));

}

void collisionDetect(GameState *game,FILE *fptr,LOGIN *loginInfo)
{
    if(game->man.x>=30220&&game->man.x<=30320&&game->man.y>=24&&game->man.y<=180)
    {
        game->man.saved = 30220;
        if(played4==0)
         Mix_PlayChannel(-1,game->levelSound,0);
            played4++;

            //file
             int checkpoint=30220;
             if(loginInfo->loginned==1)
            saveCheckpoint(fptr,loginInfo,checkpoint);

    }
    else if(game->man.x>=20220&&game->man.x<=20320&&game->man.y>=24&&game->man.y<=180)
    {
        game->man.saved=20220;

        if(played3==0)
         Mix_PlayChannel(-1,game->levelSound,0);
         played3++;
            //file
        int checkpoint=20220;
        if(loginInfo->loginned==1)
        saveCheckpoint(fptr,loginInfo,checkpoint);

    }
    else if(game->man.x>=10220&&game->man.x<=10320&&game->man.y>=24&&game->man.y<=180)
     {
         game->man.saved=10220;
         if(game->man.destroyed<10)
         {
            game->man.destroyed=10;
         }
         if(played2==0)
         Mix_PlayChannel(-1,game->levelSound,0);
        played2++;


            //file

        int checkpoint=10220;
        if(loginInfo->loginned==1)
        saveCheckpoint(fptr,loginInfo,checkpoint);
     }
    else if(game->man.x>=5200&&game->man.x<=5320&&game->man.y>=24&&game->man.y<=180)
     {
         game->man.saved=5220;
         if(played1==0)
         Mix_PlayChannel(-1,game->levelSound,0);
        played1++;

        int checkpoint=5220;
        if(loginInfo->loginned==1)
        saveCheckpoint(fptr,loginInfo,checkpoint);


     }


for(int i=0;i<MAX_BULLETS;i++)
{
        for(int j=0;j<NUM_STARS;j++)
        {
            if(game->stars[j].destroyed!=1&&game->bullets[i].active)
            {

            if(bulletCollide(game->bullets[i].x,game->bullets[i].y,game->scrollX+game->stars[j].x,game->stars[j].y))
            {
                game->stars[j].destroyed=1;
                game->stars[j].blast.exist=1;
                game->blastCountdown=-1;
                game->stars[j].blast.x=game->stars[j].x;
                game->stars[j].blast.y=game->stars[j].y;


                Mix_PlayChannel(-1,game->destroySound,0);
            }
            }

        }
}



for(int i=0;i<NUM_STARS;i++)
{


    if(game->stars[i].destroyed!=1)
    {

        if((game->man.y>600) || collide2d(game->man.x,game->man.y,game->stars[i].x,game->stars[i].y,manW,manH,64,64)||game->man.x>=14400&&game->man.x<=21600&&game->man.y>500)
        {
            if(!game->man.isDead&&game->man.spawn==0)
            {
            game->man.isDead=1;
            game->man.score=(game->man.x-220)/100;
            game->man.past=0;
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
void process(GameState *game,FILE *fptr,LOGIN *loginInfo)
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
                game->man.shootFrames = 0;
                game->man.facingLeft = 1;
                game->man.slowingDown = 0;
                game->man.isDead = 0;
                game->man.lives=3;
                game->man.destroyed=100;
                game->scrollX=0;
                game->past=400;
                for(int i=0;i<NUM_STARS;i++)
                {
                    game->stars[i].destroyed=0;
                }
                played1=0;
                played2=0;
                played3=0;
                played4=0;
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
                    game->man.shootFrames = (game->man.shootFrames + 1) % 2;
                }
                man->dy += GRAVITY;
            }

    if(man->x > 33995)
        {

        SDL_DestroyTexture(game->manFrames[0]);
        SDL_DestroyTexture(game->manFrames[1]);
        SDL_DestroyTexture(game->manFrames[2]);
        SDL_DestroyTexture(game->manFrames[3]);

        SDL_DestroyTexture(game->shootTexture[0]);
        SDL_DestroyTexture(game->shootTexture[1]);
        SDL_DestroyTexture(game->shootTexture[2]);
        SDL_DestroyTexture(game->shootTexture[3]);
         SDL_DestroyTexture(game->shootTexture[4]);
    SDL_DestroyTexture(game->shootTexture[5]);
    SDL_DestroyTexture(game->shootTexture[6]);
    SDL_DestroyTexture(game->shootTexture[7]);
        SDL_DestroyTexture(game->trophy);

        }

             if(man->x > 34000 &&(man->y<550))
            {
                int checkpoint=999999;
                saveCheckpoint(fptr,loginInfo,checkpoint);


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


if(game->man.facingLeft)
    {

            game->scrollX = -game->man.x + 600;  // **** VERYYYYYYYYYYY IMPORTANTTTTT
            if(game->scrollX < game->past)
            {
                game->past=game->scrollX;
                game->man.past=game->man.x-600;
            }
            else
            {
                game->scrollX=game->past;

            }

            if (game->scrollX > 0)
                game->scrollX = 0;
    }
            if (!man->onLedge)
            {
                game->man.animFrames = 2;
                game->man.shootFrames = 2;
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
                        //file
                        printf("%d",game->man.lives);


                        if(loginInfo->loginned==1)
                        saveLives(fptr,loginInfo,game);


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
                        game->scrollX=0;
                        game->past=400;
                        for(int i=0;i<NUM_STARS;i++)
                        {
                            game->stars[i].blast.exist=0;
                        }
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
        if (!game->bullets[i].active&&game->man.isDead==0) {
                Mix_PlayChannel(-1,game->bulletSound,0);
            game->bullets[i].active = true;
            game->man.destroyed--;
            game->bullets[i].x = game->scrollX+game->man.x+ (game->man.facingLeft == 0 ? -5 : 40 + 5);
            game->bullets[i].y = game->man.y + 15;
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





 if(game->man.saved == 5220 || game->man.saved==10220|| game->man.saved==20220||game->man.saved == 30220)
 {
     game->scrollX= -game->man.x+600;
     game->past=game->scrollX + 50;
 }




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
    sprintf(str, "Score:%.2f   Bullets:%d", game->man.score,game->man.destroyed );



    TTF_Font *font = TTF_OpenFont("font.ttf", 25);
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
    SDL_Rect textRect = {700, 35, game->labelW,game->labelH};
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






void init_game_win(GameState *game) {





    SDL_Surface *cloudSurface = SDL_LoadBMP("cloud.bmp");
if (!cloudSurface) {
    printf("Failed to load cloud image: %s\n", SDL_GetError());
    // Handle the error accordingly
}



SDL_Texture *cloudTexture = SDL_CreateTextureFromSurface(game->renderer, cloudSurface);
if (!cloudTexture) {
    printf("Failed to create texture from cloud surface: %s\n", SDL_GetError());
    // Handle the error accordingly
}
SDL_FreeSurface(cloudSurface); // We don't need the surface anymore


int cloudX = 0;
int cloudY = rand() % 50; // Adjust the range as needed
int cloudVelocity = 45; // Adjust the velocity as needed


    int scrollX = 0;

    // Animation loop
    int manVelocity =10; // Adjust the velocity as needed
    SDL_Event event;
    while (game->statusState != STATUS_STATE_WIN) {
        // Process events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                // Handle quit event and exit the loop
                game->statusState = STATUS_STATE_WIN;
                break;
            }

            // Handle other events as needed
        }

        // Update man's position
        game->man.x += manVelocity;
game->scrollX = -game->man.x + 600;  // **** VERYYYYYYYYYYY IMPORTANTTTTT
            if (game->scrollX > 0)
                game->scrollX = 0;


        // Render the man
        SDL_Rect vehicleRect = { game->scrollX + game->man.x, game->man.y - 60, 120, 80 };
        SDL_RenderCopy(game->renderer, game->vehicleTexture, NULL, &vehicleRect);

         cloudX -= cloudVelocity;

    // If the cloud goes off the screen, reset its position to the right side
    if (cloudX <= -100) {
        cloudX = 1200;
        cloudY = rand() % (600/ 2); // Adjust the range as needed
    }

    // Render the cloud
    SDL_Rect cloudRect = { cloudX, cloudY, 100,50 };
    SDL_RenderCopy(game->renderer, cloudTexture, NULL, &cloudRect);


    if(smoke==1)
    {

    SDL_Rect smokeRect = {game->scrollX+game->man.x-40,game->man.y-22, 50, 50};
    SDL_RenderCopy(game->renderer, game->smokeTexture[0], NULL, &smokeRect);
    smoke=0;
    SDL_Delay(10);
    }

    else if(smoke==0)
    {

    SDL_Rect smoke2Rect = {game->scrollX+game->man.x-20,game->man.y-22, 50, 50};
    SDL_RenderCopy(game->renderer, game->smokeTexture[1], NULL, &smoke2Rect);
    smoke=1;
    SDL_Delay(10);
    }



        // Update the screen
        SDL_RenderPresent(game->renderer);

        SDL_RenderClear(game->renderer);



       if(game->man.x>35500)
         SDL_SetRenderDrawColor(game->renderer,4, 4, 31, 255);
       else if(game->man.x>35400)
         SDL_SetRenderDrawColor(game->renderer,7, 7, 46, 255);
        else if(game->man.x>35300)
        SDL_SetRenderDrawColor(game->renderer,15, 15, 74, 255);
        else if(game->man.x>35200)
        SDL_SetRenderDrawColor(game->renderer,15, 15, 74, 255);
         else if(game->man.x>35100)
        SDL_SetRenderDrawColor(game->renderer,21, 21, 89, 255);
         else if(game->man.x>35000)
        SDL_SetRenderDrawColor(game->renderer,27, 27, 105, 255);
        else if(game->man.x>34900)
        SDL_SetRenderDrawColor(game->renderer,33, 33, 117, 255);
       else if(game->man.x>34800)
        SDL_SetRenderDrawColor(game->renderer,45, 45, 135, 255);
        else if(game->man.x>34700)
        SDL_SetRenderDrawColor(game->renderer,58, 58, 156, 255);
       else if(game->man.x>34600)
        SDL_SetRenderDrawColor(game->renderer,71, 71, 179, 255);
        else if(game->man.x>34500)
            SDL_SetRenderDrawColor(game->renderer,83, 83, 189, 255);
         else if(game->man.x>34400)
            SDL_SetRenderDrawColor(game->renderer,88, 88, 209, 255);
          else if(game->man.x>34300)
                 SDL_SetRenderDrawColor(game->renderer, 95, 95, 217, 255);
          else if(game->man.x>34200)
                 SDL_SetRenderDrawColor(game->renderer, 101, 101, 230, 255);
        else if(game->man.x>34100)
                 SDL_SetRenderDrawColor(game->renderer, 122, 122, 245, 255);
        else if(game->man.x>34000)
              SDL_SetRenderDrawColor(game->renderer, 128, 128, 255, 255);










        SDL_RenderClear(game->renderer);
        // Add a small delay to control the animation speed
        SDL_Delay(10); // Adjust the delay as needed
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



int login(FILE *fptr, LOGIN *loginInfo) {
    char username[100], password[100];
    char filename[1000], filepass[1000];
    int flag = 0, i = 0;
    int filelives, filesaved;


    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    fptr = fopen("pass.txt", "r");
    if (fptr == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    while (fscanf(fptr, "%s %s %d %d", filename, filepass, &filelives, &filesaved) != EOF) {
        if (strcmp(username, filename) == 0 && strcmp(password, filepass) == 0) {



            strncpy(loginInfo->username, username, sizeof(loginInfo->username));
            strncpy(loginInfo->password, password, sizeof(loginInfo->password));

            printf("Login successful. %s %s\n",loginInfo->username,loginInfo->password);
            loginInfo->loginned=1;


            fclose(fptr);
            flag = 1;
            break;
        }
    }

    if (flag != 1) {
        printf("Invalid username or password.\n");
        exit(1);
    }
    fclose(fptr);
    return flag;
}

void create(FILE *fptr,LOGIN *loginInfo) {
    char username[100], password[100];
    char filename[1000], filepass[1000];
    int filelives, filesaved;



    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    fptr=fopen("pass.txt","r");
    while (fscanf(fptr, "%s %s %d %d", filename, filepass, &filelives, &filesaved) != EOF)
    {
        if (strcmp(username, filename) == 0 )
        {
                printf("username already taken");
                exit(1);
        }
    }
    fclose(fptr);


    strncpy(loginInfo->username,username,sizeof(loginInfo->username));
    strncpy(loginInfo->password, password, sizeof(loginInfo->password));
    loginInfo->loginned=1;

    fptr = fopen("pass.txt", "a");
    fprintf(fptr, "%s %s 3 220\n", username, password);
    fclose(fptr);
}
void saveLives(FILE *fptr, LOGIN *loginInfo, GameState *game) {
    int newNumber, filesaved;
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temp.txt");
        return;
    }

    fptr = fopen("pass.txt", "r");
    if (fptr == NULL) {
        perror("Error opening pass.txt");
        fclose(tempFile);
        return;
    }

    char fileUsername[100], filePassword[100];
    int fileNumber;
    int found = 0;

    while (fscanf(fptr, "%s %s %d %d", fileUsername, filePassword, &fileNumber, &filesaved) != EOF) {

        if (strcmp(loginInfo->username, fileUsername) == 0 && strcmp(loginInfo->password, filePassword) == 0) {
            fprintf(tempFile, "%s %s %d %d\n", fileUsername, filePassword, game->man.lives, filesaved);
            found = 1;
        } else {
            fprintf(tempFile, "%s %s %d %d\n", fileUsername, filePassword, fileNumber, filesaved);
        }
    }


    fclose(fptr);
    fclose(tempFile);

    if (remove("pass.txt") != 0) {
        perror("Error deleting pass.txt");
        return;
    }

    if (rename("temp.txt", "pass.txt") != 0) {
        perror("Error renaming temp.txt");
        return;
    }
}

void saveCheckpoint(FILE *fptr, LOGIN *loginInfo,int checkpoint) {
    int newNumber,filelives;


    FILE *tempFile = fopen("temp.txt", "w");
    fptr = fopen("pass.txt", "r");
    char fileUsername[100], filePassword[100];
    int fileNumber;
    int found = 0;

    while (fscanf(fptr, "%s %s %d %d", fileUsername, filePassword, &filelives,&fileNumber) != EOF) {
        if (strcmp(loginInfo->username, fileUsername) == 0 && strcmp(loginInfo->password, filePassword) == 0) {
            fprintf(tempFile, "%s %s %d %d\n", fileUsername, filePassword,filelives ,checkpoint);
            found = 1;
        } else {
            fprintf(tempFile, "%s %s %d %d\n", fileUsername, filePassword,filelives ,fileNumber);
        }
    }



    fclose(fptr);
    fclose(tempFile);

    remove("pass.txt");
    rename("temp.txt", "pass.txt");
}


int main()
{
    int input, input2;
    FILE *fptr;
    LOGIN loginInfo;
    loginInfo.loginned=0;




    system("color 1F");
    printf("\n\t1. Login\n\t2. Create\n\t3.Guest\n");
    printf("Enter your choice: ");
    scanf("%d", &input);

    switch (input) {
        case 1:
            login(fptr, &loginInfo);
            break;
        case 2:
            create(fptr,&loginInfo);
            break;
        case 3:
            loginInfo.loginned=0;
            break;
        default:
            printf("Invalid choice\n");
            break;
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

    loadGame(&gameState,&fptr,&loginInfo);

    int done = 0;
    gameState.bullets = (Bullet*)malloc(MAX_BULLETS * sizeof(Bullet));
    for (int i = 0; i < MAX_BULLETS; i++) {

        gameState.bullets[i].active = false;

    }


    while (!done)
    {


        done = processEvents(window, &gameState);
        process(&gameState,&fptr,&loginInfo);

        updateposition(&gameState);
        if (isShooting&&gameState.man.destroyed>0)
        {
        shoot(&gameState);
        isShooting = false; // Reset the flag
        }
        collisionDetect(&gameState,&fptr,&loginInfo);
        doRender(renderer, &gameState);
       // printf("%d",gameState.statusState);
        SDL_Delay(6);
    }



    SDL_DestroyTexture(gameState.star);
    SDL_DestroyTexture(gameState.manFrames[0]);
    SDL_DestroyTexture(gameState.manFrames[1]);
    SDL_DestroyTexture(gameState.manFrames[2]);
    SDL_DestroyTexture(gameState.manFrames[3]);

     SDL_DestroyTexture(gameState.shootTexture[0]);
    SDL_DestroyTexture(gameState.shootTexture[1]);
    SDL_DestroyTexture(gameState.shootTexture[2]);
    SDL_DestroyTexture(gameState.shootTexture[3]);
     SDL_DestroyTexture(gameState.shootTexture[4]);
    SDL_DestroyTexture(gameState.shootTexture[5]);
    SDL_DestroyTexture(gameState.shootTexture[6]);
    SDL_DestroyTexture(gameState.shootTexture[7]);
    SDL_DestroyTexture(gameState.brickTexture[0]);
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
    Mix_FreeChunk(gameState.destroySound);


    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    SDL_Quit();

    return 0;
}


