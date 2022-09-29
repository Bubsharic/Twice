#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void dfs(int v, int color, int iw, int ih, int* col, unsigned char* mat){
    int q,w,e,r;
    col[v] = color;
    if (v+iw+1 < iw*ih){
        q = mat[v+iw+1];
        if ( (col[v+iw+1] == 0) && (abs(mat[v] - q) < 5) ){
		 dfs(v+iw+1, color, iw, ih, col, mat); };
    };

    if (v-iw-1 > 0){
        w = mat[v-iw-1];
        if ( (col[v-iw-1] == 0) && (abs(mat[v] - w) < 5) ){
		 dfs(v-iw-1, color, iw, ih, col, mat); };
    };


    if (v-iw+1 > 0){
        e = mat[v-iw+1];
        if ( (col[v-iw+1] == 0) && (abs(mat[v] - e) < 5) ){
		 dfs(v-iw+1, color, iw, ih, col, mat); };
    };
    
    if (v+iw-1 < ih*iw){
        r = mat[v+iw-1];
        if ( (col[v+iw-1] == 0) && (abs(mat[v] - r) < 5) ){
		 dfs(v+iw-1, color, iw, ih, col, mat); };
    };
};

int RGB_to_gray(char r, char g, char b){ 
	return ((r*11+g*16+5*b)/32);	
};


int main(){
	char* inputPath ="hampster.png";
	
	int iw,ih,n;
	unsigned char * idata = stbi_load(inputPath, &iw, &ih, &n, 0);
	if (idata == NULL){
		printf("ERROR! (idata == NULL)");
		return -1;
	};
	
	printf("iw = %d,  ih = %d, iw*ih =  %d\n", iw, ih, iw*ih);
	unsigned char* graph = (unsigned char*)malloc( iw*ih*n*(sizeof(unsigned char)) );
	if (graph == NULL){
		printf("ERROR! (graph == NULL)");
		return -1;
	}; 
	
	int* color = (int*)malloc(iw*ih*(sizeof(int)));
	unsigned char* odata=malloc(iw*ih*(sizeof(unsigned char)));
	if (odata == NULL){
		printf("ERROR! (odata == NULL");
	    return -1;
	};
	
	int count=0;
	for(int i=0; i<iw*ih*n; i+=n){
		char r = idata[i];
		char g = idata[i+1];
		char b = idata[i+2];
		odata[count]= RGB_to_gray(r, g, b);
		count++;	
	};
	
	 for (int i=2; i < ih-1; i++){
        for (int j=2; j < iw-1; j++){
            if (odata[iw*i+j] < 50) {odata[iw*i+j] = 0;};
            if (odata[iw*i+j] > 200) {odata[iw*i+j] = 255;};
        };
    };
    
    
    unsigned char* Filter = (unsigned char*)malloc(ih*iw*sizeof(unsigned char)); 
    
    for (int i=1; i < ih-1; i++){
        for (int j=2; j < iw-1; j++){
            
            Filter[iw*i+j] = 0.15*odata[iw*i+j] + 0.12*odata[iw*(i+1)+j] + 0.12*odata[iw*(i-1)+j];
            
            Filter[iw*i+j] = Filter[iw*i+j] + 0.12*odata[iw*i+(j+1)] + 0.12*odata[iw*i+(j-1)];
            
            Filter[iw*i+j] = Filter[iw*i+j] + 0.09*odata[iw*(i+1)+(j+1)] + 0.09*odata[iw*(i+1)+(j-1)];
            
            Filter[iw*i+j] = Filter[iw*i+j] + 0.09*odata[iw*(i-1)+(j+1)] + 0.09*odata[iw*(i-1)+(j-1)];
            
        };
    };
    
	unsigned char x, z, c;
	for(int i=2; i < ih-1; i++){
		for(int j=2; j < iw-1; j++){
			x = Filter[iw*(i-1)+j] - Filter[iw*i+(j+1)];
			
			z = Filter[iw*(i-1)+(j-1)]-Filter[iw*i+j];
			
			c = sqrt(pow(z,2) - pow(x,2));
			odata[iw*i+j] = c; 
		}; 
	};

	for(int s=0; s < iw*ih; s++){
		color[s] = 0;};

	int d=80;			
	for(int i=0; i < iw*ih; i++){
		if(color[i] == 0){
			dfs(i, d, iw, ih, color, Filter);
			d=d+30; }; };
			

	for(int i=0; i < iw*ih; i++){
		graph[i*4] = 78+color[i]+0.5*color[s-1];
		graph[i*4+1] = 46+color[i];
		graph[i*4+2] = 153+color[i];
		graph[i*4+3] = 255;}

	char *outputPath="output.png";
	char *filteredPath="filtered.png";
	stbi_write_png(filteredPath, iw, ih, 1, Filter,0);
	stbi_write_png(outputPath, iw, ih, n, graph, 0);
	stbi_image_free(idata);
	printf("Image: width = %dpx, height  %dpx , %d channels",iw,ih,n);
	
return 0;

};
