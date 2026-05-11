#include <iostream>
#include <fstream>
#include <string>
#include <limits> 
#include "hdr/traffic.h"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3){
        std::cerr << "Использование: " << argv[0] << " <input_file> [output_file]\n";
        return 1;
    }

    std::string path = argv[1];
    std::ifstream infile(path);
    if(!infile.is_open()){
        std::cerr << "Ошибка! Не удалось открыть файл: " << path << "\n";
        return 2;
    }

    double sim_time = 0.0;
    std::string model_desc;

    if (!(infile >> sim_time)){
        std::cerr << "Ошибка! Неверный формат времени симуляции\n";
        return 3;
    }
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (!(std::getline(infile, model_desc))){
        std::cerr << "Ошибка! Не удалось прочитать описание модели\n";
        return 4;
    }

    std::unique_ptr<TrafficModel> model;
    try {
        model = create_traffic_model(model_desc);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка! " << e.what() << "\n";
        return 5;
    }

    std::string output = (argc == 3) ? argv[2] : "output.csv";
    std::ofstream ofile(output);
    if(!ofile.is_open()){
        std::cerr << "Ошибка! Не удалось открыть файл: " << output << "\n";
        return 6;
    }
    std::vector<Packet> packets = model->generate(sim_time);
    for(auto& packet : packets){
        ofile << packet.arrival_time << "," << packet.size << "\n";
    }
    return 0;
}