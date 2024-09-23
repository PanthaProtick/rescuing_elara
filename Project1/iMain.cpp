#include "iGraphics.h"
#include <stdio.h>
#include <errno.h>


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void homepage();
int hmpage;
int l3back;
int winbk;
int sty;
int styimage;
void story();

int inst;
int instimage;
void instruction();

int abtus;
int abtusimage;
void aboutus();

bool musicOn = true;
int donkey, tile, ladderPic, board, heart;
const int totalPlats = 5, totalLadders = 3, totalPlats2=5, totalLadders2= 3;
int barrelI = 0, jumpI = 0, runI = 0, idleI = 0, takeHitI = 0, barrelTimer = 350, jumpTimer = 350, runTimer = 350, idleTimer = 350, takeHitTimer = 250;
int lvl;

int health = 8;

int villainhealth = 5;

int heroX = -150, heroY = 100;

int obstacleX = 1200, obstacleY = 100;
int barrelImages[4];
int obstacleSpeed = 2;
int currentBarrelImageIndex = 0;

int gameover = 0;
int timer = 0;

int idleImage;
int ridleImage;


int isJumping = 0;
int isJumpingimg = 0;
double jumpHeight = 0.0;
double gravity = 0.1;  // Lower gravity for slower descent
double jumpSpeed = 0.1;  // Lower jump speed for slower ascent
double maxJumpHeight = 250.0; // Maximum jump height
int jumping[20];


int runImages[8];
int rrunImages[8];
int background3;
int currentRunImageIndex = 0;
int isWalking = 0; // Indicates if the character is walking
int isMoving = 0; // Indicates if the character is moving
int rntimer = 0;

int leftorright = 1;//0 means left 1 means right

int sword[11];
int rsword[11];

int isAttack = 0;

int showExplosion = 0;
int explosionTimer = 0;
int explosionImage;

int rfireballImage;
int fireballImage;
int showFireExplosion = 0;
int FireExplosionTimer = 0;
int fireballX = 1000, fireballY = 180;

int win = 0;

void level3();
void level4();
void winoutro();
int villainimgright[2];
int villainimgleft[2];

int villainX = 550, villainY = 100;
int villaincurrentimage = 0;
int vhealthgone = 0;

typedef struct platform{
	int startx, endx, cordy;
}platform;

typedef struct ladder{
	int starty, endy, cordx;
}ladder;

typedef struct avatar{
	int nonInverted;
	int inverted;
	bool isInverted;
	bool isJumping=false;
	bool isClimbing=false;
	bool isRunning = false;
	bool isTakingHit = false;
	int currentPlatform=0;
	int ladderAt=0;
	int x = 0;
	float ymax;
	float y;
	int life = 5;
}avatar;

typedef struct fireball{
	int fireballImg;
	float x;
	int y;
	int currentPlatform = 0;
	bool isInitialized = false;
}fireball;

typedef struct barrel{
	float x;
	int y;
	int currentPlatform = 0;
	bool isInitialized = false;
}barrel;

platform p[totalPlats],p2[totalPlats2];
ladder l[totalLadders],l2[totalLadders2];
avatar a1;
barrel b1;
fireball f1;
int idle[4], invertedIdle[4], jump[6], invertedJump[6], run[8], invertedRun[8], barrelImg[2], takeHit[6], invertedTakeHit[6];

void showlife(){
	int multiplier = 16;
	int x = 1200 - 3*multiplier, y = 768 - 4*multiplier;
	for (int i = 0; i < a1.life; i++){
		iShowImage(x, y, 32, 32, heart);
		x -= 2*multiplier;
	}
}

void showLadder(ladder l1){
	int temp = l1.starty;
	for (int i = 0; l1.starty + 16 <= l1.endy; i++){
		iShowImage(l1.cordx, l1.starty, 16, 16, ladderPic);
		l1.starty += 16;
	}
	iShowImage(l1.cordx, l1.starty, 16, 16, ladderPic);
	l1.starty = temp;
}

