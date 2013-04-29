#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "glm.h"
#define PI 3.14159265
GLuint texture_id[30];
double z=-0.1,factor=0,zoom=0,mult=1,a,k=0;
int mark=1,view=1;
double alpha=PI/2.0,t=0;
double r = 14,R=20,change=0,speed=0,l=0;
int mouse_curr_x,mouse_curr_y,mouse_prev_x,mouse_prev_y,var=0,hit=0,kal=0;
float X=0,Z=(r+R)/2-1;
float store_curr_x,store_prev_x,delta;
GLMmodel *pmodel[100];
char *File;
GLuint mode = 0;
int increase=0;
char filenames[100][200];
int markings[100],file_count=0,file_index[100];
float flow=0;
int flow_direction=1;
//car2 variables
double alpha2=PI/2.0,car2x=0,car2z=(r+R)/2+1,k2=0,speed2=0;

bool view_flag=1,car1=1;
double w_x,w_z;
float tempx,tempz;

//lighting variables
bool head1=false,head2=false,sun=false,search1=false,search2=false,flat=true;

void loadobj(char *str)
{
	File=(char*)malloc(2000*sizeof(char));
	strcpy(File,str);
	pmodel[increase]=glmReadOBJ(File);
	if (!pmodel[increase])
	{
		printf("\nUsage:");
		exit(0);
	}
	glmUnitize(pmodel[increase]);
	glmVertexNormals(pmodel[increase],90.0,GL_TRUE);
	increase++;
	free(File);
}
void load_and_allot(char *str)
{
	strcpy(filenames[file_count],str);
	loadobj(str);
	file_count++;
}
void DrawModel(int index)
{
	glPushMatrix();
	glColor3f(1,1,1);
	if (pmodel)
	{
		//if (flat)
		{
			if (markings[index]==0)
			{
				file_index[index]=glmList(pmodel[index],GLM_SMOOTH | GLM_COLOR);
				markings[index]=1;
			}
			else
			{
				glCallList(file_index[index]);
			}
		}
		/*else
		{
			if (markings[index+file_count]==0)
			{
				file_index[index+file_count]=glmList(pmodel[index],GLM_FLAT | GLM_COLOR);
				markings[index+file_count]=1;
			}
			else
			{
				glCallList(file_index[index+file_count]);
			}
		}*/			
	}
	glPopMatrix();
}
void loadTexture(int i,char* filename,GLuint *texture)
{
	FILE *image;
	char *data;
	int width,height,j;
	char temp;
	if((image=fopen(filename,"rb"))==NULL)
	{
		fprintf(stderr,"Error: Texture file \"%s\" could not be read\n",filename);
		return;
	}
	fseek(image,18,SEEK_SET);
	if((fread(&width,4,1,image))!=1)
	{
		fprintf(stderr,"Error: Could not read image width\n");
		return;
	}
	if((fread(&height,4,1,image))!=1)
	{
		fprintf(stderr,"Error: Could not read image height\n");
		return;
	}
	if((data=(char *)malloc(3*width*height*sizeof(char)))==NULL)
	{
		fprintf(stderr,"Error: Not enough memory for texture\n");
		return;
	}
	fseek(image,54,SEEK_SET);
	if((fread(data,3*width*height,1,image))!=1)
	{
		fprintf(stderr,"Error: Image format seems not to be valid\n");
		return;
	}
	fclose(image);
	for(j=0; j<3*width*height; j=j+3)
	{
		temp=data[j];
		data[j]=data[j+2];
		data[j+2]=temp;
	}
	glGenTextures(1,&texture[i]);
	glBindTexture(GL_TEXTURE_2D,texture[i]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
//	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
}
void DrawPalmTree(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(3,3,3);
	DrawModel(9);
	glPopMatrix();
}
void DrawPine(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(2,2,2);
	DrawModel(10);
	glPopMatrix();
}
void DrawBenjamina(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(2,2,2);
	DrawModel(12);
	glPopMatrix();
}
void DrawElm(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(2,2,2);
	DrawModel(13);
	glPopMatrix();
}
void DrawBeech(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(2,2,2);
	DrawModel(14);
	glPopMatrix();
}
void initialize(void)
{
	glClearColor(0,0,0,1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,(1375.0/700.0),0.5,100);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
//	loadTexture(0,(char *)"audience.bmp",texture_id);
	loadTexture(1,(char *)"road.bmp",texture_id);
	loadTexture(2,(char *)"front.bmp",texture_id);
	loadTexture(3,(char *)"camera.bmp",texture_id);
	loadTexture(4,(char *)"front_fire.bmp",texture_id);
	loadTexture(5,(char *)"fire5.bmp",texture_id);
	loadTexture(6,(char *)"ground.bmp",texture_id);
	loadTexture(7,(char *)"fire6.bmp",texture_id);
	loadTexture(8,(char *)"images.bmp",texture_id);
//	loadTexture(9,(char *)"axle.bmp",texture_id);
	loadTexture(10,(char *)"water.bmp",texture_id);
	loadTexture(11,(char *)"grass.bmp",texture_id);
}
float sunPos=180, sunIntensity=0.2;
void lights()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LESS);
	GLfloat light_ambient[] = { sunIntensity, sunIntensity, sunIntensity, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 50+150*cos(sunPos*3.14/180),150*sin(sunPos*3.14/180),17,1.0};
	//GLfloat light_position[] = {17,20,0,0};
	GLfloat intensity[] = {sunIntensity};
	GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat high_shininess[] = { 100.0 }; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, intensity);
	if(sunPos >= 360)
		sunPos = 0;
	if(sunIntensity < -0.3 || sunPos > 180 )
	{
		sun = false;
		glDisable(GL_LIGHT0);
	}
	else if(sunIntensity > -0.3 && sunPos < 180) 
		sun = true;
	if(sun)
	{
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	//head light for car1
	GLfloat light_ambient1[] = { 0, 0, 0, 0.0 };
	GLfloat light_diffuse1[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 0.0, 1.0 };

	GLfloat light_position1[] = { X+(k+1)*sin(alpha),0.4,Z+(k+1)*cos(alpha),1.0};
	GLfloat light_direction1[] = {sin(alpha),0,cos(alpha)};
	
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.f);
	if (head1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);

	
	GLfloat light_ambient2[] = { 0, 0, 0, 1.0 };
	GLfloat light_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position2[] = { 60 ,10 ,2 ,1.0 };
