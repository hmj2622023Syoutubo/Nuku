#include "DxLib.h"
#include <string>

const int WIDTH = 1200, HEIGHT = 720;
const int FPS = 60;

enum SCENE {START,NUKU,TRAINING,SEARCH};
enum CHARACTER { GFATHER, GMOTHER, KIDS, DOG, CAT, MOUSE };

int Scene = SEARCH;
const int TurnipNumber = 20; // 画面上に表示するかぶの数
const int Character = 6; // キャラクターの数
int turnipX[TurnipNumber]; // かぶの座標
int turnipY[TurnipNumber]; // かぶの座標
int timer;
int mouseX, mouseY; // マウスの座標
int CharacterX[Character], CharacterY[Character]; // キャラクターの座標
int gachaX = 0,gachaY = 0; // ガチャの座標
int TurnipDistanceX, TurnipDistanceY; // キャラクターとかぶの距離
int clickstate; 
int precClickstate;
int	TurnipState[TurnipNumber];
int GachaState;
int imageTurnip;
int imageTurnipUp;
int imageTurnipMidle;
int imageCharacter[Character];
int imageSoil;
int imageGacha;
int imageGachaopen;
int imageGachaclose;
int BGM, moneySE,NukuSE,GachaSE;
int clickcount[TurnipNumber];
int Charactercount[Character];
int haveturnip = 100; // かぶの所持数
int NukuSpeedLevel = 1;
int NukuSpeed = 1;
int NukuUpLevel = 1;
int NukuUp = 1;
int CharacterSpeed[Character];
int CharacterSpeedLevel[Character];
int CharacterNukuSpeed[Character];
int CharacterNukuSpeedLevel[Character];
int CharacterNukuUp[Character];
int CharacterNukuUpLevel[Character];
int GachaponSpeed = 10;
int TurnipPointNukuSpeed[Character];
int TurnipPointSpeed[Character];
int TurnipPointNukuUp[Character];
int TurnipPointGacha = 100;
int TurnipPointNukuSpeedPlayer = 1;
int TurnipPointNukuUpPlayer = 100;
int CharacterTurnip[Character];
bool gachaopen = false;

int Charflg[Character] = { 1,0,0,0,0,0 };
int Select;
int DischargeCount[Character];
int page = 0;

