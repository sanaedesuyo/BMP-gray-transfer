#ifndef HOMEWORK_BMP_PROCESSOR_H
#define HOMEWORK_BMP_PROCESSOR_H

#include <string>
#include <vector>

using std::vector;
using std::string;
using u32 = unsigned int;
using u16 = unsigned short;

#pragma pack(push, 1)
struct BMPFileHeader {
    unsigned char file_type[2];
    u32 file_size;
    u16 reserved1;
    u16 reserved2;
    u32 offset;
};

struct BMPInfoHeader {
    u32 size;
    int width;
    int height;
    u16 planes;
    u16 bit_count;
    u32 compression;
    u32 image_size;
    int x_resolution;
    int y_resolution;
    u32 colors_used;
    u32 colors_important;
};
#pragma pack(pop)

class BMPReader {
public:
    static bool read_BMP(const string& filename, BMPFileHeader& file_header, BMPInfoHeader& info_header, vector<unsigned char>& pixel_data);

    static void convert_gray(const vector<unsigned char>& input, vector<unsigned char>& output, int width, int height);

    static bool save_BMP(const string& file_name, BMPFileHeader &file_header, BMPInfoHeader &info_header, const vector<unsigned char>& pixel_data);
};

#endif //HOMEWORK_BMP_PROCESSOR_H
