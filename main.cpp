#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Gutman usulida ishlatiladigan naqshlar
const std::vector<std::vector<uint8_t>> gutmann_patterns = {
    {0x55, 0xAA},           // Simmetrik naqshlar
    {0x92, 0x49, 0x24},     // Tasodifiy naqshlar
    {0x24, 0x92, 0x49},
    {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00},
    {0x92, 0x49, 0x24},
    {0x24, 0x49, 0x92},
    {0x6D, 0xB6, 0xDB},
    {0xDB, 0xB6, 0x6D},
    {0xB6, 0x6D, 0xDB}
};

void gutmann_wipe(const std::string& filename) {
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file) {
        std::cerr << "Faylni ochish imkonsiz: " << filename << std::endl;
        return;
    }

    // Fayl hajmini aniqlash
    file.seekg(0, std::ios::end);
    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(filesize);

    std::srand(std::time(nullptr)); // Tasodifiy sonlar generatorini ishga tushirish

    // Gutman usulidagi 35 martalik yozish
    for (int i = 0; i < 35; ++i) {
        if (i < 4 || i > 31) {
            // Dastlabki 4 va oxirgi 4 o'tish uchun tasodifiy bitlar yozish
            for (auto& byte : buffer) {
                byte = std::rand() % 256;
            }
        } else {
            // Gutman naqshlarini yozish
            const std::vector<uint8_t>& pattern = gutmann_patterns[(i - 4) % gutmann_patterns.size()];
            for (std::streamsize j = 0; j < filesize; ++j) {
                buffer[j] = pattern[j % pattern.size()];
            }
        }

        // Ma'lumotlarni faylga yozish
        file.seekp(0);
        file.write(reinterpret_cast<char*>(buffer.data()), filesize);
        file.flush();
    }

    // Faylni tozalash
    file.close();

    // Faylni oxirgi marta o'chirish uchun faylni 0x00 bilan to'ldirish
    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (file) {
        std::fill(buffer.begin(), buffer.end(), 0x00);
        file.write(reinterpret_cast<char*>(buffer.data()), filesize);
        file.flush();
        file.close();
    }

    // Faylni tizimdan o'chirish
    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Fayl muvaffaqiyatli o'chirildi." << std::endl;
    } else {
        std::cerr << "Faylni o'chirishda xato: " << filename << std::endl;
    }
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string filename = "C://++.txt";
        gutmann_wipe(filename);
        std::cout << "Ma'lumotlar Gutman usuli orqali ishonchli o'chirildi." << std::endl;


    return a.exec();
}
