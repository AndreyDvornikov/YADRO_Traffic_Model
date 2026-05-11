#include "hdr/traffic.h"
#include <stdexcept>
#include <random>
#include <sstream>


class EqualModel : public TrafficModel {
    private:
        double period_; /// Интервал между пакетами
        int    size_;   /// Размер пакета
    public:
        EqualModel (double period, int size)
            : period_(period), size_(size)
            {
                if (period <= 0 || size <= 0) {
                    throw std::invalid_argument("Равномерная модель: период или размер не могут быть отрицательными!");
                }
        }
        std::vector<Packet> generate(double sim_time) const override
        {
            std::vector<Packet> packets;
            for (double t = 0.0; t < sim_time; t += period_) {
                packets.push_back({t, size_});
            }
            return packets;
        }
};


std::unique_ptr<TrafficModel> create_traffic_model(const std::string& model_desc)
{
    std::istringstream iss(model_desc);
    std::string model;
    if (!(iss >> model)){
        throw std::invalid_argument("Пустое описание модели");
    }

    if (model == "equal"){
        double period = 0.0;
        int size = 0;
        if (!(iss >> period >> size)){
            throw std::invalid_argument("Неверный формат для equal модели!");
        }
        return std::make_unique<EqualModel>(period, size);
    }

    throw std::invalid_argument("Неизвестная модель:" + model);
}