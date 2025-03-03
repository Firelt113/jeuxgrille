#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <memory>
#include <Windows.h>
#define RAMPLIE 0x000000
#define VIDE 0x000001
#define  WINWIDE 2000
#define  WINHEIGHT 1080
#define  ENVIE 0x00FF00
#define  MORT 0x000000

//Sprite
#define LARGEUR_DE_BASE_SPRITE 64
#define HAUTEUR_DE_BASE_SPRITE 64
#define NB_TEXTURE 9
#define CODE_BETON 1
#define CODE_BOIS 2

#define VITTESSE(x) (x*3);



typedef struct BUFFER {
	//std::shared_ptr<uint32_t[]> memory;
	void* memory;
	int width;
	int height;
}BUFFER;

typedef struct Image {
	int** tab;
	int pixheigt;
	int pixwide;
	int maxheight;
	int maxwide;
};

typedef struct rect4coin {
	tagPOINT haut_gauche;
	tagPOINT haut_droit;
	tagPOINT bas_gauche;
	tagPOINT bas_droit;
};

typedef unsigned int u32;

//0x000000 => VIDE
//0x000000 => RAMPLIE
typedef int RAMPLISSAGE;

/*
ClearScreen

def: la fonction ramplie la buffer de la couleur donner

BUFFER buffer: struct contenant le tableau des pixel

unsigned int: la couleur qui sera assigner a toutes les cases du buffer
*/
void ClearScreen(BUFFER buffer,unsigned int couleur);

/*
OuDansBuffer

def: retourne lemplacement ou se situe le pixel x,y dans le buffer en int
	 et verifie que le pixel qui doit etre acceder se situe dans le tableau.
	 Si ce nest pas le cas la fonction retourne -1.

int x: position en x de la case qui doit etre changer

int y: Position en y de la case qui doit etre changer

BUFFER buffer: struct contenant le tableau des pixel

retour: retourne lemplacement du pixel dans le buffer ou -1 
	   si lemplacement demander nest pas dans le buffer
*/
int OuDansBuffer(BUFFER buffer, int x, int y);

/*
EcrireDansBuffer

def: �crie dans le buffer a l'emplacement donn�e la couleur donn�e

BUFFER buffer: struct contenant le tableau des pixel

int PosPixel: Emplacement dans le buffer ou la couleur doit �tre �crite.

unsigne int couleur: couleur en RGB qui sera inscrite.
*/
void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur);

/*
Line

def: �crit une s�rie de couleur sous forme de ligne dans le buffer du x1 y1 au x2 y2
	 de la couleur sp�cifier.
	 
int x1: l'emplacement en x du premier point.

int y1: l'emplacement en y du premier point.

int x2: l'emplacement en x du deuxi�me point.

int y2: l'emplacement en y du deuxi�me point.

unsigne int couleur: couleur en RGB qui sera inscrite.
*/
void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur);


void rectangle(BUFFER buffer ,RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur);

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur);

static int** cree_tableau(void);

Image cree_image(int pix_whide, int pix_heigt);

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int code, unsigned int couleur, int*** sprite);

int verification_nb_blanc(Image image, int x, int y);

void delay(unsigned int nb_tmp);

void copierlimage(Image image, Image image_copie);

void ImageDansBuffer(Image image, BUFFER buffer, tagRECT rectangle, int*** tab_sprite);

int hypotenuse(double x, double y);

int** obtenirSpriteTableau(const char* nom_fichier_sprite);

void spriteDansBuffer(rect4coin rect, int** sprite, BUFFER buffer, int indice_grandeur);

void spriteDansBufferMeilleur(rect4coin rect, int** sprite, BUFFER buffer, int indice_grandeur);