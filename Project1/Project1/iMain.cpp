#include "iGraphics.h"
#include <stdio.h>
#include <errno.h>


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int donkey, tile, ladderPic, board;
const int totalPlats = 5, totalLadders = 3, totalPlats2=5, totalLadders2= 3;
int barrelI = 0, jumpI = 0, runI = 0, idleI = 0, takeHitI = 0, barrelTimer = 350, jumpTimer = 350, runTimer = 350, idleTimer = 350, takeHitTimer = 250;
int lvl;

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
int idle[4], invertedIdle[4], jump[6], invertedJump[6], run[10], invertedRun[10], barrelImg[2], takeHit[6], invertedTakeHit[6];

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
				if (p[d].cordy + 16 <= a1.y && a1.x + 16 <= p[d].endx && a1.x + 16 >= p[d].startx){
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
				if (p2[d].cordy + 16 <= a1.y && a1.x + 16 <= p2[d].endx && a1.x + 16 >= p2[d].startx){
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
			iShowImage(a1.x, a1.y, 32, 32, idle[idleI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedIdle[idleI]);
		}
		idleI++;
		idleI %= 4;
		idleTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, idle[idleI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedIdle[idleI]);
		}
		idleTimer--;
	}
}
void jumpFunc(){
	if (jumpTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, jump[jumpI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedJump[jumpI]);
		}
		jumpI++;
		jumpI %= 6;
		jumpTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, jump[jumpI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedJump[jumpI]);
		}
		jumpTimer--;
	}
}
void runFunc(){
	if (runTimer == 0){
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, run[runI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedRun[runI]);
		}
		runI++;
		runI %= 10;
		runTimer = 350;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, run[runI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedRun[runI]);
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
			iShowImage(a1.x, a1.y, 32, 32, takeHit[takeHitI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedTakeHit[takeHitI]);
		}
		takeHitI++;
		takeHitI %= 6;
		if (takeHitI == 0){
			a1.isTakingHit = false;
		}
		takeHitTimer = 250;
	}
	else{
		if (a1.isInverted){
			iShowImage(a1.x, a1.y, 32, 32, takeHit[takeHitI]);
		}
		else{
			iShowImage(a1.x, a1.y, 32, 32, invertedTakeHit[takeHitI]);
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
}

void level1(){
	//Movement Mechanics
	if (a1.x <= 0){
		if (a1.currentPlatform == 3 && lvl == 1){
			lvl = 2;
			a1.x = 0;
			a1.y = 0;
			a1.ymax = 0;
			b1.isInitialized = 0;
		}
		else a1.x = 0;
	}
	if (a1.x>1168) a1.x = 1168;
	if (a1.isJumping){
		a1.y += .0625;
	}
	//Gravity 
	if (!a1.isJumping && (int)a1.ymax != (int)a1.y){
		a1.y -= .0625;
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
			if ((a1.x + 16 >= l[ladderStart + d].cordx && a1.x <= l[ladderStart + d].cordx)) {
				a1.ladderAt = ladderStart + d;
				a1.isClimbing = true;
				break;
			}
		}
	}
	else { // If climbing, check if the ladder is out of contact
		if (!(a1.x + 16 >= l[a1.ladderAt].cordx && a1.x <= l[a1.ladderAt].cordx)) {
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
	if (b1.isInitialized && abs(a1.x - b1.x) <= 16 && !a1.isTakingHit && abs(a1.y - b1.y)<=10) a1.isTakingHit = true;
	//Show Images
	showImages();
}

void level2(){
	//Movement Mechanics
	if (a1.x <= 0){
		if (a1.currentPlatform == 3) lvl = 3;
		else a1.x = 0;
	}
	if (a1.x>1168) a1.x = 1168;
	if (a1.isJumping){
		a1.y += .0625;
	}
	//Gravity 
	if (!a1.isJumping && (int)a1.ymax != (int)a1.y){
		a1.y -= .0625;
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
			if ((a1.x + 16 >= l2[ladderStart + d].cordx && a1.x <= l2[ladderStart + d].cordx)) {
				a1.ladderAt = ladderStart + d;
				a1.isClimbing = true;
				break;
			}
		}
	}
	else { // If climbing, check if the ladder is out of contact
		if (!(a1.x + 16 >= l2[a1.ladderAt].cordx && a1.x <= l2[a1.ladderAt].cordx)) {
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
		f1.x -= 0.25;
		if (f1.x <= 0) f1.isInitialized = false;
	}
	//Check Collision of Fireball and Barrel
	if (!a1.isTakingHit){
		if (f1.isInitialized && abs(a1.x - (int)f1.x) <= 10 && abs((int)a1.y - f1.y) <= 20){
			a1.isTakingHit = true;
		}
		else if (b1.isInitialized && abs(a1.x - b1.x) <= 12 && abs((int)a1.y - b1.y) <= 10){
			a1.isTakingHit = true;
		}
	}
	//Show Images
	showImages();
}

void iDraw()
{
	iClear();
	if (lvl == 1){
		level1();
	}
	else if (lvl == 2){
		level2();
	}
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
	idle[1] = iLoadImage("./assets/images/idle_5.png");
	idle[2] = iLoadImage("./assets/images/idle_9.png");
	idle[3] = iLoadImage("./assets/images/idle_12.png");
	invertedIdle[0] = iLoadImage("./assets/images/invertedIdle_1.png");
	invertedIdle[1] = iLoadImage("./assets/images/invertedIdle_5.png");
	invertedIdle[2] = iLoadImage("./assets/images/invertedIdle_9.png");
	invertedIdle[3] = iLoadImage("./assets/images/invertedIdle_12.png");
	jump[0] = iLoadImage("./assets/images/jump_1.png");
	jump[1] = iLoadImage("./assets/images/jump_3.png");
	jump[2] = iLoadImage("./assets/images/jump_9.png");
	jump[3] = iLoadImage("./assets/images/jump_10.png");
	jump[4] = iLoadImage("./assets/images/jump_20.png");
	jump[5] = iLoadImage("./assets/images/jump_22.png");
	invertedJump[0] = iLoadImage("./assets/images/Invertedjump_1.png");
	invertedJump[1] = iLoadImage("./assets/images/Invertedjump_3.png");
	invertedJump[2] = iLoadImage("./assets/images/Invertedjump_9.png");
	invertedJump[3] = iLoadImage("./assets/images/Invertedjump_10.png");
	invertedJump[4] = iLoadImage("./assets/images/Invertedjump_20.png");
	invertedJump[5] = iLoadImage("./assets/images/Invertedjump_22.png");
	run[0] = iLoadImage("./assets/images/run_1.png");
	run[1] = iLoadImage("./assets/images/run_2.png");
	run[2] = iLoadImage("./assets/images/run_3.png");
	run[3] = iLoadImage("./assets/images/run_4.png");
	run[4] = iLoadImage("./assets/images/run_5.png");
	run[5] = iLoadImage("./assets/images/run_6.png");
	run[6] = iLoadImage("./assets/images/run_7.png");
	run[7] = iLoadImage("./assets/images/run_8.png");
	run[8] = iLoadImage("./assets/images/run_9.png");
	run[9] = iLoadImage("./assets/images/run_10.png");
	invertedRun[0] = iLoadImage("./assets/images/Invertedrun_1.png");
	invertedRun[1] = iLoadImage("./assets/images/Invertedrun_2.png");
	invertedRun[2] = iLoadImage("./assets/images/Invertedrun_3.png");
	invertedRun[3] = iLoadImage("./assets/images/Invertedrun_4.png");
	invertedRun[4] = iLoadImage("./assets/images/Invertedrun_5.png");
	invertedRun[5] = iLoadImage("./assets/images/Invertedrun_6.png");
	invertedRun[6] = iLoadImage("./assets/images/Invertedrun_7.png");
	invertedRun[7] = iLoadImage("./assets/images/Invertedrun_8.png");
	invertedRun[8] = iLoadImage("./assets/images/Invertedrun_9.png");
	invertedRun[9] = iLoadImage("./assets/images/Invertedrun_10.png");
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
	lvl = 2;
	iStart();
	return 0;
}