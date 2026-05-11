#include "hdr/traffic.h"
#include <stdexcept>
#include <random>
#include <sstream>
#include <chrono>

class EqualModel : public TrafficModel {
    private:
        double period_; /// Интервал между пакетами
        int    size_;   /// Размер пакета
    public:
        EqualModel (double period, int size)
            : period_(period), size_(size)
            {
                if (period <= 0 || size <= 0) {
                    throw std::invalid_argument("Равномерная модель: период и размер не могут быть отрицательными!");
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

class PoissonModel : public TrafficModel {
    private:
        double mean_interval_;
        double mean_size_;
        mutable std::mt19937 rng_;
    public:
        PoissonModel (double interval, double size)
            :   mean_interval_(interval), mean_size_(size), 
                rng_(std::chrono::steady_clock::now().time_since_epoch().count())
            {
                if (interval <= 0 || size <= 0) {
                    throw std::invalid_argument("Пуассоновская модель: интервал и размер не могут быть отрицательными!");
                }
            }
        std::vector<Packet> generate(double sim_time) const override
        {
            std::vector<Packet> packets;
            std::exponential_distribution<double> interval_dist(1.0 / mean_interval_);
            std::exponential_distribution<double> size_dist(1.0 / mean_size_);
            double t = 0.0;
            while (t < sim_time) {
                double raw_size = size_dist(rng_);
                int pkt_size = std::max(1, static_cast<int>(std::round(raw_size)));

                packets.push_back({t, pkt_size});

                double dt = interval_dist(rng_);
                t += dt;
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
            throw std::invalid_argument("Неверный формат для equal модели! Ожидается: equal <period> <size>");
        }
        return std::make_unique<EqualModel>(period, size);
    } else if (model == "poisson") {
        double mean_interval = 0.0, mean_size = 0.0;
        if (!(iss >> mean_interval >> mean_size)) {
            throw std::invalid_argument("Неверный формат для poisson модели. Ожидается: poisson <mean_interval> <mean_size>");
        }
        return std::make_unique<PoissonModel>(mean_interval, mean_size);
    }

    throw std::invalid_argument("Неизвестная модель: " + model);
}