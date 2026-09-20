#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#define MAX_RES 100

#define MC_GRP_CONTEXT_CLIP_IDX             1
#define MC_GRP_CONTEXT_FG_PIXEL_IDX         5
#define MC_GRP_CONTEXT_BG_PIXEL_IDX         6




uint64_t DEFAULT_WIDTH=340;
uint64_t DEFAULT_HEIGHT=440;
struct _MC_GrpContext
{
    int m_mask; 
    int m_clip[4];
    int m_fgpxl;
    int m_bgpxl;
    int m_transpxl;
    int m_alpha;
    int m_offsetX;
    int m_offsetY;
    int m_param1;
    int m_reserved;
    int m_font;
    int m_style;
};

struct _MC_GrpFrameBuffer
{
    uint32_t width;
    uint32_t height;
    SDL_Window* window;
	SDL_Renderer* renderer;
};
typedef enum MH_Event {
	/// 시스템을를 종료한다. 종료시켜주는 이벤트.
	MH_EXIT_EVENT = 1,
	/// 키가  눌릴때 알려 주는 이벤트.
	MH_KEY_PRESSEVENT,
	/// 키가 떼어질때 알려 주는 이벤트.
	MH_KEY_RELEASEEVENT,
	/// 키를 누르고 있는경우 알려 주는 이벤트.	
	MH_KEY_REPEATEVENT,	
	/// 타이머가 만료될때 알려 주는 이벤트.
	MH_TIMER_EVENT,
	/// SMS 메시지가 수신되었음을 알려 주는 이벤트.
	MH_SMS_EVENT,
	/// 전화가 왔음을 알려 주는 이벤트.
	MH_CALL_EVENT,
	///어넌시에이터(Annuciator) 정보가 변경 되었을 때 알려 주는 이벤트.
	MH_ANN_EVENT,
	/// 네트워크 관련 정보가 변경되었을 때 알려주는 이벤트. 
	MH_NETWORK_EVENT,
	/// 시리얼 통신  관련 정보가 변경되었을 때 알려주는 이벤트. 
	MH_SERIAL_EVENT,
	///사운드  관련 정보가 변경되었을 때 알려주는 이벤트. 
	MH_SOUND_EVENT
} MH_Event;
int ID = 0;
int global_bufid = 0;
struct gb
{
	int width, height;
	char* buffer;
	SDL_Texture* tex;
};
struct gb global_buffer[100];
struct resource {
    int width, height;
    int bpp;       // BMP 비트 깊이
	char* path;
};
struct resource res[MAX_RES];
#define MV_KEY_PRESS_EVENT          MH_KEY_PRESSEVENT
#define MV_KEY_RELEASE_EVENT        MH_KEY_RELEASEEVENT
#define MV_KEY_REPEAT_EVENT         MH_KEY_REPEATEVENT
#define MV_SMS_EVENT                MH_SMS_EVENT
#define MV_ANN_EVENT                MH_ANN_EVENT
#define MV_CALL_EVENT               MH_CALL_EVENT
#define MV_SMS_EVENT                MH_SMS_EVENT
typedef struct _MC_GrpFrameBuffer* MC_GrpFrameBuffer;
typedef struct _MC_GrpContext MC_GrpContext;
SDL_Color fgcolor={0,0,0,255};
SDL_Color bgcolor={0,0,0,255};
MC_GrpFrameBuffer fb[2];
struct _MC_GrpFrameBuffer front;
struct _MC_GrpFrameBuffer back;
uint32_t MC_GRP_GET_FRAME_BUFFER_HEIGHT(MC_GrpFrameBuffer b) { return b->height; };
uint32_t MC_GRP_GET_FRAME_BUFFER_WIDTH(MC_GrpFrameBuffer b) { return b->width; };
void MC_knlPrintk(char* ,...);
int MC_knlSprintk(char*, char*, ...);
void MC_grpInitContext(MC_GrpContext* );
void MC_grpDrawString(MC_GrpFrameBuffer b, uint32_t , uint32_t ,char* , int , MC_GrpContext* );
MC_GrpFrameBuffer MC_grpGetScreenFrameBuffer(uint32_t );
void MC_grpFlushLcd(int , MC_GrpFrameBuffer );
void startClet();
void paintClet();
void handleCletEvent(int, int, int);
void MC_grpRepaint();
void MC_grpFillRect(MC_GrpFrameBuffer, int , int , uint32_t, uint32_t,MC_GrpContext*);
void MC_grpDrawLine(MC_GrpFrameBuffer , int , int , int , int , MC_GrpContext* );
void MC_grpDrawArc(MC_GrpFrameBuffer , int ,int , int , int ,int  ,int ,MC_GrpContext* );
void MC_grpSetContext(MC_GrpContext* , int , int);
int MC_grpGetPixelFromRGB(int , int , int );
int SDLEvent_to_code(SDL_Event,int*,int*);
int get_keycode(SDL_Event);

typedef SDL_Texture* MC_GrpImage;
uint32_t MC_knlGetResourceID(char* name, uint32_t* size);
int MC_knlCalloc(int size);
int MC_knlGetResource(int id, int bufid,int size);
int MC_grpCreateImage(MC_GrpImage* newimg, int bufid, int monitor, int size);
void MC_grpDrawImage(MC_GrpFrameBuffer, int dx,int dy,int w,int h,MC_GrpImage img,int sx,int sy,MC_GrpContext*);
void MC_grpDestroyImage(MC_GrpImage img);