void showPlatform(platform p1){
	int temp = p1.startx;
	for (int i = 0; p1.startx + 16 <= p1.endx; i++){
		iShowImage(p1.startx, p1.cordy, 16, 16, tile);
		p1.startx += 16;
	}
	p1.startx = temp;
}
void gravityCheck(){
	if (lvl == 1){
		if (!a1.isClimbing&&!a1.isJumping){
			for (int d = totalPlats - 1; d >= 0; d--){
				if (p[d].cordy + 16 <= a1.y && a1.x + 36 <= p[d].endx && a1.x + 16 >= p[d].startx){
					a1.ymax = p[d].cordy + 16;
					a1.currentPlatform = d;
					break;
				}
			}
		}
	}
	else if (lvl == 2){
		if (!a1.isClimbing&&!a1.isJumping){
			for (int d = totalPlats - 1; d >= 0; d--){
				if (p2[d].cordy + 16 <= a1.y && a1.x + 36 <= p2[d].endx && a1.x + 16 >= p2[d].startx){
					a1.ymax = p2[d].cordy + 16;
					a1.currentPlatform = d;
					break;
				}
			}
		}
	}
}
void idleFunc(){
	if (idleTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, idle[idleI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedIdle[idleI]);
		}
		idleI++;
		idleI %= 4;
		idleTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, idle[idleI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedIdle[idleI]);
		}
		idleTimer--;
	}
}
void jumpFunc(){
	if (jumpTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, jump[jumpI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedJump[jumpI]);
		}
		jumpI++;
		jumpI %= 6;
		jumpTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, jump[jumpI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedJump[jumpI]);
		}
		jumpTimer--;
	}
}
void runFunc(){
	if (runTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, run[runI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedRun[runI]);
		}
		runI++;
		runI %= 8;
		runTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, run[runI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedRun[runI]);
		}
		runTimer--;
	}
}
void barrelFunc(){
	if (barrelTimer == 0){
		barrelI++;
		barrelI %= 2;
		iShowImage(b1.x, b1.y, 16, 16, barrelImg[barrelI]);
		barrelTimer = 350;
	}
	else{
		iShowImage(b1.x, b1.y, 16, 16, barrelImg[barrelI]);
		barrelTimer--;
	}
}
void takeHitFunc(){
	if (takeHitTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, takeHit[takeHitI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedTakeHit[takeHitI]);
		}
		takeHitI++;
		takeHitI %= 6;
		if (takeHitI == 0){
			a1.isTakingHit = false;
			a1.life--;
		}
		takeHitTimer = 250;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 48, 48, takeHit[takeHitI]);
		}
		else{
			iShowImage(a1.x, a1.y, 48, 48, invertedTakeHit[takeHitI]);
		}
		takeHitTimer--;
	}
}
void showImages(){
	//Display Ladders
	if (lvl == 1){
		for (int d = 0; d < totalLadders; d++){
			showLadder(l[d]);
		}
	}
	else if (lvl == 2){
		for (int d = 0; d < totalLadders; d++){
			showLadder(l2[d]);
		}
	}
	//Display Platforms
	if (lvl == 1){
		for (int d = 0; d < totalPlats; d++){
			showPlatform(p[d]);
		}
	}
	else if (lvl == 2){
		for (int d = 0; d < totalPlats; d++){
			showPlatform(p2[d]);
		}
	}
	//Display Donkey
	if (lvl == 1){
		iShowImage(0, p[totalPlats - 1].cordy + 14, 32, 32, donkey);
	}
	else if (lvl == 2){
		iShowImage(0, p2[totalPlats - 1].cordy + 14, 32, 32, donkey);
	}
	//Display Board
	if (lvl == 1){
		iShowImage(0, p[3].cordy + 16, 32, 32, board);
	}
	else if (lvl == 2){
		iShowImage(0, p2[3].cordy + 16, 32, 32, board);
	}
	//Show Avatar
	if (!a1.isJumping&&!a1.isRunning&&!a1.isTakingHit){
		idleFunc();
	}
	else if (a1.isTakingHit) takeHitFunc();
	else if (a1.isJumping){
		jumpFunc();
	}
	else if(a1.isRunning){
		runFunc();
	}
	//Show Barrel
	if (lvl == 1){
		if (b1.isInitialized){
			barrelFunc();
			b1.x -= .0625;
			if (b1.x == p[b1.currentPlatform].startx) b1.isInitialized = false;
		}
		else{
			b1.isInitialized = true;
			b1.currentPlatform = a1.currentPlatform;
			b1.x = p[b1.currentPlatform].endx;
			b1.y = p[b1.currentPlatform].cordy + 16;
		}
	}
	else if (lvl == 2){
		if (b1.isInitialized){
			barrelFunc();
			b1.x -= .0625;
			if (b1.x == p2[b1.currentPlatform].startx) b1.isInitialized = false;
		}
		else{
			b1.isInitialized = true;
			b1.currentPlatform = a1.currentPlatform;
			b1.x = p2[b1.currentPlatform].endx;
			b1.y = p2[b1.currentPlatform].cordy + 16;
		}
	}
	//Show fireball
	if (lvl == 2 && f1.isInitialized){
		iShowImage(f1.x, f1.y, 8, 8, f1.fireballImg);
	}

	showlife();
}

void level1(){
	//Movement Mechanics
	if (a1.life == 0){
		gameover = 1;
	}
	if (gameover) {
		iSetColor(255, 0, 0);
		// Centered "Game Over" text
		iText(555, 381, "Game Over", GLUT_BITMAP_HELVETICA_18);  // Centered horizontally and vertically

		// Centered and positioned below "Game Over"
		iText(460, 340, "press r to go to the main menu", GLUT_BITMAP_HELVETICA_18);  // Centered and placed below


		return;
	}
	if (a1.x <= 0){
		if (a1.currentPlatform == 3 && lvl == 1){
			lvl = 2;
			a1.x = 0;
			a1.currentPlatform = 0;
			a1.ymax = p2[0].cordy+16;
			a1.y = a1.ymax;
			b1.isInitialized = false;
			f1.isInitialized = false;
			a1.life = 5;
		}
		else a1.x = 0;
	}
	if (a1.x>1168) a1.x = 1168;
	if (a1.isJumping){
		a1.y += 0.03125;
	}
	//Gravity 
	if (!a1.isJumping && (int)a1.ymax != (int)a1.y){
		a1.y -= 0.03125;
	}
	if (a1.ymax > a1.y) a1.y = a1.ymax;
	if ((a1.y - a1.ymax) >= 32) a1.isJumping = false;
	//Climbing Mechanics
	int ladderStart = -1, availableLadders = 0; //Check available ladders
	for (int d = 0; d < totalLadders; d++){
		if (l[d].starty == p[a1.currentPlatform].cordy + 16){
			if (ladderStart == -1) ladderStart = d;
			availableLadders++;
		}
	}
	// If not climbing, check for ladders in contact while moving horizontally
	if (!a1.isClimbing) {
		for (int d = 0; d < availableLadders; d++) {
			// Check if character's x-position is within ladder range
			if ((a1.x + 32 >= l[ladderStart + d].cordx && a1.x + 6  <= l[ladderStart + d].cordx)) {
				a1.ladderAt = ladderStart + d;
				a1.isClimbing = true;
				break;
			}
		}
	}
	else { // If climbing, check if the ladder is out of contact
		if (!(a1.x + 32 >= l[a1.ladderAt].cordx && a1.x + 6 <= l[a1.ladderAt].cordx)) {
			a1.isClimbing = false;
		}
	}
	// Check if reached a new platform
	if ((int)a1.ymax >= p[a1.currentPlatform + 1].cordy + 16 && a1.currentPlatform != totalPlats - 1) {
		a1.currentPlatform++;
	}

	// Check if went downwards
	if ((int)a1.ymax < p[a1.currentPlatform].cordy + 16) {
		a1.currentPlatform--;
	}
	//Gravity Check Function
	gravityCheck();
	//Check Collision of Barrels
	if (a1.isInverted){
		if (b1.isInitialized && abs(a1.x - b1.x) <= 16 && !a1.isTakingHit && abs((int)a1.y - b1.y) <= 10) a1.isTakingHit = true;
	}
	else{
		if (b1.isInitialized && abs(a1.x+40 - b1.x) <= 16 && !a1.isTakingHit && abs((int)a1.y - b1.y) <= 10) a1.isTakingHit = true;
	}
	//Show Images
	showImages();
}