//	GLfloat light_position2[] = { 60 ,10 ,18 ,1.0 };
	GLfloat light_direction2[] = {0,-1,0};
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 50.f);
//	glEnable(GL_LIGHT2);
 

	//head light for car2
	GLfloat light_ambient3[] = { 0, 0, 0, 0.0 };
	GLfloat light_diffuse3[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_specular3[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_position3[] = { car2x+(k2+1)*sin(alpha2),0.4,car2z+(k2+1)*cos(alpha2),1.0 };
	GLfloat light_direction3[] = {sin(alpha2),0,cos(alpha2)};
	
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular3);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_direction3);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.f);
	if (head2)
		glEnable(GL_LIGHT3);
	else
		glDisable(GL_LIGHT3);

/*  GLfloat light_ambient4[] = { 0, 0, 0, 1.0 };
	GLfloat light_diffuse4[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular4[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position4[] = { 0,0.2,0 ,1.0 };
	//GLfloat light_direction4[] = {-1,0,0};
	GLfloat light_direction4[] = {cos(ngle*3.14/180),0,-sin(ngle*3.14/180)};
	
	glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient4);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse4);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular4);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 20.f);
  glPushMatrix();
    
    glTranslatef(0,0,-4);
    glRotatef(90,1,0,0);
    glRotatef(-180,0,1,0);
      //    glRotatef(theta,0,1,0);
    glScalef(0.16,0.16,0.12);
    glScalef(2,2,2);
    glTranslatef(0,0.11,0);
		glScalef(0.15,0.15,0.15);
    glTranslatef(mycar.X,mycar.Y+mycar.hcm,mycar.Z);
    glRotatef(mycar.theta,0,1,0);
    glRotatef(180,0,1,0);
	glLightfv(GL_LIGHT4, GL_POSITION, light_position4);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_direction4);
	glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_direction3);
  if(ssea==true)
	  glEnable(GL_LIGHT3);
  else
    glDisable(GL_LIGHT3);
  if(shea==true)
	glEnable(GL_LIGHT4);
  else
    glDisable(GL_LIGHT4);
    glPopMatrix();  */

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}
void downtown(void)
{
	glPushMatrix();
		glTranslatef(0,2,r-3);
		glScalef(3,3,3);
		DrawModel(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5,2,r-3);
		glScalef(3,3,3);
		DrawModel(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(10,2,r-3);
		glScalef(3,3,3);
		DrawModel(2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(15,2,r-3);
		glScalef(3,3,3);
		DrawModel(3);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(20,2,r-3);
		glScalef(3,3,3);
		DrawModel(4);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25,2,r-3);
		glScalef(3,3,3);
		DrawModel(5);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,2,R+3);
		glRotatef(180,0,1,0);
		glScalef(3,3,3);
		DrawModel(6);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5,2,R+3);
		glRotatef(180,0,1,0);
		glScalef(3,3,3);
		DrawModel(6);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(10,2,R+3);
		glRotatef(180,0,1,0);
		glScalef(3,3,3);
		DrawModel(4);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(15,2,R+3);
		glRotatef(180,0,1,0);
		glScalef(3,3,3);
		DrawModel(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(20,2,R+3);
		glRotatef(180,0,1,0);
		glScalef(3,3,3);
		DrawModel(0);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25,2,R+3);
		glScalef(3,3,3);
		DrawModel(3);
	glPopMatrix();
	DrawPalmTree(30,2,R+3);
	DrawPalmTree(-5,2,r-3);
	DrawPalmTree(7,2,r-3);
	DrawPalmTree(13,2,R+3);
	DrawPalmTree(18,2,r-3);
	DrawPine(7,2,R+3);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_id[11]);
	for (float h=r-20;h<R+20;h=h+10)
	{
		for (float g=-10;g<50;g=g+10)
		{
			glBegin(GL_QUADS);
				glTexCoord2i(0,0);
				glVertex3f(g,-0.08,h);
				glTexCoord2i(0,1);
				glVertex3f(g,-0.08,h+10);
				glTexCoord2i(1,1);
	glVertex3f(g+10,-0.08,h+10);
				glTexCoord2i(1,0);
				glVertex3f(g+10,-0.08,h);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
}
void track(float left,float right,float front, float back)
{
	float inc=(front-back)/100.0;
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glNormal3f(0,1,0);
	glBindTexture(GL_TEXTURE_2D,texture_id[1]);
	for (float i=back;i<=front;i=i+inc)
	{
		glBegin(GL_QUADS);
			glTexCoord2i(0,1);
			glVertex3f(i,0,left);
			glTexCoord2i(1,1);
			glVertex3f(i+inc,0,left);
			glTexCoord2i(1,0);
			glVertex3f(i+inc,0,right);
			glTexCoord2i(0,0);
			glVertex3f(i,0,right);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}
void draw(void)
{
	if (speed!=0)
		k=k+speed;
	if (speed2!=0)
		k2=k2+speed2;
	if (car1)
	{
		tempx=X+(k*sin(alpha));
		tempz=Z+(k*cos(alpha));
		a=sqrt((tempx*tempx)+(tempz*tempz));
	}
	else
	{
		tempx=car2x+(k2*sin(alpha2));
		tempz=car2z+(k2*cos(alpha2));
		a=sqrt((tempx*tempx)+(tempz*tempz));
	}
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	float temp_m=((1.55*(cos(alpha-(PI/2.0))))-((-0.2)*(sin((PI/2.0)-alpha))))+tempx;
	float temp_n=((-1.55*(sin(alpha-(PI/2.0))))+((-0.2)*(cos((PI/2.0)-alpha))))+tempz;
	float temp=sqrt((temp_m*temp_m)+(temp_n*temp_n));
	temp_m=((-0.35*(cos(alpha-(PI/2.0))))-((-0.2)*(sin((PI/2.0)-alpha))))+tempx;
	temp_n=((0.35*(sin(alpha-(PI/2.0))))+((-0.2)*(cos((PI/2.0)-alpha))))+tempz;
	temp=sqrt((temp_m*temp_m)+(temp_n*temp_n));
	if (temp>=R+3.6 and speed!=0)
		hit=1;
	glEnable(GL_DEPTH_TEST);
	//with car view
	if (view==1)
	{
		zoom=15+factor;
		if (zoom<10)
		{
			zoom=10;
			factor=-5;
		}
		if (zoom>30)
		{
			zoom=30;
			factor=15;
		}
		w_x=tempx;
		w_z=tempz;
		gluLookAt(w_x,zoom,w_z,tempx,0,tempz,1,0,0);
	}
	// overhead view
	else if (view==2)
	{
		zoom=8+factor;
		if (zoom<7)
		{
			factor=-1;
			zoom=7;
		}
		if (zoom>9)
		{
			factor=1;
			zoom=9;
		}
		gluLookAt(tempx-(zoom*sin(alpha)),7,tempz-(zoom*cos(alpha)),tempx,0.35,tempz,0,1,0);
	}
	//Front View
	else if (view==3)
	{
		gluLookAt(tempx+(5*sin(alpha+0.1)),0.45,tempz+(5*cos(alpha+0.1)),tempx,0.45,tempz,0,1,0);
	}
	// Roof Top View
	else if (view==4)
	{
		float a1,b1,c1;
		float a2,b2,c2;
		a1=((0.3*(cos(alpha-(PI/2.0))))-(-0.5*(sin((PI/2.0)-alpha))))+tempx;
		b1=0.6+0.35;
		c1=((-0.3*(sin(alpha-(PI/2.0))))+(-0.5*(cos((PI/2.0)-alpha))))+tempz;
		a2=((4*(cos(alpha-(PI/2.0))))-(-0.5*(sin((PI/2.0)-alpha))))+tempx;
		b2=-0.3+0.35;
		c2=((-4*(sin(alpha-(PI/2.0))))+(-0.5*(cos((PI/2.0)-alpha))))+tempz;
		gluLookAt(a1,b1,c1,a2,b2,c2,0,1,0);
	}
	//Slide Box View
	if (view==5)
	{
		zoom=4+factor;
		if (zoom<3)
		{
			factor=-1;
			zoom=3;
		}
		if (zoom>6)
		{
			factor=2;
			zoom=6;
		}
		float a1,b1,c1;
		float a2,b2,c2;
		a1=((0.35*(cos(alpha-(PI/2.0))))-(z*(sin((PI/2.0)-alpha))))+tempx;
		b1=0.5+0.35;
		c1=((-0.35*(sin(alpha-(PI/2.0))))+(z*(cos((PI/2.0)-alpha))))+tempz;
		a2=((zoom*(cos(alpha-(PI/2.0))))-(z*(sin((PI/2.0)-alpha))))+tempx;
		b2=-0.3+0.35;
		c2=(((-1)*zoom*(sin(alpha-(PI/2.0))))+(z*(cos((PI/2.0)-alpha))))+tempz;
		gluLookAt(a1,b1,c1,a2,b2,c2,0,1,0);
	}

	lights();
	glNormal3f(0,1,0);
	track(r,R,150,-150);
	glPushMatrix();
		glTranslatef(X+(k*sin(alpha)),0.35,Z+(k*cos(alpha)));
		glPushMatrix();
			glRotatef((alpha-(PI/2.0))*(180.0/PI),0,1,0);
			glRotatef(90,0,1,0);
			glColor3f(1,1,1);
			DrawModel(20);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(car2x+(k2*sin(alpha2)),0.35,car2z+(k2*cos(alpha2)));
		glPushMatrix();
			glRotatef((alpha2-(PI/2.0))*(180.0/PI),0,1,0);
			glRotatef(90,0,1,0);
			glColor3f(1,1,1);
			DrawModel(21);
		glPopMatrix();
	glPopMatrix();
	downtown();

	glutSwapBuffers();
}
void keyboard1 (unsigned char key, int x1, int y1)
{
	//with car view
	if (key=='1')
	{
		view=1;
		mult=1;
		factor=0;
		w_x=tempx;
		w_z=tempz;
	}
	//overhead view
	else if (key=='2')
	{
		view=2;
		mult=0.2;
		factor=0;
	}
	//front view
	else if (key=='3')
	{
		view=3;
		mult=0.01;
		factor=0;
	}
	//roof top view
	else if (key=='4')
	{
		view=4;
		mult=0.01;
		factor=0;
	}
	//slide box view
	else if (key=='5')
	{
		view=5;
		mult=0.2;
		factor=0;
	}
	else if (key=='w')
	{
		sunPos-=2;
	}
	else if (key=='e')
	{
		sunPos+=2;
	}
	else if (key=='c')
	{
		car1=!car1;
	}
	else if (key=='v')
	{
		view_flag=!view_flag;
	}
	else if (key=='<')
		head1=!head1;
	else if (key=='>')
		head2=!head2;
	else if (key=='s')
		sun=!sun;
	else if (key=='+')
		sunIntensity+=0.1;
	else if (key=='-')
		sunIntensity-=0.1;
}
void keyboard2(int key,int x,int y)
{
	if (key==GLUT_KEY_RIGHT)
	{
		if (speed!=0)
		{
			X=X+(k*sin(alpha));
			Z=Z+(k*cos(alpha));
			alpha-=0.05;
			k=0;
		}
	}
	if (key==GLUT_KEY_LEFT)
	{
		if (speed!=0)
		{
			X=X+(k*sin(alpha));
			Z=Z+(k*cos(alpha));
			alpha+=0.05;
			k=0;
		}
	}
	if (key==GLUT_KEY_UP)
	{
		if (speed<0.7)
		{
			speed=speed+0.01;
		}
	}
	if (key==GLUT_KEY_DOWN)
	{
		if (speed>-0.5)
		{
			speed=speed-0.01;
		}
	}
}
/*void mouse (int button, int state, int, int)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (view!=1 and view!=10 and view!=4)
				factor=factor+mult;
			else if (view==4)
			{
				kal=1;
				store_prev_x=mouse_curr_x;
			}
			else
				var=1;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (view!=1 and view!=10 and view!=4)
				factor=factor-mult;
			else if (view==4)
			{
				//store_curr_x=mouse_curr_x;
				//delta+=(store_curr_x-store_prev_x)/1374.0;
				kal=0;
			}
			else
				var=0;
		}
		else if (button==3)
		{
			factor=factor+mult;
		}
		else if (button==4)
		{
			factor=factor-mult;
		}
	}
}*/
void mouse(int button,int state,int x,int y)
{
	if (state==GLUT_DOWN)
	{
		if (view_flag==0)
		{
			if (button==GLUT_RIGHT_BUTTON)
			{
				if (speed2!=0)
				{
					car2x=car2x+(k2*sin(alpha2));
					car2z=car2z+(k2*cos(alpha2));
					alpha2-=0.05;
					k2=0;
				}
			}
			else if (button==GLUT_LEFT_BUTTON)
			{
				if (speed2!=0)
				{
					car2x=car2x+(k2*sin(alpha2));
					car2z=car2z+(k2*cos(alpha2));
					alpha2+=0.05;
					k2=0;
				}
			}
			else if (button==3)
			{
				if (speed2<0.7)
				{
					speed2=speed2+0.01;
				}
			}
			else if (button==4)
			{
				if (speed2>-0.5)
				{
					speed2=speed2-0.01;
				}
			}
		}
		else
		{
			if (button == GLUT_LEFT_BUTTON)
			{
				factor=factor+mult;
			}
			else if (button == GLUT_RIGHT_BUTTON)
			{
				factor=factor-mult;
			}
			else if (button==3)
			{
				factor=factor+mult;
			}
			else if (button==4)
			{
				factor=factor-mult;
			}
		}
	}
}
void mouseMovement(int x,int y)
{
	mouse_curr_x=x;
	mouse_curr_y=y;
	if (kal)
		delta+=(mouse_curr_x-store_prev_x)/1374.0;
}
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(1375,700);
	glutInitWindowPosition(1,1);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutCreateWindow("CAR");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutKeyboardFunc(keyboard1);
	glutSpecialFunc(keyboard2);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMovement);
	initialize();
	
	for (int i=0;i<100;i++)
		pmodel[i]=NULL;
	load_and_allot((char *)"house-01.obj");      // 00
	load_and_allot((char *)"house-02.obj");      // 01
	load_and_allot((char *)"house-05.obj");      // 02     
	load_and_allot((char *)"house-07.obj");      // 03
	load_and_allot((char *)"house-08.obj");      // 04
	load_and_allot((char *)"house-09.obj");     // 05
	load_and_allot((char *)"house-11.obj");     // 06
	load_and_allot((char *)"house-12.obj");     // 07
	load_and_allot((char *)"VW-new-beetle.obj");  // 08
	load_and_allot((char *)"Tree/001.obj");   // 09
	load_and_allot((char *)"pine.obj");       // 10 
	load_and_allot((char *)"suspensionbridge.obj");  // 11
	load_and_allot((char *)"benjamina.obj");   // 12
	load_and_allot((char *)"elm.obj");         // 13
	load_and_allot((char *)"beech.obj");       // 14
	load_and_allot((char *)"chanakya.obj");    // 15
	load_and_allot((char *)"parseh.obj");      // 16
	load_and_allot((char *)"text.obj");        // 17 
	load_and_allot((char *)"tower.obj");        // 18      
	load_and_allot((char *)"color.obj");        // 19
	load_and_allot((char *)"yellow_car.obj");	//20
	load_and_allot((char *)"red_car.obj");	//21
	load_and_allot((char *)"sky.obj");       //22
	file_count=23;

	glutMainLoop();
	return 0;
}
