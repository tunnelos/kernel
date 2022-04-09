//////////////////////////////////////////////////////////////////////////////
//
//                stb_image    32-bit Pixel Processing
//

typedef struct
{
   int width;
   int height;
   int stride;
   unsigned int *data;
} stb_image;

#define stb_image_define(i,w,h,d)        \
    ((i)->width=(i)->stride=(w),(i)->height=(h),(i)->data=(d))
#define stb_image_subimage(i,j,x,y,w,h)  \
    ((i)->width=(w),(i)->height=(h),     \
     (i)->stride=(j)->stride,            \
     (i)->data=&stb_image_pixel(j,x,y))
#define stb_image_pixel(i,x,y)    ((i)->data[(y)*(i)->stride+(x)])
#define stb_image_opaque(p)       ((p) >= 0xff000000)
#define stb_image_transparent(p)  ((p) <  0x01000000)

#define stb_r(p)   ( (p)        & 0xff)
#define stb_g(p)   (((p) >>  8) & 0xff)
#define stb_b(p)   (((p) >> 16) & 0xff)
#define stb_a(p)   (((p) >> 24) & 0xff)

#define stb_rgba(r,g,b,a) (((a) << 24) + ((b) << 16) + ((g) << 8) + (r))
#define stb_rgb(r,g,b)    stb_rgba(r,g,b,255)

#define stb_rb(p)   ((p) & 0xff00ff)
#define stb_g2(p)   ((p) & 0x00ff00)

#define stb_image_over(p,q)              \
     (stb_image_opaque(p)      ? (p) :   \
      stb_image_transparent(p) ? (q) :   \
      stb_image_over2(p,q,stb_a(p)))
#define stb_image_blend(p,q,a)           \
     ((a) == 0xff ? (p) : (a) == 0 ? (q) : stb_image_over2(p,q,a)

#define stb_image_blend_half(p,q)        \
     (  (((p) & 0xfefefefe) >> 1)        \
      + (((q) & 0xfefefefe) >> 1)        \
      + ((p) & (q) & 0x01010101))  // manually compute carry of bottom bits

#define stb_image_over_rb(p,q,a)         \
      (stb_rb(p) + ( (stb_rb(q)-stb_rb(p)) * (a) >> 8))
#define stb_image_over_g(p,q,a)          \
      (stb_g2(q) + ( (stb_g2(p)-stb_g2(q)) * (a) >> 8))

#define stb_image_over2(p,q,a)           \
      (stb_rb(stb_image_over_rb(p,q,a)) + \
       stb_g2(stb_image_over_g( p,q,a)) + \
       0xff000000)

#ifdef __cplusplus
   #define STB_EXTERN   extern "C"
#else
   #define STB_EXTERN   extern
#endif

/*
  unsigned char * stb_csample_uint8(src,chan,stride,width,height,int x,int y)
  int   * stb_csample_int  (src,chan,stride,width,height,int x,int y)
  float * stb_csample_float(src,chan,stride,width,height,int x,int y)
                                     -- retval = &src[(y*stride)+x)*chan)]
                                     -- but clamped to stay in rectangle

  void    stb_bilerp_uint8  (out,src,stride,width,height,float x,float y)
  void    stb_bilerp_uint8_3(out,src,stride,width,height,float x,float y)
  void    stb_bilerp_uint8_4(out,src,stride,width,height,float x,float y)
  void    stb_bilerp_int    (out,src,stride,width,height,float x,float y)
  void    stb_bilerp_float  (out,src,stride,width,height,float x,float y)
  void    stb_bilerp_float_3(out,src,stride,width,height,float x,float y)
                                     -- bilerped sample (clamped at edges),
                                     -- stored into *out; unsigned char_3 writes 3
                                     -- values, unsigned char_4 writes 4; others 1
*/
STB_EXTERN unsigned char * stb_csample_uint8(unsigned char *src, int channels, int stride,
                                        int width, int height, int x, int y);
STB_EXTERN int   * stb_csample_int  (int   *src, int channels, int stride,
                                        int width, int height, int x, int y);
STB_EXTERN float * stb_csample_float(float *src, int channels, int stride,
                                        int width, int height, int x, int y);

STB_EXTERN void stb_bilerp_uint8  (unsigned char *src, int stride, int w, int h,
                                                          float x, float y);
STB_EXTERN void stb_bilerp_uint8_3(unsigned char *src, int stride, int w, int h,
                                                          float x, float y);
STB_EXTERN void stb_bilerp_uint8_4(unsigned char *src, int stride, int w, int h,
                                                          float x, float y);
STB_EXTERN void stb_bilerp_float  (float *src, int stride, int w, int h,
                                                          float x, float y);
STB_EXTERN void stb_bilerp_float_3(float *src, int stride, int w, int h,
                                                          float x, float y);
STB_EXTERN void stb_bilerp_int    (int   *src, int stride, int w, int h,
                                                          float x, float y);

#ifdef STB_DEFINE
unsigned char * stb_csample_uint8(unsigned char *src, int channels, int stride,
                          int width, int height, int x, int y)
{
   if ((unsigned) x >= (unsigned) width ) x = (x < 0) ? 0 : width -1;
   if ((unsigned) y >= (unsigned) height) y = (y < 0) ? 0 : height-1;
   if (channels == 1)
      return src + (y * stride + x);
   else
      return src + (y * stride + x) * channels;
}

int * stb_csample_int(int *src, int channels, int stride,
                      int width, int height, int x, int y)
{
   if ((unsigned) x >= (unsigned) width ) x = (x < 0) ? 0 : width -1;
   if ((unsigned) y >= (unsigned) height) y = (y < 0) ? 0 : height-1;
   if (channels == 1)
      return src + (y * stride + x);
   else
      return src + (y * stride + x) * channels;
}

float * stb_csample_float(float *src, int channels, int stride,
                          int width, int height, int x, int y)
{
   if ((unsigned) x >= (unsigned) width ) x = (x < 0) ? 0 : width -1;
   if ((unsigned) y >= (unsigned) height) y = (y < 0) ? 0 : height-1;
   if (channels == 1)
      return src + (y * stride + x);
   else
      return src + (y * stride + x) * channels;
}

#define STB__SAMPLE_INTEGERIZE(x,y,ix,iy) \
   int ix = (int) floor(x);               \
   int iy = (int) floor(y);               \
   x -= ix;                               \
   y -= iy

#define STB__SAMPLE_CLAMP(x,width)        \
   if (x+1 < 0) x = -1; else if (x >= width) x = width-1

#define STB__SAMPLE_GET_POINTERS(src,stride,w,h, s00,s01,s10,s11, x,y, ch)  \
   s00 = src + (stride*y + x) * ch;                                         \
   s10 = s00 + stride         * ch;                                         \
   if (y+1 == 0 ) s00 = s10;                                                \
   if (y+1 == h ) s10 = s00;                                                \
   s01 = s00 + ch;                                                          \
   s11 = s10 + ch;                                                          \
   if (x+1 == 0 ) s00 = s01, s10 = s11;                                     \
   if (x+1 == w ) s01 = s00, s11 = s10;                                                  

void stb_bilerp_uint8(unsigned char *src, int stride, int width, int height, float x, float y)
{
   unsigned char *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 1);

   v00 = *s00, v01 = *s01, v10 = *s10, v11 = *s11;
   v0 = v00 + x * (v01 - v00);
   v1 = v10 + x * (v11 - v10);
   *src = (unsigned char) (v0 + y * (v1 - v0));
}