void level2(){
	if (a1.life == 0){
		gameover = 1;
	}
	if (gameover) {
		iSetColor(255, 0, 0);
		// Centered "Game Over" text
		iText(555, 381, "Game Over", GLUT_BITMAP_HELVETICA_18);  // Centered horizontally and vertically

		// Centered and positioned below "Game Over"
		iText(460, 340, "press r to go to the main menu", GLUT_BITMAP_HELVETICA_18);  // Centered and placed below


		return;
	}
	//Movement Mechanics
	if (a1.x <= 0){
		if (a1.currentPlatform == 3) lvl = 3;
		else a1.x = 0;
	}
	if (a1.x>1168) a1.x = 1168;
	if (a1.isJumping){
		a1.y += 0.03125;
	}
	//Gravity 
	if (!a1.isJumping && (int)a1.ymax != (int)a1.y){
		a1.y -= 0.03125;
	}
	if (a1.ymax > a1.y) a1.y = a1.ymax;
	if ((a1.y - a1.ymax) >= 32) a1.isJumping = false;
	//Climbing Mechanics
	int ladderStart = -1, availableLadders = 0; //Check available ladders
	for (int d = 0; d < totalLadders; d++){
		if (l2[d].starty == p2[a1.currentPlatform].cordy + 16){
			if (ladderStart == -1) ladderStart = d;
			availableLadders++;
		}
	}
	// If not climbing, check for ladders in contact while moving horizontally
	if (!a1.isClimbing) {
		for (int d = 0; d < availableLadders; d++) {
			// Check if character's x-position is within ladder range
			if ((a1.x + 32 >= l2[ladderStart + d].cordx && a1.x + 6 <= l2[ladderStart + d].cordx)) {
				a1.ladderAt = ladderStart + d;
				a1.isClimbing = true;
				break;
			}
		}
	}
	else { // If climbing, check if the ladder is out of contact
		if (!(a1.x + 32 >= l2[a1.ladderAt].cordx && a1.x + 6 <= l2[a1.ladderAt].cordx)) {
			a1.isClimbing = false;
		}
	}
	// Check if reached a new platform
	if ((int)a1.ymax >= p[a1.currentPlatform + 1].cordy + 16 && a1.currentPlatform != totalPlats - 1) {
		a1.currentPlatform++;
	}

	// Check if went downwards
	if ((int)a1.ymax < p2[a1.currentPlatform].cordy + 16) {
		a1.currentPlatform--;
	}
	//Gravity Check Function
	gravityCheck();
	//Fireball
	if (!f1.isInitialized){
		f1.isInitialized = true;
		f1.x = 1200;
		int min = p2[a1.currentPlatform].cordy + 16, max = p2[a1.currentPlatform + 1].cordy;
		f1.y= min + (rand() % (max - min + 1));
	}
	else{
		f1.x -= 0.125;
		if (f1.x <= 0) f1.isInitialized = false;
	}
	//Check Collision of Fireball and Barrel
	if (!a1.isTakingHit){
		//Check Collision of Barrels
		if (a1.isInverted){
			if (b1.isInitialized && abs(a1.x - b1.x) <= 16 && !a1.isTakingHit && abs((int)a1.y - b1.y) <= 10) a1.isTakingHit = true;
		}
		else{
			if (b1.isInitialized && abs(a1.x + 40 - b1.x) <= 16 && !a1.isTakingHit && abs((int)a1.y - b1.y) <= 10) a1.isTakingHit = true;
		}
		//Check Collision of Fireball
		if (a1.isInverted){
			if (f1.isInitialized && abs(a1.x - (int)f1.x) <= 16 && abs((int)a1.y - f1.y) <= 12 && !a1.isTakingHit){
				a1.isTakingHit = true;
			}
		}
		else{
			if (f1.isInitialized && abs(a1.x + 40 - (int)f1.x) <= 16 && abs((int)a1.y - f1.y) <= 12 && !a1.isTakingHit){
				a1.isTakingHit = true;
			}
		}
	}
	//Show Images
	showImages();
}

void iDraw()
{
	iClear();

	if (lvl == 0)
	{
		if (sty == 1)
		{
			story();
		}
		else if (abtus == 1)
		{
			aboutus();
		}
		else if (inst == 1)
		{
			instruction();
		}
		else
			homepage();
	}
	
	else if (lvl == 1){
		level1();
	}
	else if (lvl == 2){
		level2();
	}
	else if (lvl == 3)
		level3();
	else if (lvl == 4)
		level4();
	else if (win == 1)
		winoutro();

	timer++;
}

void instruction()
{
	iShowImage(0, 100, 1200, 662, instimage);
	iText(0, 30, "Press R to go to the main menu", GLUT_BITMAP_TIMES_ROMAN_24);
}

