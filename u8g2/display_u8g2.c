#include "main.h"
#include "u8g2.h"
#include "display_u8g2.h"
#include <stdbool.h>
#include <stdio.h>

//Vari�veis para o menu e display:
int itemSelecionado = 0; // 0 = acumulada; 1 = gas; 2 = configuracoes. || 0 max; 1 min; 2 voltar.
int gas = 0; //menu e display
int currentPage = 1; //menu e display
bool editMax = false;
bool editMin = false;

//Vari�veis l�gica
float valorMax;
float valorMin;
int acumulada = 99000, inst = 2;

//BITMAPS:
static unsigned char bola[9][20] = {
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0x84, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0xfe, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0xfc, 0x00, 0xfe, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00}};

static unsigned char acumulada_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x0f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f };

static unsigned char co2_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xfe, 0xff,
   0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f };

static unsigned char caixa_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x02,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x18, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07 };

static unsigned char reset_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
   0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xfe, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xfc,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00 };

static unsigned char labsolda[] = {
   0xe0, 0x03, 0xf0, 0x07, 0x78, 0x08, 0x3f, 0x71, 0x21, 0x43, 0x61, 0x46,
   0xc1, 0x44, 0x87, 0x7c, 0x08, 0x0e, 0xf0, 0x07, 0xe0, 0x03 };

static unsigned char seta[] = {
   0x0c, 0x06, 0x7f, 0x06, 0x0c };
	 
void draw(u8g2_t *u8g2)
{
	unsigned long drawDelay = HAL_GetTick();
	
//Variaveis para o display
	static int frameBola = 0;
	static bool downtoup = true;
	static unsigned long updateFrameBola = 0;
	static unsigned long blink = 0;
	
//Configuracao do display
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFont(u8g2, u8g2_font_6x10_tr);
	u8g2_SetBitmapMode(u8g2, 1);
	
//Animacao bola
	if (HAL_GetTick() - updateFrameBola >= 200) {
	frameBola += (downtoup ? 1 : -1);
		if (frameBola >= 8) {
			downtoup = false;
      } else if (frameBola <= 0) {
			downtoup = true;
      }
	updateFrameBola = HAL_GetTick();
	}
		u8g2_DrawXBM(u8g2, 118, 54, 10, 10, bola[frameBola]);
	
//Logo Labsolda
		u8g2_DrawXBM(u8g2, 113, 0, 15, 11, labsolda);
	
//P�gina 1
	if (currentPage == 1){

	//Acumulada e instantanea
		u8g2_DrawStr(u8g2, 3, 8, "Vazao");
		//Acumulada
		char acumuladaStr[20];
		sprintf(acumuladaStr, "Acumulada: %.1fL", valorMax);
		u8g2_DrawStr(u8g2, 5, 20, acumuladaStr);
		//Instantanea
		char instantaneaStr[20];
		sprintf(instantaneaStr, "Instant.: %.1fL/s", valorMin);
		u8g2_DrawStr(u8g2, 5, 32, instantaneaStr);
		
	//Argonio ou CO2
		u8g2_DrawStr(u8g2, 3, 46, "Gas: ");
		
		
		if (gas == 0){
			u8g2_DrawStr(u8g2, 33, 46, "CO2");
		} else {
			u8g2_DrawStr(u8g2, 33, 46, "Argonio");
		}
			
	//Configuracao
			u8g2_DrawStr(u8g2, 3, 60, "Configuracoes");

	//Chamada de bitmaps para o menu:
		//Acumulada
		if (itemSelecionado == 0){u8g2_DrawXBM(u8g2, 2, 11, 126, 12, acumulada_selecao);}

		//Gas
		else if (itemSelecionado == 1){
			if (gas == 0) {
				u8g2_DrawXBM(u8g2, 0, 37, 55, 12, co2_selecao);
				u8g2_DrawXBM(u8g2, 56, 40, 7, 5, seta); //Seta do menu
			}
			else {
				u8g2_DrawXBM(u8g2, 0, 37, 85, 12, caixa_selecao);
				u8g2_DrawXBM(u8g2, 86, 40, 7, 5, seta); //Seta do menu
			}
    }
		
		//Configuracao
		else if (itemSelecionado == 2){
			u8g2_DrawXBM(u8g2, 0, 51, 85, 12, caixa_selecao);
			u8g2_DrawXBM(u8g2, 86, 54, 7, 5, seta); //Seta do menu
		}
	} //Fim pagina 1

	
//Pagina 2
	else if (currentPage == 2){
		
	//Voltar
		u8g2_DrawStr(u8g2, 3, 60, "Voltar");
		
	//Minimo
		char minStr[20];
		sprintf(minStr, "Minimo: %.1f", valorMin);
		u8g2_DrawStr(u8g2, 3, 21, minStr);
		
	//Maximo
		char maxStr[20];
		sprintf(maxStr, "Maximo: %.1f", valorMax);
		u8g2_DrawStr(u8g2, 3, 9, maxStr);
		
	//Resetar configuracoes
		u8g2_DrawStr(u8g2, 3, 34, "Resetar configs.");//7 de altura
		
		u8g2_DrawStr(u8g2, 3, 47, "Salvar configuracoes");

	//Chamada de bitmaps para o menu:
		//Voltar
		if (itemSelecionado == 2){
			u8g2_DrawXBM(u8g2, 0, 51, 55, 12, co2_selecao);
			u8g2_DrawXBM(u8g2, 56, 54, 7, 5, seta);
		}
		
		//Minimo caixa de selecao
		else if (itemSelecionado == 1){
			u8g2_DrawXBM(u8g2, 0, 12, 85, 12, caixa_selecao);
		}
		
		//Maximo caixa de selecao
		else if (itemSelecionado == 0){
			u8g2_DrawXBM(u8g2, 0, 0, 85, 12, caixa_selecao);
		}
		
		//Reset config
		else if (itemSelecionado == 3){
			u8g2_DrawXBM(u8g2, 0, 25, 97, 12, reset_selecao);
			u8g2_DrawXBM(u8g2, 98, 28, 7, 5, seta);
		}
		
		//Salvar config
		else if (itemSelecionado == 4){
			u8g2_DrawXBM(u8g2, 0, 38, 97, 12, reset_selecao);
		}
		
		//Maximo seta de selecao
		if (!editMax && itemSelecionado == 0){
			u8g2_DrawXBM(u8g2, 86, 3, 7, 5, seta);
		} else if (editMax) {
			if (HAL_GetTick() - blink <= 1000){
				u8g2_DrawXBM(u8g2, 86, 3, 7, 5, seta);
			} 
			else if (HAL_GetTick() - blink >= 1400){
				blink = HAL_GetTick();
			}
		}
		
		//Minimo seta de selecao
		if (!editMin && itemSelecionado == 1){
			u8g2_DrawXBM(u8g2, 86, 21, 7, 5, seta);
		} else if (editMin) {
			if (HAL_GetTick() - blink <= 1000){
				u8g2_DrawXBM(u8g2, 86, 21, 7, 5, seta);
			} 
			else if (HAL_GetTick() - blink >= 1400){
				blink = HAL_GetTick();
			}
		}
			
	}//Fim pagina 2
		
//Envia buffer
	u8g2_SendBuffer(u8g2);
}

