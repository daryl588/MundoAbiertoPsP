#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>
#include <stdio.h>
PSP_MODULE_INFO("MundoAbiertoPSP", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
static int mission=0, completed=0, x=120, y=130, car=0, paint=0;
static const char *missions[]={"Encuentra el punto de encuentro","Recorre el barrio","Llega al taller","Visita la plaza","Explora el distrito","Regresa a casa"};
static const char *cars[]={"Compacto","Sedan","Deportivo","Moto","Pickup","Van","Taxi","4x4"};
static const char *colors[]={"Rojo","Azul","Verde","Negro","Blanco","Amarillo","Morado","Gris"};
int main(void){
 sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG); pspDebugScreenInit(); SceCtrlData pad;
 while(1){ sceCtrlReadBufferPositive(&pad,1); if(pad.Buttons&PSP_CTRL_START) break;
 if(pad.Buttons&PSP_CTRL_TRIANGLE){mission=(mission+1)%6;completed=0;}
 if(pad.Buttons&PSP_CTRL_CIRCLE) car=(car+1)%8;
 if(pad.Buttons&PSP_CTRL_SELECT) paint=(paint+1)%8;
 if(pad.Buttons&PSP_CTRL_CROSS) completed=1;
 if(pad.Buttons&PSP_CTRL_SQUARE)x+=2;
 if(pad.Buttons&PSP_CTRL_LEFT)x-=2;if(pad.Buttons&PSP_CTRL_RIGHT)x+=2;
 if(pad.Buttons&PSP_CTRL_UP)y-=2;if(pad.Buttons&PSP_CTRL_DOWN)y+=2;
 if(x<0)x=0;if(x>480)x=480;if(y<0)y=0;if(y>272)y=272;
 pspDebugScreenClear();
 pspDebugScreenPrintf("MUNDO ABIERTO PSP - DEMO 1.1\n");
 pspDebugScreenPrintf("-------------------------------\nPrototipo exploracion urbana\n\n");
 pspDebugScreenPrintf("Vehiculo: %s\nPintura: %s\n",cars[car],colors[paint]);
 pspDebugScreenPrintf("Mision %d/6: %s\n%s\n\n",mission+1,missions[mission],completed?"OBJETIVO COMPLETADO":"EN CURSO");
 pspDebugScreenPrintf("Posicion: %d,%d\n\nD-pad: mover\nCIRCLE: cambiar vehiculo\nTRIANGLE: cambiar mision\nX: completar objetivo\nSELECT: cambiar pintura\nSQUARE: acelerar/nitro simulado\nSTART: salir\n",x,y);
 sceDisplayWaitVblankStart(); }
 sceKernelExitGame(); return 0;
}