void aboutus()
{
	iShowImage(0, 100, 1200, 662, abtusimage);
	
	iText(0, 30, "Press R to go to the main menu", GLUT_BITMAP_TIMES_ROMAN_24);
}
void story()
{
	iShowImage(0, 100, 1200, 662, styimage);
	iSetColor(255, 255, 255);
	iText(0, 70, "Giant manages to grab Princess ELara.",  GLUT_BITMAP_TIMES_ROMAN_24);
	iText(0, 50, "To save her, the prince must now arrive and engage in combat with the donkey.", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(0, 30, "Press R to go to the main menu", GLUT_BITMAP_TIMES_ROMAN_24);
}

void homepage()
{
	iShowImage(0, 0, 1200, 762, hmpage);

	iSetColor(150, 100, 79);
	iFilledRectangle(700, 200, 250, 60);
	iSetColor(100, 110, 90);
	iFilledRectangle(700, 300, 250, 60);
	iSetColor(200, 0, 0);
	iFilledRectangle(700, 400, 250, 60);
	iSetColor(190, 100, 0);
	iFilledRectangle(700, 100, 250, 60);

	iSetColor(255, 255, 255);
	iText(745, 325, "INSTRUCTION", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(770, 225, "ABOUT US", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(784, 425, "START", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(784, 125, "STORY", GLUT_BITMAP_TIMES_ROMAN_24);

}
void level3()
{
	//iShowImage(0, 0, 1200, 762, l3back);

	for (int i = 0; i < health; i++)
	{
		iSetColor(0, 255, 0);
		iFilledRectangle(0 + (50 * i), 700, 50, 20);
	}


	if (gameover) {
		iSetColor(255, 0, 0);
		// Centered "Game Over" text
		iText(555, 381, "Game Over", GLUT_BITMAP_HELVETICA_18);  // Centered horizontally and vertically

		// Centered and positioned below "Game Over"
		iText(460, 340, "press r to go to the main menu", GLUT_BITMAP_HELVETICA_18);  // Centered and placed below


		return;
	}

	iSetColor(150, 150, 150);
	iFilledRectangle(0, 0, 1200, 100);

	if (showFireExplosion)
	{
		iShowImage(fireballX, fireballY, 75, 75, explosionImage);
		FireExplosionTimer++;
		if (FireExplosionTimer > 500)
		{
			showFireExplosion = 0;
			FireExplosionTimer = 0;
			fireballX = 1000;
		}
	}
	else
	{
		iShowImage(fireballX, fireballY, 50, 50, fireballImage);
		if (timer % 60 == 0) {
			fireballX -= obstacleSpeed + 3;
		}
		if (fireballX < 0) {
			fireballX = 1200;
		}
	}

	if (showExplosion) {
		iShowImage(obstacleX, obstacleY, 75, 75, explosionImage);
		explosionTimer++;
		if (explosionTimer > 500) {  // 2 seconds assuming 60 FPS
			showExplosion = 0;
			explosionTimer = 0;
			obstacleX = 1200;  // Reset barrel position
		}
	}
	else {
		// Draw Rolling Barrel
		iShowImage(obstacleX, obstacleY, 50, 50, barrelImages[currentBarrelImageIndex]);

		// Update obstacle position
		if (timer % 60 == 0) {  // Update position every half second (assuming 60 FPS)
			obstacleX -= obstacleSpeed;
			currentBarrelImageIndex = (currentBarrelImageIndex + 1) % 4;  // Cycle through barrel images
		}
		if (obstacleX < 0) {
			obstacleX = 1200;
		}
	}


	if (abs(obstacleX - heroX) < 300 && isAttack == 1)
	{
		showExplosion = 1;
	}


	if (isAttack == 1 && leftorright == 1)
	{
		iShowImage(heroX, heroY, 500, 350, sword[currentRunImageIndex]);
		if (timer % 900 == 0) {
			if (currentRunImageIndex == 10)
			{
				isAttack = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 11;
		}
	}
	else if (isAttack == 1 && leftorright == 0)
	{
		iShowImage(heroX, heroY, 500, 350, rsword[currentRunImageIndex]);
		if (timer % 900 == 0) {
			if (currentRunImageIndex == 10)
			{
				isAttack = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 11;
		}
	}
	else if (isJumpingimg == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, jumping[currentRunImageIndex]);
		if (timer % 250 == 0) {
			if (currentRunImageIndex == 19)
			{
				isJumpingimg = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 20;
		}
	}
	else if (isMoving == 1 && leftorright == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, runImages[currentRunImageIndex]);
		if (timer % 1300 == 0) { // Update image index every 10 frames for smoother animation
			currentRunImageIndex = (currentRunImageIndex + 1) % 8;
		}
	}
	else if (isMoving == 1 && leftorright == 0)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, rrunImages[currentRunImageIndex]);
		if (timer % 1300 == 0) { // Update image index every 10 frames for smoother animation
			currentRunImageIndex = (currentRunImageIndex + 1) % 8;
		}
	}
	else if (leftorright == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, idleImage);

	}
	else if (leftorright == 0)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, ridleImage);
	}



	// Apply gravity and jump mechanics
	if (isJumping) {
		jumpHeight += jumpSpeed;
		if (jumpHeight >= maxJumpHeight) {
			isJumping = 0;
		}
	}
	else {
		if (jumpHeight > 0.0) {
			jumpHeight -= gravity;
		}
	}

	// Check for collisions with Mario
	if (!showExplosion && obstacleX < heroX + 265 && obstacleX > heroX + 200 && heroY + (int)jumpHeight < obstacleY + 50) {
		//gameover = 1;
		showExplosion = 1;
		obstacleX = 1200;
		health--;
	}
	if (!showFireExplosion && heroX + 200 < fireballX &&  fireballX < heroX + 220 && heroY + (int)jumpHeight < fireballY + 50)
	{
		showFireExplosion = 1;
		health--;
	}

	if (health == 0)
		gameover = 1;

	//going to level 4
	if (heroX >= 800)
	{
		isAttack = 0;
		jumpHeight = 0.0;
		isJumping = 0;
		currentRunImageIndex = 0;
		isWalking = 0; // Indicates if the character is walking
		isMoving = 0; // Indicates if the character is moving
		isJumping = 0;
		leftorright = 1;//0 means left 1 means right
		heroX = 180;
		showFireExplosion = 0;
		FireExplosionTimer = 0;
		fireballX = 550;
		lvl = 4;
	}
}


