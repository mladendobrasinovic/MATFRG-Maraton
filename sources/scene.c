#include "maraton.h"

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
    glTranslatef(avatar.x * field_w, avatar.y * field_w, 0);
    glutSolidCube(1.00);
    glPopMatrix();
}

void draw_seg(segment_t *seg_p, int offset)
{
    GLfloat seg_z_offset = -(field_w * (SEG_LENGTH * offset));
    int i, j;

    glPushMatrix();
    glTranslatef(0, 0, seg_z_offset);

    for(i = 0; i < SEG_LENGTH; i++)
    {
	for(j = 0; j < TRK_WIDTH; j++)
	{
	    enum field curr_field = (*seg_p)[i][j];
	    
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

void draw_track()
{
    /* Zelimo da avatar stoji ravno nad stazom */
    GLfloat track_y_offset = -(avatar_h + field_h) / 2;
    /* Crtanje pocinje od prvog polja, zelimo da je koordinatni pocetak iznad
     * treceg */
    GLfloat track_x_offset = -(2 * field_w);

    glPushMatrix();
    glTranslatef(track_x_offset,
		 track_y_offset,
		 avatar.z * field_w);

    /* Crtaj zasebno segmente staze */
    draw_seg(prev_seg, OFFSET_PREV);
    draw_seg(curr_seg, OFFSET_CURR);
    draw_seg(next_seg, OFFSET_NEXT);
    
    glPopMatrix();
}

void set_scene()
{
    GLfloat directional_position[] = {2.4, 4.5, 1.6, 0};
    GLfloat directional_diffuse[] = {.99 , .97, .83, 1};
    GLfloat ambient_light[] = {.20, .21, .35, 1};

    GLdouble dist = 3.0;
    GLdouble z = -(2 * M_SQRT2);
    GLdouble dist_z = dist + z;

    /* Postavi matricu pogleda i modela, koje zajedno transformisu svet u nas
     * koordinatni sistem, u odnosu na kameru */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(dist, dist, dist_z,	/* Lokacija kamere, x == y == z */
    	      0, 0, z,		        /* Tacka ka kojoj je kamera okrenuta */
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
