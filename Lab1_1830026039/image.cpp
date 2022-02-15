unsigned char *tempin, *tempout;
 int size;
 Image *outimage;

 outimage=CreateNewImage(image,"#testing Swap",image->Width/2,image->Height/2);
 tempin=image->data;
 tempout=outimage->data;

 if(image->Type==GRAY)   size  = image->Width * image->Height;
 else if(image->Type==COLOR) size  = image->Width * image->Height * 3;//彩色图有RGB三层

 for(int i = 0;i < image->Height / 2; ++i){//读取纵坐标上的点
  for(int j = 0; j < image->Width / 2; ++j){//读取横坐标上的点
   tempout[image->Width * i / 2 + j] = tempin[2 * image->Width * i + 2 * j];
  }
 }
 return(outimage);