void level4()
{
	//iShowImage(0, 0, 1200, 762, l3back);

	if (gameover) {
		iSetColor(255, 0, 0);
		// Centered "Game Over" text
		iText(555, 381, "Game Over", GLUT_BITMAP_HELVETICA_18);  // Centered horizontally and vertically

		// Centered and positioned below "Game Over"
		iText(470, 340, "press r to go to the main menu", GLUT_BITMAP_HELVETICA_18);  // Centered and placed below

		return;
	}

	if (health == 0)
		gameover = 1;

	if (villainhealth == 0)
	{
		lvl = 5;
		win = 1;
	}


	for (int i = 0; i < health; i++)
	{
		iSetColor(0, 255, 0);
		iFilledRectangle(0 + (50 * i), 700, 50, 20);
	}

	for (int i = 0; i < villainhealth; i++)
	{
		iSetColor(255, 0, 0);
		iFilledRectangle(0 + (50 * i), 650, 50, 20);
	}

	if (leftorright == 1 && !vhealthgone &&isAttack == 1 && heroX >= 205 && heroX <= 350)
	{
		vhealthgone = 1;
		villainhealth--;
	}

	if (leftorright == 0 && !vhealthgone &&isAttack == 1 && heroX >= 400 && heroX <= 515)
	{
		vhealthgone = 1;
		villainhealth--;
	}

	if (heroX < villainX)
	{

		iShowImage(villainX, villainY, 150, 150, villainimgleft[villaincurrentimage]);
		if (timer % 1500 == 0) {
			villaincurrentimage = (villaincurrentimage + 1) % 2;
		}

		if (showFireExplosion)
		{
			iShowImage(fireballX, fireballY, 75, 75, explosionImage);
			FireExplosionTimer++;
			if (FireExplosionTimer > 500)
			{
				showFireExplosion = 0;
				FireExplosionTimer = 0;
				fireballX = 550;
			}
		}
		else
		{
			if (fireballX > 550)
			{
				iShowImage(fireballX, fireballY, 50, 50, rfireballImage);
				if (timer % 60 == 0) {
					fireballX += obstacleSpeed;
				}
				if (fireballX > 1200) {
					fireballX = 550;
				}
			}
			else
			{

				iShowImage(fireballX, fireballY, 50, 50, fireballImage);

				if (timer % 60 == 0) {
					fireballX -= obstacleSpeed;
				}
				if (fireballX < 0) {
					fireballX = 550;
				}
			}
		}

	}
	else
	{
		iShowImage(villainX, villainY, 150, 150, villainimgright[villaincurrentimage]);
		if (timer % 1500 == 0) {
			villaincurrentimage = (villaincurrentimage + 1) % 2;
		}
		if (showFireExplosion)
		{
			iShowImage(fireballX, fireballY, 75, 75, explosionImage);
			FireExplosionTimer++;
			if (FireExplosionTimer > 500)
			{
				showFireExplosion = 0;
				FireExplosionTimer = 0;
				fireballX = 550;
			}
		}
		else
		{
			if (fireballX < 550)
			{
				iShowImage(fireballX, fireballY, 50, 50, fireballImage);

				if (timer % 60 == 0) {
					fireballX -= obstacleSpeed;
				}
				if (fireballX < 0) {
					fireballX = 550;
				}
			}
			else
			{
				iShowImage(fireballX, fireballY, 50, 50, rfireballImage);
				if (timer % 60 == 0) {
					fireballX += obstacleSpeed;
				}
				if (fireballX > 1200) {
					fireballX = 550;
				}
			}
		}
	}


	if (!showFireExplosion && heroX + 200 < fireballX &&  fireballX < heroX + 220 && heroY + (int)jumpHeight < fireballY + 50)
	{
		showFireExplosion = 1;
		health--;
	}


	if (isAttack == 1 && leftorright == 1)
	{
		iShowImage(heroX, heroY, 500, 350, sword[currentRunImageIndex]);
		if (timer % 900 == 0) {
			if (currentRunImageIndex == 10)
			{
				isAttack = 0;
				vhealthgone = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 11;
		}
	}
	else if (isAttack == 1 && leftorright == 0)
	{
		iShowImage(heroX, heroY, 500, 350, rsword[currentRunImageIndex]);


		if (timer % 900 == 0) {
			if (currentRunImageIndex == 10)
			{
				isAttack = 0;
				vhealthgone = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 11;
		}
	}
	else if (isJumpingimg == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, jumping[currentRunImageIndex]);
		if (timer % 250 == 0) {
			if (currentRunImageIndex == 19)
			{
				isJumpingimg = 0;
			}
			currentRunImageIndex = (currentRunImageIndex + 1) % 20;
		}
	}
	else if (isMoving == 1 && leftorright == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, runImages[currentRunImageIndex]);
		if (timer % 1300 == 0) { // Update image index every 10 frames for smoother animation
			currentRunImageIndex = (currentRunImageIndex + 1) % 8;
		}
	}
	else if (isMoving == 1 && leftorright == 0)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, rrunImages[currentRunImageIndex]);
		if (timer % 1300 == 0) { // Update image index every 10 frames for smoother animation
			currentRunImageIndex = (currentRunImageIndex + 1) % 8;
		}
	}
	else if (leftorright == 1)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, idleImage);

	}
	else if (leftorright == 0)
	{
		iShowImage(heroX, heroY + (int)jumpHeight, 500, 350, ridleImage);
	}



	// Apply gravity and jump mechanics
	if (isJumping) {
		jumpHeight += jumpSpeed;
		if (jumpHeight >= maxJumpHeight) {
			isJumping = 0;
		}
	}
	else {
		if (jumpHeight > 0.0) {
			jumpHeight -= gravity;
		}
	}

}

