/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "2310027",              /* Student ID */

    "Doruk Gerçel",     /* full name */
    "e231002@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * Sobel KERNEL
 ***************/

/******************************************************
 * Your different versions of the sobel functions  go here
 ******************************************************/

/* 
 * naive_sobel - The naive baseline version of Sobel 
 */

char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim,int *src, int *dst) {
    int i,j;
    /*int ker[3][3] = {{-1, 0, 1}, 
                     {-2, 0, 2}, 
                     {-1, 0, 1}};  
	*/
    for(j = 0; j < dim; j+=2){
        for(i = 0; i < dim; i+=4) {	
                dst[j*dim+i]=0;
                dst[(j+1)*dim+i] = 0;
                dst[j*dim+i+1] = 0;
                dst[(j+1)*dim+i+1] = 0;
                dst[j*dim+i+2] = 0;
                dst[(j+1)*dim+i+2] = 0;
                dst[j*dim+i+3] = 0;
                dst[(j+1)*dim+i+3] = 0;

                if(j != 0 ){
                if(i!=0 ){
                dst[j*dim+i] = src[(j + (-1))*dim+(i + (-1))] * (-1) + 
                               src[(j + (-1))*dim+(i + (1))] +
                               src[(j)*dim+(i + (-1))] * (-2) + 
                               src[(j)*dim+(i + (1))] * (2) + 
                               src[(j + (1))*dim+(i + (-1))] * (-1) + 
                               src[(j + (1))*dim+(i + (1))];
                }

                dst[j*dim+(i+1)] = src[(j + (-1))*dim+((i+1) + (-1))] * (-1) + 
                                   src[(j + (-1))*dim+((i+1) + (1))] +
                                    src[(j)*dim+((i+1) + (-1))] * (-2) + 
                                    src[(j)*dim+((i+1) + (1))] * (2) +
                                    src[(j + (1))*dim+((i+1) + (-1))] * (-1) + 
                                    src[(j + (1))*dim+((i+1) + (1))];
                
                dst[j*dim+(i+2)] = src[(j + (-1))*dim+((i+2) + (-1))] * (-1) + 
                                   src[(j + (-1))*dim+((i+2) + (1))] +
                                    src[(j)*dim+((i+2) + (-1))] * (-2) + 
                                    src[(j)*dim+((i+2) + (1))] * (2) +
                                    src[(j + (1))*dim+((i+2) + (-1))] * (-1) + 
                                    src[(j + (1))*dim+((i+2) + (1))];

                if(i + 3 != dim - 1){
                dst[j*dim+(i+3)] = src[(j + (-1))*dim+((i+3) + (-1))] * (-1) + 
                                   src[(j + (-1))*dim+((i+3) + (1))] +
                                    src[(j)*dim+((i+3) + (-1))] * (-2) + 
                                    src[(j)*dim+((i+3) + (1))] * (2) +
                                    src[(j + (1))*dim+((i+3) + (-1))] * (-1) + 
                                    src[(j + (1))*dim+((i+3) + (1))];
                }
                }
                

                if(j+1 != dim-1){
                if(i!=0){
                dst[(j+1)*dim+i] = src[((j+1) + (-1))*dim+(i + (-1))] * (-1) + 
                                    src[((j+1) + (-1))*dim+(i + (1))] +
                                    src[((j+1))*dim+(i + (-1))] * (-2) + 
                                    src[((j+1))*dim+(i + (1))] * (2) +
                                    src[((j+1) + (1))*dim+(i + (-1))] * (-1) + 
                                    src[((j+1) + (1))*dim+(i + (1))];
                }

                dst[(j+1)*dim+(i+1)] = src[((j+1) + (-1))*dim+((i+1) + (-1))] * (-1) + 
                                    src[((j+1) + (-1))*dim+((i+1) + (1))] +
                                    src[((j+1))*dim+((i+1) + (-1))] * (-2) + 
                                    src[((j+1))*dim+((i+1) + (1))] * (2) +
                                    src[((j+1) + (1))*dim+((i+1) + (-1))] * (-1) + 
                                    src[((j+1) + (1))*dim+((i+1) + (1))];

                dst[(j+1)*dim+(i+2)] = src[((j+1) + (-1))*dim+((i+2) + (-1))] * (-1) + 
                                    src[((j+1) + (-1))*dim+((i+2) + (1))] +
                                    src[((j+1))*dim+((i+2) + (-1))] * (-2) + 
                                    src[((j+1))*dim+((i+2) + (1))] * (2) +
                                    src[((j+1) + (1))*dim+((i+2) + (-1))] * (-1) + 
                                    src[((j+1) + (1))*dim+((i+2) + (1))];

                if(i + 3 != dim - 1){
                dst[(j+1)*dim+(i+3)] = src[((j+1) + (-1))*dim+((i+3) + (-1))] * (-1) + 
                                   src[((j+1) + (-1))*dim+((i+3) + (1))] +
                                    src[(j+1)*dim+((i+3) + (-1))] * (-2) + 
                                    src[(j+1)*dim+((i+3) + (1))] * (2) +
                                    src[((j+1) + (1))*dim+((i+3) + (-1))] * (-1) + 
                                    src[((j+1) + (1))*dim+((i+3) + (1))];
                }
                }       
      	}
        
    }
    
}
/* 
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */

char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst) 
{

       naive_sobel(dim,src,dst);

}

/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_sobel_functions() {
    add_sobel_function(&naive_sobel, naive_sobel_descr);   
    add_sobel_function(&sobel, sobel_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

/******************************************************
 * Your different versions of the mirror func  go here
 ******************************************************/

/* 
 * naive_mirror - The naive baseline version of mirror 
 */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim,int *src,int *dst) {
    
 	 int i,j;

  for(i = 0; i < dim; i+=4)
        for(j = 0; j < dim; j++) {
            int* ptr1 = dst + i*dim+j;
            int* ptr2 = src+ (i+1)*dim-1-j;
            *(ptr1) = *(ptr2);
            *(ptr1+dim) = *(ptr2+dim);
            *(ptr1+2*dim) = *(ptr2+2*dim);
            *(ptr1+3*dim) = *(ptr2+3*dim);
            
        }

}


/* 
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst) 
{

        
 	naive_mirror(dim,src,dst);

}

/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_mirror_functions() {
    add_mirror_function(&naive_mirror, naive_mirror_descr);   
    add_mirror_function(&mirror, mirror_descr);   
    /* ... Register additional test functions here */
}
