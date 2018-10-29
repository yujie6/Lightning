#include "SDL2_header.h"
#include<bits/stdc++.h>

const int Game::SCREEN_WIDTH	= 960;
const int Game::SCREEN_HEIGHT	= 720;
const std::string Game::TitleName = "A Simple Game Demo";

using namespace Game;

std::map<int, bool> keyboard;

std::default_random_engine e;
std::uniform_int_distribution<unsigned>u (50,SCREEN_WIDTH-50); //get random position

PointD posPlayer, velocityPlayer;
PointD posEnemy[10], velocityEnemy;
PointD posBullet[20], velocityBullet, posEnemyBullet[20], velocityEnemyBullet, velocityboss;//original bullet
//the most frequently used one
struct Object{
int width;
int height;
PointD pos;
bool out;};
bool outbullet[20], outenemybullet[20], flag = true; //the situation of origianl bullet
int enemyNumber, imageNumber, i, newplace[10], spacetime = 0, spacebullet, superweapen;
int score = 0, block[10], lives = 3, timeoff = 0, checkpoint, check[3], bossblock;//block is lives of enemy
double speedPlayer;
Object SWplus, liveplus, bulletplus, morebullet[20], enemy1bullet[20], moremorebullet[20], enemy2bullet[20], boom[3], boss, bossbullet[20], bossbullet1[20];
std::string to_string (int val);
bool bulletok;
Image *imagePlayer, *imageBullet, *imageEnemy, *imageboom, *images[100], *imageliveplus,*imageSWplus,*imageBulletplus, *imageenemybullet;
Image *imageboomenemy, *imageboss;
void loadPictures()
{
	imagePlayer = loadImage( "player.png"	);
	imageBullet = loadImage( "bullet.png"	);
	imageEnemy	= loadImage( "player_u.png" );
	imageenemybullet = loadImage( "enemybullet.png");
	imageboom = loadImage( "d3.png" );
	imageliveplus = loadImage( "liveplus.png" );
	imageSWplus = loadImage( "SWplus.png" );
	imageBulletplus = loadImage("bulletplus.png");
	imageBulletplus = loadImage("bulletplus.png");
	imageboomenemy = loadImage( "boomenemy.png" );
	imageboss = loadImage("boss.png");
}

void initialize()
{
	//Display FPS
	FPS_DISPLAY = true;
    e.seed(time(NULL));
	//Initialize vairables
	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT-150 ); //initial position
	posEnemy[1] = PointD( SCREEN_WIDTH/2-200, 0);
	posEnemy[0] = PointD( SCREEN_WIDTH/2, 0);
	posEnemy[2] = PointD( SCREEN_WIDTH/2+200, 0);
	for (i = 0; i< 20; i++) {outbullet[i] = false; outenemybullet[i] = false; enemy1bullet[i].out=false;enemy2bullet[i].out=false;}
	velocityBullet = PointD(0, -7);
	velocityEnemyBullet = PointD(0, 5);
	velocityEnemy = PointD(0, 2);
	velocityboss = PointD(0, 1);
	spacebullet = 0;
	enemyNumber = 1;
	speedPlayer = 5;
	superweapen = 1;
	canvasColor = {255, 206, 235, 255};
    SWplus.out = false;
    liveplus.out = false;
    bulletplus.out = false;
    bulletok = false;
    boss.pos = PointD(SCREEN_WIDTH/2, 0);
    boss.out = false;
    bossblock = 50;
	//Load pictures from files
	loadPictures();
	getImageSize(imageliveplus,liveplus.width,liveplus.height);
	getImageSize(imageSWplus,SWplus.width,SWplus.height);
    getImageSize(imageBulletplus,bulletplus.width,bulletplus.height);
    getImageSize(imageboss,boss.width,boss.height);
}