void logicaMenu(bool botaocima, bool botaoselect, bool botaobaixo)
{
	static unsigned long tempoItemUp = 0;
	static unsigned long tempoItemDown = 0;
	static unsigned long tempobotaoselect = 0;
	static unsigned long tempoEditMax = 0;
	static unsigned long tempoEditMin = 0;
	static unsigned long tempoAcumuladaRst = 0;
	
//Pagina 1
	if (currentPage == 1)
	{
	//Mover selecao do menu	
	if (botaocima && (HAL_GetTick() - tempoItemUp >= 100)){itemSelecionado -= 1; tempoItemUp = HAL_GetTick();}
	if (botaobaixo && (HAL_GetTick() - tempoItemDown >= 100)){itemSelecionado += 1; tempoItemDown = HAL_GetTick();}	
		
	//Selecionar
		if (botaoselect && (HAL_GetTick() - tempobotaoselect >= 100)){
		//Gas
			if (itemSelecionado == 1){
				if (gas == 0){gas = 1;} else {gas = 0;}
			}
		//Configuracao
			if (itemSelecionado == 2){currentPage = 2; itemSelecionado = 0;}
			tempobotaoselect = HAL_GetTick();
		}
	
	//Acumulada (zera apos 1.5s pressionado)
		if (itemSelecionado == 0)
		{
			if (botaoselect)
			{
				if (tempoAcumuladaRst == 0){tempoAcumuladaRst = HAL_GetTick();}
				else if (HAL_GetTick() - tempoAcumuladaRst >= 1500){acumulada = 0.0; tempoAcumuladaRst = 0;}
			} else {tempoAcumuladaRst = 0;}
		}
		
	}//Fim pagina 1
	
//Pagina 2
	else if (currentPage == 2){
	//Zera tempoAcumuladaRst para prevenir bugs
		tempoAcumuladaRst = 0;
		
	//Mover selecao menu (considerando editMax e editMin)
		if (botaocima && HAL_GetTick() - tempoItemUp >= 100 && !editMin && !editMax){itemSelecionado -= 1; tempoItemUp = HAL_GetTick();}
    if (botaobaixo && HAL_GetTick() - tempoItemDown >= 100 && !editMin && !editMax){itemSelecionado += 1; tempoItemDown = HAL_GetTick();}
		
	//Voltar
		if (itemSelecionado == 2 && botaoselect){currentPage = 1; itemSelecionado = 0;}
	
	//Editar valor maximo
		//Habilita edicao
		if (itemSelecionado == 0){
			if (botaoselect){
				if (tempobotaoselect == 0){
					editMax = !editMax; 
					tempobotaoselect = HAL_GetTick();
				}
			} else {tempobotaoselect = 0;}
		}
		//Edita o valor
		if (editMax){
				if (botaocima && valorMax < 9.99){	//Aumenta valor
						if (tempoItemUp == 0){tempoItemUp = HAL_GetTick();}
						else if (HAL_GetTick() - tempoItemUp >= 500 && valorMax < 9.49){valorMax += 0.5;}
						if (HAL_GetTick() - tempoEditMax >= 100 && HAL_GetTick() - tempoItemUp < 500) {valorMax += 0.1; tempoEditMax = HAL_GetTick();}
						tempoEditMax = HAL_GetTick();
						
				} else if (botaobaixo && valorMax > (valorMin + 0.01)) // Diminui valor
				{
					if (tempoItemDown == 0){tempoItemDown = HAL_GetTick();}
					else if (HAL_GetTick() - tempoItemDown >= 500 && valorMax > 0.51){valorMax -= 0.5;}
					if (HAL_GetTick() - tempoEditMax >= 100 && HAL_GetTick() - tempoItemDown < 500) {valorMax -= 0.1; tempoEditMax = HAL_GetTick();}
					tempoEditMax = HAL_GetTick();
				} else {tempoItemUp = 0; tempoItemDown = 0;}
		} 
		
	//Editar valor minimo
		//Habilita edicao
		if (itemSelecionado == 1){
			if (botaoselect){
				if (tempobotaoselect == 0){
					editMin = !editMin; 
					tempobotaoselect = HAL_GetTick();
				}
			} else {tempobotaoselect = 0;}
		}
		//Edita o valor
		if (editMin){
				if (botaocima && valorMin < (valorMax - 0.01)){ // Aumenta valor
					if (tempoItemUp == 0){tempoItemUp = HAL_GetTick();}
					else if (HAL_GetTick() - tempoItemUp >= 500 && valorMin < (valorMax - 0.51)){valorMin += 0.5;}
					if (HAL_GetTick() - tempoEditMin >= 100 && HAL_GetTick() - tempoItemUp < 500) {valorMin += 0.1; tempoEditMin = HAL_GetTick();}
					tempoEditMin = HAL_GetTick();
				
				}
				else if (botaobaixo && valorMin > 0.06){ // Diminui valor
					if (tempoItemDown == 0){tempoItemDown = HAL_GetTick();}
					else if (HAL_GetTick() - tempoItemDown >= 500 && valorMin > 0.51){valorMin -= 0.5;}
					if (HAL_GetTick() - tempoEditMin >= 100 && HAL_GetTick() - tempoItemDown < 500) {valorMin -= 0.1; tempoEditMin = HAL_GetTick();}
					tempoEditMin = HAL_GetTick();
					
				} else {tempoItemUp = 0; tempoItemDown = 0;}
		}		

	}//Fim da pagina 2
	
	if (itemSelecionado >= 4){itemSelecionado = 0;}
	else if (itemSelecionado <= -1){itemSelecionado = 2;}
}

void atualiza_u8g2(u8g2_t* u8g2, bool botaocima, bool botaoselect, bool botaobaixo, unsigned long* delaydesenho){
	logicaMenu(botaocima, botaoselect, botaobaixo);
	if (HAL_GetTick() - *delaydesenho >= 200){draw(u8g2); *delaydesenho = HAL_GetTick();}
}

void inicializa_u8g2(u8g2_t* u8g2, u8x8_msg_cb hw_spi, u8x8_msg_cb stm32){
	u8g2_Setup_st7920_s_128x64_f2(u8g2, U8G2_R0, hw_spi, stm32);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2, 0);
	
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}