#include <fstream>
#include <cstring>
#include "bmp_processor.h"

using std::ofstream;
using std::ifstream;
using std::runtime_error;

bool BMPReader::read_BMP(const string &filename, BMPFileHeader &file_header, BMPInfoHeader &info_header,
                         vector<unsigned char> &pixel_data) {
    ifstream file(filename, std::ios::binary);
    if (!file) {
        throw runtime_error("无法打开文件" + filename);
    }

    file.read(reinterpret_cast<char*>(&file_header), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(BMPInfoHeader));

    if (file_header.file_type[0] != 'B' || file_header.file_type[1] != 'M' ||
        info_header.bit_count != 24 || info_header.compression != 0) {
        throw runtime_error("不支持的BMP文件格式");
    }

    int width = std::abs(info_header.width);
    int height = std::abs(info_header.height);
    int row_size = (width * 24 + 31) / 32 * 4;
    info_header.image_size = row_size * height;

    pixel_data.resize(info_header.image_size);
    file.seekg(file_header.offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixel_data.data()), info_header.image_size);

    file.close();
    return true;
}

void BMPReader::convert_gray(const vector<unsigned char> &input, vector<unsigned char> &output, int width, int height) {
    output.resize(input.size() / 3);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int offset = (y * width + x) * 3;
            int gray = static_cast<int>(0.299 * input[offset + 2] + 0.587 * input[offset + 1] + 0.114 * input[offset]);
            output[y * width + x] = static_cast<unsigned char>(gray);
        }
    }
}

bool BMPReader::save_BMP(const string &file_name, BMPFileHeader &file_header, BMPInfoHeader &info_header,
                         const vector<unsigned char> &pixel_data) {
    ofstream file(file_name, std::ios::binary);
    if (!file) {
        throw runtime_error("无法创建文件" + file_name);
    }

    info_header.bit_count = 8;
    info_header.compression = 0;
    info_header.colors_used = 256;
    info_header.colors_important = 256;

    int width = std::abs(info_header.width);
    int height = std::abs(info_header.height);
    int row_size = (width + 3) & ~3;
    info_header.image_size = row_size * height;

    file_header.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 256 * 4 + info_header.image_size; // 加上颜色表大小
    file_header.offset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 256 * 4;

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(BMPInfoHeader));


    std::vector<unsigned char> color_table(256 * 4);
    for (int i = 0; i < 256; i++) {
        color_table[i * 4 + 0] = i;
        color_table[i * 4 + 1] = i;
        color_table[i * 4 + 2] = i;
        color_table[i * 4 + 3] = 0;
    }
    file.write(reinterpret_cast<const char*>(color_table.data()), 256 * 4);

    std::vector<unsigned char> row(row_size);
    for (int y = 0; y < height; y++) {
        memcpy(row.data(), &pixel_data[y * width], width);
        // 添加填充字节
        memset(&row[width], 0, row_size - width);
        file.write(reinterpret_cast<const char*>(row.data()), row_size);
    }

    file.close();
    return true;
}
