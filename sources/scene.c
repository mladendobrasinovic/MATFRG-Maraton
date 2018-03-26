#include "maraton.h"

GLfloat avatar_h = 1;			   /* Visina avatara */
GLfloat field_w = M_SQRT_2, field_h = 0.3; /* Sirina i visina polja */

void draw_field(enum field fld)
{
    GLfloat white_base[] = {.99, .99, .99, 1};
    GLfloat gray_base[] = {.22, .22, .22, 1};
    GLfloat *field_base = NULL;

    /* Odredjujemo kojeg je materijala polje */
    if(fld == FLD_W)
	field_base = white_base;
    else if(fld == FLD_G)
	field_base = gray_base;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, field_base);
    glShadeModel(GL_SMOOTH);
    
    /* Skaliranje se odnosi na dimenzije polja */
    glPushMatrix();
    glScalef(field_w, field_h, field_w);
    glutSolidCube(1.0);
    glPopMatrix();
}

void draw_avatar()
{
    GLfloat material_base[] = {.24, .82, .27, 1};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_base);
    glShadeModel(GL_SMOOTH);

    glPushMatrix();
    glTranslatef(avatar_x_shift * field_w, 0, 0);
    glutSolidCube(1.00);
    glPopMatrix();
}

void draw_track()
{
    /* Zelimo da avatar stoji ravno nad stazom */
    GLfloat track_y_offset = -(avatar_h + field_h) / 2;
    /* Crtanje pocinje od prvog polja, zelimo da je koordinatni pocetak iznad
     * treceg */
    GLfloat track_x_offset = -(2 * field_w);
    int i, j;

    glPushMatrix();
    glTranslatef(track_x_offset,
		 track_y_offset,
		 avatar_z_shift * field_w);

    for(i = 0; i < SEG_LENGTH; i++)
    {
	for(j = 0; j < TRK_WIDTH; j++)
	{
	    enum field curr_field = curr_seg[i][j];
	    
	    if(curr_field == FLD_X) /* Nema polja na ovoj poziciji */
		continue;
	    
	    glPushMatrix();
	    glTranslatef(field_w * j, 0, -field_w * i);
	    draw_field(curr_field);
	    glPopMatrix();
	}
    }
    glPopMatrix();
}

void set_scene()
{
    GLfloat directional_position[] = {2.4, 4.5, 1.6, 0};
    GLfloat directional_diffuse[] = {.99 , .97, .84, 1};
    GLfloat ambient_light[] = {.20, .21, .33, 1};

    GLdouble dist = 3.0;
    GLdouble z_shift = -(2 * M_SQRT_2);
    GLdouble dist_z = dist + z_shift;

    /* Postavi matricu pogleda i modela, koje zajedno transformisu svet u nas
     * koordinatni sistem, u odnosu na kameru */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(dist, dist, dist_z,	/* Lokacija kamere, x == y == z */
    	      0, 0, z_shift,		/* Tacka ka kojoj je kamera okrenuta */
    	      0, 1, 0);		        /* Y osu gledamo kao na uspravnu */
    
    /* Postavi staticko svetlo, ovo radimo sa postavljenom GL_MODELVIEW
     * matricom */
    glLightfv(GL_LIGHT0, GL_POSITION, directional_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, directional_diffuse);

    /* Postavi ambijentalno svetlo */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
    
    glEnable(GL_LIGHT0);
}

void draw_scene()
{
    draw_track();
    draw_avatar();
}