void drawPlayer()
{
	int w,h;
	getImageSize( imagePlayer, w, h );
	if (lives>=4) drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2 );//change the apperence
	else drawImage(imageboom, posPlayer.x-w/2, posPlayer.y-h/2);
}
void drawBackground()
{
	Rect rect = {70, 50, 80, 90};

	setPenColor((Color){255, 255, 0, 255});

	drawRect( rect, true );
}
void drawHint()
{
    Image *text0 = textToImage( "< Superweapen: >" + std::to_string(superweapen)  , 35, (Color){0, 0, 128, 255} );
	Image *text = textToImage( "< Your score: >" + std::to_string(score)  , 35, (Color){64, 224, 208, 255} );
	Image *text1 = textToImage( "< Your lives : >" + std::to_string(lives) , 35, (Color){64, 224, 208, 255} );
	int w,h;

	getImageSize( text, w, h );
	drawImage( text1, SCREEN_WIDTH/2-w/2-100, SCREEN_HEIGHT-h );
	drawImage( text, SCREEN_WIDTH-w, h);
	drawImage( text0, SCREEN_WIDTH/2+w/2-100, SCREEN_HEIGHT-h );
	cleanup(text,text1,text0);
}
void drawBullet()
{
	int w,h;
	getImageSize( imageBullet, w, h );
	//player's bullet
	for (i = 0; i < 20; i++){
        if(outbullet[i])
        {drawImage( imageBullet, posBullet[i].x-w/2, posBullet[i].y-h);}
    }
    if (bulletok)
    {
        for (i = 0; i < 20; i++){
            if(morebullet[i].out)
            {drawImage( imageBullet, morebullet[i].pos.x-w/2, morebullet[i].pos.y-h);}
        }
    }
    //enemy's bullet
    for (i = 0; i < 20; i++){
	if(outenemybullet[i])
        {drawImage( imageenemybullet, posEnemyBullet[i].x-w/2, posEnemyBullet[i].y-h);}
    }
    for (i = 0; i < 20; i++){
	if(enemy1bullet[i].out)
        {drawImage( imageenemybullet, enemy1bullet[i].pos.x-w/2, enemy1bullet[i].pos.y-h);}
    }
    for (i = 0; i < 20; i++){
	if(enemy2bullet[i].out)
        {drawImage( imageenemybullet, enemy2bullet[i].pos.x-w/2, enemy2bullet[i].pos.y-h);}
    }
    //boss bullet
    if (boss.out)
    {
        for (i = 0; i < 20; i++){
            if (bossbullet[i].out)
            {drawImage( imageenemybullet, bossbullet[i].pos.x-w/2, bossbullet[i].pos.y-h);}
            if (bossbullet1[i].out)
            {drawImage( imageenemybullet, bossbullet1[i].pos.x-w/2, bossbullet1[i].pos.y-h);}
        }
    }
}
void drawEnemy()
{
	int w,h;
	getImageSize( imageEnemy, w, h );
	//get three enemy
	for (i = 0; i < 3; i++){
	drawImage( imageEnemy, posEnemy[i].x-w/2, posEnemy[i].y-h/2 , 1, 1, 180);
	}
	//get the image of boom!
	for (i = 0; i < 3; i++){
	if (boom[i].out){
        drawImage( imageboomenemy, boom[i].pos.x-w/2, boom[i].pos.y-h/2, 0.15, 0.15);
        if (timeoff > check[i] + 30) boom[i].out = false;
                }
	}
	// get boss out
	if (boss.out) {drawImage( imageboss, boss.pos.x-boss.width/2 , boss.pos.y-boss.height/2, 1, 1, 180);}
}
void drawtool()
{
    int w, h;
    //get three special tools
    if (SWplus.out){
    getImageSize( imageSWplus, w, h );
	drawImage( imageSWplus, SWplus.pos.x-w/2, SWplus.pos.y-h/2 , 1, 1);
	}
	if (liveplus.out){
	getImageSize( imageliveplus, w, h );
	drawImage( imageliveplus, liveplus.pos.x-w/2, liveplus.pos.y-h/2 , 1, 1);
	}
	if (bulletplus.out){
	getImageSize( imageBulletplus, w, h );
	drawImage( imageBulletplus, bulletplus.pos.x-w/2, bulletplus.pos.y-h/2 , 1, 1);
	}
}

