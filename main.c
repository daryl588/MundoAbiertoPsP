
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspkernel.h>

PSP_MODULE_INFO("MundoAbiertoPSP", 0, 1, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define W 480
#define H 272
#define STRIDE 512

static unsigned int __attribute__((aligned(16))) list[262144];

typedef struct {
    unsigned int color;
    float x, y, z;
} Vertex;

static void rect(float x, float y, float w, float h,
                 unsigned int color) {
    Vertex *q = sceGuGetMemory(6 * sizeof(Vertex));

    q[0] = (Vertex){color, x, y, 0};
    q[1] = (Vertex){color, x+w, y, 0};
    q[2] = (Vertex){color, x, y+h, 0};
    q[3] = (Vertex){color, x+w, y, 0};
    q[4] = (Vertex){color, x+w, y+h, 0};
    q[5] = (Vertex){color, x, y+h, 0};

    sceGuDrawArray(
        GU_TRIANGLES,
        GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
        6, 0, q
    );
}

static void setup(void) {
    sceGuInit();
    sceGuStart(GU_DIRECT, list);

    sceGuDrawBuffer(GU_PSM_8888, (void*)0, STRIDE);
    sceGuDispBuffer(W, H, (void*)0x88000, STRIDE);
    sceGuDepthBuffer((void*)0x110000, STRIDE);

    sceGuOffset(2048-W/2, 2048-H/2);
    sceGuViewport(2048, 2048, W, H);
    sceGuScissor(0, 0, W, H);

    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDisable(GU_DEPTH_TEST);

    sceGuFinish();
    sceGuSync(0, 0);
    sceGuDisplay(GU_TRUE);
}

static void drawScene(int px, int py, int mission) {
    int i;

    sceGuStart(GU_DIRECT, list);
    sceGuClearColor(GU_RGBA(135,185,232,255));
    sceGuClear(GU_COLOR_BUFFER_BIT);

    /* Cielo y terreno */
    rect(0, 0, W, H, GU_RGBA(135,185,232,255));
    rect(0, 145, W, 127, GU_RGBA(87,148,91,255));

    /* Edificios */
    rect(8, 95, 48, 65, GU_RGBA(119,123,135,255));
    rect(62, 78, 52, 82, GU_RGBA(155,134,120,255));
    rect(120, 110, 48, 50, GU_RGBA(108,123,145,255));
    rect(174, 72, 58, 88, GU_RGBA(154,157,165,255));
    rect(240, 102, 57, 58, GU_RGBA(126,141,123,255));
    rect(304, 68, 64, 92, GU_RGBA(139,119,113,255));
    rect(374, 91, 48, 69, GU_RGBA(119,125,138,255));
    rect(430, 78, 45, 82, GU_RGBA(161,139,115,255));

    /* Ventanas */
    for (i = 0; i < 480; i += 60) {
        rect(i+10, 90, 8, 10, GU_RGBA(226,214,168,255));
        rect(i+28, 90, 8, 10, GU_RGBA(226,214,168,255));
        rect(i+10, 115, 8, 10, GU_RGBA(226,214,168,255));
    }

    /* Aceras y carretera */
    rect(0, 160, W, 27, GU_RGBA(193,184,160,255));
    rect(0, 187, W, 85, GU_RGBA(56,59,66,255));
    rect(0, 185, W, 3, GU_RGBA(243,215,138,255));

    for (i = 0; i < W; i += 85) {
        rect(i+16, 225, 42, 4, GU_RGBA(229,229,220,255));
    }

    /* Personaje */
    rect(px-7, py-18, 14, 18, GU_RGBA(52,120,212,255));
    rect(px-5, py-28, 10, 10, GU_RGBA(214,160,120,255));
    rect(px-7, py, 5, 10, GU_RGBA(39,54,74,255));
    rect(px+2, py, 5, 10, GU_RGBA(39,54,74,255));

    /* Interfaz */
    rect(8, 8, 150, 42, GU_RGBA(32,40,51,255));
    rect(14, 15, 90, 5, GU_RGBA(114,225,161,255));
    rect(14, 27, mission*18+12, 5, GU_RGBA(229,197,93,255));

    /* Minimapa */
    rect(390, 10, 78, 68, GU_RGBA(32,40,51,255));
    rect(396, 16, 66, 56, GU_RGBA(183,198,173,255));
    rect(424, 16, 8, 56, GU_RGBA(91,96,102,255));
    rect(396, 39, 66, 7, GU_RGBA(91,96,102,255));
    rect(396+(px*62/W), 16+(py*52/H), 4, 4,
         GU_RGBA(233,67,67,255));

    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

int main(void) {
    SceCtrlData pad;
    int x = 240;
    int y = 205;
    int mission = 0;
    unsigned int old = 0;

    setup();
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    for (;;) {
        sceCtrlReadBufferPositive(&pad, 1);

        if (pad.Buttons & PSP_CTRL_START)
            break;

        if (pad.Buttons & PSP_CTRL_LEFT) x -= 2;
        if (pad.Buttons & PSP_CTRL_RIGHT) x += 2;
        if (pad.Buttons & PSP_CTRL_UP) y -= 2;
        if (pad.Buttons & PSP_CTRL_DOWN) y += 2;

        if (x < 8) x = 8;
        if (x > 472) x = 472;
        if (y < 40) y = 40;
        if (y > 258) y = 258;

        if ((pad.Buttons & PSP_CTRL_TRIANGLE) &&
            !(old & PSP_CTRL_TRIANGLE)) {
            mission = (mission + 1) % 6;
        }

        drawScene(x, y, mission);
        old = pad.Buttons;
    }

    sceGuTerm();
    sceKernelExitGame();
    return 0;
}
