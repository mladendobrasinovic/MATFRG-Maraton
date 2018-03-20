#include "maraton.h"

void draw_track()
{
    GLfloat avatar_h = 1, field_h = 0.3; /* Visine avatara i polja staze (po
					 * Y-koordinati) */
    GLfloat field_w = M_SQRT_2;	/* Sirina polja */
    /* Zelimo da avatar stoji ravno nad stazom */
    GLfloat track_y_offset = -(avatar_h + field_h) / 2;
    /* Iscrtavanje pocinje od prvog polja, zelimo da je avatar nad trecim */
    GLfloat track_x_offset = -(2 * field_w);
    
    GLfloat white_base[] = {0.8, 0.8, 0.8, 1};
    GLfloat gray_base[] = {0.2, 0.2, 0.2, 1};
    int i, j;
    
    glPushMatrix();
    /* glScalef(M_SQRT_2, 1, M_SQRT_2); */
    glTranslatef(track_x_offset, track_y_offset, avatar_shift * field_w);

    for(i = 0; i < SEG_LENGTH; i++)
    {
	glPushMatrix();
	glTranslatef(0, 0, -field_w * i);
	
	for(j = 0; j < TRK_WIDTH; j++)
	{
	    GLfloat *field_base;
	    if(!(curr_seg[i][j] == FLD_O))
		continue;
	       
	    glPushMatrix();
	    glTranslatef(field_w * j, 0, 0);
	    
    	    /* XOR bitova na polju jedinice (parnosti) predstavlja funkciju boje
	     * sahovnice */
	    if((j % 2 == 1) ^ (i % 2 == 0))
		field_base = gray_base;
	    else
		field_base = white_base;
	    
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, field_base);
	    glMaterialfv(GL_FRONT, GL_AMBIENT, field_base);
	    /* Skaliranje se odnosi na dimenzije polja */
	    glScalef(field_w, field_h, field_w);
	    glutSolidCube(1.0);
	    
	    glPopMatrix();
	}
	glPopMatrix();
    }
    glPopMatrix();
}

void draw_scene()
{
    /* Zelimo lepu narandzastu boju avatara */
    GLfloat material_base[] = {0.9, 0.25, 0.33, 1}; /* Osnovna boja za difuzno i
						 * ambijentalno osvetljenje */
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_base);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_base);    
    
    glShadeModel(GL_FLAT);
    glutSolidCube(1.00);

    draw_track();
}

void set_scene()
{
    GLfloat light_pos[] = {2, 3, 1.5, 1}; /* Pozicija ne predstavlja direkciono
					  * svetlo (w = 1) */
    GLfloat light_diff[] = {0.85, 0.85, 0.85, 1};
    GLfloat light_ambi[] = {0.3, 0.3, 0.3, 1};

    /* Postavi staticko svetlo */
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambi);
    glEnable(GL_LIGHT0);
}
