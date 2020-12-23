#include "bmp.hpp"

void BMP::BMP_read(
    std::string bmp_name
) {

    FILE* fp = fopen(bmp_name.c_str(), "rb");
    if (fp == NULL) {
        std::cout << "open file failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "begin to read bmp file..." << std::endl;
    // get length of bmp file
    fseek(fp, 0, SEEK_END);
    size_t bmp_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fread(&this->bmp_hdr, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&this->bmp_infoHdr, sizeof(BITMAPINFOHEADER), 1, fp);

    // bmp size include data and color_table
    this->bmp_size = this->bmp_hdr.bfSize - sizeof(BITMAPFILEHEADER)
                - sizeof(BITMAPINFOHEADER);
    this->bmp_width = this->bmp_infoHdr.biWidth;
    this->bmp_height = this->bmp_infoHdr.biHeight;
    // it has to be noticed that bmp_BITpx, not bmp_BYTEpx!
    this->bmp_bitPx = this->bmp_infoHdr.biBitCount;

    /* bmp format says: bytes per line should be divided 4 exactly.
     * e.g.
     * byte per line without padding     padding     byte per line
     *               16                      0              16
     *               14                      2              16
     *               15                      1              16
     *               21                      3              24
     *
     * so byte_per_line_without_padding need +3
     */
    size_t byte_per_line_without_padding = this->bmp_bitPx * this->bmp_width / 8;
    this->bmp_bytePerLine = (byte_per_line_without_padding+3) / 4 * 4;

    if (this->bmp_bitPx == 8) {
        this->color_tablePtr = new RGBQUAD[256];
        fread(this->color_tablePtr, sizeof(RGBQUAD), 256, fp);
    }
    else if (this->bmp_bitPx == 1) {
        this->color_tablePtr = new RGBQUAD[2];
        fread(this->color_tablePtr, sizeof(RGBQUAD), 2, fp);
    }
    else
        ;

    // 在堆上申请一大块空间
    this->data = new u_char[this->bmp_bytePerLine * this->bmp_height];
    fread(this->data, 1, this->bmp_bytePerLine * this->bmp_height, fp);
    fclose(fp);
}

void BMP::BMP_write(
    std::string bmp_name,
    u_char* img_data,
    const int width,
    const int height,
    const int bitPerPixel
) {
    FILE *fp_out = fopen(bmp_name.c_str(), "wb");
    if (fp_out == NULL) {
        std::cout << "create or open file failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "begin write file..." << std::endl;

    // 有关 file_hdr 的写入
    BITMAPFILEHEADER file_hdr = this->bmp_hdr;
    fwrite(&file_hdr, sizeof(BITMAPFILEHEADER), 1, fp_out);

    // 有关 info_hdr 的写入
    BITMAPINFOHEADER info_hdr = this->bmp_infoHdr;
    fwrite(&info_hdr, sizeof(BITMAPINFOHEADER), 1, fp_out);

    // 有关 color_table 的写入
    if (bitPerPixel == 8)
        fwrite(color_tablePtr, sizeof(RGBQUAD), 256, fp_out);
    else if (bitPerPixel == 1)
        fwrite(color_tablePtr, sizeof(RGBQUAD), 2, fp_out);
    else
        ;

    fwrite(img_data, width * height, 1, fp_out);
    fclose(fp_out);

    std::cout << "end..." << std::endl;
}