// 文字列を表示する関数
void DrawText(int x, int y, int col, const char* txt, int val, int siz)
{
	SetFontSize(siz);
	DrawFormatString(x, y, col, txt, val);
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("抜く"); // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
	ChangeWindowMode(true); // ウィンドウモードで起動
	if (DxLib_Init() == -1)return -1; // ライブラリ初期化　エラーが起きたら終了
	SetBackgroundColor(255, 255, 255); // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

	// 画像読み込み
	imageTurnip = LoadGraph("image/turnip.png");
	imageTurnipUp = LoadGraph("image/turnipup.png");
	imageTurnipMidle = LoadGraph("image/turnipmidle.png");
	imageCharacter[GFATHER] = LoadGraph("image/grandfather.png");
	imageCharacter[GMOTHER] = LoadGraph("image/grandmother.png");
	imageCharacter[KIDS] = LoadGraph("image/grandchild.png");
	imageCharacter[DOG] = LoadGraph("image/dog.png");
	imageCharacter[CAT] = LoadGraph("image/cat.png");
	imageCharacter[MOUSE] = LoadGraph("image/mouse.png");
	imageSoil = LoadGraph("image/soil.png");
	imageGacha = LoadGraph("image/gachagacha.png");
	imageGachaopen = LoadGraph("image/gacha hiraki.png");
	imageGachaclose = LoadGraph("image/gacha toji.png");

	// BGMを読み込む
	BGM = LoadSoundMem("music/BGM.wav");
	moneySE = LoadSoundMem("music/お金.wav");
	NukuSE = LoadSoundMem("music/かぶ引き抜く .wav");
	GachaSE = LoadSoundMem("music/ガチャ開ける.wav");
	
	GachaState = imageGachaclose;
	for (int i = 0; i < TurnipNumber; i++) // かぶの初期位置と状態を設定
	{
		TurnipState[i] = imageTurnipUp;
		turnipX[i] = GetRand(WIDTH-200)+100;
		turnipY[i] = GetRand(HEIGHT-150)+100;
	}
	for (int i = 0; i < Character; i++) // 強化に必要なかぶの数を設定
	{
		TurnipPointNukuSpeed[i] = 1;
		TurnipPointSpeed[i] = 1;
		TurnipPointNukuUp[i] = 100;
	}
	for (int i = 0; i < Character; i++) // キャラクターの初期設定
	{
		CharacterTurnip[i] = GetRand(TurnipNumber - 1);
		CharacterX[i] = WIDTH / 2;
		CharacterY[i] = HEIGHT / 2;
		CharacterSpeed[i] = 1;
		CharacterSpeedLevel[i] = 1;
		CharacterNukuSpeed[i] = 1;
		CharacterNukuSpeedLevel[i] = 1;
		CharacterNukuUp[i] = 1;
		CharacterNukuUpLevel[i] = 1;
	}

	while (1)
	{
		ClearDrawScreen(); // 画面をクリアする
		GetMousePoint(&mouseX, &mouseY);
		precClickstate = clickstate;
		clickstate = GetMouseInput() & MOUSE_INPUT_LEFT;
		switch (Scene)
		{
		case START:
			DrawText(275, 100, 0x000000, "かぶをぬくげーむ", 0, 75);
			DrawText(500, 400, 0x000000, "クリック", 0, 50);
			DrawBox(500, 400, 700, 450, 0x000000, false);
			if (clickstate != 0 && precClickstate == 0)
			{
				if (mouseX >= 500 && mouseX <= 700 && mouseY >= 400 && mouseY <= 450)
				{
					Scene = NUKU;
					PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
				}
			}
			break;

		case NUKU:
			DrawExtendGraph(0, 0, WIDTH, HEIGHT, imageSoil, true);
			for (int i = 0; i < TurnipNumber; i++)
			{
				DrawExtendGraph(turnipX[i]-100, turnipY[i]-100, turnipX[i]+100, turnipY[i]+100, TurnipState[i], true);
				if (mouseX >= turnipX[i] -100 && mouseX <= turnipX[i] + 100 && mouseY >= turnipY[i]-100 && mouseY <= turnipY[i] + 100 && clickstate != 0)
				{
					clickcount[i]+= NukuSpeed;
					DrawBox(mouseX-100, mouseY+25, mouseX + 100, mouseY + 50, 0x000000, false);
					DrawBox(mouseX-100, mouseY+25, mouseX-100 + clickcount[i], mouseY + 50, 0x00ffff, true);
					if(clickcount[i] >= 100 && TurnipState[i] == imageTurnipUp)
					{
						TurnipState[i] = imageTurnipMidle;
					}
					if(clickcount[i] >= 200)
					{
						PlaySoundMem(NukuSE, DX_PLAYTYPE_BACK);
						haveturnip+=NukuUp;
						TurnipState[i] = imageTurnipUp;
						turnipX[i] = GetRand(WIDTH-200)+100;
						turnipY[i] = GetRand(HEIGHT-150)+100;
						clickcount[i] = 0;
						if(i == CharacterTurnip[GFATHER])
						{
							CharacterTurnip[GFATHER] = GetRand(TurnipNumber - 1);
						}
					}
				}
				else if(clickcount[i] >= 100)
				{
					clickcount[i] = 100;
				}
				else
				{
					clickcount[i] = 0;
				}
			}

			for (int i = 0; i < Character; i++)
			{
				if(Charflg[i] == 0)
				{
					continue;
				}
				TurnipDistanceX = turnipX[CharacterTurnip[i]] - CharacterX[i];
				TurnipDistanceY = turnipY[CharacterTurnip[i]] - CharacterY[i];

				if (TurnipDistanceX >= -CharacterSpeed[i] && TurnipDistanceX <= CharacterSpeed[i] && TurnipDistanceY >= -CharacterSpeed[i] && TurnipDistanceY <= CharacterSpeed[i])
				{
					Charactercount[i] += CharacterNukuSpeed[i];
					DrawBox(CharacterX[i] - 100, CharacterY[i] + 25, CharacterX[i] + 100, CharacterY[i] + 50, 0x000000, false);
					DrawBox(CharacterX[i] - 100, CharacterY[i] + 25, CharacterX[i] - 100 + Charactercount[i], CharacterY[i] + 50, 0x00ffff, true);
					if (Charactercount[i] >= 100 && TurnipState[CharacterTurnip[i]] == imageTurnipUp)
					{
						TurnipState[CharacterTurnip[i]] = imageTurnipMidle;
					}
					if (Charactercount[i] >= 200)
					{
						PlaySoundMem(NukuSE, DX_PLAYTYPE_BACK);
						TurnipState[CharacterTurnip[i]] = imageTurnipUp;
						turnipX[CharacterTurnip[i]] = GetRand(WIDTH - 200) + 100;
						turnipY[CharacterTurnip[i]] = GetRand(HEIGHT - 150) + 100;
						CharacterTurnip[i] = GetRand(TurnipNumber - 1);
						haveturnip += CharacterNukuUp[i];
						Charactercount[i] = 0;
					}
				}
				if (TurnipDistanceX > -CharacterSpeed[i])
				{
					CharacterX[i] += CharacterSpeed[i];
				}
				if (TurnipDistanceX < CharacterSpeed[i])
				{
					CharacterX[i] -= CharacterSpeed[i];
				}
				if (TurnipDistanceY > -CharacterSpeed[i])
				{
					CharacterY[i] += CharacterSpeed[i];
				}
				if (TurnipDistanceY < CharacterSpeed[i])
				{
					CharacterY[i] -= CharacterSpeed[i];
				}
				DrawExtendGraph(CharacterX[i] - 100, CharacterY[i] - 200, CharacterX[i] + 100, CharacterY[i], imageCharacter[i], true);
			}
			DrawBox(25, 25, 300, 50, 0x000000, true);
			DrawText(50, 25, 0xffffff, "かぶの数: %d", haveturnip, 25);
			DrawBox(WIDTH - 110, HEIGHT - 55, WIDTH - 40, HEIGHT - 20, 0x000000, true);
			DrawText(WIDTH -100, HEIGHT-50, 0xffffff, "強化",0, 25);
			DrawBox(WIDTH - 210, HEIGHT - 55, WIDTH - 140, HEIGHT - 20, 0x000000, true);
			DrawText(WIDTH -200, HEIGHT-50, 0xffffff, "探す",0, 25);
			
			if(clickstate != 0 && precClickstate == 0)
			{
				if(mouseX >= WIDTH - 110 && mouseX <= WIDTH - 40 && mouseY >= HEIGHT - 55 && mouseY <= HEIGHT - 20)
				{
					Scene = TRAINING;
				}
				if(mouseX >= WIDTH - 210 && mouseX <= WIDTH - 140 && mouseY >= HEIGHT - 55 && mouseY <= HEIGHT - 20)
				{
					Scene = SEARCH;
				}
			}
			break;

		case TRAINING:
			DrawBox(25, 25, 300, 50, 0x000000, true);
			DrawText(50, 25, 0xffffff, "かぶの数: %d", haveturnip, 25);
			DrawText(50, 50, 0x000000, "プレイヤーがかぶを抜く速度アップ", 0, 30);
			DrawText(600, 50, 0x000000, "レベル%d", NukuSpeedLevel, 30);
			DrawText(790, 50, 0x000000, ": %d", TurnipPointNukuSpeedPlayer, 30);
			DrawText(1130, 50, 0x000000, "↑up", 0, 30);
			DrawBox(1130, 50, 1200, 80, 0x000000, false);
			DrawBox(750, 50, 780, 80, 0x000000, true);
			DrawExtendGraph(750, 50, 780, 80, imageTurnip, true);
			DrawText(50, 100, 0x000000, "プレイヤーが抜くかぶの数アップ", 0, 30);
			DrawText(600, 100, 0x000000, "レベル%d", NukuUpLevel, 30);
			DrawText(790, 100, 0x000000, ": %d", TurnipPointNukuUpPlayer, 30);
			DrawText(1130, 100, 0x000000, "↑up", 0, 30);
			DrawBox(1130, 100, 1200, 130, 0x000000, false);
			DrawBox(750, 100, 780, 130, 0x000000, true);
			DrawExtendGraph(750, 100, 780, 130, imageTurnip, true);
			
			DrawText(50, 200, 0x000000, "かぶを抜く速度アップ", 0, 30);
			DrawText(600, 200, 0x000000, "レベル%d", CharacterNukuSpeedLevel[page], 30);
			DrawText(790, 200, 0x000000, ": %d", TurnipPointNukuSpeed[page], 30);
			DrawText(1130, 200, 0x000000, "↑up", 0, 30);
			DrawBox(1130, 200, 1200, 230, 0x000000, false);
			DrawBox(750, 200, 780, 230, 0x000000, true);
			DrawExtendGraph(750, 200, 780, 230, imageTurnip, true);
			DrawText(50, 250, 0x000000, "移動速度アップ", 0, 30);
			DrawText(600, 250, 0x000000, "レベル%d", CharacterSpeedLevel[page], 30);
			DrawText(790, 250, 0x000000, ": %d", TurnipPointSpeed[page], 30);
			DrawText(1130, 250, 0x000000, "↑up", 0, 30);
			DrawBox(1130, 250, 1200, 280, 0x000000, false);
			DrawBox(750, 250, 780, 280, 0x000000, true);
			DrawExtendGraph(750, 250, 780, 280, imageTurnip, true);
			DrawText(50, 300, 0x000000, "抜くかぶの数アップ", 0, 30);
			DrawText(600, 300, 0x000000, "レベル%d", CharacterNukuUpLevel[page], 30);
			DrawText(790, 300, 0x000000, ": %d", TurnipPointNukuUp[page], 30);
			DrawText(1130, 300, 0x000000, "↑up", 0, 30);
			DrawBox(1130, 300, 1200, 330, 0x000000, false);
			DrawBox(750, 300, 780, 330, 0x000000, true);
			DrawExtendGraph(750, 300, 780, 330, imageTurnip, true);

			DrawText(1100, 650, 0x000000, "戻る", 0, 30);
			DrawBox(1100, 650, 1160, 680, 0x000000, false);
			DrawExtendGraph(400, 450, 600, 680, imageCharacter[page], true);
			if (page != 0)
			{
				DrawText(50, HEIGHT/2, 0x000000, "←", 0, 30);
				DrawBox(50, HEIGHT / 2, 80, HEIGHT / 2 + 30, 0x000000, false);
			}
			if(page != Character - 1)
			{
				DrawText(1150, HEIGHT / 2, 0x000000, "→", 0, 30);
				DrawBox(1150, HEIGHT / 2, 1180, HEIGHT / 2 + 30, 0x000000, false);
			}
			if (clickstate != 0 && precClickstate == 0)
			{
				if (mouseX >= 1130 && mouseX <= 1200 && mouseY >= 50 && mouseY <= 80) // プレイヤーの抜く速度アップ
				{
					if(TurnipPointNukuSpeedPlayer <= haveturnip) 
					{
						NukuSpeedLevel++;
						NukuSpeed++;
						haveturnip-=TurnipPointNukuSpeedPlayer;
						TurnipPointNukuSpeedPlayer+= NukuSpeedLevel * 2;
						PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					}
				}
				if (mouseX >= 1130 && mouseX <= 1200 && mouseY >= 100 && mouseY <= 130)
				{
					if (TurnipPointNukuUpPlayer <= haveturnip)
					{
						NukuUpLevel++;
						NukuUp++;
						haveturnip -= TurnipPointNukuUpPlayer;
						TurnipPointNukuUpPlayer += NukuUpLevel * 10;
						PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					}
				}
				if(mouseX >= 1130 && mouseX <= 1200 && mouseY >= 200 && mouseY <= 230)
				{
					if(TurnipPointNukuSpeed[page] <= haveturnip)
					{
						CharacterNukuSpeedLevel[page]++;
						CharacterNukuSpeed[page]++;
						haveturnip-=TurnipPointNukuSpeed[page];
						TurnipPointNukuSpeed[page] += CharacterNukuSpeedLevel[page] * 2;
						PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					}
				}
				if (mouseX >= 1130 && mouseX <= 1200 && mouseY >= 250 && mouseY <= 280)
				{
					if(TurnipPointSpeed[page] <= haveturnip)
					{
						CharacterSpeedLevel[page]++;
						CharacterSpeed[page]++;
						haveturnip-= TurnipPointSpeed[page];
						TurnipPointSpeed[page] += CharacterSpeedLevel[page]*2;
						PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					}
				}
				if (mouseX >= 1130 && mouseX <= 1200 && mouseY >= 300 && mouseY <= 330)
				{
					if (TurnipPointNukuUp[page] <= haveturnip)
					{
						CharacterNukuUpLevel[page]++;
						CharacterNukuUp[page]++;
						haveturnip -= TurnipPointNukuUp[page];
						TurnipPointNukuUp[page] += CharacterNukuUpLevel[page]*10;
						PlaySoundMem(moneySE, DX_PLAYTYPE_BACK);
					}
				}

				if (page != Character - 1)
				{
					if (mouseX >= 1150 && mouseX <= 1180 && mouseY >= HEIGHT / 2 && mouseY <= HEIGHT / 2 + 30)
					{
						page++;
					}
				}
				if (page != 0)
				{
					if (mouseX >= 50 && mouseX <= 80 && mouseY >= HEIGHT / 2 && mouseY <= HEIGHT / 2 + 30)
					{
						page--;
					}
				}

				if (mouseX >= 1100 && mouseX <= 1160 && mouseY >= 650 && mouseY <= 680)
				{
					Scene = NUKU;
				}
			}
			break;

		case SEARCH:
			DrawExtendGraph(400, 0, 800, 500, imageGacha, true);
			DrawText(50, 25, 0x000000, "かぶの数: %d", haveturnip, 25);
			DrawBox(560, 610, 590, 640, 0x000000, true);
			DrawExtendGraph(560, 610, 590, 640, imageTurnip, true);
			DrawText(580, 650, 0x00000,"回す", 0, 30);
			DrawBox(575, 650, 645, 680, 0x000000, false);
			DrawText(600, 610, 0x000000, ": %d", TurnipPointGacha, 30);
			DrawText(1100, 650, 0x000000, "戻る", 0, 30);
			DrawBox(1100, 650, 1160, 680, 0x000000, false);
			DrawText(50, 100, 0x000000, "排出した数", 0, 20);
			for(int i = 0; i < Character; i++)
			{
				DrawExtendGraph(50, 130 + 100 * i, 150, 200 + 100 * i, imageCharacter[i], true);
				DrawText(150, 160 + 100 * i, 0x000000, ": %d", DischargeCount[i], 20);
			}
			if(clickstate != 0 && precClickstate == 0)
			{
				if(mouseX >= 575 && mouseX <= 645 && mouseY >= 650 && mouseY <= 680)
				{
					if (TurnipPointGacha <= haveturnip && gachaopen == false)
					{
						gachaopen = true;
						haveturnip -= TurnipPointGacha;
						Select = GetRand(Character - 1);
						DischargeCount[Select]++;
						if (DischargeCount[Select] <= 10)
						{
							if (Charflg[Select] == 1)
							{
								CharacterNukuSpeed[Select]++;
								CharacterSpeed[Select]++;
								CharacterNukuUp[Select]++;
							}
							else
							{
								Charflg[Select] = 1;
							}
						}
						else
						{
							DischargeCount[Select]--;
							haveturnip += 50;
						}
					}
				}
				if (mouseX >= 1100 && mouseX <= 1160 && mouseY >= 650 && mouseY <= 680)
				{
					Scene = NUKU;
				}
			}
			if(gachaopen == true)
			{
				DrawExtendGraph(gachaX + 450, gachaY -300,gachaX + 750, gachaY, GachaState, true);
				if (gachaY < 550)
				{
					gachaY += GachaponSpeed;
				}
				if (gachaY == 550)
				{
					timer++;
					if (timer < 100)
					{
						if(timer % 10 < 5)
						{
							gachaX +=1;
						}
						else
						{
							gachaX -= 1;
						}
					}
					if (timer == 100)
					{
						PlaySoundMem(GachaSE, DX_PLAYTYPE_BACK);
					}
					if (timer >= 100)
					{
						GachaState = imageGachaopen;
						DrawExtendGraph(475, 300, 775, 500, imageCharacter[Select], true);
					}
					if(timer >= 150)
					{
						gachaopen = false;
						GachaState = imageGachaclose;
						timer = 0;
						gachaX = 0;
						gachaY = 0;
					}
				}
			}
			break;
		}
		ScreenFlip(); // 裏画面の内容を表画面に反映させる
		WaitTimer(1000 / FPS); // 一定時間待つ
		if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break; // ESCキーが押されたら終了
	}
	DxLib_End(); // DXライブラリ使用の終了処理
	return 0; // ソフトの終了
}