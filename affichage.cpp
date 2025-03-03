#include "affichage.h"
#include <Windows.h>
#include <math.h>


void ClearScreen(BUFFER buffer, unsigned int couleur)
{
	unsigned int* pixel = (unsigned int*)buffer.memory;
	for (int y = 0; y < buffer.height; y++) {
		for (int x = 0; x < buffer.width; x++) {
			*pixel++ = 0x000000;
		}
	}
}

int OuDansBuffer(BUFFER buffer, int x, int y)
{
	int EmpTab;
	if(x > 100 && y > 100 && x < buffer.width - 100 && y < buffer.height-100)
	  return (y  * buffer.width )+ x;

	else {
		return -1;
	}

}

void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur)
{
	if (PosPixel < buffer.height * buffer.width && PosPixel >=0) {
		unsigned int* pixel = (unsigned int*)buffer.memory + PosPixel;
		*pixel = couleur;
	}
}

void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (abs(x1 - x2) >= abs(y1 - y2))
	{
		if (x2 - x1 > 0)
		{
			double a = ((double)y2 - (double)y1) / ((double)x2 - (double)x1);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x1; x < x2; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (x1 - x2 > 0)
		{
			double a;
			if (x1 - x2 == 0)
				a = buffer.height;

			else
				a = (double)y1 - (double)y2 / ((double)x1 - (double)x2);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x2; x < x1; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	else      //avec y comme independante
	{
		if (y2 - y1 > 0)
		{
			double a = ((double)x2 - (double)x1) / ((double)y2 - (double)y1);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y1; y < y2; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (y1 - y2 > 0)
		{
			double a = (double)x1 - (double)x2 / ((double)y1 - (double)y2);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y2; y < y1; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	
}

void rectangle(BUFFER buffer, RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (ramplissage == VIDE) {
		Line(buffer, x1, y1, x1, y2, couleur);
		Line(buffer, x1, y1, x2, y1, couleur);
		Line(buffer, x2, y1, x2, y2, couleur);
		Line(buffer, x1, y2, x2, y2, couleur);
	}
	else if(ramplissage == RAMPLIE)
	{
		unsigned int* pixel = (unsigned int*)buffer.memory;
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
	}
}

int** cree_tableau()
{
	int** tab = (int**)malloc(WINHEIGHT * sizeof(int*));

	for (int i = 0; i < WINHEIGHT; i++)
	{
		tab[i] = (int*)malloc(WINWIDE * sizeof(int));
	}

	return tab;
}

Image cree_image(int pix_whide, int pix_heigt)
{
	Image image;
	image.pixheigt = pix_heigt;
	image.pixwide = pix_whide;
	image.tab = cree_tableau();
	return image;
}

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur)
{
	for (int i = x - r; i < x + r; i++)
		for (int j = y - r; j < y + r; j++)
		{
			if (sqrt(pow(j - y, 2) + pow(i - x, 2)) <= r)
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, i, j), couleur);
		}
}

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int code, unsigned int couleur, int*** sprite)
{
	double ratio = (double)image.pixheigt / (double)image.pixwide;

	int type = code % 10;
	int type_top = (code / 10) % 10;
	int nb_etage = (code / 100);
	int indice_grandeur = HAUTEUR_DE_BASE_SPRITE / image.pixheigt;

	tagPOINT point1 = { point.x, point.y + image.pixheigt };
	tagPOINT point2 = { point.x + image.pixwide / 2, point.y + image.pixheigt / 2 };	//image.pixheigt - image.pixheigt/2 = + image.pixheigt / 2

	//cree les rectangle qui definice la zone du sprite
	rect4coin rect_sprite;
	rect4coin rect_sprite_gauche;
	rect_sprite.haut_gauche = point1;
	rect_sprite.haut_droit = point2;
	rect_sprite_gauche.haut_gauche = point2;
	rect_sprite_gauche.haut_droit = point1;
	rect_sprite_gauche.haut_droit.x += image.pixwide;

	if (nb_etage > 0) {
		for (int i = 0; i < nb_etage; i++) {
			if (i > 0) {
				rect_sprite.haut_gauche.y += image.pixheigt;
				rect_sprite.haut_droit.y += image.pixheigt;
				rect_sprite_gauche.haut_gauche.y += image.pixheigt;
				rect_sprite_gauche.haut_droit.y += image.pixheigt;
			}
			spriteDansBuffer(rect_sprite, sprite[type], buffer, indice_grandeur);
			spriteDansBuffer(rect_sprite_gauche, sprite[type], buffer, indice_grandeur);
		}
	}

	rect_sprite.bas_gauche = rect_sprite.haut_droit;
	rect_sprite.haut_droit.x = (image.pixwide / 2) + rect_sprite.haut_gauche.x;
	rect_sprite.haut_droit.y = (image.pixheigt / 2) + rect_sprite.haut_gauche.y;
	rect_sprite.bas_droit.x = (image.pixwide / 2) + rect_sprite.bas_gauche.x;
	rect_sprite.bas_droit.y = (image.pixheigt / 2) + rect_sprite.bas_gauche.y;

	spriteDansBufferMeilleur(rect_sprite, sprite[type_top], buffer, indice_grandeur);
}

int verification_nb_blanc(Image image, int x, int y)
{
	int nb = 0;
	for(int j = y-1; j< y + 2; j++)
		for (int i = x - 1; i < x + 2; i++)
		{
			if ((i != x || j != y)&& image.tab[j][i] == ENVIE)
			{
				nb++;
			}
		}
	return nb;
}

void delay(unsigned int nb_tmp)
{
	int i = 0;
	while (i < nb_tmp)
		i++;;
}

void copierlimage(Image image, Image image_copie)
{
	for(int j = 0; j< WINHEIGHT; j++)
		for (int i = 0; i < WINWIDE; i++) {
			image_copie.tab[j][i] = image.tab[j][i];
		}
}

void ImageDansBuffer(Image image, BUFFER buffer, tagRECT rectanglee, int*** tab_sprite)
{
	/*for (int j = 1; j < (buffer.height * 2 / image.pixheigt) - 1;j++)
		for (int i = (buffer.width * 2 / image.pixwide) * 1.05 - 2; i > 1;i--)
		{
			tagPOINT point;
			
			point.x = (i * ((double)image.pixwide / 2)) + (j * ((double)image.pixwide)/2) - (buffer.width/1.7) ;
			point.y = (buffer.height/2) + (i * ((double)image.pixheigt/2)) - (j * ((double)image.pixheigt/2)) ;

			if (i + rectanglee.left < WINWIDE && j + rectanglee.bottom < WINHEIGHT && j + rectanglee.bottom - (buffer.height / image.pixheigt) >= 0 && i + rectanglee.left - (buffer.width / image.pixwide) >=0 && point.x >= 0 && point.y > 20 && point.y < buffer.height - 20 && point.x < buffer.width - 80) {
				ecrire_point(buffer, image, point, image.tab[j + rectanglee.bottom - (buffer.height / image.pixheigt)][i + rectanglee.left - (buffer.width / image.pixwide)], 0xdddddd, tab_sprite[0]);
			}
		}*/
		int i_initial = (buffer.width / image.pixwide) * 1.05 - 2;
	int i_initial_max = i_initial * 2;
	int j_initial = 0;
	int n = 0;
	while (i_initial > 0) {
		int i = i_initial, j = j_initial;
		n++;
		while (i < i_initial_max) {
			tagPOINT point;

			point.x = (i * ((double)image.pixwide / 2)) + (j * ((double)image.pixwide) / 2) - (buffer.width / 1.7);
			point.y = (buffer.height / 2) + (i * ((double)image.pixheigt / 2)) - (j * ((double)image.pixheigt / 2));

			if (i + rectanglee.left < WINWIDE && j + rectanglee.bottom < WINHEIGHT && j + rectanglee.bottom - (buffer.height / image.pixheigt) >= 0 && i + rectanglee.left - (buffer.width / image.pixwide) >= 0 && point.x >= 0 && point.y > 20 && point.y < buffer.height - 20 && point.x < buffer.width - 80) {
				ecrire_point(buffer, image, point, image.tab[j + rectanglee.bottom - (buffer.height / image.pixheigt)][i + rectanglee.left - (buffer.width / image.pixwide)], 0xdddddd, tab_sprite);
			}
			i++;
			j++;
		}
		if (n % 2 == 0){
			i_initial--;
			i_initial_max--;
		}
		else
			j_initial++;
	}
}

int hypotenuse(double x, double y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

int** obtenirSpriteTableau(const char* nom_fichier_sprite) {
	int** tab = cree_tableau();

	FILE* add_fich = fopen(nom_fichier_sprite, "r");

	for (int i = 0; i < LARGEUR_DE_BASE_SPRITE * HAUTEUR_DE_BASE_SPRITE; i++) {
		int x= 0, y= 0;
		fscanf(add_fich, "%d %d", &x, &y);
		fscanf(add_fich, "%d  ", &tab[y][x]);
	}
	fclose(add_fich);
	return tab;
}

void spriteDansBuffer(rect4coin rect, int** sprite, BUFFER buffer, int indice_grandeur) {
	double ratio = ((double)rect.haut_droit.y - (double)rect.haut_gauche.y)/((double)rect.haut_droit.x - (double)rect.haut_gauche.x);

	for(int j = 0; j < HAUTEUR_DE_BASE_SPRITE; j = j+indice_grandeur)
		for (int i = 0; i < LARGEUR_DE_BASE_SPRITE; i = i + indice_grandeur) {
			int x = rect.haut_gauche.x +( i / indice_grandeur);
			int y = rect.haut_gauche.y - (j / indice_grandeur) + ((i / indice_grandeur) * ratio);
			EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), sprite[j][i]);
		}
}