void draw()
{
	drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	drawHint();
	drawtool();
}
void deal()
{
	bool move = false;
	int w,h;
	getImageSize( imagePlayer, w, h );
	//move
	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,-1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,+1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		velocityPlayer = velocityPlayer + PointD(-1,0)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		velocityPlayer = velocityPlayer + PointD(+1,0)*speedPlayer;
		move = true;
	}
	// get superweapon
	if( keyboard['f'] && superweapen > 0 && timeoff > checkpoint + 120)
	{
        checkpoint = timeoff;
        superweapen--;
        for (i = 0; i < 3;i++) {block[i] = 5;}
        for (i = 0; i < 20; i++)
        {
            outenemybullet[i] = false;
        }
        for (i = 0; i < 20; i++)
        {
            enemy1bullet[i].out = false;
        }
        for (i = 0; i < 20; i++)
        {
            enemy2bullet[i].out = false;
        }
        for (i = 0; i < 20; i++)
        {
            bossbullet[i].out = false;
        }
        for (i = 0; i < 20; i++)
        {
            bossbullet1[i].out = false;
        }
        if(boss.out) bossblock -= 50; //hit boss
	}
	//shoot
	if( keyboard[KEY_SPACE])
	{
        //control ratio by mod15, spacetime is used to check the time passed
        if (spacetime == 0){
            if (bulletok){
                for (int i = 0; i < 20; i++)
                {
                    if (!outbullet[i])
                    {posBullet[i].x = posPlayer.x-w/2;posBullet[i].y = posPlayer.y; outbullet[i] = true; break;}
                }
                for (int i = 0; i < 20; i++)
                {
                    if (!morebullet[i].out)
                    {morebullet[i].pos.x = posPlayer.x+w/2;morebullet[i].pos.y=posPlayer.y; morebullet[i].out = true; break;}
                }
                spacetime = (spacetime+1) % 15;
            }
            else{
                for (int i = 0; i < 20; i++)
                {
                    if (!outbullet[i])
                    {posBullet[i] = posPlayer; outbullet[i] = true; break;}
                }
                spacetime = (spacetime+1) % 15;
            }
        }
        else spacetime = (spacetime+1) % 15;
	}
	//Limit player's speed
	double len = velocityPlayer.length();
	if( len > speedPlayer )
	{
		velocityPlayer = velocityPlayer/len*speedPlayer;
	}
	//Calculate new position
	PointD newposition;
	newposition = posPlayer + velocityPlayer;
	if (newposition.x +w/2 < SCREEN_WIDTH && newposition.x -w/2> 0 && newposition.y+h/2 < SCREEN_HEIGHT && newposition.y-h/2 >0)
        posPlayer = newposition;
    //get bullet
    PointD newposBullet[20];
    //control ratio of enemy's bullet
    if (spacebullet == 0){
            for (int i = 0; i < 20; i++)
            {
                if (!outenemybullet[i])
                {posEnemyBullet[i] = posEnemy[0]; outenemybullet[i] = true; break;}
            }
            for (int i = 0; i < 20; i++)
            {
                if (!enemy1bullet[i].out)
                {enemy1bullet[i].pos = posEnemy[1]; enemy1bullet[i].out = true; break;}
            }
            for (int i = 0; i < 20; i++)
            {
                if (!enemy2bullet[i].out)
                {enemy2bullet[i].pos = posEnemy[2]; enemy2bullet[i].out = true; break;}
            }
            //boss's bullet shoot
            if (boss.out)
            {
                for (int i = 0; i < 20; i++)
                {
                    if (!bossbullet[i].out)
                    {bossbullet[i].pos = boss.pos + PointD(50, 0); bossbullet[i].out = true; break;}
                }
                for (int i = 0; i < 20; i++)
                {
                    if (!bossbullet1[i].out)
                    {bossbullet1[i].pos = boss.pos - PointD(50, 0); bossbullet1[i].out = true; break;}
                }
            }
            spacebullet = (spacebullet+1) % 80;
        }
    else {spacebullet = (spacebullet+1) % 80; }
    //player's bullet move foward
    for (i = 0; i < 20; i++)
    {
        if (outbullet[i]){
        newposBullet[i] = posBullet[i] + velocityBullet;
        if (newposBullet[i].y < 0)
            {outbullet[i] = false;}
        else posBullet[i] = newposBullet[i];}
    }
    //player's extra bullet
    if (bulletok)
    {
        for (i = 0; i < 20; i++)
        {
            if (morebullet[i].out){
                newposBullet[i] = morebullet[i].pos + velocityBullet;
            if (newposBullet[i].y < 0)
            {morebullet[i].out = false;}
            else morebullet[i].pos = newposBullet[i];}
        }
    }
    //enemy's bullet move forward
    for (i = 0; i < 20; i++)
    {
        if (outenemybullet[i]){
            posEnemyBullet[i] = posEnemyBullet[i] + velocityEnemyBullet;
            if (posEnemyBullet[i].y > SCREEN_HEIGHT)
                {outenemybullet[i] = false; posEnemyBullet[i].y = 0;}
            if (posEnemyBullet[i].y > posPlayer.y-h/2&& posEnemyBullet[i].x < posPlayer.x+w/2&& posEnemyBullet[i].x> posPlayer.x-w/2&&posEnemyBullet[i].y < posPlayer.y+h/2)
                {outenemybullet[i] = false; posEnemyBullet[i].y = 0;lives--; bulletok = false; break;}
        }
        if (enemy1bullet[i].out){
            enemy1bullet[i].pos = enemy1bullet[i].pos + velocityEnemyBullet;
            if (enemy1bullet[i].pos.y > SCREEN_HEIGHT)
                {enemy1bullet[i].out = false; enemy1bullet[i].pos.y = 0;}
            if (enemy1bullet[i].pos.y > posPlayer.y-h/2&& enemy1bullet[i].pos.x < posPlayer.x+w/2&& enemy1bullet[i].pos.x> posPlayer.x-w/2&&enemy1bullet[i].pos.y < posPlayer.y+h/2)
                {enemy1bullet[i].out = false; enemy1bullet[i].pos.y = 0;lives--; bulletok = false; break;}
        }
        if (enemy2bullet[i].out){
            enemy2bullet[i].pos = enemy2bullet[i].pos + velocityEnemyBullet;
            if (enemy2bullet[i].pos.y > SCREEN_HEIGHT)
                {enemy2bullet[i].out = false; enemy2bullet[i].pos.y = 0;}
            if (enemy2bullet[i].pos.y > posPlayer.y-h/2&& enemy2bullet[i].pos.x < posPlayer.x+w/2&& enemy2bullet[i].pos.x> posPlayer.x-w/2&&enemy2bullet[i].pos.y <posPlayer.y+h/2)
                {enemy2bullet[i].out = false; enemy2bullet[i].pos.y = 0;lives--; bulletok = false; break;}
        }
    }
    //boss's bullet mov forward
    if (boss.out){
        for (i = 0; i < 20; i++){
        if (bossbullet[i].out){
            bossbullet[i].pos = bossbullet[i].pos + PointD(1, 4);
            if (bossbullet[i].pos.y > SCREEN_HEIGHT)
                {bossbullet[i].out = false; bossbullet[i].pos.y = 0;}
            if (bossbullet[i].pos.y > posPlayer.y-h/2&& bossbullet[i].pos.x < posPlayer.x+w/2&& bossbullet[i].pos.x> posPlayer.x-w/2&&bossbullet[i].pos.y <posPlayer.y+h/2)
                {bossbullet[i].out = false; bossbullet[i].pos.y = 0;lives--; bulletok = false; break;}
        }
        }
        for (i = 0; i < 20; i++){
        if (bossbullet1[i].out){
            bossbullet1[i].pos = bossbullet1[i].pos + PointD(-1, 4);
            if (bossbullet1[i].pos.y > SCREEN_HEIGHT)
                {bossbullet1[i].out = false; bossbullet1[i].pos.y = 0;}
            if (bossbullet1[i].pos.y > posPlayer.y-h/2&& bossbullet1[i].pos.x < posPlayer.x+w/2&& bossbullet1[i].pos.x> posPlayer.x-w/2&&bossbullet1[i].pos.y <posPlayer.y+h/2)
                {bossbullet1[i].out = false; bossbullet1[i].pos.y = 0;lives--; bulletok = false; break;}
        }
        }
    }
    // get enemy shot
    // player's original bullet
    for (i = 0; i < 20; i++)
    {
        if (outbullet[i])
        {
            if (posBullet[i].x < posEnemy[0].x+w/2 &&posBullet[i].x> posEnemy[0].x-w/2 && posBullet[i].y < posEnemy[0].y + h/2&&posBullet[i].y > posEnemy[0].y - h/2)
            {outbullet[i] = false; block[0]++;}
            if (posBullet[i].x < posEnemy[1].x+w/2 &&posBullet[i].x> posEnemy[1].x-w/2 && posBullet[i].y < posEnemy[1].y + h/2&&posBullet[i].y > posEnemy[1].y - h/2)
            {outbullet[i] = false; block[1]++;}
            if (posBullet[i].x < posEnemy[2].x+w/2 &&posBullet[i].x> posEnemy[2].x-w/2 && posBullet[i].y < posEnemy[2].y + h/2&&posBullet[i].y > posEnemy[2].y - h/2)
            {outbullet[i] = false; block[2]++;}
            if(boss.out)
            {
                if(posBullet[i].x<boss.pos.x+boss.width/2&&posBullet[i].x>boss.pos.x-boss.width/2&&posBullet[i].y<boss.pos.y+boss.height/2&&posBullet[i].y>boss.pos.y-boss.height/2)
                {outbullet[i] = false; bossblock--;}
            }
        }
    }
    //player's extra bullet
    if (bulletok)
    {
        for (i = 0; i < 20; i++)
        {
            if (morebullet[i].out)
            {
                if (morebullet[i].pos.x < posEnemy[0].x+w/2 &&morebullet[i].pos.x> posEnemy[0].x-w/2 && morebullet[i].pos.y < posEnemy[0].y + h/2&&morebullet[i].pos.y > posEnemy[0].y - h/2)
                {morebullet[i].out = false; block[0]++;}
                if (morebullet[i].pos.x < posEnemy[1].x+w/2 &&morebullet[i].pos.x> posEnemy[1].x-w/2 && morebullet[i].pos.y < posEnemy[1].y + h/2&&morebullet[i].pos.y > posEnemy[1].y - h/2)
                {morebullet[i].out = false; block[1]++;}
                if (morebullet[i].pos.x < posEnemy[2].x+w/2 &&morebullet[i].pos.x> posEnemy[2].x-w/2 && morebullet[i].pos.y < posEnemy[2].y + h/2&&morebullet[i].pos.y > posEnemy[2].y - h/2)
                {morebullet[i].out = false; block[2]++;}
                if(boss.out)
                {
                    if(morebullet[i].pos.x<boss.pos.x+boss.width/2&&morebullet[i].pos.x>boss.pos.x-boss.width/2&&morebullet[i].pos.y<boss.pos.y+boss.height/2&&morebullet[i].pos.y>boss.pos.y-boss.height/2)
                    {morebullet[i].out = false; bossblock--;}
                }
            }
    }

    }
    //get new enemy
    for (i = 0; i< 3; i++){
    if (posEnemy[i].y > SCREEN_WIDTH)
    {
        posEnemy[i] = PointD(newplace[i], 0);
        block[i] = 0;
	} }
	//determine scores to get tools out
	if(bossblock <= 0){score += 50; boss.out = false; lives++; bossblock = 50;}
	for (i = 0; i < 3; i++){
	if (block[i] <= 4 ) {posEnemy[i] = posEnemy[i] + velocityEnemy;}
    else {
        boom[i].out = true;
        boom[i].pos = posEnemy[i];
        check[i] = timeoff;
        score += 5;
        if (score % 40 == 30)
        {
            SWplus.out = true;
            SWplus.pos = posEnemy[i];
            if (posEnemy[i].y > SCREEN_HEIGHT ) SWplus.pos.y = SCREEN_HEIGHT-SWplus.height/2;
            if (posEnemy[i].y < 0 ) SWplus.pos.y = SWplus.height/2;
        }
        if (score % 40 == 20)
        {
            liveplus.out = true;
            liveplus.pos = posEnemy[i];
        }
        if (score % 30 == 10)
        {
            bulletplus.out = true;
            bulletplus.pos = posEnemy[i];
            if (posEnemy[i].y > SCREEN_HEIGHT ) bulletplus.pos.y = SCREEN_HEIGHT-bulletplus.height/2;
            if (posEnemy[i].y < 0 ) bulletplus.pos.y = bulletplus.height/2;
        }
        posEnemy[i] = PointD(newplace[i], 0);
        block[i] = 0;
    }
    }
    //get boss move forward
    if (boss.out)
    {
        if(posPlayer.x<boss.pos.x+(boss.width)/2&&posPlayer.x>boss.pos.x-(boss.width)/2&&posPlayer.y<boss.pos.y+(boss.height)/2&&posPlayer.y>boss.pos.y-(boss.height)/2)
        {lives--;}
        boss.pos = boss.pos + velocityboss;
    }
    //player get tools
    if (SWplus.out)
    {
        if (fabs(posPlayer.x-SWplus.pos.x)<=(w+SWplus.width)/2&& fabs(posPlayer.y-SWplus.pos.y)<=(h+SWplus.height)/2)
        {
            SWplus.out = false;
            superweapen++;
            score += 5;
        }
    }
    if (liveplus.out)
    {
        if (posPlayer.x<liveplus.pos.x+w/2&&posPlayer.x+w/2>liveplus.pos.x&&posPlayer.y+h/2>liveplus.pos.y&&posPlayer.y<liveplus.pos.y+h/2)
        {
            liveplus.out = false;
            lives++;
            score += 5;
        }
    }
    if (bulletplus.out)
    {
        if (posPlayer.x<bulletplus.pos.x+w/2&&posPlayer.x+w/2>bulletplus.pos.x&&posPlayer.y+h/2>bulletplus.pos.y&&posPlayer.y<bulletplus.pos.y+h/2)
        {
            bulletplus.out = false;
            bulletok = true;
            score += 5;
        }
    }
	//Stop player
	if(!move)
	{
		velocityPlayer = velocityPlayer * 0.8;
		if( velocityPlayer.length() < 0.1 )
			velocityPlayer = PointD();
	}
}

