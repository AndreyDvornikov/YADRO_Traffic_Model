#pragma once

#include <memory>
#include <string>
#include <vector>


/// Структура, представляющая сетевой пакет.
struct Packet {
    double arrival_time; /// Время прихода пакета от начала симуляции (сек). 
    int    size;         /// Размер пакета (байты).
};

/// Абстрактный интерфейс модели трафика. Паттерн: Стратегия.
class TrafficModel {
    public:
        virtual ~TrafficModel() = default;

        /// Генерирует количество пакетов за указанное время.
        /// @param sim_time Время симуляции в секундах.
        /// @return Вектор пакетов, отсортированный по времени прибытия.
        /// @throws std::invalid_argument, если sim_time < 0.
        virtual std::vector<Packet> generate(double sim_time) const = 0;
};

/// Фабрика для создания модели трафика по текстовому описанию.
/// @param model_desc Строка формата "<model> <interval> <size>".
/// @return Указатель на созданную модель.
/// @throws std::invalid_argument, если описание не распознано.
std::unique_ptr<TrafficModel> create_traffic_model(const std::string& model_description);