void winoutro()
{
	iShowImage(0, 100, 1200, 662, winbk);
	iText(0, 70, "The prince saved the princess by killing the giant", GLUT_BITMAP_TIMES_ROMAN_24);
	iText(0, 50, "Press R to go to the main menu", GLUT_BITMAP_TIMES_ROMAN_24);
	//iSetColor(255, 0, 255);
	//iText(350, 381, "Congratulations YOU HAVE WON THE GAME", GLUT_BITMAP_TIMES_ROMAN_24);
	//iText(460, 350, "Press C to go to main menu", GLUT_BITMAP_TIMES_ROMAN_24);


}

/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{
	
}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{


		if (lvl == 0)
		{
			if (lvl == 0 && ((mx >= 702 && mx <= 949) && (my >= 402 && my <= 459)))
			{
				lvl = 1;
			}
			else if (lvl == 0 && ((mx >= 702 && mx <= 949) && (my >= 102 && my <= 159)))
			{
				sty = 1;
			}
			else if (lvl == 0 && ((mx >= 702 && mx <= 949) && (my >= 302 && my <= 359)))
			{
				inst = 1;
			}
			else if (lvl == 0 && ((mx >= 702 && mx <= 949) && (my >= 202 && my <= 259)))
			{
				abtus = 1;
			}
		}


		isAttack = 1;
		//printf("vhealthgone = %d ", vhealthgone);
	//	printf("villainhealth = %d ", villainhealth);
	//	printf("herox = %d ", heroX);
	//	printf(" mx = %d\n", mx);

		printf("x=%d y=%d\n", mx, my);

		
	}
	
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{

	if (sty == 1 && key == 'r')
	{
		sty = 0;
	}
	if (abtus == 1 && key == 'r')
	{
		abtus = 0;
	}
	if (inst == 1 && key == 'r')
	{
		inst = 0;
	}
	if (win == 1 && key == 'r')
	{
		win = 0;
		lvl = 0; 
	}

	if (lvl < 3)
	{
		if (key == 'a'){
			if (a1.isInverted){
				a1.isInverted = false;
			}
			a1.x -= 3;
			a1.isRunning = true;
		}
		else if (key == 'd'){
			if (!a1.isInverted){
				a1.isInverted = true;
			}
			a1.x += 3;
			a1.isRunning = true;
		}
		else if (key == ' '){
			if (!a1.isJumping){
				a1.isJumping = true;
			}
		}
		else if (key == 'w' &&a1.isClimbing){
			if (lvl == 1){
				if ((int)a1.ymax + 8 <= l[a1.ladderAt].endy + 16){
					a1.y += 8;
					a1.ymax += 8;
				}
			}
			else if (lvl == 2){
				if ((int)a1.ymax + 8 <= l2[a1.ladderAt].endy + 16){
					a1.y += 8;
					a1.ymax += 8;
				}
			}
		}
		else if (key == 's' &&a1.isClimbing){
			if (lvl == 1){
				if ((int)a1.ymax != l[a1.ladderAt].starty){
					a1.y -= 8;
					a1.ymax -= 8;
				}
			}
			else if (lvl == 2){
				if ((int)a1.ymax != l2[a1.ladderAt].starty){
					a1.y -= 8;
					a1.ymax -= 8;
				}
			}
		}
		else a1.isRunning = false;
		if (key == 'r' && gameover) {
			lvl = 0;
			a1.ymax = p[0].cordy + 16;
			a1.y = a1.ymax;
			a1.currentPlatform = 0;
			a1.life = 5;
		}
	}
	else
	{
		if (key == ' ' && !gameover) {
			if (!isJumping) {
				isJumping = 1;
				isJumpingimg = 1;
			}
		}

		if (key == 'r' && gameover) {
			fireballX = 1000;// Restart game
			gameover = 0;
			jumpHeight = 0.0;
			isJumping = 0;
			obstacleX = 1200;
			currentRunImageIndex = 0;
			isWalking = 0; // Indicates if the character is walking
			isMoving = 0; // Indicates if the character is moving
			isJumping = 0;
			leftorright = 1;//0 means left 1 means right
			health = 10;
			heroX = -150;
			lvl = 0;

		}

		if (key == 'd' && !gameover) {
			// Move right
			isWalking = 1;
			isMoving = 1;
			heroX += 5;  // Move the character to the right
			leftorright = 1;
		}
		if (key == 'a' && !gameover) { // Move right
			isWalking = 1;
			isMoving = 1;
			heroX -= 5;  // Move the character to the right
			leftorright = 0;
		}
		if (key == 's' && !gameover) { // Move right
			isWalking = 0;
			isMoving = 0;
			// Move the character to the right
		}
	}
	
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

	
	if (key == GLUT_KEY_RIGHT)
	{
		if (musicOn)
		{
			musicOn = false;
			PlaySound(0, 0, 0);
		}
		else
		{
			musicOn = true;
			PlaySound("music\\musicUndertale.wav", NULL, SND_LOOP | SND_ASYNC);
		}
	

	}
	if (key == GLUT_KEY_LEFT)
	{
		
	}
	
	if (key == GLUT_KEY_HOME)
	{
		
	}
	
}