int work( bool &quit )
{
    newplace[0]=u(e); //???
    newplace[1]=u(e);
    newplace[2]=u(e);
    if(lives > 0 && flag){
	deal();

	draw();

	timeoff++;
    }
    //get special event
    else if(timeoff < 7200){
    flag = false;
    cleanup( imagePlayer ,imageEnemy);
	cleanup( imageBullet, imageenemybullet);
	Image *text2 = textToImage( "< You died! Your score is: " + std::to_string(score) + ">", 40, (Color){0, 0, 0, 255} );
	int w,h;
	getImageSize( text2, w, h );
	drawImage( text2, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2 - h/2);
    }
    if (timeoff >= 7200) {
    flag = false;
    cleanup(  imageEnemy ,imagePlayer);
	cleanup( imageBullet, imageenemybullet);
	Image *text2 = textToImage( "< You survive! Your score is: " + std::to_string(score) + ">", 40, (Color){255, 99, 71, 255} );
	int w,h;
	getImageSize( text2, w, h );
	drawImage( text2, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2 - h/2);
    }
    //boss time!
    if (timeoff == 1000) {boss.out = true;}
    if (timeoff == 3000) {boss.out = true; boss.pos = PointD(SCREEN_WIDTH/4, 0); bossblock = 80; velocityboss = PointD(0.3, 0.7);}
    if (timeoff == 6000) {boss.out = true; boss.pos = PointD(3*SCREEN_WIDTH/4, 0); bossblock = 120; velocityboss = PointD(-0.3, 0.5);}
    //quit
	if( keyboard[KEY_ESC] )
		quit = true;
	return 0;
}

void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
	keyboard[keyValue] = true;
}

void keyUp()
{
	keyboard[keyValue] = false;
}

void finale()
{
	//Delete all images
	cleanup( imagePlayer, imageEnemy );
	cleanup( imageBullet, imageenemybullet);
}