void spriteDansBufferMeilleur(rect4coin rect, int** sprite, BUFFER buffer, int indice_grandeur) {
	double c = ((double)rect.bas_droit.y - (double)rect.haut_droit.y) / ((double)rect.bas_droit.x - (double)rect.haut_droit.x);
	double b = ((double)rect.bas_gauche.y - (double)rect.haut_gauche.y) / ((double)rect.bas_gauche.x - (double)rect.haut_gauche.x);
	
	int hauteur = HAUTEUR_DE_BASE_SPRITE / (indice_grandeur), largeur = LARGEUR_DE_BASE_SPRITE/indice_grandeur;
	int xi = rect.haut_gauche.x;
	int yi = rect.haut_gauche.y;
	int n = 0;
	while(n < (int)(hauteur*(1 + abs(b)))) {
		if (n % ((int)(1 / abs(b))+1) == 0) {
			yi--;
			n++;
		}
		else {
			xi++;
			n++;
		}
		double a = (double)((rect.haut_droit.y + (n * c)) - (rect.haut_gauche.y + (n * b)))/ (double)(rect.haut_droit.x - rect.haut_gauche.x);
		for (int i = 0; i < largeur; i++) {
			int x = xi + (i);
			int y = yi + (i) * a;

			EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), sprite[(int)(n * indice_grandeur/ (1 + abs(b)))][i * indice_grandeur]);
		}
	}
}