int main()
{
	if (musicOn)
		PlaySound("music\\musicUndertale.wav", NULL, SND_LOOP | SND_ASYNC);



	///srand((unsigned)time(NULL));
	iInitialize(1200, 764, "My Project");
	///updated see the documentations
	//Taking Images
	donkey = iLoadImage("./assets/images/donkey.png");
	tile = iLoadImage("./assets/images/plat.png");
	ladderPic = iLoadImage("./assets/images/ladder.png");
	f1.fireballImg = iLoadImage("./assets/images/fireball.png");
	a1.nonInverted = iLoadImage("./assets/images/avatar.png");
	a1.inverted = iLoadImage("./assets/images/avatarinverted.png");
	idle[0] = iLoadImage("./assets/images/idle_1.png");
	idle[1] = iLoadImage("./assets/images/idle_2.png");
	idle[2] = iLoadImage("./assets/images/idle_3.png");
	idle[3] = iLoadImage("./assets/images/idle_4.png");
	invertedIdle[0] = iLoadImage("./assets/images/invertedIdle_1.png");
	invertedIdle[1] = iLoadImage("./assets/images/invertedIdle_2.png");
	invertedIdle[2] = iLoadImage("./assets/images/invertedIdle_3.png");
	invertedIdle[3] = iLoadImage("./assets/images/invertedIdle_4.png");
	jump[0] = iLoadImage("./assets/images/jump_1.png");
	jump[1] = iLoadImage("./assets/images/jump_2.png");
	jump[2] = iLoadImage("./assets/images/jump_3.png");
	jump[3] = iLoadImage("./assets/images/jump_4.png");
	jump[4] = iLoadImage("./assets/images/jump_5.png");
	jump[5] = iLoadImage("./assets/images/jump_6.png");
	invertedJump[0] = iLoadImage("./assets/images/Invertedjump_1.png");
	invertedJump[1] = iLoadImage("./assets/images/Invertedjump_2.png");
	invertedJump[2] = iLoadImage("./assets/images/Invertedjump_3.png");
	invertedJump[3] = iLoadImage("./assets/images/Invertedjump_4.png");
	invertedJump[4] = iLoadImage("./assets/images/Invertedjump_5.png");
	invertedJump[5] = iLoadImage("./assets/images/Invertedjump_6.png");
	run[0] = iLoadImage("./assets/images/run_1.png");
	run[1] = iLoadImage("./assets/images/run_2.png");
	run[2] = iLoadImage("./assets/images/run_3.png");
	run[3] = iLoadImage("./assets/images/run_4.png");
	run[4] = iLoadImage("./assets/images/run_5.png");
	run[5] = iLoadImage("./assets/images/run_6.png");
	run[6] = iLoadImage("./assets/images/run_7.png");
	run[7] = iLoadImage("./assets/images/run_8.png");
	invertedRun[0] = iLoadImage("./assets/images/Invertedrun_1.png");
	invertedRun[1] = iLoadImage("./assets/images/Invertedrun_2.png");
	invertedRun[2] = iLoadImage("./assets/images/Invertedrun_3.png");
	invertedRun[3] = iLoadImage("./assets/images/Invertedrun_4.png");
	invertedRun[4] = iLoadImage("./assets/images/Invertedrun_5.png");
	invertedRun[5] = iLoadImage("./assets/images/Invertedrun_6.png");
	invertedRun[6] = iLoadImage("./assets/images/Invertedrun_7.png");
	invertedRun[7] = iLoadImage("./assets/images/Invertedrun_8.png");
	barrelImg[0] = iLoadImage("./assets/images/barrel.png");
	barrelImg[1] = iLoadImage("./assets/images/barrel_2.png");
	board = iLoadImage("./assets/images/left.png");
	takeHit[0] = iLoadImage("./assets/images/take_hit_1.png");
	invertedTakeHit[0] = iLoadImage("./assets/images/inverted_take_hit_1.png");
	takeHit[1] = iLoadImage("./assets/images/take_hit_2.png");
	invertedTakeHit[1] = iLoadImage("./assets/images/inverted_take_hit_2.png");
	takeHit[2] = iLoadImage("./assets/images/take_hit_3.png");
	invertedTakeHit[2] = iLoadImage("./assets/images/inverted_take_hit_3.png");
	takeHit[3] = iLoadImage("./assets/images/take_hit_4.png");
	invertedTakeHit[3] = iLoadImage("./assets/images/inverted_take_hit_4.png");
	takeHit[4] = iLoadImage("./assets/images/take_hit_5.png");
	invertedTakeHit[4] = iLoadImage("./assets/images/inverted_take_hit_5.png");
	takeHit[5] = iLoadImage("./assets/images/take_hit_6.png");
	invertedTakeHit[5] = iLoadImage("./assets/images/inverted_take_hit_6.png");
	//Taking File pointer and Inputs via file
	FILE *ptr;
	errno_t err;
	err = fopen_s(&ptr, "./assets/input.txt", "r");
	if (err != 0) {
		fprintf(stderr, "Error opening input file: %d\n", err);
		return 1;
	}
	for (int d = 0; d < totalPlats; d++){
		fscanf_s(ptr, "%d %d %d", &p[d].startx, &p[d].endx, &p[d].cordy);
	}
	for (int d = 0; d < totalLadders; d++){
		fscanf_s(ptr, "%d %d %d", &l[d].starty, &l[d].endy, &l[d].cordx);
	}
	for (int d = 0; d < totalPlats2; d++){
		fscanf_s(ptr, "%d %d %d", &p2[d].startx, &p2[d].endx, &p2[d].cordy);
	}
	for (int d = 0; d < totalLadders2; d++){
		fscanf_s(ptr, "%d %d %d", &l2[d].starty, &l2[d].endy, &l2[d].cordx);
	}
	fclose(ptr);
	a1.ymax = p[0].cordy + 16;
	a1.y = a1.ymax;
	a1.currentPlatform = 0;




	background3 = iLoadImage("background3.jpg");


	hmpage = iLoadImage("images//homepage.jpg");
	// Load the PNG images
	idleImage = iLoadImage("images//idle_1.png");
	ridleImage = iLoadImage("images//ridle_1.png");

	runImages[0] = iLoadImage("images//run_1.png");
	runImages[1] = iLoadImage("images//run_2.png");
	runImages[2] = iLoadImage("images//run_3.png");
	runImages[3] = iLoadImage("images//run_4.png");
	runImages[4] = iLoadImage("images//run_5.png");
	runImages[5] = iLoadImage("images//run_6.png");
	runImages[6] = iLoadImage("images//run_7.png");
	runImages[7] = iLoadImage("images//run_8.png");
	rrunImages[0] = iLoadImage("images//rrun_1.png");
	rrunImages[1] = iLoadImage("images//rrun_2.png");
	rrunImages[2] = iLoadImage("images//rrun_3.png");
	rrunImages[3] = iLoadImage("images//rrun_4.png");
	rrunImages[4] = iLoadImage("images//rrun_5.png");
	rrunImages[5] = iLoadImage("images//rrun_6.png");
	rrunImages[6] = iLoadImage("images//rrun_7.png");
	rrunImages[7] = iLoadImage("images//rrun_8.png");

	sword[0] = iLoadImage("images//1_atk_1.png");
	sword[1] = iLoadImage("images//1_atk_2.png");
	sword[2] = iLoadImage("images//1_atk_3.png");
	sword[3] = iLoadImage("images//1_atk_4.png");
	sword[4] = iLoadImage("images//1_atk_5.png");
	sword[5] = iLoadImage("images//1_atk_6.png");
	sword[6] = iLoadImage("images//1_atk_7.png");
	sword[7] = iLoadImage("images//1_atk_8.png");
	sword[8] = iLoadImage("images//1_atk_9.png");
	sword[9] = iLoadImage("images//1_atk_10.png");
	sword[10] = iLoadImage("images//1_atk_11.png");
	rsword[0] = iLoadImage("images//r1_atk_1.png");
	rsword[1] = iLoadImage("images//r1_atk_2.png");
	rsword[2] = iLoadImage("images//r1_atk_3.png");
	rsword[3] = iLoadImage("images//r1_atk_4.png");
	rsword[4] = iLoadImage("images//r1_atk_5.png");
	rsword[5] = iLoadImage("images//r1_atk_6.png");
	rsword[6] = iLoadImage("images//r1_atk_7.png");
	rsword[7] = iLoadImage("images//r1_atk_8.png");
	rsword[8] = iLoadImage("images//r1_atk_9.png");
	rsword[9] = iLoadImage("images//r1_atk_10.png");
	rsword[10] = iLoadImage("images//r1_atk_11.png");

	jumping[0] = iLoadImage("images//jump_1.png");
	jumping[1] = iLoadImage("images//jump_2.png");
	jumping[2] = iLoadImage("images//jump_3.png");
	jumping[3] = iLoadImage("images//jump_4.png");
	jumping[4] = iLoadImage("images//jump_5.png");
	jumping[5] = iLoadImage("images//jump_6.png");
	jumping[6] = iLoadImage("images//jump_7.png");
	jumping[7] = iLoadImage("images//jump_8.png");
	jumping[8] = iLoadImage("images//jump_9.png");
	jumping[9] = iLoadImage("images//jump_10.png");
	jumping[10] = iLoadImage("images//jump_11.png");
	jumping[11] = iLoadImage("images//jump_12.png");
	jumping[12] = iLoadImage("images//jump_13.png");
	jumping[13] = iLoadImage("images//jump_14.png");
	jumping[14] = iLoadImage("images//jump_15.png");
	jumping[15] = iLoadImage("images//jump_16.png");
	jumping[16] = iLoadImage("images//jump_17.png");
	jumping[17] = iLoadImage("images//jump_18.png");
	jumping[18] = iLoadImage("images//jump_19.png");
	jumping[19] = iLoadImage("images//jump_20.png");

	barrelImages[0] = iLoadImage("images//Barrel1.png");
	barrelImages[1] = iLoadImage("images//Barrel2.png");
	barrelImages[2] = iLoadImage("images//Barrel3.png");
	barrelImages[3] = iLoadImage("images//Barrel4.png");

	explosionImage = iLoadImage("images//explosion.png");
	fireballImage = iLoadImage("images//fireball.png");
	rfireballImage = iLoadImage("images//rfireball.png");

	villainimgright[0] = iLoadImage("images//DK2.png");
	villainimgright[1] = iLoadImage("images//DK3rev.png");

	villainimgleft[0] = iLoadImage("images//DK2.png");
	villainimgleft[1] = iLoadImage("images//DK3.png");
	l3back = iLoadImage("images//level3background.jpg");
	winbk = iLoadImage("images//winbck.jpg");
	styimage = iLoadImage("images//styimage.jpg");
	abtusimage = iLoadImage("images//abtusimage.png");
	instimage = iLoadImage("images//instruction.jpg");

	heart = iLoadImage("./assets/images/heart.png");

	lvl = 0;
	iStart();
	return 0;
}