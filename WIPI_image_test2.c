#include "WIPI_ver1.c"
#define FIRSTIMG "star.bmp"
#define SECONDIMG "star.bmp"
MC_GrpContext cxt;
MC_GrpFrameBuffer bpl;
uint32_t width;
uint32_t height;
uint32_t imgPos;
void imageView(uint32_t pos)
{
	char buff[256];
	int32_t resid;
	uint32_t mBufID;
	int32_t size;
	int32_t ret;
	MC_GrpImage newImg;
	switch(pos)
	{
		case 1:
			MC_knlSprintk(buff,FIRSTIMG);
			break;
		case 2:
			MC_knlSprintk(buff,SECONDIMG);
			break;
	}
	resid=MC_knlGetResourceID(buff,&size);
	mBufID=MC_knlCalloc(size);
	MC_knlGetResource(resid,mBufID,size);
	ret=MC_grpCreateImage(&newImg,mBufID,0,size);
	MC_knlPrintk("ret: %d, buff id: %d, size : %d, ret : %d",ret,mBufID,size,ret);
	MC_grpDrawImage(bpl,10,20,21,21,newImg,0,0,&cxt);
	MC_grpDestroyImage(newImg);
	MC_grpFlushLcd(0,bpl);
	MC_grpRepaint();
}
void startClet()
{
	MC_grpInitContext(&cxt);
	bpl=MC_grpGetScreenFrameBuffer(0);
	width=MC_GRP_GET_FRAME_BUFFER_WIDTH(bpl);
	height=MC_GRP_GET_FRAME_BUFFER_HEIGHT(bpl);
	MC_grpSetContext(&cxt,MC_GRP_CONTEXT_FG_PIXEL_IDX,MC_grpGetPixelFromRGB(0x77,0x77,0x77));
	MC_grpFillRect(bpl,0,0,width,height,&cxt);
	MC_grpFlushLcd(0,bpl);
}
void paintClet()
{
	MC_grpFlushLcd(0,bpl);
}
void handleCletEvent(int type,int parm1, int parm2)
{
	if(type==MH_KEY_PRESSEVENT)
	{
		switch(parm1)
		{
			case '1':
				imgPos=1;
				imageView(imgPos);
				break;
			case '2':
				imgPos=2;
				imageView(imgPos);
				break;
		}
	}
}