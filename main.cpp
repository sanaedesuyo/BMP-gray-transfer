#include <iostream>
#include "bmp_processor.h"

using std::cin;
using std::cout;

int main() {

    string input_name, output_name;
    cout << "请输入bmp文件名: ";
    cin >> input_name;

    try {
        output_name = "GRAY" + input_name;
        BMPFileHeader file_header;
        BMPInfoHeader info_header;
        vector<unsigned char> pixel_data, grayscale_data;

        BMPReader::read_BMP(input_name, file_header, info_header, pixel_data);
        BMPReader::convert_gray(pixel_data, grayscale_data, info_header.width, info_header.height);
        BMPReader::save_BMP(output_name, file_header, info_header, grayscale_data);

        cout << "灰度图像已保存为" << output_name << std::endl;
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}