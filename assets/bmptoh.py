def RGBToWord(r,g,b):
	rvalue=0
	rvalue = rvalue + (g>>5)	
	rvalue = rvalue + ((g & 7)<< 13)
	rvalue = rvalue + ((r>>3)<<8)
	rvalue = rvalue + ((b >> 3) << 3)
	return rvalue

def RGBToWord1(r,g,b):
# image format 5:6:5 (RGB)
	r = r >> 3;
	g = g >> 2;
	b = b >> 3;
	rvalue = (r << 11) + (g << 6)+b
	return rvalue	
from PIL import Image
ImageFileName="image.bmp"
im=Image.open(ImageFileName)
print(im.format,im.size,im.mode)
pixels=list(im.getdata())
for px in pixels:
	print(RGBToWord(px[0],px[1],px[2]),end=',')
	
