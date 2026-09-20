#include "wipi_mini_header.h"


void MC_grpInitContext(MC_GrpContext* con)
{
    con->m_mask = 0;
    con->m_fgpxl = 0xFFFFFF; // 흰색
    con->m_bgpxl = 0x000000; // 검은색
    con->m_transpxl = 0;
    con->m_alpha = 0xFF;
    con->m_offsetX = 0; // 기본 위치 조정
    con->m_offsetY = 0;
    con->m_param1 = 0;
    con->m_reserved = 0;
    con->m_font = 0;
    con->m_style = 0;
	uint8_t r1=(con->m_fgpxl >> 16) & 0xFF;
	uint8_t g1=(con->m_fgpxl >> 8) & 0xFF;
	uint8_t b1=(con->m_fgpxl) & 0xFF;
	fgcolor.r=r1;
	fgcolor.g=g1;
	fgcolor.b=b1;
	fgcolor.a=con->m_alpha;
	r1=(con->m_bgpxl >> 16) & 0x00;
	g1=(con->m_bgpxl >> 8) & 0x00;
	b1=(con->m_bgpxl) & 0x00;
	bgcolor.r=r1;
	bgcolor.g=g1;
	bgcolor.b=b1;
	bgcolor.a=con->m_alpha;
	if(fb[0]!=NULL)
		SDL_SetRenderDrawColor(fb[0]->renderer, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.a);
	if(fb[1]!=NULL)
		SDL_SetRenderDrawColor(fb[1]->renderer, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.a);
}
void MC_knlPrintk(char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}
int MC_knlSprintk(char* buffer, char* fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	int written=vsnprintf(buffer,1024,fmt,args);
	va_end(args);
	return written;
}