void stb_bilerp_uint8_3(unsigned char *src, int stride, int width, int height, float x, float y)
{
   int i;
   unsigned char *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 3);
   for (i=0; i < 3; ++i) {
      v00 = *s00++, v01 = *s01++, v10 = *s10++, v11 = *s11++;
      v0 = v00 + x * (v01 - v00);
      v1 = v10 + x * (v11 - v10);
      *src++ = (unsigned char) (v0 + y * (v1 - v0));
   }
}

void stb_bilerp_uint8_4(unsigned char *src, int stride, int width, int height, float x, float y)
{
   int i;
   unsigned char *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 4);
   for (i=0; i < 4; ++i) {
      v00 = *s00++, v01 = *s01++, v10 = *s10++, v11 = *s11++;
      v0 = v00 + x * (v01 - v00);
      v1 = v10 + x * (v11 - v10);
      *src++ = (unsigned char) (v0 + y * (v1 - v0));
   }
}

void stb_bilerp_float(float *src, int stride, int width, int height, float x, float y)
{
   float *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 1);

   v00 = *s00, v01 = *s01, v10 = *s10, v11 = *s11;
   v0 = v00 + x * (v01 - v00);
   v1 = v10 + x * (v11 - v10);
   *src = (v0 + y * (v1 - v0));
}

void stb_bilerp_float_3(float *src, int stride, int width, int height, float x, float y)
{
   int i;
   float *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 3);
   for (i=0; i < 3; ++i) {
      v00 = *s00++, v01 = *s01++, v10 = *s10++, v11 = *s11++;
      v0 = v00 + x * (v01 - v00);
      v1 = v10 + x * (v11 - v10);
      *src++ = (v0 + y * (v1 - v0));
   }
}

void stb_bilerp_int(int *src, int stride, int width, int height, float x, float y)
{
   int *s00,*s01,*s10,*s11, v00,v01,v10,v11;
   float v0,v1;
   STB__SAMPLE_INTEGERIZE(x,y,ix,iy);
   STB__SAMPLE_CLAMP(ix,width);
   STB__SAMPLE_CLAMP(iy,height);
   STB__SAMPLE_GET_POINTERS(src,stride,width,height, s00,s01,s10,s11, ix,iy, 1);

   v00 = *s00, v01 = *s01, v10 = *s10, v11 = *s11;
   v0 = v00 + x * (v01 - v00);
   v1 = v10 + x * (v11 - v10);
   *src = (int) (v0 + y * (v1 - v0));
}
#endif