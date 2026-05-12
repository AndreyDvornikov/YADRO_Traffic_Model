#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
#include <stdexcept>
#include "hdr/traffic.h"

struct SimCfg {
    double sim_time;
    std::string model_desc;
};

SimCfg parseConfig(const std::string& filepath) {
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
        throw std::runtime_error("Ошибка! Не удалось открыть файл: " + filepath);
    }

    SimCfg config;
    if (!(infile >> config.sim_time)) {
        throw std::runtime_error("Ошибка! Неверный формат времени симуляции");
    }
    if (config.sim_time < 0.0 || config.sim_time > 3600.0) {
        throw std::runtime_error("Ошибка! Время симуляции должно быть от 0 до 3600 секунд");
    }

    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (!std::getline(infile, config.model_desc)) {
        throw std::runtime_error("Ошибка! Не удалось прочитать описание модели");
    }
    return config;
}

void writeCSV(const std::vector<Packet>& packets, const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        throw std::runtime_error("Ошибка! Не удалось открыть файл для записи: " + filepath);
    }
    for (const auto& p : packets) {
        out << p.arrival_time << "," << p.size << "\n";
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Использование: " << argv[0] << " <input_file> [output_file]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = (argc == 3) ? argv[2] : "output.csv";

    try {
        SimCfg config = parseConfig(inputPath);

        auto model = create_traffic_model(config.model_desc);
        auto packets = model->generate(config.sim_time);

        writeCSV(packets, outputPath);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 2;
    }

    return 0;
}