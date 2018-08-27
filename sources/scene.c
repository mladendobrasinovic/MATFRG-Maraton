#include "maraton.h"

void set_primitive_material(GLfloat *material_base)
{
    GLfloat dim_material[] = {.0, .0, .0, 1};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_base);
    glMaterialfv(GL_FRONT, GL_SPECULAR, dim_material);
    glMaterialfv(GL_FRONT, GL_SHININESS, dim_material);
    glShadeModel(GL_SMOOTH);
}

void draw_field(enum field_type fld)
{
    GLfloat white_base[] = {.99, .99, .99, 1};
    GLfloat gray_base[] = {.22, .22, .22, 1};
    GLfloat *field_base = NULL;

    /* Odredjujemo kojeg je materijala polje. */
    if(fld == FLD_W)
	field_base = white_base;
    else if(fld == FLD_G)
	field_base = gray_base;
    
    /* Skaliranje se odnosi na dimenzije polja. */
    glPushMatrix();
    glScalef(field_w, field_h, field_w);
    
    set_primitive_material(field_base);
    glutSolidCube(1.0);
    glPopMatrix();
}

void draw_avatar()
{
    GLfloat avatar_base[] = {.24, .82, .27, 1};

    glPushMatrix();
    /* Pozicija avatara po z-osi je vec uracunata u pomeraj koordinatnog
     * sistema. */
    glTranslatef(avatar.x * field_w, avatar.y * field_w, 0);
    
    set_primitive_material(avatar_base);
    glutSolidCube(1.00);
    glPopMatrix();
}

void draw_coin(coin_t coin)
{
    GLfloat yellow_base[] = {.94, .91, .36, 1};
    GLfloat red_base[] = {.87, .34, .32, 1};
    GLfloat blue_base[] = {.45, .44, .87, 1};
    GLfloat *coin_base = NULL;

    /* Odredjujemo boju novcica. */
    switch(coin.type)
    {
    case COIN_YEL:
	coin_base = yellow_base;
	break;
    case COIN_RED:
	coin_base = red_base;
	break;
    case COIN_BLU:
	coin_base = blue_base;
	break;
    }

    glPushMatrix();
    /* Pozicija objekata je u koordinatnom sistemu sa obrnutom z-osom.  */
    glTranslatef(coin.x * field_w, coin_height * field_w,
		 -(coin.z * field_w));
    glRotatef(coin_rotation(), 0, 1, 0);
    
    set_primitive_material(coin_base);
    /* Skaliramo primitivni oblik jer funkcija ne prima argument velicine.  */
    glScalef(coin_radius, coin_radius, coin_radius);
    glutSolidIcosahedron();
    glPopMatrix();
}

void draw_seg(segment_t *seg_p, int offset)
{
    /* Pomeramo stazu skladno sa ocekivanom pozicijom segmenta, offset govori
     * koliko duzina segmenata treba pomeriti trenutni. */
    GLfloat seg_z_offset = -(field_w * (SEG_LENGTH * offset));
    int i, j;

    glPushMatrix();
    glTranslatef(0, 0, seg_z_offset);

    for(i = 0; i < SEG_LENGTH; i++)
    {
	for(j = 0; j < TRK_WIDTH; j++)
	{
	    enum field_type curr_field = seg_p->track[i][j];
	    
	    if(curr_field == FLD_X) /* Nema polja na ovoj poziciji. */
		continue;
	    
	    glPushMatrix();
	    glTranslatef(field_w * j, 0, -field_w * i);
	    draw_field(curr_field);
	    glPopMatrix();
	}
    }

    for(i = 0; i < seg_p->len_coins; i++)
	draw_coin(seg_p->coins[i]);
    
    glPopMatrix();
}

void draw_track()
{
    /* Zelimo da avatar stoji ravno nad stazom. */
    GLfloat track_y_offset = -(avatar_h + field_h) / 2;
    /* Crtanje pocinje od prvog polja, zelimo da je koordinatni pocetak iznad
     * treceg. */
    GLfloat track_x_offset = -(2 * field_w);

    glPushMatrix();
    glTranslatef(track_x_offset,
		 track_y_offset,
		 avatar.z * field_w);

    /* Crtamo zasebno segmente staze */
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

    /* Postavljamo matricu pogleda i modela, koje zajedno transformisu svet u nas
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