void MC_grpDrawString(MC_GrpFrameBuffer b, uint32_t x, uint32_t y,char* string, int len, MC_GrpContext* con)
{
    if(len==-1)
	{
		len=strlen(string);
	}
	else if(len<=0)
	{
		return;
	}
	char string2[1000]={0,};
	strncpy(string2,string,len);
	TTF_Font* font = TTF_OpenFont(".\\lib\\NanumGothic.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return;
    }
	
    SDL_Surface* surface = TTF_RenderText_Solid(font, string2, fgcolor);
    if (!surface) {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(b->renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {x+con->m_offsetX, y+con->m_offsetY, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);

    SDL_RenderCopy(b->renderer, texture, NULL, &dstRect);
    

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

MC_GrpFrameBuffer MC_grpGetScreenFrameBuffer(uint32_t buffer_number)
{
    fb[buffer_number]->width = DEFAULT_WIDTH;
    fb[buffer_number]->height = DEFAULT_HEIGHT;
    fb[buffer_number]->window = SDL_CreateWindow("SDL2 Example",
                                                 SDL_WINDOWPOS_CENTERED,
                                                 SDL_WINDOWPOS_CENTERED,
                                                 DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                                 SDL_WINDOW_SHOWN);
	fb[buffer_number]->renderer=SDL_CreateRenderer(fb[buffer_number]->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(fb[buffer_number]->renderer, fgcolor.r,fgcolor.g,fgcolor.b,fgcolor.a);  //컬러 설정 여기임
    SDL_RenderClear(fb[buffer_number]->renderer);
    return fb[buffer_number];
}
void MC_grpFlushLcd(int monitor, MC_GrpFrameBuffer b)
{
	SDL_RenderPresent(fb[monitor]->renderer);
}
void MC_grpFillRect(MC_GrpFrameBuffer b, int x, int y, uint32_t w, uint32_t h ,MC_GrpContext* con)
{
	SDL_Rect r={x,y,x+w,y+h};
	SDL_RenderFillRect(b->renderer, &r);
}
void MC_grpDrawLine(MC_GrpFrameBuffer b, int x1, int y1, int x2, int y2, MC_GrpContext* con)
{
	SDL_RenderDrawLine(b->renderer,x1,y1,x2,y2);
}
void MC_grpDrawArc(MC_GrpFrameBuffer b, int x, int y, int w, int h, int sd, int ed, MC_GrpContext* con)
{
    int cx = x + w / 2;
    int cy = y + h / 2;
    int rx = w / 2;
    int ry = h / 2;

    for(double i = sd; i <= ed; i += 0.5)
    {
        double rad = i * M_PI / 180.0;
        int x_cal = cx + rx * cos(rad);
        int y_cal = cy + ry * sin(rad);
        SDL_RenderDrawPoint(b->renderer, x_cal, y_cal);
    }
}
void MC_grpFillArc(MC_GrpFrameBuffer b, int x, int y, int w, int h, int sd, int ed, MC_GrpContext* con)
{
    int cx = x + w / 2;
    int cy = y + h / 2;
    int rx = w / 2;
    int ry = h / 2;

    for(double i = sd; i <= ed; i += 0.1)
    {
        double rad = i * M_PI / 180.0;
        int x_cal = cx + rx * cos(rad);
        int y_cal = cy + ry * sin(rad);
		SDL_RenderDrawLine(b->renderer,cx,cy,x_cal,y_cal);
    }
}
void MC_grpSetContext(MC_GrpContext* con, int num, int pv)
{
	uint64_t address=(uint64_t)con;
	int* portion=(int*)address+num;
	//printf("%d\n",portion);
	*portion=pv;
	if(num==MC_GRP_CONTEXT_FG_PIXEL_IDX)
	{
		uint8_t r1=(con->m_fgpxl >> 16) & 0xFF;
		uint8_t g1=(con->m_fgpxl >> 8) & 0xFF;
		uint8_t b1=(con->m_fgpxl) & 0xFF;
		fgcolor.r=r1;
		fgcolor.g=g1;
		fgcolor.b=b1;
		fgcolor.a=con->m_alpha;
		SDL_SetRenderDrawColor(fb[0]->renderer, fgcolor.r,fgcolor.g,fgcolor.b,fgcolor.a);
		SDL_SetRenderDrawColor(fb[1]->renderer, fgcolor.r,fgcolor.g,fgcolor.b,fgcolor.a);
	}
	else if(num==MC_GRP_CONTEXT_BG_PIXEL_IDX)
	{
		uint8_t r1=(con->m_bgpxl >> 16) & 0xFF;
		uint8_t g1=(con->m_bgpxl >> 8) & 0xFF;
		uint8_t b1=(con->m_bgpxl) & 0xFF;
		bgcolor.r=r1;
		bgcolor.g=g1;
		bgcolor.b=b1;
		bgcolor.a=con->m_alpha;
		SDL_SetRenderDrawColor(fb[0]->renderer, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.a);
		SDL_SetRenderDrawColor(fb[1]->renderer, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.a);
	}
	else if(num==8)//수정 필요
	{
		bgcolor.a=con->m_alpha;
		fgcolor.a=con->m_alpha;
		SDL_SetRenderDrawColor(fb[0]->renderer, fgcolor.r,fgcolor.g,fgcolor.b,fgcolor.a);
		SDL_SetRenderDrawColor(fb[1]->renderer, bgcolor.r,bgcolor.g,bgcolor.b,bgcolor.a);
	}
}
int MC_grpGetPixelFromRGB(int rr, int gg, int bb)
{
	uint32_t pixel=0;
	pixel=pixel | (rr<<16) | (gg<<8) | bb;
	return pixel;
}
void MC_grpRepaint()
{
	paintClet();
	if(fb[0]->renderer!=NULL)
		SDL_RenderPresent(fb[0]->renderer);
	if(fb[1]->renderer!=NULL)
		SDL_RenderPresent(fb[1]->renderer);
}
int get_keycode(SDL_Event e)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_UP:
			return -1;
		case SDLK_DOWN:
			return -2;
		case SDLK_LEFT:
			return -3;
		case SDLK_RIGHT:
			return -4;
		case SDLK_z://ok
			return -5;
		case SDLK_u://flipup
			return -18;
		case SDLK_d://flipdown
			return -17;
		case SDLK_0:
			return 48;
		case SDLK_1:
			return 49;
		case SDLK_2:
			return 50;
		case SDLK_3:
			return 51;
		case SDLK_4:
			return 52;
		case SDLK_5:
			return 53;
		case SDLK_6:
			return 54;
		case SDLK_7:
			return 55;
		case SDLK_8:
			return 56;
		case SDLK_9:
			return 57;
		default:
			return 0;
	}
}
int SDLEvent_to_code(SDL_Event event, int* parm1, int* parm2)
{
	if(event.type==SDL_KEYDOWN)
	{
		*parm1=get_keycode(event);
		*parm2=0;
		if(event.key.repeat==0)
			return MV_KEY_PRESS_EVENT;
		else
		{
			return MV_KEY_REPEAT_EVENT;
		}
	}
	else if(event.type==SDL_KEYUP)
	{
		*parm1=get_keycode(event);
		*parm2=0;
		return MV_KEY_RELEASE_EVENT;
	}
	else
	{
		return -1;
	}
}
//이미지 처리
uint32_t MC_knlGetResourceID(char* buff, uint32_t* size)
{
	unsigned char size_buf[54];
	char header[54];
	FILE* fp=fopen(buff,"rb");
	res[ID].path=buff;
	int id=ID;
    fread(header, 1, 54, fp);
    res[id].width  = *(int*)&header[18];
    res[id].height = *(int*)&header[22];
	*size=res[id].width*res[id].width;
	ID++;
	fclose(fp);
	return id;
}
int MC_knlCalloc(int size)
{
	global_buffer[global_bufid].buffer=malloc(size*4);
	int id=global_bufid;
	global_bufid++;
	return id;
}
int MC_knlGetResource(int id, int bufid,int size)
{
	if (id < 0 || id >= MAX_RES) return 0;
	 FILE* fp = fopen(res[id].path, "rb");
    if (!fp) return 0;

    unsigned char header[54];
    fread(header, 1, 54, fp);

    global_buffer[bufid].width  = *(int*)&header[18];
    global_buffer[bufid].height = *(int*)&header[22];
    res[id].bpp    = *(short*)&header[28];

    int row_bytes = 0;
    if (res[id].bpp == 24) {
        row_bytes = ((res[id].width*3 +3)/4)*4;
    } else if (res[id].bpp == 32) {
        row_bytes = res[id].width*4;
    } else {
        fclose(fp);
        return 0;
    }

    if (!global_buffer[bufid].buffer) { fclose(fp); return 0; }

    fseek(fp, *(int*)&header[10], SEEK_SET);

    unsigned char* row = malloc(row_bytes);
    if (!row) { free(global_buffer[bufid].buffer); fclose(fp); return 0; }

    for (int y=0; y<res[id].height; y++) {
        fread(row, 1, row_bytes, fp);
        int dest_y = res[id].height - 1 - y; // BMP bottom-up

        for (int x=0; x<res[id].width; x++) {
            int buf_idx = (dest_y*res[id].width + x)*4;
            if (res[id].bpp == 24) {
                int row_idx = x*3;
                global_buffer[bufid].buffer[buf_idx+0] = row[row_idx + 2]; // R
                global_buffer[bufid].buffer[buf_idx+1] = row[row_idx + 1]; // G
                global_buffer[bufid].buffer[buf_idx+2] = row[row_idx + 0]; // B
                global_buffer[bufid].buffer[buf_idx+3] = 255;              // A
            } else {
                int row_idx = x*4;
                global_buffer[bufid].buffer[buf_idx+0] = row[row_idx + 2]; // R
                global_buffer[bufid].buffer[buf_idx+1] = row[row_idx + 1]; // G
                global_buffer[bufid].buffer[buf_idx+2] = row[row_idx + 0]; // B
                global_buffer[bufid].buffer[buf_idx+3] = row[row_idx + 3]; // A
            }
        }
    }

    free(row);
    fclose(fp);
    return 1;
}
int MC_grpCreateImage(MC_GrpImage* newimg, int bufid, int monitor, int size)
{
		*newimg = SDL_CreateTexture(
        fb[monitor]->renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STATIC,
        global_buffer[bufid].width,
        global_buffer[bufid].height
    );
    SDL_UpdateTexture(*newimg, NULL, global_buffer[bufid].buffer, global_buffer[bufid].width*4);
    SDL_SetTextureBlendMode(*newimg, SDL_BLENDMODE_NONE);
    return 1;
}
void MC_grpDrawImage(MC_GrpFrameBuffer b,int dx,int dy,int w,int h,MC_GrpImage img,int sx,int sy,MC_GrpContext* con) {
    SDL_Rect dst = { dx, dy, w, h };
    SDL_RenderCopy(b->renderer, img, NULL, &dst);
	SDL_RenderPresent(b->renderer);
}
void MC_grpDestroyImage(MC_GrpImage img)
{
	SDL_DestroyTexture(img);
}
int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    fb[0] = &front;
    fb[1] = &back;
	fb[0]->renderer=NULL;
	fb[1]->renderer=NULL;
    startClet();
	paintClet();
	if(fb[0]->renderer!=NULL)
		SDL_RenderPresent(fb[0]->renderer);
	if(fb[1]->renderer!=NULL)
		SDL_RenderPresent(fb[1]->renderer);
	SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {  // 창 닫기 이벤트
                running = 0;
            }
			else
			{
				int parm1, parm2;
				int wevent=SDLEvent_to_code(event,&parm1,&parm2);
				if(wevent==-1)
					continue;
				else
					handleCletEvent(wevent, parm1, parm2);//인자 3개
			}
        }
    }
    TTF_Quit();
    SDL_Quit();
    return 0;
}
