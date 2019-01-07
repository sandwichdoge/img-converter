#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/sod/sod.h"


/*Convert image format
 *SUPPORTED INPUT FORMATS: JPEG, PNG, BMP, HDR, PSD, TGA, PIC, PPM, PGM, PBM
 *SUPPORTED OUTPUT FORMATS: JPEG, PNG, BMP*/


void print_help(const char *prog_name);
int convert_image(const char *src_path, const char *dst_path, int quality, int size);
char *file_get_extension(const char *str);


/*OS-agnostic, so we need to parse args manually without getopt()
 *Command should look like this:
 *img-converter <src> <dst> [-q=QUALITY] [-s=SIZE]*/
int main(int argc, char const *argv[])
{
    /*PARSE ARGUMENTS*/

    /*check if arg count is valid*/
    if (argc < 3) {
        print_help(argv[0]);
        return -1;
    }

    /*paths of src and dst files*/
    const char *src_path = argv[1];
    const char *dst_path = argv[2];

    /*other opts*/
    char *quality_ptr;
    char *size_ptr;
    int quality = 100;
    int size = 100;

    if (argc >= 4) {
        for (int i = 1; i < argc; ++i) { //parse additional opts
            /*quality of dst image*/
            if (memcmp(argv[i], "-q=", strlen("-q=")) == 0) {
                quality_ptr = (char*)argv[i] + strlen("-q=");
                sscanf(quality_ptr, "%d", &quality);

                if (quality < 50) quality = 50;
                else if (quality > 100) quality = 100;
            }
            /*size of dst image*/
            else if (memcmp(argv[i], "-s=", strlen("-s=")) == 0) {
                size_ptr = (char*)argv[i] + strlen("-s=");
                sscanf(size_ptr, "%d", &size);

                if (size <= 0) size = 10;
                else if (size > 100) size = 100;
            }
        }
    }
    
    /*CONVERT THE IMAGE*/
    int ret = convert_image(src_path, dst_path, quality, size);
    switch (ret) {
        case 0:
            printf("Done! Saved to: %s\n", dst_path);
            break;
        case -1:
            printf("Error! Cannot open input image.\n");
            break;
        case -2:
            printf("Error! Output image format is not supported.\n");
            break;
        default:
            printf("Problem saving output. Code: %d\n", ret);
    }

    return 0;
}


/*Convert image format
 *Return:
 * -On success: 0
 * -On failure: error code (negative integer)
 *SUPPORTED INPUT FORMATS: JPEG, PNG, BMP, HDR, PSD, TGA, PIC, PPM, PGM, PBM
 *SUPPORTED OUTPUT FORMATS: JPEG, PNG, BMP*/
int convert_image(const char *src_path, const char *dst_path, int quality, int size)
{
    int ret = 0;
    float s = (float)size / 100;
    char *dst_ext = file_get_extension(dst_path);
    sod_img img_in = sod_img_load_from_file(src_path, 3); if (img_in.data == 0) return -1; //can't open file or load img
    int new_w = img_in.w * s;
    int new_h = img_in.h * s;
    
    sod_img img_out = sod_resize_image(img_in, new_w, new_h);

    if (strcmp(dst_ext, ".png") == 0) {
        ret = sod_img_save_as_png(img_out, dst_path);
    }
    else if (strcmp(dst_ext, ".jpeg") == 0 || strcmp(dst_ext, ".jpg") == 0) {
        ret = sod_img_save_as_jpeg(img_out, dst_path, quality);
    }
    else if (strcmp(dst_ext, ".bmp") == 0) {
        unsigned char* raw_blob = sod_image_to_blob(img_out);
        ret = sod_img_blob_save_as_bmp(dst_path, raw_blob, img_out.w, img_out.h, 3);
    }
    else {
        ret = -2; //output format not supported
    }

    sod_free_image(img_in);
    sod_free_image(img_out);
    return ret;
}


void print_help(const char *prog_name)
{
    printf("Image-Converter: Convert format of images\n"
            "Usage: %s <src> <dst> [-q=quality] [-s=size]\n", prog_name);
}


char *file_get_extension(const char *str)
{
	char *ext = strrchr(str, '.');
	return ext;
}